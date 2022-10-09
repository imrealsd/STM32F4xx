#include <stm32f407xx.h>
#include <stdint.h>
#include "exti.h"

void setup_external_interrupt(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;               /* GPIOB clk enable */
    GPIOB->MODER &= ~(0b11 << GPIO_MODER_MODE0_Pos);   /* PB0 input pin */
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD0_1;                /* Pull down */ 
    
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] =  SYSCFG_EXTICR1_EXTI0_PB;       /* PB0 as interrupt source */
    EXTI->IMR  |= EXTI_IMR_MR0;                         /* line 0 interrupt unmasked*/
    EXTI->RTSR |= EXTI_RTSR_TR0;                        /* Rising edge trigger on line 0*/
   
    NVIC_SetPriority(EXTI0_IRQn,0);                     /* Interrupt priority*/
    NVIC_EnableIRQ(EXTI0_IRQn);                         /* Interrupt enable*/
}


void clear_exti_pending_flag(int8_t interrupt_line)
{
    EXTI->PR |= (1 << interrupt_line);
}
