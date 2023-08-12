#include "sys_clock.h"
#include "delay.h"
#include "ADC1.h"
#include "ADC2.h"
#include "uart.h"

void calculate_joystick_pos (int16_t, int16_t);

int main()
{   
    setup_system_clk();
    setup_adc1_single_channel();
    setup_adc2_single_channel();
    init_uart();
    int16_t x_axis, y_axis;
   
    while (1){
        x_axis = read_adc1();
        y_axis = read_adc2();
        calculate_joystick_pos (x_axis, y_axis);
        delay_ms(100);
    }
}

void calculate_joystick_pos (int16_t x, int16_t y)
{
    if (x < 10){
        send_uart_str("LEFT\n");
        
       

    } else if (x > 4090){
        send_uart_str("RIGHT\n");
        
    

    } else if (y < 10){
        send_uart_str("UP\n");
        
    }else if (y > 4090){
        send_uart_str("DOWN\n");
        
    }
}

