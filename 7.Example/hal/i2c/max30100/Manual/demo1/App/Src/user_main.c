/**
 ******************************************************************************
 * @file    user_main.c
 * @author  张东
 * @version V1.0.0
 * @date    2019-12-27
 * @brief   应用程序逻辑入口
 ******************************************************************************
 */
/*--Include-start-------------------------------------------------------------*/
#include "user_main.h"
#include "MAX30100.h"
#include "ST7735.h"
#include "blood.h"
#include "dma.h"
#include "gui.h"
#include "spi.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_dma.h"
#include "tim.h"
/*Include end ----------------------------------------------------------------*/

/*Global Data Space ----------------------------------------------------------*/

// 程序初始化
void User_Setup(void) {
    // chip init
    //__HAL_SPI_ENABLE(&hspi2);
    //__HAL_DMA_ENABLE_IT(&hdma_spi2_tx,DMA_IT_TC);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    // device init
    Lcd_Init();
    Lcd_Clear(BLACK);

    max30100_init();

    // app init
    blood_Setup();
}

// 程序循环
void User_Loop(void) {
    blood_Loop();
}

// 程序中断
void User_Interrupt(void) {
    blood_Interrupt();
}
