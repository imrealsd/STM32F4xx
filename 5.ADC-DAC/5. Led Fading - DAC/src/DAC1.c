#include <stm32f407xx.h>
#include "DAC1.h"

void setup_dac1_ch1 (void)
{
    /* GPIO config PA4 */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODE4_Msk;

    /* DAC1 Channel 1 Config */
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC1->CR |= DAC_CR_EN1; 
    DAC1->CR |= DAC_CR_BOFF1;
}

void write_dac1_ch1 (int16_t data)
{  
    /* 12 bit dac , highest value 4019[output 3.3v] */
    DAC1->DHR12R1 = data;
}