#include <stm32f407xx.h>
#include <stdint.h>
#ifndef I2C1_H
#define I2C1_H
void setup_I2C1_SM (void);
void I2C1_start (void);
void I2C1_write_addr (int8_t, char*);
void I2C1_write_data (int8_t);
void I2C1_stop (void);
#endif