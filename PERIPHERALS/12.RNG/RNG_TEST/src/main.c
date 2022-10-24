#include <stdint.h>
#include "RNG.h"
#include "delay.h"
#include "sys_clock.h"
#include "UART1.h"
#define ARR_SIZE 11
void reset_array(char arr[]);
void send_to_serial_monitor(uint32_t data);

int main(void)
{   
    uint32_t rand_num;
    setup_system_clk();
    setup_uart1(9600);
    setup_RNG();

    while (1){
        rand_num = get_Random_Number();
        send_to_serial_monitor(rand_num);
        delay_ms(1000);
    }
}

void send_to_serial_monitor(uint32_t data)
{
    char str[ARR_SIZE];
    uint8_t index = 0;
    reset_array(str);

    while (data){
        uint8_t temp = data % 10;
        str[ARR_SIZE-2-index] = temp + 48;
        index++;
        data = data / 10; 
    }
    uart1_send_string("RNG OUTPUT: ");
    uart1_send_string(str);
    uart1_send_1byte('\n');
}

void reset_array(char arr[])
{   
    arr[ARR_SIZE-1] = 0;
    for (uint8_t i = 0; i < ARR_SIZE-1; i++){
        arr[i] = '0';
    }
}