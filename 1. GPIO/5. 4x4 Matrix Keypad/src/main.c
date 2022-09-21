#include <stm32f407xx.h>
#include <stdint.h>
#include "sys_clock.h"
#include "keypad.h"
#include "onboard_led.h"

void delay (int32_t);
void led_blink (int8_t);

int main()
{   
    int8_t key;
    setup_system_clk();
    setup_onboard_led();
    setup_keypad_gpio();
    onboard_led_off();
    key = scan_keypad();

    while (1){
        while (key == 0){
            key = scan_keypad();
            delay(50);
        }
        led_blink(key);
        key = 0;
    }
}

void led_blink (int8_t num){           // led will blink the key-value times

    while(num--){
        onboard_led_on();
        delay(100);
        onboard_led_off();
        delay(100);
    }
}

void delay (int32_t val)
{   
    volatile int32_t i;
    for (i = 0; i < val *2000; i++);
}