#include <stm32f407xx.h>
#include <stdint.h>
#include "delay.h"
#include "stepper.h"

void setup_stepper_motor (void)
{
    /* GPIO A CLK ENABLE */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA0 TO PA3 HIGH SPEED OUTPUT */
    GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0;
    GPIOA->MODER |= GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0;

    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_1 | GPIO_OSPEEDR_OSPEED1_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_1 | GPIO_OSPEEDR_OSPEED3_1;
}

void stepper_wave_drive (void)               // min torque , min power consuption
{
    for (int8_t i = 0; i < 4; i++){
        GPIOA->ODR = (1 << i);
        delay_ms(2);
    }
}

void stepper_full_drive (void)              // max torque , max power consuption
{
    for (int8_t i = 0; i < 3; i++){

        GPIOA->ODR = (0b11 << i);
        delay_ms(10);
    }
}

void stepper_half_drive (void)
{
    GPIOA->ODR = 0x1;
    delay_ms(2);
    GPIOA->ODR = 0x3;
    delay_ms(2);
    GPIOA->ODR = 0x4;
    delay_ms(2);
    GPIOA->ODR = 0xC;
    delay_ms(2);
}