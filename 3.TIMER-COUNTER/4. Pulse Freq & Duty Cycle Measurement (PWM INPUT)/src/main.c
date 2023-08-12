#include "sys_clock.h"
#include "pwm_input.h"
#include "uart.h"
#include "delay.h"
#include <stdint.h>


void send_info_to_pc (char *, char *);

int main()
{
    setup_system_clk();
    setup_pulse_freq_duty_cycle();
    init_uart();
    delay_ms(1000);

    char duty_cycle[3];
    char freq[3];
    int32_t temp1, temp2;

    while (1){
        
        temp1 = get_pulse_freq();
        temp2 = get_pulse_duty_cycle();

        if (temp1 >= 0 && temp2 >= 0){

            freq[1] = (temp1 % 10) + 48;
            freq[0] = ((temp1 / 10)%10) + 48;

            duty_cycle[1] = (temp2 % 10) + 48;
            duty_cycle[0] = ((temp2 / 10)%10) + 48;
        }

        send_info_to_pc(freq, duty_cycle);
        delay_ms(2000);
    }
}

void send_info_to_pc (char freq[], char duty_cycle[])
{   
    send_uart_str("---------------------\n");
    send_uart_str("Frequency :");
    send_uart_str(freq);
    send_uart_str("Hz\n");

    send_uart_str("Duty Cycle:");
    send_uart_str(duty_cycle);
    send_uart_str("%\n");
    send_uart_str("---------------------\n");
}