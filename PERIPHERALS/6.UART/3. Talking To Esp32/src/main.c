#include "sys_clock.h"
#include "delay.h"
#include "UART1.h"
#include "onboard_button_led.h"
#include <stdint.h>


int main()
{   
    setup_system_clk();
    setup_uart1(9600);
    setup_onboard_led_and_button();
    turn_off_led();
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);
    int8_t count = 0;
   
    while (1){

        if (button_is_pressed()){

            if (count == 0){
                uart1_send_1byte('o');
                count++;
                delay_ms(500);
            } else {
                uart1_send_1byte('f');
                count = 0;
                delay_ms(500);
            }
        }
    }
}

void USART1_IRQHandler(void){

    volatile int8_t ch = uart1_recieve_1byte();
    if (ch == 'o'){
        turn_on_led();
    } else {
        turn_off_led();
    }
}