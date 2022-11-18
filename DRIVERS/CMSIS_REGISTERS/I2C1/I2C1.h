#include <stm32f407xx.h>
#include <stdint.h>
#ifndef I2C1_H
#define I2C1_H
void setup_I2C1_SM (void);
void setup_I2C1_FM (void);
void I2C1_start (void);
void I2C1_write_addr (int8_t, char*);
void I2C1_write_data (int8_t);
void I2C1_stop (void);
void I2C1_read (int32_t, int8_t[],int);        // [slave-address+read-bit , buffer array address, size of data]

#endif
