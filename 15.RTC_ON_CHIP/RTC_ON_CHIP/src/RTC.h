#include <stdint.h>
#ifndef RTC_H
#define RTC_H
void setup_on_chip_RTC(void);
void get_rtc_time(uint32_t *hour, uint32_t *minute, uint32_t *seconds);
void get_rtc_date(uint32_t *year, uint32_t *month, uint32_t *date, uint32_t *week_day);
uint8_t is_RTC_initialised(void);
#endif 
