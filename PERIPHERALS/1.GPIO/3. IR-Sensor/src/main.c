#include <stm32f407xx.h>
#include <stdint.h>
#include "sys_clock.h"
#include "ir_sensor.h"

int main()
{
    setup_system_clk();
    setup_ir_sensor();

    while (1){

        if (object_is_detected())
            turn_on_led();
        else
            turn_off_led();
    }
}
