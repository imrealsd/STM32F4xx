/* LED BLINK CODE 
 * Owner: Subhaeep Dhang , Date: 18.06.2022 
 * MCU: STM32F407VET6    , Framework: ARM CMSIS
*/

#include <stm32f407xx.h>
#include "sys_clock.h"
#include "onboard_led.h"
#include <stdint.h>

void wait (int32_t);

int main()
{
    setup_system_clk();
    setup_onboard_led();

    while (1){

        onboard_led_on();
        wait(50);
        onboard_led_off();
        wait(50);
    }
}

void wait (int32_t val)
{
    volatile int32_t i;
    for (i = 0; i < (val *2000); i++);
}
