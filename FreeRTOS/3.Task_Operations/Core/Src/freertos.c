/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include <stdint.h>
#include <string.h>

osThreadId Task1Handle;
osThreadId Task2Handle;
osThreadId Task3Handle;

/* USER CODE END FunctionPrototypes */

void start_task1(void const *argument);
void start_task2(void const *argument);
void start_task3(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{

	/* definition and creation of Task1 */
	osThreadDef(Task1, start_task1, osPriorityAboveNormal, 0, 128);
	Task1Handle = osThreadCreate(osThread(Task1), NULL);

	/* definition and creation of Task2 */
	osThreadDef(Task2, start_task2, osPriorityNormal, 0, 128);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	osThreadDef(Task3, start_task3, osPriorityBelowNormal, 0, 2048);
	Task2Handle = osThreadCreate(osThread(Task3), NULL);
}

/**
 * @brief  Function implementing the Task1 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_task1 */
void start_task1(void const *argument)
{
	char *msg = "Hello from Task1\r\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t*) msg, strlen(msg), 100);
		osDelay(500);
	}
}

/**
 * @brief Function implementing the Task2 thread.
 * @param argument: Not used
 * @retval None
 */
void start_task2(void const *argument)
{
	int count = 0;
	char *msg = "Hello from Task2\r\n";

	for (;;) {
		count++;
		HAL_UART_Transmit(&huart1, (uint8_t*) msg, strlen(msg), 100);
		osDelay(500);

		if (count == 5) {

			HAL_UART_Transmit(&huart1, (uint8_t*) "suspending Task1\r\n", 20,
					100);
			osThreadSuspend(Task1Handle);
		}

		if (count == 10) {
			HAL_UART_Transmit(&huart1, (uint8_t*) "resuming Task1\r\n", 20,
					100);
			osThreadResume(Task1Handle);
			count = 0;
		}
	}
}

void start_task3(void const *argument)
{
	uint8_t thread_list[1024];

	for (;;) {
		osThreadList(thread_list);
		HAL_UART_Transmit(&huart1, (uint8_t*) thread_list,
				strlen((char*) thread_list), 100);
		osDelay(2000);
	}
}

