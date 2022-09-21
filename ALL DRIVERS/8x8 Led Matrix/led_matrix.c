#include <stm32f407xx.h>
#include <stdint.h>
#include "delay.h"
#include "led_matrix.h"


/* 8X8 LED MATRIX: 
 * ROW 1 TO 8 [GND] -> CONNECTED TO PD0 TO PD7
 * COL 1 TO 8 [VCC] -> CONNECTED TO PA7 TO PA0   [ reverse order]
*/

void setup_led_matrix_gpio (void)
{
    /* setup PA and PD clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;

    /* PA0 to PA7 & PD0 to PD7 highspeed output */
    GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0;
    GPIOA->MODER |= GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0;
    GPIOA->MODER |= GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1 | GPIO_OSPEEDER_OSPEEDR1_1 | GPIO_OSPEEDER_OSPEEDR2_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1 | GPIO_OSPEEDER_OSPEEDR4_1 | GPIO_OSPEEDER_OSPEEDR5_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1;

    GPIOD->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0;
    GPIOD->MODER |= GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0;
    GPIOD->MODER |= GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1 | GPIO_OSPEEDER_OSPEEDR1_1 | GPIO_OSPEEDER_OSPEEDR2_1;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1 | GPIO_OSPEEDER_OSPEEDR4_1 | GPIO_OSPEEDER_OSPEEDR5_1;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1;

    /* turn off all led [rows->high] & [cols->low]  */
    GPIOA->ODR &= 0xFF00;
    GPIOD->ODR |= 0x00FF;
}


void show_static_letter (char ch)                                  // function must be in a loop in main code
{   
    // letter[0] -> A, lette[1]->B... 
    char letters[26][8] = {{0x3C, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42},            
                           {0xF8, 0x84, 0x84, 0xF8, 0xF8, 0x84, 0x84, 0xF8},
                           {0x3C, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x3C}, 
                           {0xF8, 0x84, 0x82, 0x82, 0x82, 0x82, 0x84, 0xF8}};  

    int8_t index = ch - 65;                                      // [capital letter [A->65 ascii]]  

    for (int8_t row = 0; row < 8; row++){
        GPIOD->ODR &= ~(1 << row);                               // turn on a single row
        GPIOA->ODR = (0xFFFF & letters[index][row]);             // write col value
        delay_ms(1);
        GPIOD->ODR |= 0x00FF;                                    // turn off all rows
    }
}
