#include <stm32f407xx.h>
#include <stdint.h>
#include "Motor.h"

void setup_motor_driver_gpio (void)
{
    /* Enable port A clk */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA0, PA1 highspeed output */
    GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1 | GPIO_OSPEEDR_OSPEED1_1;

    /* Make 2 pins low */
    GPIOA->ODR &= ~(0x0002);
}

void rotate_motor (int8_t direction)
{
    if (direction == CLOCKWISE){
        GPIOA->ODR |=  (0x1 << GPIO_ODR_OD0_Pos);
        GPIOA->ODR &= ~(0x1 << GPIO_ODR_OD1_Pos);

    } else if (direction == ANTI_CLOCKWISE){
        GPIOA->ODR &= ~(0x1 << GPIO_ODR_OD0_Pos);
        GPIOA->ODR |=  (0x1 << GPIO_ODR_OD1_Pos);
    
    } else {
        GPIOA->ODR &= ~(0x1 << GPIO_ODR_OD0_Pos);
        GPIOA->ODR &= ~(0x1 << GPIO_ODR_OD1_Pos);
    }
}