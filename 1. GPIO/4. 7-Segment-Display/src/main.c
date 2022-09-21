#include <stm32f407xx.h>
#include <stdint.h>
#include "sys_clock.h"
#include "seven_segment.h"

int main()
{
    setup_system_clk();
    setup_seven_segment_gpio();

    while (1){
        count_upto_number(9999);
        delay(500);
        count_down_number(9999);
        delay(500);
        // count_down_number(9999);       // 9999 --> 0000
        // delay(500);
        // count_upto_number(9999);       // 0000 --> 9999
        // delay(500);
    }
}

