#include <stm32f407xx.h>
#include <stdint.h>
#include "sys_clock.h"
#include "UART1.h"
#define STR_LEN 5

char str[10];
int8_t count = 0;

int main()
{
    setup_system_clk();
    setup_uart1(9600);
    NVIC_SetPriority(USART1_IRQn,0);
    NVIC_EnableIRQ(USART1_IRQn);

    while (1){
        
        if (count > (STR_LEN-1)){                // terminate by size or by receiving special character (inside isr)
            uart1_send_string(str);
            uart1_send_1byte('\n');
            count = 0;
        }
    }    
}
 
void USART1_IRQHandler(void){
    
    str[count] = uart1_recieve_1byte();
    count++;
}
