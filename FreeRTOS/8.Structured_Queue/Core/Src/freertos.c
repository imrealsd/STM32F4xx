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
#include "usart.h"
#include "string.h"
#include "stdio.h"

typedef struct {

	int id;
	int value;

} myStruct;


/* USER CODE END Variables */
osThreadId SenderTaskHandle;
osThreadId ReceiverTaskHandle;
osMessageQId myStructuredQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void senderTask_init(void const *argument);
void receiverTask_init(void const *argument);

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
	/* Create the queue(s) */
	/* definition and creation of myStructuredQueue */
	osMessageQDef(myStructuredQueue, 5, myStruct);
	myStructuredQueueHandle = osMessageCreate(osMessageQ(myStructuredQueue),
			NULL);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of SenderTask */
	osThreadDef(SenderTask, senderTask_init, osPriorityAboveNormal, 0, 128);
	SenderTaskHandle = osThreadCreate(osThread(SenderTask), NULL);

	/* definition and creation of ReceiverTask */
	osThreadDef(ReceiverTask, receiverTask_init, osPriorityNormal, 0, 128);
	ReceiverTaskHandle = osThreadCreate(osThread(ReceiverTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_senderTask_init */
/**
 * @brief  Function implementing the SenderTask thread.
 * @param  argument: Not used
 * @retval None
 */
void senderTask_init(void const *argument)
{
	char msg[100];
	myStruct txStruct = {404,890};

	for (;;) {

		strcpy(msg, "Entering into sender task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		osMessagePut(myStructuredQueueHandle, (uint32_t ) &txStruct, osWaitForever);
		strcpy(msg, "Data is written into queue\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		strcpy(msg, "Leaving sender Task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		osDelay(1000);
	}
}

/* USER CODE BEGIN Header_receiverTask_init */
/**
 * @brief Function implementing the ReceiverTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_receiverTask_init */
void receiverTask_init(void const *argument)
{
	char msg[100];
	osEvent rxData;
	int ID;
	int VAL;

	for (;;) {

		strcpy(msg, "Entering into receiver task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		rxData = osMessageGet(myStructuredQueueHandle, osWaitForever);
		strcpy(msg, "Data read from queue\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		ID =  ((myStruct *) rxData.value.p)->id;
		VAL = ((myStruct *) rxData.value.p)->value;

		sprintf(msg, "Received Data - ID: %d, Value: %d\r\n", ID, VAL);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		strcpy(msg, "Leaving receiver Task\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

		osDelay(4000);
	}
}


