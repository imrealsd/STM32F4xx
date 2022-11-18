#include <stm32f407xx.h>
#include <stdint.h>
#include "ADC2.h"


void setup_adc1_single_channel (void)               // PA1 -> ADC123_IN1 [REFER TO DATASHEET PAGE 47 FOR PIN MAPPING]
{   
    /* CONFIG GPIO IN ANALOG MODE */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODE1_Msk;

    /* ADC2 PERPH CLK ENABLE 16Mhz APB2 CLK*/      // [must be done before ADON]
    RCC->APB2ENR |= RCC_APB2ENR_ADC2EN; 

    /* POWER ON ADC */
    ADC2->CR2 |= ADC_CR2_ADON;

    /* ADC2 CONVERSION CLK (ADC_CLK) ENABLE [16/8 = 2 mhz] */
    ADC->CCR |= (0b11 << ADC_CCR_ADCPRE_Pos);      

    /* ADC2 SINGLE CONVERSION CHANNEL CONFIG */    
    ADC2->SQR1 &= ~(ADC_SQR1_L_Msk);                  // one conversion
    ADC2->SQR3 |= (0x1 << ADC_SQR3_SQ1_Pos);          // 1st conversion on channel 1 [GPIA PA01 PIN]

    /* DATA ALLIGNMENT (right) */
    ADC2->CR2 &= ~(1 << ADC_CR2_ALIGN_Pos);
} 

int16_t read_adc1 (void)
{
    /* start conversion */
    ADC2->CR2 |= ADC_CR2_SWSTART;
    /* check conversion complete flag */
    while (!(ADC2->SR & ADC_SR_EOC));    
    /* read & return converted data */
    int16_t data = ADC2->DR;
    /* ADC stops automatically [single conv mode, cont bit = 0] */
    return data;
}


