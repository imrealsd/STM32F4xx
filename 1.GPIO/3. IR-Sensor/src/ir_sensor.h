#include <stdint.h>

#ifndef IR_H 
#define IR_H

void setup_ir_sensor (void);
int8_t object_is_detected (void);
void turn_on_led (void);
void turn_off_led (void);

#endif