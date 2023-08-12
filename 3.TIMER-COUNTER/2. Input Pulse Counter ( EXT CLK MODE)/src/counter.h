#include <stdint.h>
#include <stm32f407xx.h>
#ifndef COUNTER
#define COUNTER
void setup_risingEdge_counter (void);
int32_t get_counter_value (void);
#endif