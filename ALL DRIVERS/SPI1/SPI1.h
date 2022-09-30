#include <stm32f407xx.h>
#include <stdint.h>
#ifndef SPI1_H
#define SPI1_H
void setup_SPI1(void);
void cs_Enable(void);
void cs_Disable(void);
void send_SPI1_16bit(int16_t);
#endif 