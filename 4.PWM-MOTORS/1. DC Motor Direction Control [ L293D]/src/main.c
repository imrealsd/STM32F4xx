#include <stm32f407xx.h>
#include <stdint.h>
#include "sys_clock.h"
#include "Motor.h"

void delay (int32_t);

int main()
{
    setup_system_clk();
    setup_motor_driver_gpio();

    while (1){
        rotate_motor(CLOCKWISE);
        delay(400);
        rotate_motor(ANTI_CLOCKWISE);
        delay(400);
    }
}

void delay (int32_t val)
{
    volatile int32_t i;
    for (i = 0; i < val * 2000; i++);
}