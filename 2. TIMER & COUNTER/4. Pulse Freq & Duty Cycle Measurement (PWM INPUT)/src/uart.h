#ifndef UART_H
#define UART_H

void init_uart (void);
void send_uart (char);
void send_uart_str (char*);
char recieve_uart (void);
int uart_available (void);

#endif