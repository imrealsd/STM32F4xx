#include <stm32f407xx.h>
#include <stdint.h>
#include "pwm_input.h"



void setup_pwm_input (void)
{
    /* pulse input gpio [PB6 , Timer 4 ch 1]  */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE6_1;                    // AF mode
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_1;                    // pull down
    GPIOB->AFR[0] |= (0x2 << GPIO_AFRL_AFSEL6_Pos);        // AF2

    /* Basic timer setting */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;                    // timer 4 peripheral clock enable
    TIM4->CR1 &= ~(TIM_CR1_CEN);                           // timer disable
    TIM4->PSC = 16-1;                                      // timer clock freq = 16/16 =1mhz [internal clock] [1 count 1 micro sec]
    TIM4->CNT = 0;
    TIM4->ARR = 0xFFFFFFFF;

    /* Minimum input freq can be measured is
     *(TIM_CLK_FREQ/ARR) = 1000000/4,294,967,295 = 0.002 hz
    */
    
    /* timer 4 config [pwm input mode] */                  // CH1 detect rising edge , Ch2 detect faling edge
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;                       // timer 4 channel 1 input , mapped to TI1
    TIM4->CCER &= ~ (TIM_CCER_CC1P | TIM_CCER_CC1NP);      // detect rising edge
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;                       // timer 4 channel 2 input , mappped to TI1
    TIM4->CCER |= TIM_CCER_CC2P;                           // detect faling edge
    TIM4->CCER &= ~(TIM_CCER_CC2NP);
    TIM4->SMCR |= (0b101 << TIM_SMCR_TS_Pos);              // trigger input from TI1
    TIM4->SMCR |= (0b100 << TIM_SMCR_SMS_Pos);             // slave mode controller in reset mode [counter reset on rising edge]

    TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;           // enable capture for channel 1 into CCR1 register & chnnel 2 into CCR2 register
    TIM4->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC2IE;         // input capture flag enable
    TIM4->CR1  |= TIM_CR1_CEN;                             // counter enable
}

/* here total period is the value captured  @ CCR1 x 10^-6 sec */

float get_pulse_freq (void)
{
    int32_t cap1;
    float period, freq;

    while (!(TIM4->SR & TIM_SR_CC1IF));                      // rising edge [then counter reset ]
    cap1 = TIM4->CCR1;
    TIM4->SR &= ~(TIM_SR_CC1IF);


    period = (float)(cap1 - 0) / (float)1000000;              // in sec
    freq = 1 / period;                                        // in Hz
    return freq;
}


float get_pulse_duty_cycle (void)
{
    int32_t cap1, cap2;
    float duty_cycle;

    while (!(TIM4->SR & TIM_SR_CC1IF));                         // rising edge then counter reset
    cap1 = TIM4->CCR1;
    TIM4->SR &= ~(TIM_SR_CC1IF);

    while (!(TIM4->SR & TIM_SR_CC2IF));                         // failing edge
    cap2 = TIM4->CCR2;
    TIM4->SR &= ~(TIM_SR_CC2IF);

   duty_cycle = (float)(cap2) / (float)(cap1);
   duty_cycle = duty_cycle * 100;
   return duty_cycle;
}

float get_pulse_on_time (void)
{
    int32_t cap1, cap2;
    float onTime;

    while (!(TIM4->SR & TIM_SR_CC1IF));                         // rising edge then counter reset
    cap1 = TIM4->CCR1;
    TIM4->SR &= ~(TIM_SR_CC1IF);

    while (!(TIM4->SR & TIM_SR_CC2IF));                         // failing edge
    cap2 = TIM4->CCR2;
    TIM4->SR &= ~(TIM_SR_CC2IF);

    onTime = (float)(cap2)/(float)(1000000);                    // sec
    return onTime;
}