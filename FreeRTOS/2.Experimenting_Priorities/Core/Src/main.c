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
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"


/* Thread Handlers */
osThreadId Task1Handle;
osThreadId Task2Handle;
osThreadId Task3Handle;
osThreadId Task4Handle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
void Task1_init(void const *arguments);
void Task2_init(void const *arguments);
void Task3_init(void const *arguments);
void Task4_init(void const *arguments);



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
	MX_FREERTOS_Init();

	osThreadDef(Task1, Task1_init, osPriorityNormal, 0, 128);
	Task1Handle = osThreadCreate(osThread(Task1), NULL);

	osThreadDef(Task2, Task2_init, osPriorityAboveNormal, 0, 128);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	osThreadDef(Task3, Task3_init, osPriorityBelowNormal, 0, 128);
	Task3Handle = osThreadCreate(osThread(Task3), NULL);

	osThreadDef(Task4, Task4_init, osPriorityHigh, 0, 128);
	Task4Handle = osThreadCreate(osThread(Task4), NULL);


	/* Start scheduler */
	osKernelStart();
}

/**
 * Task with highest priority
 */
void Task1_init(void const *arguments)
{
	for (;;) {
		char *msg = "Hello From Task1\r\n";
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osDelay(1000);
	}
}

/**
 * Task with medium priority
 */
void Task2_init(void const *arguments)
{
	for (;;) {
		char *msg = "Hello From Task2\r\n";
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osDelay(1000);
	}
}

/**
 * Task with low priority
 */
void Task3_init(void const *arguments)
{
	for (;;) {
		char *msg = "Hello From Task3\r\n";
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osDelay(1000);
	}
}

void Task4_init(void const *arguments)
{
	char *msg1 = "Changing Priorities\r\n";
	char *msg2 = "ERROR !!\r\n";
	int count = 0;

	for (;;) {
		count ++;
		if (count == 5) {
			HAL_UART_Transmit(&huart1, (uint8_t *)msg1, strlen(msg1), 100);
			count = 0;

			if (osThreadSetPriority(Task1Handle, osPriorityRealtime) != osOK) {
				HAL_UART_Transmit(&huart1, (uint8_t *)msg2, strlen(msg2), 100);
			}

			if (osThreadGetPriority(Task1Handle) != osPriorityRealtime) {
				HAL_UART_Transmit(&huart1, (uint8_t *)msg2, strlen(msg2), 100);
			}

			osThreadTerminate(Task4Handle);
		}
		osDelay(1000);
	}
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
	Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
	Error_Handler();
	}
}



/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
	HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
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

#ifdef  USE_FULL_ASSERT
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

