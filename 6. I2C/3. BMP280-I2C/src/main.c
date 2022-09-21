#include "sys_clock.h"
#include "delay.h"
#include "I2C1.h"
#include "UART1.h"
#include "bpm280_I2C.h"
#include <string.h>

void int_to_string (int32_t data);
void reset_array (char arr[]);

int main()
{   
    setup_system_clk();
    setup_uart1(9600);
    bmp280_init();
    uart1_send_string("init complete\n");


	while(1) {
        uart1_send_string("\n\n--------start---------------\n");
		bmp280_measure();
        int32_t p =  bmp280_getpressure();
        uart1_send_string("Pressure: ");
        int_to_string(p);
        uint32_t t = bmp280_gettemperature();
        uart1_send_string("\nTemparature: ");
        int_to_string(t);
        uart1_send_string("\n\n\n-----stop-------------\n\n\n\n");
        if (p == t){uart1_send_string("same val\n");}
        delay_ms(2000);
    }
}

void int_to_string (int32_t data)
{   
    char str[10];
    reset_array(str);
    int index = 0, len;

    while (data){
        int8_t temp = data % 10;
        str[index] = temp + 48;
        data = data / 10;
        index++;
    }
    len = strlen(str);
    /* reverse */
    for (int i = 0; i < len/2; i++){
        char temp = str[i];
        str[i] = str[len-1-i];
        str[len-1-i] = temp;
    }
    uart1_send_string(str);
    reset_array(str);
}

void reset_array (char arr[])
{
    for (int i = 0; i < 10; i++){
        arr[i] = 0;
    }
}