#include "sys_clock.h"
#include "delay.h"
#include "SPI1.h"
#include <stdint.h>
void MAX_7221_send(uint8_t,uint8_t);
void setup_MAX_7221(void);
void reset_MAX_7221(void);

int main(void)
{
    setup_system_clk();
    setup_SPI1();
    delay_ms(2000);
    setup_MAX_7221();
    reset_MAX_7221();

    while (1){
        MAX_7221_send(0x01,0xFF);
        delay_ms(1000);
        MAX_7221_send(0x01,0x00);
        delay_ms(1000);
    }
}

void MAX_7221_send(uint8_t addr, uint8_t data) 
{   
    send_SPI1_16bit((addr << 8)| data);
}

void setup_MAX_7221(void)
{
    MAX_7221_send(0x09,0x00);                // disable decoding [ not required in matrix]
    MAX_7221_send(0x0B,0x07);                // scan digit 0 to 7
    MAX_7221_send(0x0C,0x01);                // turn on chip
    MAX_7221_send(0x0A,0x0F);                // max intensity
}

void reset_MAX_7221(void)
{
    MAX_7221_send(0x01,0x00);
    MAX_7221_send(0x02,0x00);
    MAX_7221_send(0x03,0x00);
    MAX_7221_send(0x04,0x00);
    MAX_7221_send(0x05,0x00);
    MAX_7221_send(0x06,0x00);
    MAX_7221_send(0x07,0x00);
    MAX_7221_send(0x08,0x00);
}