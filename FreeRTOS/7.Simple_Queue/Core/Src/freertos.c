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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "usart.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId HpSendTaskHandle;
osThreadId LpSendTaskHandle;
osThreadId ReceiveTaskHandle;
osMessageQId myQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void hpSendTask_init(void const *argument);
void lpSendTask_init(void const *argument);
void receiveTask_init(void const *argument);

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
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* definition and creation of myQueue */
	osMessageQDef(myQueue, 5, uint64_t);
	myQueueHandle = osMessageCreate(osMessageQ(myQueue), NULL);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of HpSendTask */
	osThreadDef(HpSendTask, hpSendTask_init, osPriorityAboveNormal, 0, 128);
	HpSendTaskHandle = osThreadCreate(osThread(HpSendTask), NULL);

	/* definition and creation of LpSendTask */
	osThreadDef(LpSendTask, lpSendTask_init, osPriorityNormal, 0, 128);
	LpSendTaskHandle = osThreadCreate(osThread(LpSendTask), NULL);

	/* definition and creation of ReceiveTask */
	osThreadDef(ReceiveTask, receiveTask_init, osPriorityBelowNormal, 0, 128);
	ReceiveTaskHandle = osThreadCreate(osThread(ReceiveTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}


/**
 * @brief  Function implementing the HpSendTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_hpSendTask_init */
void hpSendTask_init(void const *argument)
{
	char msg[100];
	int value = 100;

	for (;;) {
		strcpy(msg, "Entering HP Send Task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		strcpy(msg, "Sending Data to Queue\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osMessagePut(myQueueHandle, value, osWaitForever);
		strcpy(msg, "Leaving HP Send Task\r\n\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osDelay(2000);
	}
}


/**
 * @brief Function implementing the LpSendTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_lpSendTask_init */
void lpSendTask_init(void const *argument)
{
	char msg[100];
	int value = 20;

	for (;;) {
		strcpy(msg, "Entering LP Send Task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		strcpy(msg, "Sending Data to Queue\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osMessagePut(myQueueHandle, value, osWaitForever);
		strcpy(msg, "Leaving LP Send Task\r\n\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osDelay(3000);
	}
}


/**
 * @brief Function implementing the ReceiveTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_receiveTask_init */
void receiveTask_init(void const *argument)
{
	osEvent msgQueue;
	char msg[100];

	for (;;) {
		strcpy(msg, "Entering Receiving Task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		strcpy(msg, "Getting Data from Queue\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		msgQueue = osMessageGet(myQueueHandle, osWaitForever);
		sprintf(msg, "Received Data: %ld\r\n", msgQueue.value.v);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		strcpy(msg, "Leaving Receiving Task\r\n\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
		osDelay(1000);
	}
}

