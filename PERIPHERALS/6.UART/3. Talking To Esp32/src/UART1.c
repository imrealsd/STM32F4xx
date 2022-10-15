#include <stm32f407xx.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "UART1.h"
#define CLK 16000000



void setup_uart1 (int32_t baud_rate)
{   
    /* UART1 GPIO CONFIG */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                 // enable clk for gpio a
    GPIOA->MODER |= GPIO_MODER_MODE9_1;                  // tx
    GPIOA->MODER |= GPIO_MODER_MODE10_1;                 // rx

    GPIOA->AFR[1] |= (0x7 << GPIO_AFRH_AFSEL9_Pos);      // AF7 for rx and tx
    GPIOA->AFR[1] |= (0x7 << GPIO_AFRH_AFSEL10_Pos);

    /* BASIC USART CONFIG */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 &= ~(USART_CR1_M);
    USART1->CR2 &= ~(USART_CR2_STOP);

    /*UART1 Baud RATE CONFIG */
    int16_t mantissa;
    float userdiv;
    float temp;
    int8_t frac;
    userdiv = (float)CLK / ((float)(16)*(float)baud_rate);
    mantissa = (int16_t)userdiv;
    temp = userdiv - (float)mantissa;
    temp = (temp)*(float)16;
    frac =  nearest_int(temp);

    USART1->BRR |= (mantissa << USART_BRR_DIV_Mantissa_Pos);
    USART1->BRR |= (frac << USART_BRR_DIV_Fraction_Pos);

    /* USART TX & RX CONFIG */
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_RXNEIE;            // receive complete interrupt enable
}

void uart1_send_1byte (int8_t data)
{
    while (!(USART1->SR & USART_SR_TXE));      // wait for TDR buffer empty
    USART1->DR = data;
    while (!(USART1->SR & USART_SR_TC));       // wait for TC bit to set [trasmission complete]
    USART1->SR &= ~ (1 << USART_SR_TC_Pos); 
}

int8_t uart1_data_avaiilable (void)
{
    if (USART1->SR & USART_SR_RXNE)
        return 1;
    else 
        return 0;
}

int8_t uart1_recieve_1byte (void)
{
    int8_t data = USART1->DR;
    return data;
}

void uart1_send_string (char *str)
{
    while (*str != 0){
        uart1_send_1byte(*str);
        str++;
    }
}

int8_t nearest_int (float val)
{
    float ceil_val = ceil(val);
    float floor_val = floor(val);
    float temp1 = ceil_val - val;
    float temp2 = val - floor_val;

    if (temp1 > temp2)
        return floor_val;
    else
        return ceil_val;
}

