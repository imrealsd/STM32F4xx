#include <stm32f407xx.h>
#include "sys_clock.h"
#include "lcd.h"
#include <string.h>



int main()
{
    setup_system_clk();
    setup_lcd_gpio();
    delay(1000);
    start_lcd();
    
    // set_cursor(0,0);
    // lcd_string("  STM32F407VET6 ");
    // set_cursor(1,1);
    // lcd_string("  ARM CORTEX M4");

   
    while (1){
        set_cursor(1,15);
        scroll_text("MCU : STM32F407VET6");
        clear_lcd();
        set_cursor(0,15);
        scroll_text("CPU: ARM CORTEX M4");
        clear_lcd();
    }
}





