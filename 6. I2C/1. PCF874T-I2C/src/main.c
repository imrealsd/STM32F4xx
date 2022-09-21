#include "sys_clock.h"
#include "delay.h"
#include "I2C1.h"
#define SLAVE_ADDR 0x27

int main()
{
    setup_system_clk();
    setup_I2C1_SM();
    delay_ms(200);  

    I2C1_start();
    I2C1_write_addr(SLAVE_ADDR, "WRITE");

    while (1){
        I2C1_write_data(0xFF);
        delay_ms(500);
        I2C1_write_data(0x00);
        delay_ms(500);
    }
}