#include <stdint.h>
#include "I2C1.h"
#include "delay.h"
#include "i2c_lcd.h"

/* I2C-LCD connection details [pcf874t ic with lcd]:
 * P0 -> RS , P1 -> R/W , P2 -> EN , P3 -> LED 
 * P4 to P7 [D4 to D7 of lcd] [higher nibble]
*/

void start_i2c_lcd (void)
{   
    setup_I2C1_SM();

    /* reseting all pins of i/o expander */
    I2C1_start();
    I2C1_write_addr(0x27, "WRITE");
    I2C1_write_data(0x00);
    I2C1_stop();

    /* lcd 4bit config code */
    i2c_lcd_cmd(0x02);
    i2c_lcd_cmd(0x28);
    i2c_lcd_cmd(0x0C);
    i2c_lcd_cmd(0x01);
    i2c_lcd_cmd(0x80);
    
}

void i2c_lcd_clear (void)
{
    i2c_lcd_cmd(0x01);
}

void i2c_lcd_set_cursor (int8_t row, int8_t col)
{
    if (row == 0){
        i2c_lcd_cmd((0x80 + col));

    } else if (row == 1){
        i2c_lcd_cmd((0xc0+col));
    }
}


void i2c_lcd_cmd (int8_t cmd)
{   
    /* cmd_en0 = 0x08 [rs = 0, En = 0, led on]
     * cmd_en1 = 0x0C [rs = 0, en = 1, led 0n]
    */
    int8_t cmd_en1 = 0x0C;
    int8_t cmd_en0 = 0x08;
    int8_t high_nibble = cmd & 0xF0;
    int8_t low_nibble  = (cmd << 4) & 0xF0;

    I2C1_start();
    I2C1_write_addr(0x27, "WRITE");
 
    /* send higher nibble  */
    I2C1_write_data(high_nibble);
    I2C1_write_data(cmd_en0|high_nibble);
    delay_ms(1);
    I2C1_write_data(cmd_en1|high_nibble);
    delay_ms(1);
    I2C1_write_data(cmd_en0|high_nibble);

  
    /* send lower nibble */
    I2C1_write_data(low_nibble);
    I2C1_write_data(cmd_en0|low_nibble);
    delay_ms(1);
    I2C1_write_data(cmd_en1|low_nibble);
    delay_ms(1);
    I2C1_write_data(cmd_en0|low_nibble);

    I2C1_stop();
}

void i2c_lcd_data (int8_t data)
{
    /* dta_en0 = 0x09 [rs = 0, En = 0, led on]
     * dta_en1 = 0x0D [rs = 0, en = 1, led 0n]
    */
    int8_t dta_en1 = 0x0D;
    int8_t dta_en0 = 0x09;
    int8_t high_nibble = data & 0xF0;
    int8_t low_nibble  = (data << 4) & 0xF0;

    I2C1_start();
    I2C1_write_addr(0x27, "WRITE");
 
    /* send higher nibble  */
    I2C1_write_data(high_nibble);
    I2C1_write_data(dta_en0|high_nibble);
    delay_ms(1);
    I2C1_write_data(dta_en1|high_nibble);
    delay_ms(1);
    I2C1_write_data(dta_en0|high_nibble);
  
    /* send lower nibble */
    I2C1_write_data(low_nibble);
    I2C1_write_data(dta_en0|low_nibble);
    delay_ms(1);
    I2C1_write_data(dta_en1|low_nibble);
    delay_ms(1);
    I2C1_write_data(dta_en0|low_nibble);
   
    I2C1_stop();
}

void i2c_lcd_string (char* str)
{
    while (*str != 0){
        i2c_lcd_data(*str);
        str++;
    }
}