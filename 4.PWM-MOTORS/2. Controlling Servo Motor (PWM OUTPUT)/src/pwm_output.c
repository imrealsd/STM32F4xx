#include <stm32f407xx.h>
#include <stdint.h>
#include "pwm_output.h"

void start_pwm_output (float duty_cycle)
{   
    /* PWM gpio config [timer 5 channel 1 (PA0) */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODE0_1;                    // AF mode
    GPIOA->AFR[0] |= (0x2 << GPIO_AFRL_AFSEL0_Pos);        // AF2

    /* Timer 5 config [PWM MODE 1]  */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;                       // 16 Mhz APB1 clk for timer 5 pripheral
    TIM5->CR1 &= ~(TIM_CR1_CEN);                              // counter disable
    TIM5->PSC = 15;                                           // 1 MHz clk to counter [1 count = 1 micro sec]
    TIM5->CNT = 0;
    TIM5->ARR = 20 * 1000;                                    // total pwm period = 20 mili sec , freq = 50hz

    /* duty cycle calculation */
    float frac = duty_cycle / (float)100;
    float temp_CCR1 = (float) (20 * 1000) * frac;
    TIM5->CCR1 = (int32_t)temp_CCR1;                          // compare value acc to given duty cycle
    
    TIM5->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos);             // timer 5 channel 1 configured as pwm mode 1
    TIM5->CCMR1 |= TIM_CCMR1_OC1PE;                           // output compare preload enable
    TIM5->CR1 |= TIM_CR1_ARPE;                                // auto reload preload enable
    TIM5->CCER &= ~(TIM_CCER_CC1P);                           // output active high
    TIM5->CCER |= TIM_CCER_CC1E;                              // OC1 output enable 

    /* initialize registers with the provided values and enable counter */
    TIM5->EGR |= TIM_EGR_UG;
    TIM5->CR1 |= TIM_CR1_CEN;                                
}