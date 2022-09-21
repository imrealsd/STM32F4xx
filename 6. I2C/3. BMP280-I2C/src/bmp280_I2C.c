#include "I2C1.h"
#include "bpm280_I2C.h"
#include "delay.h"
#include <string.h>
#include <math.h>
#include "UART1.h"

#define BMP280_ADDR 0x76            // SDB pin connected to GND

#define BMP280_ID_REG		0xD0
#define BMP280_ID_VAL		0x58 

#define BMP280_CAL_REG_FIRST	0x88
#define BMP280_CAL_REG_LAST  	0xA1
#define BMP280_CAL_DATA_SIZE	(BMP280_CAL_REG_LAST+1 - BMP280_CAL_REG_FIRST)

#define BMP280_STATUS_REG	0xF3
#define BMP280_CONTROL_REG	0xF4
#define BMP280_CONFIG_REG	0xF5

#define BMP280_PRES_REG		0xF7
#define BMP280_TEMP_REG		0xFA
#define BMP280_RAWDATA_BYTES	6	// 3 bytes pressure, 3 bytes temperature

// Global variables for the sake of simplicity.
// We provide wrapper macros for them in bmp280.h.
int32_t _bmp280_temp;
uint32_t _bmp280_pres;


/* this function is static -> only accessable in this file */
static void bmp280_writemem (int8_t reg, int8_t val)
{
    I2C1_start();
    I2C1_write_addr(BMP280_ADDR, "WRITE");
    I2C1_write_data(reg);
    I2C1_write_data(val);
    I2C1_stop();
}

void bmp280_readmem (int8_t reg, int8_t buff[], int8_t bytes)
{	
	I2C1_start();
    I2C1_write_addr(BMP280_ADDR,"WRITE");
    I2C1_write_data(reg);
    I2C1_start();
	I2C1_read((BMP280_ADDR << 1 | 1), buff, bytes);
	I2C1_stop();
}


static union bmp280_cal_union {

    int8_t bytes[BMP280_CAL_DATA_SIZE];
    struct {
        int16_t  dig_t1;
		int16_t  dig_t2;
		int16_t  dig_t3;
		int16_t  dig_p1;
		int16_t  dig_p2;
		int16_t  dig_p3;
		int16_t  dig_p4;
		int16_t  dig_p5;
		int16_t  dig_p6;
		int16_t  dig_p7;
		int16_t  dig_p8;
		int16_t  dig_p9;
    };
} bmp280_cal;


/* Read calibration Registers */
static void bmp280_getcalibration(void)
{
	memset(bmp280_cal.bytes, 0, sizeof(bmp280_cal));

	bmp280_readmem(
		BMP280_CAL_REG_FIRST,
		bmp280_cal.bytes,
		BMP280_CAL_DATA_SIZE
	);
}

uint8_t bmp280_init(void)
{	
	setup_uart1(9600);
	int8_t buffer[1];
	setup_I2C1_SM();
    delay_us(10);

	// look up the ID register
	buffer[0] = 0;
	bmp280_readmem(BMP280_ID_REG, buffer, 1);

	if (buffer[0] != BMP280_ID_VAL)
		return 0;

	bmp280_getcalibration();
	bmp280_set_config(0, 4, 0); // 0.5 ms delay, 16x filter, no 3-wire SPI
	bmp280_set_ctrl(2, 5, 3);   // T oversample x2, P over x2, normal mode

	return 1;
}

uint8_t bmp280_get_status(void)
{
	int8_t data[1];
	bmp280_readmem(BMP280_STATUS_REG, data, 1);
	return data[0];
}

void bmp280_set_ctrl(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode)
{
	bmp280_writemem(BMP280_CONTROL_REG,
		((osrs_t & 0x7) << 5)
		| ((osrs_p & 0x7) << 2)
		| (mode & 0x3)
	);
}

void bmp280_set_config(uint8_t t_sb, uint8_t filter, uint8_t spi3w_en)
{
	bmp280_writemem(BMP280_CONFIG_REG,
		((t_sb & 0x7) << 5)
		| ((filter & 0x7) << 2)
		| (spi3w_en & 1)
	);
}

#define bmp280_20bit_reg(b1, b2, b3)	( \
	((int32_t)(b1) << 12) \
	| ((int32_t)(b2) << 4) \
	| ((int32_t)(b3) >> 4) \
)
	
void bmp280_measure(void)
{
	int8_t data[BMP280_RAWDATA_BYTES];
	int32_t temp_raw, pres_raw,
		var1, var2, t_fine;
	
	// read the raw ADC data from the I2C registers
	bmp280_readmem(BMP280_PRES_REG, data, BMP280_RAWDATA_BYTES);
	pres_raw = bmp280_20bit_reg(data[0], data[1], data[2]);
	temp_raw = bmp280_20bit_reg(data[3], data[4], data[5]);


	// The following code is based on a 32-bit integer code
	// from the BMP280 datasheet

	// compute the temperature
	var1 = ((((temp_raw >> 3) - ((int32_t)bmp280_cal.dig_t1 << 1)))
		* ((int32_t)bmp280_cal.dig_t2)) >> 11;
	var2 = (((((temp_raw >> 4) - ((int32_t)bmp280_cal.dig_t1))
		* ((temp_raw >> 4) - ((int32_t)bmp280_cal.dig_t1))) >> 12)
		* ((int32_t)bmp280_cal.dig_t3)) >> 14;
	t_fine = var1 + var2;
	_bmp280_temp = (t_fine * 5 + 128) >> 8;
	

	// compute the pressure
	var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)bmp280_cal.dig_p6);
	var2 = var2 + ((var1 * ((int32_t)bmp280_cal.dig_p5)) << 1);
	var2 = (var2 >> 2) + (((int32_t)bmp280_cal.dig_p4) << 16);
	var1 = (((bmp280_cal.dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3)
		+ ((((int32_t)bmp280_cal.dig_p2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((int32_t)bmp280_cal.dig_p1)) >> 15);

	if (var1 == 0) {
		_bmp280_pres = 0;
	} else {
		_bmp280_pres = (((uint32_t)(((int32_t)1048576)-pres_raw)
			- (var2 >> 12))) * 3125;
		if (_bmp280_pres < 0x80000000) {
			_bmp280_pres = (_bmp280_pres << 1) / ((uint32_t)var1);
		} else {
			_bmp280_pres = (_bmp280_pres / (uint32_t)var1) * 2;
		}
		var1 = (((int32_t)bmp280_cal.dig_p9) * ((int32_t)(((_bmp280_pres>>3) * (_bmp280_pres >> 3)) >> 13))) >> 12;
		var2 = (((int32_t)(_bmp280_pres >> 2)) * ((int32_t)bmp280_cal.dig_p8)) >> 13;
		_bmp280_pres = (uint32_t)((int32_t)_bmp280_pres + ((var1 + var2 + bmp280_cal.dig_p7) >> 4));
	}
}
