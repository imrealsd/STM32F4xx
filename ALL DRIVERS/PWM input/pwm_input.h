#include <stdint.h>
#ifndef PULSE_H
#define PULSE_H
void setup_pwm_input (void);
float get_pulse_freq (void);
float get_pulse_duty_cycle (void);
float get_pulse_on_time (void);
#endif