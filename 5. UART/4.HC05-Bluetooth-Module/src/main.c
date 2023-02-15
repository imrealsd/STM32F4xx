
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


/**
 * PA0   : HC05 VCC
 * PA1   : HC05 EN
 * UART1 : pc tp stm32 communication   [baud rate 115200]
 * UART2 : stm32 to hc05 communication [baud rate 38400] 
*/



/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "hc05.h"

struct btModule {

	char  name[MAX_RESPONSE_LEN];
	char  address[MAX_RESPONSE_LEN];
	char  version[MAX_RESPONSE_LEN];
	char   mode[MAX_RESPONSE_LEN];
} hc05;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{	
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();

	if (HC05_verifyATMode() == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *)"Currently in AT mode\r\n", 23, HAL_MAX_DELAY);
	}

	if (HC05_getModuleInfo((char* const ) &hc05.name ,(char* const ) &hc05.address,
	        (char* const) &hc05.version,(char* const ) &hc05.mode) == HC05_OK){

		HAL_UART_Transmit(&huart1, (uint8_t *)"Module Info:\r\n", 23, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05.name,    strlen(hc05.name), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05.address, strlen(hc05.address), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05.version, strlen(hc05.version), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05.mode,    strlen(hc05.mode), HAL_MAX_DELAY);
	}

	while (1);
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
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
