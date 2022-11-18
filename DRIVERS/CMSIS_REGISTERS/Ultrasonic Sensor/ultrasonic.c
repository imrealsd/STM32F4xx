#include <stm32f407xx.h>
#include <stdint.h>
#include "pwm_input.h"
#include "delay.h"
#define SOUND_SPEED 34000                   // cm / s

void setup_ultrasonic (void)
{
    setup_pwm_input();
    /* PB7 TRIGGER PIN, PB6 INPUT PWM ECHO PIN */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE7_0;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_1;
    GPIOB->BSRR = GPIO_BSRR_BR7;
}

float get_distance (void)
{
    /* 10 us pulse to trigger pin */
    GPIOB->BSRR = GPIO_BSRR_BS7;
    delay_us(10);
    GPIOB->BSRR = GPIO_BSRR_BR7;
    /* check echo pin for getting distance, measuring 5 times & taking mean*/
    float distance = get_pulse_on_time() * (float)(SOUND_SPEED);
    distance = distance / 2;
    /* delay atleast 60 ms before next trigger */
    delay_ms(60);
    return distance;
}