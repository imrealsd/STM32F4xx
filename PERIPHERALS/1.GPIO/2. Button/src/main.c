/* LED WITH BUTTON CODE 
 * Owner: Subhaeep Dhang , Date: 19.06.2022 
 * MCU: STM32F407VET6    , Framework: ARM CMSIS
*/

#include <stm32f407xx.h>
#include "sys_clock.h"
#include "onboard_butto_led.h"
#include <stdint.h>
void wait (int32_t);

int main()
{   
    int8_t count = 0 ;
    setup_system_clk();
    setup_onboard_led_and_button();

    while (1){
        if (button_is_pressed()){
            count++;
            wait(80);
        }
        if (count == 1){
            turn_on_led();
        } else {
            turn_off_led();
            count = 0;
        }
    }
}

void wait (int32_t val)
{
    volatile int32_t i;
    for (i = 0; i < (val * 2000); i++);
}