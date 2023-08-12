/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
/*Private Includes*/
#include "fonts.h"
#include "ssd1306.h"
#include "bitmap.h"

/*private function prototypes*/
void SystemClock_Config(void);
static void test_basicLibraryFunctions(void);
static void test_bitmapFunction(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{   
    /* Initialise clocks & peripherals */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    /*Initialize oled display*/
    SSD1306_Init ();

    test_basicLibraryFunctions();
    test_bitmapFunction();
    SSD1306_Clear();

    while (1){}
}

static void test_bitmapFunction(void)
{
    SSD1306_Clear();
    SSD1306_DrawBitmap(30, 0, GITHUB_logo, 128, 64, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(1000);

    SSD1306_Clear();
    SSD1306_DrawBitmap(0, 0, ECEN_logo, 128, 64, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(1000);

    SSD1306_Clear();
    SSD1306_DrawBitmap(0, 0, Linux_logo, 128, 64, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(1000);
}


static void test_basicLibraryFunctions(void)
{   
    SSD1306_GotoXY (0,0); 
    SSD1306_Puts ("HELLO", &Font_16x26, 1);
    SSD1306_GotoXY (0, 30); 
    SSD1306_Puts ("WORLD !!", &Font_11x18, 1); 
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_ToggleInvert();
    SSD1306_UpdateScreen(); 
    SSD1306_ToggleInvert();
    SSD1306_UpdateScreen(); 
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_Fill(SSD1306_COLOR_WHITE);
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_DrawPixel(10,20,SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_GotoXY(10,10);
    SSD1306_Putc('S', &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_DrawLine(0, 0, 127, 63, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_DrawTriangle(50, 0, 0, 20, 126, 30, SSD1306_COLOR_WHITE);
    SSD1306_ToggleInvert();
    SSD1306_UpdateScreen();
    HAL_Delay(200);
    SSD1306_ToggleInvert();
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_DrawFilledRectangle(10,10,80,40, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(400);

    SSD1306_Clear();
    SSD1306_DrawFilledCircle(63, 31, 10, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(400);


    SSD1306_Clear();
    SSD1306_GotoXY(0, 20);
    SSD1306_Puts("Hello", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(500);

    SSD1306_ScrollRight(0x00, 0xff);  // scroll entrire display [row 0-127]
    HAL_Delay(350);
    SSD1306_ScrollLeft (0x00, 0xff);
    HAL_Delay(350);
    SSD1306_Stopscroll();
    /*Diagonal srolls are not working*/

}   



/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
