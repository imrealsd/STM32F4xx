#include "sys_clock.h"
#include "delay.h"
#include "UART1.h"
#include "onboard_led.h"
#include <string.h>

char str[10];
int8_t count = 0;

int main()
{
    setup_system_clk();
    setup_onboard_led();
    setup_uart1(9600);
    onboard_led_off();

    while (1){
        
        while (!uart1_data_avaiilable());
        char ch = uart1_recieve_1byte();

        if (ch == 'o' || ch == 'O'){
            uart1_send_string("Turning on LED\n");
            onboard_led_on();
        } else {
            uart1_send_string("Turning off LED\n");
            onboard_led_off();
        }
    }    
}
 

