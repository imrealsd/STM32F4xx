#include <stm32f407xx.h>
#include <stdint.h>
#include "onboard_led.h"

void setup_on_chip_RTC(void)
{   
    /* ------------------------- clock setup and initialization ---------------------------------------------- */

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                      /* power-controller clk enable */
    PWR->CR |= PWR_CR_DBP;                                  /* RTC reg. write protection disable */

    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY));
    onboard_led_blink();

    RCC->BDCR |= (0b01 << RCC_BDCR_RTCSEL_Pos);             /* 32.7 KHZ LSE as clk source of RTC */  
    RCC->BDCR |= RCC_BDCR_RTCEN;                            /* RTC CLK enable */

    RTC->WPR = 0xCA;                                        /* RTC all registers write protection diable */
    RTC->WPR = 0x53;

    RTC->ISR |= RTC_ISR_INIT;                               /* Enter init initialization mode */
    while (!(RTC->ISR & RTC_ISR_INITF));                    /* check if the initialization mode started or not */

    RTC->PRER |= (128 << RTC_PRER_PREDIV_A_Pos);            /* set prescalar to input 1hz clk to rtc */
    RTC->PRER |= (256 << RTC_PRER_PREDIV_S_Pos); 
    
    RTC->CR &= ~(RTC_CR_FMT);                               /* 24hrs/day format */

    /*------------------------------ Year, Month, Day, Date setup -------------------------------------------------*/

    RTC->DR = 0;                                            /* set date, month, week-day, year */ 
    RTC->DR |= (0b0010 << RTC_DR_YU_Pos);                   /* year: 22 -> year unit: 2, year ten: 2 */
    RTC->DR |= (0b0010 << RTC_DR_YT_Pos);

    RTC->DR |= (0b0000 << RTC_DR_MU_Pos);                   /* Month: october->[10] : month unit:0 , month ten: 1 */
    RTC->DR |= (0b1 << RTC_DR_MT_Pos);

    RTC->DR |= (0b0101 << RTC_DR_DU_Pos);                   /* Date: OCT 5 : date unit: 5 , date ten: 0 */
    RTC->DR |= (0b0 << RTC_DR_DT_Pos);

    RTC->DR |= (0b011 << RTC_DR_WDU_Pos);                   /* week day: wednesday [3] */

    /*------------------------------ Hour, minute, seccond setup -------------------------------------------------*/

    RTC->TR = 0;
    RTC->TR |= (0b1000 << RTC_TR_HU_Pos);                       /* Hour: 08 */
    RTC->TR &= ~(0b111 << RTC_TR_HT_Pos);

    RTC->TR |= (0b1000 << RTC_TR_MNU_Pos);                      /* minute: 58 */
    RTC->TR |= (0b101 << RTC_TR_MNT_Pos);

    RTC->TR &= ~(0b1111 << RTC_TR_SU_Pos);                      /* Seconds: 00 */
    RTC->TR &= ~(0b111 << RTC_TR_ST_Pos);

    /*-------------------------------- exit init mode --------------------------------------------------------------*/

    RTC->ISR &= ~(RTC_ISR_INIT);
}


void get_rtc_time(uint8_t *hour, uint8_t *minute, uint8_t *seconds)
{
    while (!(RTC->ISR & RTC_ISR_RSF));             /* wait for RSF to set [calander shadow register updated] */

    uint32_t rtc_tr_val = RTC->TR;
    uint32_t dummy_read = RTC->DR | RTC->SSR;


    uint8_t hour_unit = rtc_tr_val & (0b1111 << RTC_TR_HU_Pos);
    uint8_t hour_ten  = rtc_tr_val & (0b111  << RTC_TR_HT_Pos);
    hour_unit = hour_unit >> RTC_TR_HU_Pos;
    hour_ten = hour_ten >> RTC_TR_HT_Pos;
    *hour = (hour_ten * 10) + hour_unit;

    uint8_t min_unit = rtc_tr_val & (0b1111 << RTC_TR_MNU_Pos);
    uint8_t min_ten  = rtc_tr_val & (0b111  << RTC_TR_MNT_Pos);
    min_unit = min_unit >> RTC_TR_MNU_Pos;
    min_ten = min_ten >> RTC_TR_MNT_Pos;
    *minute = (min_ten * 10) + min_unit;

    uint8_t sec_unit = rtc_tr_val & (0b1111 << RTC_TR_SU_Pos);
    uint8_t sec_ten  = rtc_tr_val & (0b111  << RTC_TR_ST_Pos);
    sec_unit = sec_unit >> RTC_TR_SU_Pos;
    sec_ten = sec_ten >> RTC_TR_ST_Pos;
    *seconds = (sec_ten * 10) + sec_unit;
}




// void get_rtc_date(uint8_t *year, uint8_t *month, uint8_t *date, char *week_day)
// {
//     while (!(RTC->ISR & RTC_ISR_RSF));             /* wait for RSF to set [calander shadow register updated] */
//     uint32_t rtc_tr_val = RTC->TR;

//     uint8_t hour_uint = rtc_tr_val & (0b1111 << RTC_TR_HU_Pos);
//     uint8_t hour_ten  = rtc_tr_val & (0b111  << RTC_TR_HT_Pos);
//     *hour = (hour_ten * 10) + hour_uint;

//     uint8_t min_uint = rtc_tr_val & (0b1111 << RTC_TR_MNU_Pos);
//     uint8_t min_ten  = rtc_tr_val & (0b111  << RTC_TR_MNT_Pos);
//     *minute = (min_ten * 10) + min_uint;

//     uint8_t sec_uint = rtc_tr_val & (0b1111 << RTC_TR_SU_Pos);
//     uint8_t sec_ten  = rtc_tr_val & (0b111  << RTC_TR_ST_Pos);
//     *seconds = (sec_ten * 10) + sec_uint;
// }

uint8_t is_RTC_initialised(void)
{   
    /* check rtc is initialised or not afetr system reset */
    if (RTC->ISR & RTC_ISR_INITS)
        return 1;
    else
        return 0;
}

