#include <stdint.h>
#ifndef UART1_H
#define UART1_H
void setup_uart1 (int32_t);
void uart1_send_1byte (int8_t);
int8_t nearest_int (float);
void uart1_send_string (char *str);
int8_t uart1_recieve_1byte (void);
int8_t uart1_data_avaiilable (void);
#endif