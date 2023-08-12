#include <stdint.h>

#ifndef ONBOARD_LED_BUTTON_H
#define ONBOARD_LED_BUTTON_H

void setup_onboard_led_and_button (void);
int8_t button_is_pressed (void);
void turn_on_led (void);
void turn_off_led (void);

#endif