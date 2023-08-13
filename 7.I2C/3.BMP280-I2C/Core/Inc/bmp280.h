#ifndef BMP280_H
#define BMP280_H

#include "stm32f4xx_hal.h"
#include "i2c.h"

uint8_t bmp280_init(void);		     // call this first
uint8_t bmp280_get_status(void);	 // read the status register
void bmp280_set_config(uint8_t t_sb, uint8_t filter, uint8_t spi3w_en);  // set the configuration register      
void bmp280_set_ctrl(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode);      // set the oversampling and mode (this starts the conversion)
float bmp280_getpressure(void);      // return prssure value in hpa     
float bmp280_gettemperature(void);   // return temparature value in celcius
double bmp280_getaltitude(void);     // return altutude value in meters

#endif