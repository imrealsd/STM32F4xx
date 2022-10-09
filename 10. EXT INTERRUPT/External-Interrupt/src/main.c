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

void EXTI0_IRQHandler(void)
{   
    clear_exti_pending_flag(EXTI_LINE0);
    uart1_send_string("Button Pressed !!\n");
}