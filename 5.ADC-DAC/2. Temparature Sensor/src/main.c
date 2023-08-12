#include "sys_clock.h"
#include "delay.h"
#include "ADC1.h"
#include "uart.h"

float convert_to_temp (int16_t);
void send_data_to_pc (float);

int main()
{   
    setup_system_clk();
    setup_adc1_single_channel();
    init_uart();
    int16_t raw_adc;
    float temp;
   
    while (1){
        raw_adc = read_adc1();
        temp = convert_to_temp(raw_adc);
        send_data_to_pc (temp);
        delay_ms(500);
    }
}

float convert_to_temp (int16_t raw_adc)
{
    /* thermistor range -24 to +80 degree C [diff 104]
     * ADC range 0 to 4096 [12 bit] [diff 4096]
    */
    float frac = (float)(104)/(float)(4096);

    /* IF [temp increase -> voltage increase] */
    float temp = -24 + (frac * (float)raw_adc); 

    /* IF [temp increae -> volt decrease] */
    // float temp = 80 - (frac * (float)(4096))

    return temp;
}

void send_data_to_pc (float temp)
{   
    temp = temp * 100;

    /* Before point [3 digits] */
    int8_t d5 = (((int)temp) / 10000) % 10;
    int8_t d4 = (((int)temp) / 1000) % 10;
    int8_t d3 = (((int)temp) / 100) % 10;

    /* After point 2 digits */
    int8_t d2 = ((int)temp / 10) % 10;
    int8_t d1 = (int)temp % 10;

    send_uart_str("-> ");
    send_uart(d5+48);
    send_uart(d4+48);
    send_uart(d3+48);
    send_uart('.');
    send_uart(d2+48);
    send_uart(d1+48);
    send_uart_str(" C\n");
}
