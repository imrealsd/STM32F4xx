#include "pwm_output.h"
#include "servo.h"

void servo_rotate_left (void)
{
    start_pwm_output(2.5);                    // 1ms high pulse [7.35 % duty cycle]    
}

void servo_rotate_right (void)
{
    start_pwm_output(12.5);                  // 2ms high pulse [10 % duty cycle]
}

void servo_default_pos (void)
{
    start_pwm_output(5);                // 1.5 ms high pulse [7.5 % duty cycle]
}