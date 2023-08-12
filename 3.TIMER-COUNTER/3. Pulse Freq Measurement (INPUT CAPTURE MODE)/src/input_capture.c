#include <stm32f407xx.h>
#include <stdint.h>
#include "input_capture.h"

void setup_pulse_freq (void)
{
    /* pulse input gpio [PC6 , Timer 3 ch 1]  */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODE6_1;                    // AF mode
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD6_1;                    // pull down
    GPIOC->AFR[0] |= (0x2 << GPIO_AFRL_AFSEL6_Pos);        // AF2

    /* Basic timer setting */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;                    // timer 3 peripheral clock enable
    TIM3->CR1 &= ~(TIM_CR1_CEN);                           // timer disable
    TIM3->PSC = 16-1;                                      // timer clock freq = 16/16 =1mhz [internal clock] [1 count 1 micro sec]
    TIM3->CNT = 0;
    TIM3->ARR = 0xFFFFFFFF;

    /* Minimum input freq can be measured is
     *(TIM_CLK_FREQ/ARR) = 1000000/4,294,967,295 = 0.002 hz
    */
    
    /* timer 3 config [input capture mode] */
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;                       // timer 3 channel 1 input , mapped to TI1
    TIM3->CCER &= ~ (TIM_CCER_CC1P | TIM_CCER_CC1NP);      // detect rising edge
    TIM3->CCMR1 &= ~ (TIM_CCMR1_IC1PSC);                   // no prescaler for input capture
    TIM3->CCER |= TIM_CCER_CC1E;                           // enable capture for channel 1 into CCR1 register
    TIM3->DIER |= TIM_DIER_CC1IE;                          // input capture flag enable
    TIM3->CR1 |= TIM_CR1_CEN;                              // counter enable
}

int32_t get_capture_value (void)
{   
    int32_t capture = 0;
    while (!(TIM3->SR & TIM_SR_CC1IF));
    capture = TIM3->CCR1;                                   // counter value @  rising edge 
    TIM3->SR &= ~(1 << TIM_SR_CC1IF_Pos);                   // reset flag
    return capture;
}

float get_pulse_freq (void)
{
    int32_t cap_1 = get_capture_value();
    int32_t cap_2 = get_capture_value();
    int32_t diff = cap_2 - cap_1;
    float period = 0, freq = 0;
   

    if (diff > 0){
        period = (float)diff / (float)1000000;             // in sec
        freq = 1 / period;
    }
    return freq;
}
