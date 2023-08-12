#include "sys_clock.h"
#include "input_capture.h"
#include "uart.h"
#include "delay.h"
#include <stdint.h>

void blink (void);

int main()
{
    setup_system_clk();
    setup_pulse_freq();

    init_uart();
    int32_t val;
    char str[3];

    while (1){

        val = get_pulse_freq();

        if (val > 0){
            str[1] = (val % 10)+48;
            str[0] = ((val / 10) % 10) + 48;
        }
        send_uart_str(str);
        send_uart_str("Hz\n");
        delay_ms(500);
    }
}

