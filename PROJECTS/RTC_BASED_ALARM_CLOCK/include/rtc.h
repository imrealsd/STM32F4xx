
#ifndef RTC_H
#define RTC_H

/*Includes*/
#include <stm32f407xx.h>
#include <stdint.h>

/*Driver APIs*/
void RTC_init(void);
void RTC_set_time(uint8_t hour, uint8_t minute, uint8_t second);
void RTC_get_time(uint8_t *p_hour, uint8_t *p_minute, uint8_t *p_second);

#endif 
