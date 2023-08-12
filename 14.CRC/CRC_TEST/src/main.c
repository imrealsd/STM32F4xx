#include "sys_clock.h"
#include "UART1.h"
#include "crc.h"
#include "delay.h"
#define ARR_SIZE 11  // max unsigned 32 bit val [10digit] + nul character [for terminating array read]

void send_to_serial_monitor(uint32_t);
void reset_array(char []);

int main(void)
{
    setup_system_clk();
    setup_uart1(9600);
    setup_CRC();

    while (1){
        write_CRC_input(0x88FF6611);   // write any 32bit value
        delay_ms(2);
        uint32_t crc_output = read_CRC_output();
        send_to_serial_monitor(crc_output);
        delay_ms(2000);
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
    uart1_send_string("CRC OUTPUT: ");
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