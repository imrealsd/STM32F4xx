/***********************************************************************
* File: aht21b.h
*
* Description: aht21b library functions prototypes & macros.
*
* Author: Subhadeep Dhang
*
* Date: August 12, 2023
************************************************************************/

#ifndef AHT21B_H
#define AHT21B_h

/*----------Driver Defines-----------------*/

#define AHT21B_ADDRESS      (0x38 << 1)
#define CMD_GET_STATUS      0x71
#define CMD_TRIG_CONV       0xAC
#define TRIG_CONV_ARG1      0x33
#define TRIG_CONV_ARG2      0x00
#define STATUS_OK           0x18
#define SIZE_TRIG_CONV_CMD  3
#define SIZE_SESNOR_DATA    6
#define ONE_BYTE            8
#define ONE_NIBBLE          4
#define UPPER_NIBBLE        0xF0
#define LOWER_NIBBLE        0x0F


/*--------- Driver APIs----------------*/

/**
 * @brief  check initialization status of AHT21B
 * @param  None
 * @retval None
 */ 
void aht21b_setup(void);

/**
 * @brief  get humidity value AHT21B
 * @param  None
 * @retval float
 */ 
float aht21b_get_humidity(void);

/**
 * @brief  get temparature value from AHT21B
 * @param  None
 * @retval float
 */ 
float aht21b_get_temperature(void);

#endif