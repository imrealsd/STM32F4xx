#include <stm32f407xx.h>
#include <stdint.h>
#include "RTC.h"


void set_RTC_time_and_date(uint32_t hour, uint32_t minute, uint32_t sec, uint32_t year, 
                                            uint32_t month, uint32_t date, uint32_t wkd)
{
    /* ------------------------- clock setup and initialization ---------------------------------------------- */

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;         /* power-controller clk enable */
    PWR->CR |= PWR_CR_DBP;                     /* RTC reg. write protection disable */

    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY));

    RCC->BDCR |= (0b01 << RCC_BDCR_RTCSEL_Pos); /* 32.7 KHZ LSE as clk source of RTC */
    RCC->BDCR |= RCC_BDCR_RTCEN;                /* RTC CLK enable */

    RTC->WPR = 0xCA;                            /* RTC all registers write protection diable */
    RTC->WPR = 0x53;

    while (!(RTC->ISR & RTC_ISR_RSF));          /* wait for RSF bit to set before entering into init mode */

    RTC->ISR |= RTC_ISR_INIT;                   /* Enter init initialization mode */
    while (!(RTC->ISR & RTC_ISR_INITF));        /* check if the initialization mode started or not */

    RTC->PRER = 0;
    RTC->PRER |= (127 << RTC_PRER_PREDIV_A_Pos); /* set prescalar to input 1hz clk to rtc */
    RTC->PRER |= (255 << RTC_PRER_PREDIV_S_Pos);

    RTC->CR &= ~(RTC_CR_FMT);                    /* 24hrs/day format */


    /* assemble TR & DR value and write at once 
     * otherwise it will not work
    */

    /*------------------------------ Year, Month, Day, Date setup -------------------------------------------------*/

    int8_t year_ten = year / 10;
    int8_t year_unit = year % 10;
    int8_t mon_ten = month / 10;
    int8_t mon_unit = month % 10;
    int8_t date_ten = date / 10;
    int8_t date_unit = date % 10;


    uint32_t temp = 0;                         
    temp |= (year_unit << RTC_DR_YU_Pos);       
    temp |= (year_ten << RTC_DR_YT_Pos);

    temp |= (mon_unit << RTC_DR_MU_Pos);        
    temp |= (mon_ten << RTC_DR_MT_Pos);

    temp |= (date_unit << RTC_DR_DU_Pos);      
    temp |= (date_ten << RTC_DR_DT_Pos);

    temp |= (wkd << RTC_DR_WDU_Pos);            

    RTC->DR = temp;

    /*------------------------------ Hour, minute, seccond setup -------------------------------------------------*/

    int8_t hour_ten = hour / 10;
    int8_t hour_unit = hour % 10;
    int8_t min_ten = minute / 10;
    int8_t min_unit = minute % 10;
    int8_t sec_ten = sec / 10;
    int8_t sec_unit = sec % 10;

    temp = 0;
    temp |= (hour_unit << RTC_TR_HU_Pos);         
    temp |=  (hour_ten << RTC_TR_HT_Pos);
    temp |= (min_unit << RTC_TR_MNU_Pos);        
    temp |= (min_ten << RTC_TR_MNT_Pos);
    temp |= (sec_unit << RTC_TR_SU_Pos);        
    temp |= (sec_ten << RTC_TR_ST_Pos);

    RTC->TR = temp;

    /*-------------------------------- exit init mode --------------------------------------------------------------*/

    RTC->ISR &= ~(RTC_ISR_INIT);
    RTC->WPR = 0xFF;                             /* enable write protection */
}



void get_rtc_time(uint32_t *hour, uint32_t *minute, uint32_t *seconds)
{   
    while (!(RTC->ISR & RTC_ISR_RSF)); /* wait for RSF to set [calander shadow register updated] */

    /* usage of volatile is must
     * otherwise compiler will optimize 
     * and cached value will be used
    */

    volatile uint32_t rtc_tr_val = RTC->TR;
    volatile uint32_t dummy_read = RTC->DR;


    volatile uint32_t hour_unit = rtc_tr_val & (0b1111 << RTC_TR_HU_Pos);
    volatile uint32_t hour_ten = rtc_tr_val & (0b111 << RTC_TR_HT_Pos);
    hour_unit = hour_unit >> RTC_TR_HU_Pos;
    hour_ten = hour_ten >> RTC_TR_HT_Pos;
    *hour = (hour_ten * 10) + hour_unit;

    volatile uint32_t min_unit = rtc_tr_val & (0b1111 << RTC_TR_MNU_Pos);
    volatile uint32_t min_ten = rtc_tr_val & (0b111 << RTC_TR_MNT_Pos);
    min_unit = min_unit >> RTC_TR_MNU_Pos;
    min_ten = min_ten >> RTC_TR_MNT_Pos;
    *minute = (min_ten * 10) + min_unit;

    volatile uint32_t sec_unit = rtc_tr_val & (0b1111 << RTC_TR_SU_Pos);
    volatile uint32_t sec_ten = rtc_tr_val & (0b111 << RTC_TR_ST_Pos);
    sec_unit = sec_unit >> RTC_TR_SU_Pos;
    sec_ten = sec_ten >> RTC_TR_ST_Pos;
    *seconds = (sec_ten * 10) + sec_unit;

    RTC->ISR &= ~(RTC_ISR_RSF);                    // clear RSF bit afetr every read
}


void get_rtc_date(uint32_t *year, uint32_t *month, uint32_t *date, uint32_t *week_day)
{
    while (!(RTC->ISR & RTC_ISR_RSF));             /* wait for RSF to set [calander shadow register updated] */
    volatile uint32_t dummy_read = RTC->TR;
    volatile uint32_t rtc_dr_val = RTC->DR;

    uint32_t year_uint = rtc_dr_val & (0b1111 << RTC_DR_YU_Pos);
    uint32_t year_ten  = rtc_dr_val & (0b1111  << RTC_DR_YT_Pos);
    year_uint = year_uint >> RTC_DR_YU_Pos;
    year_ten = year_ten >> RTC_DR_YT_Pos;
    *year = (year_ten * 10) + year_uint;

    uint32_t mon_uint = rtc_dr_val & (0b1111 << RTC_DR_MU_Pos);
    uint32_t mon_ten  = rtc_dr_val & (0b1  << RTC_DR_MT_Pos);
    mon_uint = mon_uint >> RTC_DR_MU_Pos;
    mon_ten = mon_ten >> RTC_DR_MT_Pos;
    *month = (mon_ten * 10) + mon_uint;

    uint32_t date_uint = rtc_dr_val & (0b1111 << RTC_DR_DU_Pos);
    uint32_t date_ten  = rtc_dr_val & (0b11  << RTC_DR_DT_Pos);
    date_uint = date_uint >> RTC_DR_DU_Pos;
    date_ten = date_ten >> RTC_DR_DT_Pos;
    *date = (date_ten * 10) + date_uint;

    uint32_t wkd = rtc_dr_val & (0b111 << RTC_DR_WDU_Pos);
    wkd = wkd >> RTC_DR_WDU_Pos;
    *week_day = wkd;

    RTC->ISR &= ~(RTC_ISR_RSF);                    // clear RSF bit afetr every read
}



uint8_t is_RTC_initialised(void)
{
    /* check rtc is initialised or not afetr system reset */
    if (RTC->ISR & RTC_ISR_INITS)
        return 1;
    else
        return 0;
}

