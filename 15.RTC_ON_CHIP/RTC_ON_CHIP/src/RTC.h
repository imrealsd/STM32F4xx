#include <stdint.h>
#ifndef RTC_H
#define RTC_H
void setup_on_chip_RTC(void);
void get_rtc_time(uint8_t *hour, uint8_t *minute, uint8_t *seconds);
uint8_t is_RTC_initialised(void);
#endif 
