#include "sys_clock.h"
#include "delay.h"
#include "i2c_lcd.h"


int main()
{   
    setup_system_clk();
    start_i2c_lcd();
    
    i2c_lcd_set_cursor(0,5);
    i2c_lcd_string("Hello");
    i2c_lcd_set_cursor(1,5);
    i2c_lcd_string("World");

    while (1){}
}