/**
  ******************************************************************************
  * File Name          : WWDG.c
  * Description        : This file provides code for the configuration
  *                      of the WWDG instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "wwdg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

WWDG_HandleTypeDef hwwdg;

/* WWDG init function */
void MX_WWDG_Init(void)
{

  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  hwwdg.Init.Window = 95;
  hwwdg.Init.Counter = 127;
  hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* wwdgHandle)
{

  if(wwdgHandle->Instance==WWDG)
  {
  /* USER CODE BEGIN WWDG_MspInit 0 */

  /* USER CODE END WWDG_MspInit 0 */
    /* WWDG clock enable */
    __HAL_RCC_WWDG_CLK_ENABLE();

    /* WWDG interrupt Init */
    HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
  /* USER CODE BEGIN WWDG_MspInit 1 */

  /* USER CODE END WWDG_MspInit 1 */
  }
}
 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
