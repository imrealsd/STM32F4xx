/***********************************************************************
* File: bh1750.c
*
* Description: Implementation of bh1750 library functions.
*
* Dependencies: "I2C.h" & "stm32f4xx_hal.h" provided  by STMicroelectronics
*
* Author: Subhadeep Dhang
*
* Date: August 25, 2023
************************************************************************/


/*Includes*/
#include "stm32f4xx_hal.h"
#include "bh1750.h"
#include "i2c.h"

/*Private Macros*/

#define RX_BUFF_SIZE    2
#define TX_BUFF_SIZE    1
#define ONE_BYTE        8
 
/*Private function prototypes*/


/*Function Definitations*/

void bh1750_init(uint8_t measurement_mode)
{
    bh1750_write_command(CMD_PWR_DOWN);
    bh1750_write_command(CMD_PWR_ON);
    
    /**
     * If using continious mode , no need to send measurement_mode command 
     * repeatedly, send it one time (by init func), then it's ready to be read, 
     * data will be updated automatically in the bh1750 data register.
     * 
     * If using one time mode, each time measurement_mode command should
     * be sent, then wait for the measurement to be done ,
     * then data is ready to be read
     */
    bh1750_write_command(measurement_mode);

    /*Delay 120 ms (max) for the conversion to be done*/
    HAL_Delay(120);
}


float bh1750_get_illuminance(void)
{
    uint8_t rx_buff[RX_BUFF_SIZE] = {0, 0};
    uint16_t raw_value = 0;
    float lux_value;

    /**
     * BH1750 sends data in 2 bytes- high byte & low byte
     * Typical Frame: [ST + {I2C_addr + R} + ACK + {High Byte} + ACK + {Low Byte} + NACK + SP]
    */
    HAL_I2C_Master_Receive(&hi2c1, BH1750_ADDR_L, rx_buff, RX_BUFF_SIZE, HAL_MAX_DELAY);

    /*calculate lux value from raw sensor value*/
    raw_value |= (uint16_t) rx_buff[1] & 0xFF;
    raw_value |= (uint16_t) (rx_buff[0] << ONE_BYTE);
    lux_value = (float) raw_value / 1.2;

    return lux_value;
}


void bh1750_write_command(uint8_t cmd)
{
    /**
     * BH1750 is not able to accept plural command.
     * I2C transmit data frame should be limited to one command.
     * Typical Frame: [ ST + {I2C_ADDR + W} + ACK + {Command} + ACK + SP ]
    */
    HAL_I2C_Master_Transmit(&hi2c1, BH1750_ADDR_L, (uint8_t*) &cmd, TX_BUFF_SIZE, HAL_MAX_DELAY);
}

