#include <stdint.h>
#ifndef CRC_H
#define CRC_H
void setup_CRC(void);
void write_CRC_input(uint32_t);
uint32_t read_CRC_output(void);
#endif