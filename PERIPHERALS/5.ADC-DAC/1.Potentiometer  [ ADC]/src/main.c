#include "sys_clock.h"
#include "delay.h"
#include "ADC1.h"
#include "uart.h"

float convert_to_voltage (int16_t);
void send_data_to_pc (float);

int main()
{   
    int16_t raw_adc;
    float volt;
    setup_system_clk();
    setup_adc1_single_channel();
    init_uart();
   
    while (1){
        raw_adc = read_adc1();
        volt = convert_to_voltage(raw_adc);
        send_data_to_pc (volt);
        delay_ms(500);
    }
}

float convert_to_voltage (int16_t raw_adc)
{
    float volt;    // max  = 3.3 , min 0
    float frac = (3.3 / (float)4096);
    volt = raw_adc * frac;
    return volt;
}

void send_data_to_pc (float volt)
{
    float temp = volt * 10;
    int8_t d1 = (int)temp / 10;
    int8_t d2 = (int)temp % 10;
    send_uart_str("-> ");
    send_uart(d1+48);
    send_uart('.');
    send_uart(d2+48);
    send_uart_str(" volt\n");
}

