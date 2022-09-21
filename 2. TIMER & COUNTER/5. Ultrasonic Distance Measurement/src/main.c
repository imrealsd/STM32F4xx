#include <stm32f407xx.h>
#include <stdint.h>
#include "sys_clock.h"
#include "pwm_input.h"
#include "uart.h"
#include "delay.h"
#include "ultrasonic.h"


void send_data_to_pc (float);

int main()
{   
    float distance = 0;
    setup_system_clk();
    setup_ultrasonic();
    init_uart();

    while (1){
        distance = get_distance();
        send_data_to_pc(distance);  
        delay_ms(200);
    }
}


void send_data_to_pc (float distance)
{   
    char data[6];
    int32_t temp;
    distance = 100 * distance;
    temp = (int)distance;

    data[5] = 0;                        // null char
    data[4] = (temp % 10) + 48;
    data[3] = ((temp / 10) % 10) + 48;
    data[2] = '.';
    data[1] = ((temp / 100) % 10) + 48;
    data[0] = ((temp / 1000) % 10) + 48;

    send_uart_str(data);
    send_uart_str(" cm\n");
}