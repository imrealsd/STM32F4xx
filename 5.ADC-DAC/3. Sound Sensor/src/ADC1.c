#include <stm32f407xx.h>
#include <stdint.h>
#include "ADC1.h"


void setup_adc1_single_channel (void)        // PA3 -> ADC123_IN3 [REFER TO DATASHEET PAGE 49 FOR PIN MAPPING]
{   
    /* CONFIG GPIO IN ANALOG MODE */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODE3_Msk;

    /* ADC1 PERPH CLK ENABLE 16Mhz APB2 CLK*/      // [must be done before ADON]
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 

    /* POWER ON ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* ADC1 CONVERSION CLK (ADC_CLK) ENABLE [16/8 = 2 mhz] */
    ADC->CCR |= (0b11 << ADC_CCR_ADCPRE_Pos);      

    /* ADC1 SINGLE CONVERSION CHANNEL CONFIG */    
    ADC1->SQR1 &= ~(ADC_SQR1_L_Msk);                  // one conversion
    ADC1->SQR3 |= (0x3 << ADC_SQR3_SQ1_Pos);          // 1st conversion on channel 3 [GPIAD PA03 PIN]

    /* DATA ALLIGNMENT (right) */
    ADC1->CR2 &= ~(1 << ADC_CR2_ALIGN_Pos);
} 

int16_t read_adc1 (void)
{
    /* start conversion */
    ADC1->CR2 |= ADC_CR2_SWSTART;
    /* check conversion complete flag */
    while (!(ADC1->SR & ADC_SR_EOC));    
    /* read & return converted data */
    int16_t data = ADC1->DR;
    /* ADC stops automatically [single conv mode, cont bit = 0] */
    return data;
}

void setup_ad1_dual_channel (void){           // PA1 -> ADC123_IN1 , PA2 -> ADC123_IN2

    /* CONFIG GPIO IN ANALOG MODE */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODE1_Msk | GPIO_MODER_MODE2_Msk;

    /* ADC1 PERPH CLK ENABLE 16Mhz APB2 CLK*/      // [must be done before ADON]
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 

    /* POWER ON ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* ADC1 CONVERSION CLK (ADC_CLK) ENABLE [16/8 = 2 mhz] */
    ADC->CCR |= (0b11 << ADC_CCR_ADCPRE_Pos);      

    /* ADC1 SINGLE CONVERSION CHANNEL CONFIG */    
    ADC1->SQR1  = 1;                                  // two conversion
    ADC1->SQR3 |= (0x1 << ADC_SQR3_SQ1_Pos);          // 1st conversion on channel 1 [GPOIA PA01 PIN]
    ADC1->SQR3 |= (0x1 << ADC_SQR3_SQ1_Pos);          // 2nd conversion on channel 2 [GPIOA PA02 PIN]

    /* DATA ALLIGNMENT (right) */
    ADC1->CR2 &= ~(1 << ADC_CR2_ALIGN_Pos);
}
