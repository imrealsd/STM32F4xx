#include <stm32f407xx.h>
#include "uart.h"

void init_uart (void)
{   
    /*----- uart1 pin config --------------------------*/

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                 // enable clk for gpio a
    GPIOA->MODER |= GPIO_MODER_MODE9_1;                  // tx
    GPIOA->MODER |= GPIO_MODER_MODE10_1;                 // rx

    GPIOA->AFR[1] |= (0x7 << GPIO_AFRH_AFSEL9_Pos);      // AF7 for rx and tx
    GPIOA->AFR[1] |= (0x7 << GPIO_AFRH_AFSEL10_Pos);

    /*------- uart controller config ------------------------*/

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                 // enable clk for uart1 [ 16 mhz ahb1 clk]
    USART1->CR1 |= USART_CR1_UE;                          // enable uart

    USART1->BRR = (0x68 << USART_BRR_DIV_Mantissa_Pos);   // baud rate 9600
    USART1->BRR |= (0x3 << USART_BRR_DIV_Fraction_Pos);
    USART1->CR1 |= (USART_CR1_TE)|(USART_CR1_RE);         // enable tx and rx

}

void send_uart (char data)
{   
    USART1->DR = data;

    while (!(USART1->SR & USART_SR_TC));                   // wait for TC bit to set [trasmission complete]
    USART1->SR &= ~ (1 << USART_SR_TC_Pos);                // reset TC bit
}


void send_uart_str (char *str)
{
    while (*str != '\0'){
        send_uart(*str);
        str++;
    }
}

int uart_available (void)
{
    if (USART1->SR & USART_SR_RXNE_Msk){
        return 1;
    }
    return 0;
}


char recieve_uart (void)
{
    char val;
    val = USART1->DR;          // RXNE bit is reset after reading usart->DR
    return val;
} 
