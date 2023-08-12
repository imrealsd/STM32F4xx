#include "sys_clock.h"
#include "RTC.h"
#include "delay.h"
#include "UART1.h"
#include <stdint.h>

void send_to_pc(uint32_t, char*);

int main(void)
{   
    setup_system_clk();
    setup_uart1(9600);
    set_RTC_time_and_date(8,55,30,22,OCTOBER, 8, SATURDAY);

    uint32_t hour, min, sec, year, month, date, week_day;

    while (1) {
        
        if (is_RTC_initialised()){

            get_rtc_time(&hour, &min, &sec);
            get_rtc_date(&year, &month, &date, &week_day);

            send_to_pc(hour,"Hour  :");
            send_to_pc(min,"Minute:");
            send_to_pc(sec,"Second:");
            send_to_pc(year,"Year  :20");
            send_to_pc(month,"Month :");
            send_to_pc(date,"Date  :");
            send_to_pc(week_day,"Wk-Day:");
            uart1_send_string("---------\n");

            delay_ms(5000);
        }
    }
}


void send_to_pc(uint32_t val, char *name)
{
    char buff[3];
    
    int_to_string(val,buff,3);
    uart1_send_string(name);
    uart1_send_string (buff);
    uart1_send_1byte('\n');
}