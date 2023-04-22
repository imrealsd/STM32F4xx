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
osThreadId LpTaskHandle;
osThreadId HpTaskHandle;
osThreadId MpTaskHandle;
osMutexId myMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void lpTask_init(void const *argument);
void hpTask_init(void const *argument);
void mpTask_init(void const *argument);

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

	char msg[100];
	/* definition and creation of myMutex */
	osMutexDef(myMutex);
	myMutexHandle = osMutexCreate(osMutex(myMutex));

	if (myMutexHandle != NULL) {
		memset(msg, 0, 100);
		strcpy(msg, "Mutex Created\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
	}

	/* Create the thread(s) */
	/* definition and creation of LpTask */
	osThreadDef(LpTask, lpTask_init, osPriorityBelowNormal, 0, 128);
	LpTaskHandle = osThreadCreate(osThread(LpTask), NULL);

	if (LpTaskHandle != NULL) {
		memset(msg, 0, 100);
		strcpy(msg, "LP Task Created\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
	}

	/* definition and creation of HpTask */
	osThreadDef(HpTask, hpTask_init, osPriorityAboveNormal, 0, 128);
	HpTaskHandle = osThreadCreate(osThread(HpTask), NULL);

	if (HpTaskHandle != NULL) {
		memset(msg, 0, 100);
		strcpy(msg, "HP Task Created\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
	}

	/* definition and creation of MpTask */
	osThreadDef(MpTask, mpTask_init, osPriorityNormal, 0, 128);
	MpTaskHandle = osThreadCreate(osThread(MpTask), NULL);

	if (MpTaskHandle != NULL) {
		memset(msg, 0, 100);
		strcpy(msg, "MP Task Created\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
	}
}

void uart1CritcalResource(char *str)
{
	char *msg1 = "Entered Critical Resource\r\n";
	char *msg2 = "Leaving Critical Resource\r\n";

	HAL_UART_Transmit(&huart1, (uint8_t *)msg1, strlen(msg1), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)str,  strlen(str), HAL_MAX_DELAY);
	/*HAL_Delay(..) is polling mode delay (just checking-witing for some flag) & it will not lead to context switching*/
	HAL_Delay(2000);
	HAL_UART_Transmit(&huart1, (uint8_t *)msg2, strlen(msg2), HAL_MAX_DELAY);
}

/**
 * @brief  Function implementing the LpTask thread.
 * @param  argument: Not used
 * @retval None
 */
void lpTask_init(void const *argument)
{
	char *msg1 = "Entered LP : Waiting For Mutex\r\n";
	char *msg2 = "Mutex Acquired\r\n";
	char *msg3 = "Leaving LP\r\n";
	char *msg4 = ":::::::: OWNER : LP TASK :::::::::\r\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *)msg1, strlen(msg1), HAL_MAX_DELAY);
		osMutexWait(myMutexHandle, osWaitForever);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg2, strlen(msg2), HAL_MAX_DELAY);
		uart1CritcalResource(msg4);
		osMutexRelease(myMutexHandle);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg3, strlen(msg3), HAL_MAX_DELAY);
		osDelay(1500);
	}
}


/**
 * @brief Function implementing the HpTask thread.
 * @param argument: Not used
 * @retval None
 */
void hpTask_init(void const *argument)
{
	char *msg1 = "Entered HP : Waiting For Mutex\r\n";
	char *msg2 = "Mutex Acquired\r\n";
	char *msg3 = "Leaving HP\r\n";
	char *msg4 = ":::::::: OWNER : HP TASK :::::::::\r\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *)msg1, strlen(msg1), HAL_MAX_DELAY);
		osMutexWait(myMutexHandle, osWaitForever);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg2, strlen(msg2), HAL_MAX_DELAY);
		uart1CritcalResource(msg4);
		osMutexRelease(myMutexHandle);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg3, strlen(msg3), HAL_MAX_DELAY);
		osDelay(1000);
	}
}


/**
 * @brief Function implementing the MpTask thread.
 * @param argument: Not used
 * @retval None
 */
void mpTask_init(void const *argument)
{
	char *msg1 = "Entered MP\r\n";
	char *msg3 = "Leaving MP\r\n";

	for (;;) {
		HAL_UART_Transmit(&huart1, (uint8_t *)msg1, strlen(msg1), HAL_MAX_DELAY);
		HAL_Delay(2000);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg3, strlen(msg3), HAL_MAX_DELAY);
		osDelay(700);
	}
}

