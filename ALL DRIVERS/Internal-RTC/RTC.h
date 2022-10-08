#include <stdint.h>
#ifndef RTC_H
#define RTC_H

/* Week Days*/
#define SUNDAY    7
#define SATURDAY  6
#define FRIDAY    5
#define THURSDAY  4
#define WEDNESDAY 3
#define TUESDAY   2
#define MONDAY    1

/* Months */
#define JANUARY    1
#define FRBRUARY   2
#define MARCH      3
#define APRIL      4
#define MAY        5
#define JUNE       6
#define JULY       7
#define AUGUST     8
#define SEPTEMBER  9
#define OCTOBER    10
#define NOVEMBER   11
#define DECEMBER   12

/* Function Prototypes*/
void set_RTC_time_and_date(uint32_t hour, uint32_t minute, uint32_t sec, uint32_t year, 
                                            uint32_t month, uint32_t date, uint32_t wkd);
void get_rtc_time(uint32_t *hour, uint32_t *minute, uint32_t *seconds);
void get_rtc_date(uint32_t *year, uint32_t *month, uint32_t *date, uint32_t *week_day);
uint8_t is_RTC_initialised(void);


// enum week_day {Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
// enum month {January = 1, February, March, April, May, June, July, August, September, October, November, December};

// typedef enum week_day Week_Day;
// typedef enum month Month;

#endif 
