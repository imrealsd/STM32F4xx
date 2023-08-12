#include <stm32f407xx.h>
#include <stdint.h>
#include "SPI1.h"
#define CLK 16000000

void setup_SPI1(void)
{   
    /* Setup peripheral clk for GPIO & SPI */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    /* Setup SPI Pins : Alternative Mode for PA5[sck], PA6[miso], PA7[mosi] */
    GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
    GPIOA->AFR[0] |= (0x5 << GPIO_AFRL_AFSEL5_Pos) | (0x5 << GPIO_AFRL_AFSEL6_Pos)|(0x5 << GPIO_AFRL_AFSEL7_Pos);
    /* High Speed GPIO */
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1 | GPIO_OSPEEDR_OSPEED7_1;
    /* Pull down for SCK [idle state logic = 0] */
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_1;
    /* GPIO setup for chip select pin PA0: output, highspeed*/
    GPIOA->MODER |= GPIO_MODER_MODE0_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1;

    /*Setup SPI controller:
     * SPI CLK = 16/16 = 1 MHZ
     * CPOL = 0, CPHASE = 0
     * 16 bit data frame
     * Bi-directional transmit only [half duplex] BIDIMODE = 0, BIDIOE = 1
     * software NSS
     * Master Mode 
    */
    SPI1->CR1 |= (0b011 << SPI_CR1_BR_Pos);
    SPI1->CR1 |= (0b1 << SPI_CR1_DFF_Pos);
    SPI1->CR1 |= (SPI_CR1_BIDIMODE) | (SPI_CR1_BIDIOE);
    SPI1->CR1 |= (SPI_CR1_SSM) | (SPI_CR1_SSI);
    SPI1->CR1 |= (SPI_CR1_MSTR);
}

void cs_Enable(void)
{
    GPIOA->BSRR = GPIO_BSRR_BR0;
} 

void cs_Disable(void)
{
    GPIOA->BSRR = GPIO_BSRR_BS0;
}

void send_SPI1_16bit(uint16_t data)
{   
    SPI1->CR1 |= SPI_CR1_SPE;
    cs_Enable();
    /* wait for TX buffer to empty [TXE to set]*/
    while (!(SPI1->SR & SPI_SR_TXE));
    /* write data to buffer */
    SPI1->DR = data;
    /* wait for transmission complete */
    while (!(SPI1->SR & SPI_SR_TXE));
    while (SPI1->SR & SPI_SR_BSY);
    cs_Disable();
    SPI1->CR1 &= ~(SPI_CR1_SPE);
}
