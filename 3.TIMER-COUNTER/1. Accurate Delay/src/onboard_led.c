#include <stm32f407xx.h>
#include "onboard_led.h"

void setup_onboard_led (void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;             // enable clock for GPIO PORT A
    GPIOA->MODER |= GPIO_MODER_MODE6_0;              // configure pin PA6 as output    [ led cathode connected to PA6]
    GPIOA->OTYPER &= ~(1 << GPIO_OTYPER_OT6_Pos);    // push-pull type
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;      // High Speed
}

void onboard_led_on (void)
{
    GPIOA->BSRR = GPIO_BSRR_BR_6;              // onboard led on
}

void onboard_led_off (void)
{
    GPIOA->BSRR = GPIO_BSRR_BS_6;              // onboard led off
}