/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD1602/lcd1602.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_I2C2_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    // 初始化(没成功显示就多按复位几次主控)

    LCD_Init();

    // 从左往右写字符串
    LCD_ShowStringEx(0, 0, "Hello");
    LCD_ShowStringEx(0, 1, "uyk!");

    // 光标闪烁
    LCD_CursorOn();
    LCD_BlinkOn();
    HAL_Delay(2000);

    // 清屏
    LCD_Clear();
    // 换行文本
    LCD_ShowStringWrap(17, 0, "hello,wrold.123456789");
    HAL_Delay(2000);

    // 显示全部字符
    LCD_Clear();
    for (int j = 0; j < 16; ++j) {
        LCD_ReturnHome();
        for (int i = 0; i < 16; ++i)
            LCD_ShowChar(j * 16 + i);
        HAL_Delay(2000);
        LCD_Clear();
    }

    // 清行
    LCD_ClearLine(0);

    // 显示自定义字符
    uint8_t bell[8]     = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
    uint8_t note[8]     = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
    uint8_t clock[8]    = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
    uint8_t heart[8]    = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
    uint8_t duck[8]     = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
    uint8_t check[8]    = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
    uint8_t cross[8]    = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
    uint8_t retarrow[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

    LCD_CreateChar(0, bell);
    LCD_CreateChar(1, note);
    LCD_CreateChar(2, clock);
    LCD_CreateChar(3, heart);
    LCD_CreateChar(4, duck);
    LCD_CreateChar(5, check);
    LCD_CreateChar(6, cross);
    LCD_CreateChar(7, retarrow);

    LCD_ReturnHome();
    for (int i = 0; i < 8; ++i)
        LCD_ShowChar(i);

    HAL_Delay(4000);
    LCD_ClearLine(0);

    // 文本右对齐光标
    LCD_AutoScrollOn();
    LCD_ShowStringEx(13, 0, "Hello");
    LCD_AutoScrollOff();  // 开了记得关

    // 光标归位
    HAL_Delay(1000);
    LCD_ReturnHome();
    HAL_Delay(2000);

    // 从右往左写字符串
    LCD_RightToLeft();
    LCD_ShowStringEx(LCD_DISPLAY_LENGTH - 1, 0, "Hello");
    LCD_ShowStringEx(LCD_DISPLAY_LENGTH - 1, 1, "uyk!");
    LCD_LeftToRight();

    // 字符滚动
    int8_t times = 3;
    while (times--) {
        HAL_Delay(400);
        LCD_ScrollLeft();
        HAL_Delay(400);
        LCD_ScrollLeft();
        HAL_Delay(400);
        LCD_ScrollRight();
        HAL_Delay(400);
        LCD_ScrollRight();
    }

    //屏幕背光
    HAL_Delay(1000);
    LCD_BacklightOff();
    HAL_Delay(1000);
    LCD_BacklightOn();

    // 结束
    LCD_Clear();
    LCD_ShowStringEx(0, 0, "ok");

    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
