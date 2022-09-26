#include "sys_clock.h"
#include "delay.h"
#include "led_matrix.h"

int main()
{
    setup_system_clk();
    setup_led_matrix_gpio();
    

    while (1){

        for (int8_t i = 0; i < 4; i++){
            for (int8_t j = 0; j < 100; j++){
                show_static_letter(65+i);
            }
            delay_ms(500);
        }
    }
}