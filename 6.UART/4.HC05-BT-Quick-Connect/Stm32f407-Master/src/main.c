
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
 * UART2 : stm32 to hc05_master communication [baud rate 38400] 
*/



/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "hc05.h"

struct btModule {

	char name       [MAX_RESPONSE_LEN];
	char address    [MAX_RESPONSE_LEN];
	char version    [MAX_RESPONSE_LEN];
	char mode       [MAX_RESPONSE_LEN];
	char state      [MAX_RESPONSE_LEN];
	char password   [MAX_RESPONSE_LEN];
	char uartSpeed  [MAX_RESPONSE_LEN];
	char bindedAddr [MAX_RESPONSE_LEN];
	
} hc05_master;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void masterModeConfigs(void);


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{	
	/*Initialise all Drivers*/
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	
	/*Enter Master Mode, do necessary configs & Bind Slave Adddress*/
	masterModeConfigs();
	
	/*leave at mode & enter data mode */
	HC05_powerReset();

	while (1) {}
}


/**
 * @brief  enter master mode & bind to slave address
 * @retval none
 */
static void masterModeConfigs(void)
{
	
	if (HC05_verifyATMode() == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *)"[+] Powered Up into AT mode\r\n", 30, HAL_MAX_DELAY);
	}

	if (HC05_setModuleName((char* const) "Master-Module") == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *)"[+] Changed Module Name to \"Master Module\" \r\n", 46, HAL_MAX_DELAY);
	}

	if (HC05_setUartSpeed((uint16_t) 38400) == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *)"[+] Set uart data baud-rate to 38400\r\n", 39, HAL_MAX_DELAY);
	}

	if (HC05_getModuleInfo((char* const ) &hc05_master.name ,(char* const ) &hc05_master.address,
		(char* const) &hc05_master.version,(char* const ) &hc05_master.mode, 
		(char* const)&hc05_master.password, (char* const)&hc05_master.uartSpeed) == HC05_OK){

		HAL_UART_Transmit(&huart1, (uint8_t *)"[+] Module Information :\r\n", 27, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.name,       strlen(hc05_master.name), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.address,    strlen(hc05_master.address), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.version,    strlen(hc05_master.version), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.mode,       strlen(hc05_master.mode), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.password,   strlen(hc05_master.password), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.uartSpeed,  strlen(hc05_master.uartSpeed), HAL_MAX_DELAY);

	}

	if (HC05_fixedAddr_masterModeEnter() == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *)"[+] Entered Fixed Address Master Mode\r\n", 40 , HAL_MAX_DELAY);
	}

	/*bind to hc05-slave module address [0022:04:010E2D]*/
	if (HC05_fixedAddr_masterModeBind((char* const) "0022,04,010E2D") == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *)"[+] Binded to Provided Slave Address:\r\n", 40, HAL_MAX_DELAY);
	}


	if (HC05_getBindedAddress((char* const) &hc05_master.bindedAddr) == HC05_OK){
		HAL_UART_Transmit(&huart1, (uint8_t *) &hc05_master.bindedAddr, strlen(hc05_master.bindedAddr), HAL_MAX_DELAY);
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
