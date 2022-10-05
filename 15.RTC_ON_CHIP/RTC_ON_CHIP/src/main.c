#include "sys_clock.h"
#include "RTC.h"
#include "delay.h"
#include "UART1.h"
#include "onboard_led.h"

void send_to_pc(uint8_t hour, uint8_t min, uint8_t sec);

int main(void)
{   
    setup_system_clk();
    setup_onboard_led();
    setup_uart1(9600);
    setup_on_chip_RTC();

    uint8_t hour, min, sec;

    while (1) {
    
        get_rtc_time(&hour, &min, &sec);
        send_to_pc(hour,min,sec);
        delay_ms(2000);
    }
}

void send_to_pc(uint8_t hour, uint8_t min, uint8_t sec)
{
    char time_buff[3];
    time_buff[2] = '\0';

    time_buff[1] = (hour % 10) + 48;
    time_buff[0] = (hour / 10) + 48;
    uart1_send_string("Hour:");
    uart1_send_string(time_buff);
    uart1_send_1byte('\n');

    time_buff[1] = (min % 10) + 48;
    time_buff[0] = (min / 10) + 48;
    uart1_send_string("Min:");
    uart1_send_string(time_buff);
    uart1_send_1byte('\n');

    time_buff[1] = (sec % 10) + 48;
    time_buff[0] = (sec / 10) + 48;
    uart1_send_string("sec:");
    uart1_send_string(time_buff);
    uart1_send_1byte('\n');
}