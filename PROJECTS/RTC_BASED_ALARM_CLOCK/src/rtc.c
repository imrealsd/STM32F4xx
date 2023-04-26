
#include "rtc.h"


void RTC_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;         /* power-controller clk enable */
    PWR->CR |= PWR_CR_DBP;                     /* RTC reg write access enable */

    RCC->BDCR |= RCC_BDCR_LSEON;               /*External Low Frequency Clock enable*/
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

    RTC->DR = 0;                                 /*RTC initial time and date setting*/
    RTC->TR = 0;

    RTC->ISR &= ~(RTC_ISR_INIT);                 /*Exit init mode*/
    RTC->WPR = 0xFF;                             /* enable write protection */
}



void RTC_set_time(uint8_t hour, uint8_t minute, uint8_t second)
{   
    uint32_t temp = 0;
    int8_t hour_ten  = hour / 10;
    int8_t hour_unit = hour % 10;
    int8_t min_ten   = minute / 10;
    int8_t min_unit  = minute % 10;
    int8_t sec_ten   = second / 10;
    int8_t sec_unit  = second % 10;

    RTC->WPR = 0xCA;                            /* RTC all registers write protection diable */
    RTC->WPR = 0x53;

    while (!(RTC->ISR & RTC_ISR_RSF));          /* wait for RSF bit to set before entering into init mode */

    RTC->ISR |= RTC_ISR_INIT;                   /* Enter init initialization mode */
    while (!(RTC->ISR & RTC_ISR_INITF));        /* check if the initialization mode started or not */

  
    temp |= (hour_unit << RTC_TR_HU_Pos);         
    temp |=  (hour_ten << RTC_TR_HT_Pos);
    temp |= (min_unit << RTC_TR_MNU_Pos);        
    temp |= (min_ten << RTC_TR_MNT_Pos);
    temp |= (sec_unit << RTC_TR_SU_Pos);        
    temp |= (sec_ten << RTC_TR_ST_Pos);

    RTC->TR = temp;                              /*set time*/

    RTC->ISR &= ~(RTC_ISR_INIT);                 /*Exit init mode*/
    RTC->WPR = 0xFF;                             /* enable write protection */
}


void RTC_get_time(uint8_t *p_hour, uint8_t *p_minute, uint8_t *p_second)
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
    *p_hour = (hour_ten * 10) + hour_unit;

    volatile uint32_t min_unit = rtc_tr_val & (0b1111 << RTC_TR_MNU_Pos);
    volatile uint32_t min_ten = rtc_tr_val & (0b111 << RTC_TR_MNT_Pos);
    min_unit = min_unit >> RTC_TR_MNU_Pos;
    min_ten = min_ten >> RTC_TR_MNT_Pos;
    *p_minute = (min_ten * 10) + min_unit;

    volatile uint32_t sec_unit = rtc_tr_val & (0b1111 << RTC_TR_SU_Pos);
    volatile uint32_t sec_ten = rtc_tr_val & (0b111 << RTC_TR_ST_Pos);
    sec_unit = sec_unit >> RTC_TR_SU_Pos;
    sec_ten = sec_ten >> RTC_TR_ST_Pos;
    *p_second = (sec_ten * 10) + sec_unit;

    RTC->ISR &= ~(RTC_ISR_RSF);                    // clear RSF bit afetr every read
}