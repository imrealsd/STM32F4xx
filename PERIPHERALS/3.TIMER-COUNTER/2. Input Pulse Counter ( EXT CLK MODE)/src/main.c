#include "sys_clock.h"
#include "counter.h"
#include "onboard_led.h"

void blink (int32_t);

int main()
{   
    setup_system_clk();
    setup_onboard_led();
    onboard_led_off();
    setup_risingEdge_counter();
 
    int32_t count_val = 0, latch = 0;

    while (1){                                              // led with blink number of input pulses times
        count_val = get_counter_value();

        if (count_val > latch){
            blink(count_val);
        }
        latch = count_val;
        
    }
}

void blink (int32_t val){

    while (val--){
        onboard_led_on();
        for (volatile int32_t i = 0; i < 100000; i++);
        onboard_led_off();
        for (volatile int32_t i = 0; i < 100000; i++);
    }
}