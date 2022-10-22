#include "sys_clock.h"
#include "UART1.h"
#include "exti.h"
#include "delay.h"
#define EXTI_LINE0 0

int main(void)
{
    setup_system_clk();
    setup_uart1(9600);
    setup_external_interrupt();
    while(1);
}

/* switch is connected to PB0
 *
 * Hardware Debouncing Prevention:
 * Adding a capacitor parallel to input to prevent debouncing    
 *
 * +5v----------- switch -----------> MCU INPUT PIN [pulled down]
 *                         |
 *                     [2.2 uF]
 *                         |
 *                        GND 
 * -----------------------------------------------------------------
 * 
 *  Software Debouncing Prevention: [ not reliable use along with hardware debouncing prevention]
 * 
 *  disable interrupt at the start of ISR
 *  enable again at the end of ISR 
 * ------------------------------------------------------------------
*/

void EXTI0_IRQHandler(void)
{   
    NVIC_DisableIRQ(EXTI0_IRQn);
    clear_exti_pending_flag(EXTI_LINE0);
    uart1_send_string("Button Pressed !!\n");
    NVIC_EnableIRQ(EXTI0_IRQn);
}