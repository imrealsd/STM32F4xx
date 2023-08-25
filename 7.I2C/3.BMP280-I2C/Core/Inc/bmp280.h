/***********************************************************************
* File: bmp280.h
*
* Description: bmp280 library functions prototypes & macros.
*
* Author: Subhadeep Dhang
*
* Date: August 22, 2023
************************************************************************/

#ifndef BMP280_H
#define BMP280_H

/*Driver Macros*/

#define BMP280_ADDR	            0x76 // 0x77

#define BMP280_ID_REG		    0xD0
#define BMP280_ID_VAL		    0x58

#define BMP280_CAL_REG_FIRST	0x88
#define BMP280_CAL_REG_LAST	    0xA1
#define BMP280_CAL_DATA_SIZE	(BMP280_CAL_REG_LAST - BMP280_CAL_REG_FIRST + 1)

#define BMP280_STATUS_REG	    0xF3
#define BMP280_CONTROL_REG	    0xF4
#define BMP280_CONFIG_REG	    0xF5

#define BMP280_PRES_REG		    0xF7
#define BMP280_TEMP_REG		    0xFA
#define BMP280_RAWDATA_BYTES    6	  


/*Driver APIs*/

/**
 * @brief initialize bmp280 sensor
 * @param none
 * @retval 1 (success) / 0 (faliure)
 */
uint8_t bmp280_init(void);	

/**
 * @brief get bmp280 status
 * @param none
 * @retval status register value 
 */
uint8_t bmp280_get_status(void);	

/**
 * @brief set bmp280 config register
 * @param t_sb standby duration 
 * @param filter time constant of IIR filter
 * @param spi3w_en 3 wire spi mode enable
 * @retval none
 */
void bmp280_set_config(uint8_t t_sb, uint8_t filter, uint8_t spi3w_en);  

/**
 * @brief set bmp280 control register
 * @param osrs_t oversampling of temparature data
 * @param osrs_p oversampling of pressure data
 * @param mode power mode of bmp280
 * @retval 
 */
void bmp280_set_ctrl(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode); 

/**
 * @brief get pressure value
 * @param none
 * @retval pressure in hpa unit
 */
float bmp280_get_pressure(void);    

/**
 * @brief get temperature value
 * @param none
 * @retval temperature in celcius unit
 */
float bmp280_get_temperature(void);   

/**
 * @brief get alitude value
 * @param none
 * @retval altitude in meter unit
 */
double bmp280_get_altitude(void);     

#endif