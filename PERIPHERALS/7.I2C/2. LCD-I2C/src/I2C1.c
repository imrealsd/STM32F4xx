#include <stm32f407xx.h>
#include <stdint.h>
#include "I2C1.h"

void setup_I2C1_SM (void)
{
    /* scl & sda gpio config PB8 -> SCL, PB9->SDA 
     * open drain , high speed and  pulled up 
    */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;
    GPIOB->AFR[1] |= (0x4 << GPIO_AFRH_AFSEL8_Pos) | (0x4 << GPIO_AFRH_AFSEL9_Pos);
    GPIOB->OTYPER |= GPIO_OTYPER_OT9_Msk | GPIO_OTYPER_OT8_Msk;  
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_Msk;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0; 

    /* I2C1 CLK EN */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    /* 16 MHZ peripheral freq [16 Mhz apb1 freq */
    I2C1->CR2 |= (16 << I2C_CR2_FREQ_Pos);  

    /* FPCLK = 16MHZ , TPCLK = 1/16000000 sec = 62.5 ns
     * For 100Khz T-high + T-low = 1/100000 = 100000 ns 
     * T-high = T-low = 5000 ns , acc to formulae - 
     * T-high = T-low = CCR * TPCLK , CCR = 5000 ns / 62.5 ns = 80
    */
    I2C1->CCR |= (80 << I2C_CCR_CCR_Pos);

    /* T-rise config [max feedback loop time] 
     * T-rise = (1000/62.5)+1 = 17
    */
    I2C1->TRISE |= (17 << I2C_TRISE_TRISE_Pos);
    /* I2C peripheral enable */
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_start (void)
{
    I2C1->CR1 |= I2C_CR1_START;
    /* wait for start bit to send */
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C1_write_addr (int8_t addr, char* operation)
{   
    /* operation READ / WRITE */
    if (operation[0] == 'W')
        addr = addr << 1;
    else if (operation[0] == 'R')
        addr = (addr << 1) | 0b1;
    /* sending address + R/W bit */
    I2C1->DR = addr;

    /* wait for address to be  matched and get ack*/
    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    /* wait for the total address phase to end */
    if (operation[0] == 'R')
        while (I2C1->SR2 & I2C_SR2_TRA);
    else if (operation[0] == 'W')
        while (!(I2C1->SR2 & I2C_SR2_TRA));
}

void I2C1_write_data (int8_t data)
{   
    /* wait for data reg to empty */
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    /* clear BTF flag */
    int8_t dummy_read =  I2C1->DR;

    /* write data */
    I2C1->DR = data;
}

void I2C1_stop (void)
{   
    /* wait for the last bit to transmit */
    while (!(I2C1->SR2 & I2C_SR1_BTF));
    /* send stop condition */
    I2C1->CR1 |= I2C_CR1_STOP;
}

// int8_t I2C1_read_data (void)
// {
    
// }
