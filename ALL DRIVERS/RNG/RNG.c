#include <stm32f407xx.h>
#include <stdint.h>
#include "RNG.h"

void setup_RNG(void)
{
    /* peripheral clock enable */
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    /* RNG dedicated 48 MHZ clk setup */
    RCC->PLLCFGR = 0x00000000;
    RCC->PLLCFGR &= ~(1 << RCC_PLLCFGR_PLLSRC_Pos);            //  16Mhz HSI clk as PLL input
    RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLM_Pos);               // 16/8 = 2Mhz vco input
    RCC->PLLCFGR |= (96 << RCC_PLLCFGR_PLLN_Pos);              // 96 x 2 = 192Mhz vco output
    RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLQ_Pos);               // VCO division factor for RNG [192/4 = 48Mhz]

    RCC->CR |= RCC_CR_PLLON;                                   // Main PLL ON
    while (!(RCC->CR & RCC_CR_PLLRDY));                        // wait for PLL ready
    RNG->CR |= RNG_CR_RNGEN;                                   // enable RNG
}

uint32_t get_Random_Number(void)
{
    uint32_t num = 0;
    if ((!(RNG->SR & RNG_SR_SECS)) && (!(RNG->SR & RNG_SR_CECS))){
        while (!(RNG->SR & RNG_SR_DRDY));
        num = RNG->DR;    // read generated random number
    }
    return num;
}