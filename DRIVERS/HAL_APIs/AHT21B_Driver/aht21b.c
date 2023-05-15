/***********************************************************************
* File: aht21b.c
*
* Description: Implementation of aht21b library functions.
*
* Dependencies: "I2C.h" & "stm32f4xx_hal.h" provided  by STMicroelectronics
*               <string.h> <stdint.h> <math.h> C standard libraries.
*
* Author: Subhadeep Dhang
*
* Date: August 12, 2023
************************************************************************/

/*Includes*/
#include "aht21b.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include <stdint.h>
#include <string.h>
#include <math.h>


/*private function prototypes*/
static float aht21b_convert_raw_humidity(uint32_t* p_raw_humidity);
static float aht21b_convert_raw_temparature(uint32_t* p_raw_temperature);
static void aht21b_get_raw_value(uint32_t* p_raw_temperature, uint32_t* p_raw_humidity);

/**
 * @brief check status of AHT21B 
*/
void aht21b_setup(void)
{   
    uint8_t tx_data  = CMD_GET_STATUS;
    uint8_t rx_data  = 0x00; 

    HAL_Delay(100);
    HAL_I2C_Master_Transmit(&hi2c1, AHT21B_ADDRESS, &tx_data, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, AHT21B_ADDRESS, &rx_data, 1, HAL_MAX_DELAY);
    if (rx_data != STATUS_OK) {
        // yet to be implemented
    }
}

/**
 * @brief read Temparature from AHT21B
*/
float aht21b_get_temperature(void)
{
    uint32_t raw_temp  = 0;
    uint32_t raw_humidity   = 0;
    float celcius_temp = 0;

    aht21b_get_raw_value(&raw_temp, &raw_humidity);
    celcius_temp = aht21b_convert_raw_temparature(&raw_temp);
    return celcius_temp;
}

/**
 * @brief read Humidity from AHT21B
*/
float aht21b_get_humidity(void)
{
    uint32_t raw_temp = 0;
    uint32_t raw_humidity  = 0;
    float humidity = 0;

    aht21b_get_raw_value(&raw_temp, &raw_humidity);
    humidity = aht21b_convert_raw_humidity(&raw_humidity);
    return humidity;
}   

/**
 * @brief read raw values of Temparature & Humidity from AHT21B
*/
static void aht21b_get_raw_value(uint32_t* p_raw_temperature, uint32_t* p_raw_humidity)
{
    uint8_t  tx_buff[SIZE_TRIG_CONV_CMD];
    uint8_t  rx_buff[SIZE_SESNOR_DATA];
    uint32_t raw_temparature = 0x00;
    uint32_t raw_humidity = 0x00;

    tx_buff[0] = CMD_TRIG_CONV;
    tx_buff[1] = TRIG_CONV_ARG1;
    tx_buff[2] = TRIG_CONV_ARG2;

    HAL_Delay(10);
    HAL_I2C_Master_Transmit(&hi2c1, AHT21B_ADDRESS, tx_buff, SIZE_TRIG_CONV_CMD, HAL_MAX_DELAY);

    WAIT_FOR_CONVERSION:
    tx_buff[0] = CMD_GET_STATUS;
    HAL_Delay(80);
    HAL_I2C_Master_Transmit(&hi2c1, AHT21B_ADDRESS, tx_buff, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, AHT21B_ADDRESS, rx_buff, 1, HAL_MAX_DELAY);
    if ((rx_buff[0] & (1 << (ONE_BYTE-1))) != 0x00) {
        goto WAIT_FOR_CONVERSION;
    }

    memset(rx_buff, 0, SIZE_SESNOR_DATA);
    HAL_I2C_Master_Receive(&hi2c1, AHT21B_ADDRESS, rx_buff, SIZE_SESNOR_DATA, HAL_MAX_DELAY);
    
    raw_humidity  = (uint32_t) rx_buff[1];
    raw_humidity  = (raw_humidity << ONE_BYTE);
    raw_humidity |= (uint32_t) rx_buff[2];
    raw_humidity  = (raw_humidity << ONE_NIBBLE);
    raw_humidity |= (uint32_t) ((UPPER_NIBBLE & rx_buff[3]) >> ONE_NIBBLE);
    *p_raw_humidity = raw_humidity;

    raw_temparature  = (uint32_t) (LOWER_NIBBLE & rx_buff[3]);
    raw_temparature  = (raw_temparature << ONE_BYTE);
    raw_temparature |= (uint32_t) rx_buff[4];
    raw_temparature  = (raw_temparature << ONE_BYTE);
    raw_temparature |= (uint32_t) rx_buff[5];
    *p_raw_temperature = raw_temparature;
}


/**
 * @brief convert raw temparature value to celcius value
*/
static float aht21b_convert_raw_temparature(uint32_t* p_raw_temperature)
{
    float celcius_temp;
    celcius_temp = (float)(*p_raw_temperature) / (float) pow(2, 20);
    celcius_temp = (celcius_temp * 200)  - 50;
    return celcius_temp;
}

/**
 * @brief convert raw humidity value to percentage value
*/
static float aht21b_convert_raw_humidity(uint32_t* p_raw_humidity)
{
    float humidity;
    humidity = (float)(*p_raw_humidity) / (float) pow(2, 20);
    humidity = (humidity * 100);
    return humidity;
}