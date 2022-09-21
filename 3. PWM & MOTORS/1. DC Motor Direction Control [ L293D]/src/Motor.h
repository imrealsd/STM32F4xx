#include <stm32f407xx.h>
#include <stdint.h>
#ifndef MOTOR_H
#define MOTOR_H
#define CLOCKWISE 0
#define ANTI_CLOCKWISE 1
void setup_motor_driver_gpio (void);
void rotate_motor (int8_t);
#endif