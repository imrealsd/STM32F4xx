#include "sys_clock.h"
#include "delay.h"
#include "DAC1.h"

int main()
{
    setup_system_clk();
    setup_dac1_ch1();

    while (1){
        
        for (int16_t br = 0; br < 4095; br = br+10){
            write_dac1_ch1(br);
            delay_ms(7);
        }
        for (int16_t br = 4094; br >= 0; br = br-10){
            write_dac1_ch1(br);
            delay_ms(7);
        }
    }
}