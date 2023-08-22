#include "bmp280.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


static struct bmp280_data_structure {
    uint32_t pressure;
    int32_t temperature;
} bmp280_data;


static union bmp280_calibration_union {
	uint8_t bytes[BMP280_CAL_DATA_SIZE];
	struct {
		uint16_t dig_t1;
		int16_t  dig_t2;
		int16_t  dig_t3;
		uint16_t dig_p1;
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


/*private function protorypes*/
static void bmp280_measure(void);
static void bmp280_read_mem(uint8_t reg_address, uint8_t rx_buff[], uint8_t buff_size);
static void bmp280_write_mem(uint8_t reg_address, uint8_t reg_Value);
static void bmp280_get_calibration(void);
static int32_t bmp280_pack_20bit(uint8_t byte1, uint8_t byte2, uint8_t byte3);


/*Function Definitations*/

uint8_t bmp280_init(void)
{
	uint8_t id_reg_val;
	HAL_Delay(10);
    
	/*Check the ID register*/
	id_reg_val = 0;
	bmp280_read_mem(BMP280_ID_REG, &id_reg_val, 1);
	if (id_reg_val != BMP280_ID_VAL)
		return 0;

	bmp280_get_calibration();

    /* 0.5 ms delay, 16x filter, no 3-wire SPI */
	bmp280_set_config(0, 4, 0); 
    /* T oversample x2, P over x2, normal mode */
	bmp280_set_ctrl(2, 5, 3); 
	return 1;
}

uint8_t bmp280_get_status(void)
{
	uint8_t status_reg_val;
	bmp280_read_mem(BMP280_STATUS_REG, &status_reg_val, 1);
	return status_reg_val;
}

float bmp280_get_temperature(void) 
{
	bmp280_measure();
	return (float)bmp280_data.temperature / 100;
}

float bmp280_get_pressure(void) 
{
	bmp280_measure();
	return (float)bmp280_data.pressure / 100;
}

double bmp280_get_altitude(void) 
{
	double alt;
	bmp280_measure();
	alt = (1 - pow(bmp280_data.pressure/(double)101325, 0.1903)) / 0.0000225577;
	return (alt/10);
}

void bmp280_set_ctrl(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode)
{   
    uint8_t ctrl_reg_val;
    ctrl_reg_val = ((osrs_t & 0x7) << 5) | ((osrs_p & 0x7) << 2) | (mode & 0x3);
	bmp280_write_mem(BMP280_CONTROL_REG, ctrl_reg_val);
}


void bmp280_set_config(uint8_t t_sb, uint8_t filter, uint8_t spi3w_en)
{   
    uint8_t conf_reg_val;
    conf_reg_val = ((t_sb & 0x7) << 5)| ((filter & 0x7) << 2)| (spi3w_en & 1);
	bmp280_write_mem(BMP280_CONFIG_REG, conf_reg_val);
}


static int32_t bmp280_pack_20bit(uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
    int32_t data;
    data = (byte1 << 12) | (byte2 << 4) | ((byte3 & 0xF0) >> 4);
    return data;
}


static void bmp280_get_calibration(void)
{
	memset(bmp280_cal.bytes, 0, sizeof(bmp280_cal));
	bmp280_read_mem( BMP280_CAL_REG_FIRST, bmp280_cal.bytes,BMP280_CAL_DATA_SIZE);
}


static void bmp280_write_mem(uint8_t reg_address, uint8_t reg_Value)
{
    HAL_I2C_Mem_Write(&hi2c2, (BMP280_ADDR << 1), reg_address, 1, &reg_Value, 1, HAL_MAX_DELAY);
}


static void bmp280_read_mem(uint8_t reg_address, uint8_t rx_buff[], uint8_t buff_size)
{
    HAL_I2C_Mem_Read(&hi2c2, (BMP280_ADDR << 1), reg_address, 1, rx_buff, buff_size, HAL_MAX_DELAY);
}

	
static void bmp280_measure(void)
{
	uint8_t raw_data[BMP280_RAWDATA_BYTES];
	int32_t temp_raw;
    int32_t pres_raw;
	int32_t var1;
    int32_t var2;
    int32_t t_fine;
	
	/*read the raw ADC data from the I2C registers*/
	bmp280_read_mem(BMP280_PRES_REG, raw_data, BMP280_RAWDATA_BYTES);
	pres_raw = bmp280_pack_20bit(raw_data[0], raw_data[1], raw_data[2]);
	temp_raw = bmp280_pack_20bit(raw_data[3], raw_data[4], raw_data[5]);

	/*The following code is based on a 32-bit integer code from the BMP280 datasheet*/

	/*compute the temperature*/
	var1 = ((((temp_raw >> 3) - ((int32_t)bmp280_cal.dig_t1 << 1)))
		* ((int32_t)bmp280_cal.dig_t2)) >> 11;
	var2 = (((((temp_raw >> 4) - ((int32_t)bmp280_cal.dig_t1))
		* ((temp_raw >> 4) - ((int32_t)bmp280_cal.dig_t1))) >> 12)
		* ((int32_t)bmp280_cal.dig_t3)) >> 14;
	t_fine = var1 + var2;
	bmp280_data.temperature = (t_fine * 5 + 128) >> 8;

	/*compute the pressure*/
	var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)bmp280_cal.dig_p6);
	var2 = var2 + ((var1 * ((int32_t)bmp280_cal.dig_p5)) << 1);
	var2 = (var2 >> 2) + (((int32_t)bmp280_cal.dig_p4) << 16);
	var1 = (((bmp280_cal.dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3)
		+ ((((int32_t)bmp280_cal.dig_p2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((int32_t)bmp280_cal.dig_p1)) >> 15);

	if (var1 == 0) {
		bmp280_data.pressure = 0;
	} else {
		bmp280_data.pressure = (((uint32_t)(((int32_t)1048576)-pres_raw)
			- (var2 >> 12))) * 3125;
		if (bmp280_data.pressure < 0x80000000) {
			bmp280_data.pressure = (bmp280_data.pressure << 1) / ((uint32_t)var1);
		} else {
			bmp280_data.pressure = (bmp280_data.pressure / (uint32_t)var1) * 2;
		}
		var1 = (((int32_t)bmp280_cal.dig_p9) * ((int32_t)(((bmp280_data.pressure>>3) * (bmp280_data.pressure >> 3)) >> 13))) >> 12;
		var2 = (((int32_t)(bmp280_data.pressure >> 2)) * ((int32_t)bmp280_cal.dig_p8)) >> 13;
		bmp280_data.pressure = (uint32_t)((int32_t)bmp280_data.pressure + ((var1 + var2 + bmp280_cal.dig_p7) >> 4));
	}
}
