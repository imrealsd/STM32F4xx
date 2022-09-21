#include "onboard_butto_led.h"
#include <stm32f407xx.h>
#include <stdint.h>

void setup_onboard_led_and_button (void)
{   
    /* Enable clock for gpio A & E  */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    /*configure PA6 as output [led] */
    GPIOA->MODER |= GPIO_MODER_MODE6_0;
    GPIOA->OTYPER &= ~(0b1 << GPIO_OSPEEDR_OSPEED6_Pos);
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1; 

    /*configure PE4 as input + pullup [button k0] */
    GPIOE->MODER &= ~(0b11 << GPIO_MODER_MODE4_Pos);
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED4_1;
    GPIOE->PUPDR |= GPIO_PUPDR_PUPD4_0;

    /* Led off initially */
    GPIOA->BSRR = GPIO_BSRR_BS_6;
} 

int8_t button_is_pressed (void)
{
    int8_t status;

    if (!(GPIOE->IDR & GPIO_IDR_IDR_4))
        status = 1;
    else
        status = 0;
    return status;
}

void turn_on_led (void)
{
    GPIOA->BSRR = GPIO_BSRR_BR_6;
}

void turn_off_led (void)
{
    GPIOA->BSRR = GPIO_BSRR_BS_6;
}