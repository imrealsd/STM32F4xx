#include "ir_sensor.h"
#include <stm32f407xx.h>
#include <stdint.h>

void setup_ir_sensor (void)
{
    /* eanble gpio A clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* configure PA0 as input + highspeed */
    GPIOA->MODER &= ~(0b11 << GPIO_MODER_MODE0_Pos);
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1;

    /*configure PA6 as output [led] */
    GPIOA->MODER |= GPIO_MODER_MODE6_0;
    GPIOA->OTYPER &= ~(0b1 << GPIO_OTYPER_OT6_Pos); 
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;

    /*initially turn off led */
    GPIOA->BSRR = GPIO_BSRR_BS6;
}

int8_t object_is_detected (void)
{
    int8_t status;
    
    if (!(GPIOA->IDR & GPIO_IDR_ID0))
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