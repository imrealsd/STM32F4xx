#include "sys_clock.h"
#include "delay.h"
#include "onboard_led.h"

int main()
{
    setup_system_clk();
    setup_onboard_led();

    while (1){

        onboard_led_on();
        delay_ms(1000);
        onboard_led_off();
        delay_ms(1000);
    }
}