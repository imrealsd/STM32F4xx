/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    wwdg.c
  * @brief   This file provides code for the configuration
  *          of the WWDG instances.
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
#include "wwdg.h"


WWDG_HandleTypeDef hwwdg;

/* WWDG init function */
void MX_WWDG_Init(void)
{
  /**
   * Max Window span: 127(0x7F) to 64(0x40): wathcdog counter has to be rewritten within this span
   * WDG input clk = HCLK/(8*4096) = 42000000/(8 * 4096) = (int) 1281 Hz : time period = 1/1281 sec  [4096 internal divider]
   * Max time delay : (1/1281) * (127-64) sec = 0.0499 sec = 49.9 mili sec
   * 
   * Now , Let window = 88
   * window span = 88 to 64 i.e  (127-88)*(1000/1281) mili sec to 49.9 mili sec = 30 ms to 49.9 ms
   * within this sapn counter has to be refreshed
   */
  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  hwwdg.Init.Window = 88;    
  hwwdg.Init.Counter = 127;
  hwwdg.Init.EWIMode = WWDG_EWI_DISABLE;

  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* wwdgHandle)
{

  if(wwdgHandle->Instance==WWDG)
  {
    __HAL_RCC_WWDG_CLK_ENABLE();
  }
}

