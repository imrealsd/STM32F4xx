#include "sys_clock.h"
#include "delay.h"
#include "ADC1.h"
#include "uart.h"

void send_data_to_pc (int16_t);

int main()
{   
    setup_system_clk();
    setup_adc1_single_channel();
    init_uart();
    int16_t raw_adc;
   
    while (1){
        raw_adc = read_adc1();
        send_data_to_pc (raw_adc);
        delay_ms(500);
    }
}


void send_data_to_pc (int16_t adc)
{   

    int8_t d4 = (((int)adc) / 1000) % 10;
    int8_t d3 = (((int)adc) / 100) % 10;
    int8_t d2 = ((int)adc / 10) % 10;
    int8_t d1 = (int)adc % 10;

    send_uart_str("-> ");
    send_uart(d4+48);
    send_uart(d3+48);
    send_uart(d2+48);
    send_uart(d1+48);
    send_uart('\n');
}
