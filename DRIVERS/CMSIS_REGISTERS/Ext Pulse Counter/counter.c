#include <stm32f407xx.h>
#include <stdint.h>
#include "counter.h"

void setup_risingEdge_counter (void)
{
    /* GPIO CLK ENABLE TI2-Timer2 ch_2 [ PA1 ]*/
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER  |= GPIO_MODER_MODE1_1;
    GPIOA->AFR[0] |= (0b0001 << GPIO_AFRL_AFSEL1_Pos);                           // Alternate function 

    /* Timer 2 config for Ext input clk mode 1 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;                                          // timer 2 peripheral clock enable
    TIM2->CCMR1  |= TIM_CCMR1_CC2S_0;                                            // channel 2 as input [mapped to TI2]
    TIM2->CCMR1  &= ~(0b1111 << TIM_CCMR1_IC2F_Pos);                             // No filter
    TIM2->CCER   &= ~ ((1 << TIM_CCER_CC2P_Pos) | (1 << TIM_CCER_CC2NP_Pos));    // polarity [rising edge]
    TIM2->SMCR   |= (0b111 << TIM_SMCR_SMS_Pos);                                 // Ext clock mode 1
    TIM2->SMCR   |= (0b110 << TIM_SMCR_TS_Pos);                                  // TI2 as input clk source
    TIM2->CNT = 0;
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->CR1    |= TIM_CR1_CEN;                                                 //  counter enable
    TIM2->DIER |= TIM_DIER_TIE;
}

int32_t get_counter_value (void)
{   
    if (TIM2->SR & TIM_SR_TIF){
        TIM2->SR &= ~(1 << TIM_SR_TIF_Pos);
    }
    int32_t val = TIM2->CNT;
    return val;
}
