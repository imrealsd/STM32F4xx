#include "pwm_input.h"
#include <stdint.h>
#ifndef ULTRASONIC
#define ULTRASONIC
void setup_ultrasonic (void);
float get_distance (void);
#endif