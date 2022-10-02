#include <stm32f407xx.h>
#include <stdint.h>

void setup_RTC(void)
{   
    RCC->BDCR |= (0b01 << RCC_BDCR_RTCSEL_Pos);                                      /* 32.7 KHZ LSE as clk source of RTC */
    RTC->PRER |= (128 << RTC_PRER_PREDIV_A_Pos) | (256 << RTC_PRER_PREDIV_S_Pos);    /* set prescalar to input 1hz clk to rtc */
    RCC->BDCR |= RCC_BDCR_RTCEN;                                                     /* RTC CLK enable */
    
}