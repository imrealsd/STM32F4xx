#include "sys_clock.h"
#include "delay.h"
#include "pwm_output.h"
#include "servo.h"

int main()
{   
    setup_system_clk();
    servo_rotate_right();

    while (1){}
    
}