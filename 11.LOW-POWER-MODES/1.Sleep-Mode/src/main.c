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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

// for receiving 1 byte uart data
uint8_t uartRxData[1];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MCU_EnterSleepMode_WFI(void);
void MCU_TaskAfterWakeup(void);


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{	
	/*Reset all hardware , setup clock & gpio*/
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();

	/**
	 * setup uart & put uart into receive_it mode (to receive 1 byte data through interrupt) 
	 * we will be using uart rx interrupt to wake up the mcu from sleep
	 */
	MX_USART1_UART_Init();
	HAL_UART_Receive_IT(&huart1,(uint8_t *)uartRxData, 1);

	while (1){
		/*mcu enters into sleep mode*/
		MCU_EnterSleepMode_WFI();

		/*resume execution form here after waking up & executing uartRX_ISR*/
		MCU_TaskAfterWakeup();
	}
	return 0;
}



/**
 * @brief  indicate led , send msg to pc & enter sleep mode 
 * @retval none
 */
void MCU_EnterSleepMode_WFI(void)
{	
	/*msg to be set to pc before going to sleep*/
	char *sleepMsg = "Going into Sleep Mode in 5 sec\r\n";

	/*turn on led, send msg to pc, wait 5 sec turn off led */ 
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_UART_Transmit(&huart1, (uint8_t *)sleepMsg, strlen(sleepMsg), 100);
	HAL_Delay(5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

	/* suspend systick clock (to avoid wake up form timer interrupt) & enter sleep mode through WFI*/
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}



/**
 * @brief  Mcu task after waking up
 * @retval none
 */
void MCU_TaskAfterWakeup(void)
{	
	/*msg to be sent to pc after waking up & perform isr*/
	char *wakeupMsg = "Wake up to Reailty !!\r\n";

	/*resume systick clock to use hal_delay func & send msg to pc*/
	HAL_ResumeTick();
	HAL_UART_Transmit(&huart1, (uint8_t *)wakeupMsg, strlen(wakeupMsg), 100);

	/*toggle led several times*/
	for (volatile int i = 0; i < 10; i++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(100);
	}
}


/**
 * @brief  uart rx complete ISR
 * @retval none
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	/*msg to be sent to pc afetr waking up*/
	char *uartMsg = "Here Comes The UART-Interrupt\r\n";

	/*setup uart for receiving another byte through interrupt & transmit msg*/
    HAL_UART_Receive_IT(huart,(uint8_t *)uartRxData, 1);
    HAL_UART_Transmit(&huart1, (uint8_t *)uartMsg, strlen(uartMsg), 100);
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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
