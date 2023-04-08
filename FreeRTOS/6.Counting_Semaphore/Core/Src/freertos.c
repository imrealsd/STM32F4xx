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
#include "gpio.h"
#include <string.h>

char rxBuff;


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
osThreadId HighTaskHandle;
osThreadId MediumTaskHandle;
osThreadId LowTaskHandle;
osThreadId VLowTaskHandle;
osSemaphoreId myCountingSemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void hightask_init(void const *argument);
void mediumTask_init(void const *argument);
void lowTask_init(void const *argument);
void vLowTask_init(void const *argument);

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

	/* Create the semaphores(s) */
	/* definition and creation of myCountingSem */
	osSemaphoreDef(myCountingSem);
	myCountingSemHandle = osSemaphoreCreate(osSemaphore(myCountingSem), 3);

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of HighTask */
	osThreadDef(HighTask, hightask_init, osPriorityAboveNormal, 0, 128);
	HighTaskHandle = osThreadCreate(osThread(HighTask), NULL);

	/* definition and creation of MediumTask */
	osThreadDef(MediumTask, mediumTask_init, osPriorityNormal, 0, 128);
	MediumTaskHandle = osThreadCreate(osThread(MediumTask), NULL);

	/* definition and creation of LowTask */
	osThreadDef(LowTask, lowTask_init, osPriorityBelowNormal, 0, 128);
	LowTaskHandle = osThreadCreate(osThread(LowTask), NULL);

	/* definition and creation of VLowTask */
	osThreadDef(VLowTask, vLowTask_init, osPriorityLow, 0, 128);
	VLowTaskHandle = osThreadCreate(osThread(VLowTask), NULL);

	HAL_UART_Receive_IT(&huart1, (uint8_t *) &rxBuff, sizeof(rxBuff));

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_hightask_init */
/**
 * @brief  Function implementing the HighTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_hightask_init */
void hightask_init(void const *argument)
{
	char *pMsg0 = "Entering High Task\r\n";
	char *pMsg1 = "Semaphore Acquired By High Task\r\n";
	char *pMsg2 = "Semaphore Released By High Task\r\n";
	char *pMsg3 = "Leaving High Task\r\n\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg0, strlen(pMsg0), 100);
		osSemaphoreWait(myCountingSemHandle, osWaitForever);
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg1, strlen(pMsg1), 100);
//		osSemaphoreRelease(myCountingSemHandle);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg2, strlen(pMsg2), 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg3, strlen(pMsg3), 100);
		osDelay(1000);
	}
}

/* USER CODE BEGIN Header_mediumTask_init */
/**
 * @brief Function implementing the MediumTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_mediumTask_init */
void mediumTask_init(void const *argument)
{
	char *pMsg0 = "Entering Medium Task\r\n";
	char *pMsg1 = "Semaphore Acquired By Medium Task\r\n";
	char *pMsg2 = "Semaphore Released By Medium Task\r\n";
	char *pMsg3 = "Leaving Medium Task\r\n\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg0, strlen(pMsg0), 100);
		osSemaphoreWait(myCountingSemHandle, osWaitForever);
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg1, strlen(pMsg1), 100);
//		osSemaphoreRelease(myCountingSemHandle);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg2, strlen(pMsg2), 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg3, strlen(pMsg3), 100);
		osDelay(1500);
	}
}

/* USER CODE BEGIN Header_lowTask_init */
/**
 * @brief Function implementing the LowTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_lowTask_init */
void lowTask_init(void const *argument)
{
	char *pMsg0 = "Entering Low Task\r\n";
	char *pMsg1 = "Semaphore Acquired By Low Task\r\n";
	char *pMsg2 = "Semaphore Released By Low Task\r\n";
	char *pMsg3 = "Leaving Low Task\r\n\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg0, strlen(pMsg0), 100);
		osSemaphoreWait(myCountingSemHandle, osWaitForever);
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg1, strlen(pMsg1), 100);
//		osSemaphoreRelease(myCountingSemHandle);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg2, strlen(pMsg2), 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg3, strlen(pMsg3), 100);
		osDelay(1200);
	}
}

/* USER CODE BEGIN Header_vLowTask_init */
/**
 * @brief Function implementing the VLowTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vLowTask_init */
void vLowTask_init(void const *argument)
{
	char *pMsg0 = "Entering Very Low Task\r\n";
	char *pMsg1 = "Semaphore Acquired By Very Low Task\r\n";
	char *pMsg2 = "Semaphore Released By Very Low Task\r\n";
	char *pMsg3 = "Leaving Very Low Task\r\n\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg0, strlen(pMsg0), 100);
		osSemaphoreWait(myCountingSemHandle, osWaitForever);
		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg1, strlen(pMsg1), 100);
//		osSemaphoreRelease(myCountingSemHandle);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg2, strlen(pMsg2), 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *) pMsg3, strlen(pMsg3), 100);
		osDelay(1600);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(&huart1, (uint8_t *) &rxBuff, sizeof(rxBuff));

	if ((rxBuff == 'r') || (rxBuff == 'R')) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		osSemaphoreRelease(myCountingSemHandle);
		osSemaphoreRelease(myCountingSemHandle);
		osSemaphoreRelease(myCountingSemHandle);
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}

}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
