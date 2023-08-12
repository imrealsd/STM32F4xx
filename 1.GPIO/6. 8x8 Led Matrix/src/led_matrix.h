#include <stm32f407xx.h>
#include <stdint.h>
#ifndef LED_MATRIX
#define LED_MATRIX
void setup_led_matrix_gpio (void);
void show_static_letter (char);
#endif