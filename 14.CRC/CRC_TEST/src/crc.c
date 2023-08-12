#include <stm32f407xx.h>
#include <stdint.h>
#include "crc.h"

void setup_CRC(void)
{   
    /* CRC peripheral clk enable*/
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
}

void write_CRC_input(uint32_t data)
{
    CRC->DR = data;
}

uint32_t read_CRC_output(void)
{   
    uint16_t data = CRC->DR;
    return data;
}

