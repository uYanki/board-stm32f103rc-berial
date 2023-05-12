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
#include "video.h"
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
int main(void)
{
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

#define FRAME_SIZE   40  // 40 byte
#define FRAME_START  40  // start frame
#define FRAME_SKIP   0   // skip n frame per step
#define BUFFER_ADDR  video_badapple
#define BUFFER_STEP  (FRAME_SIZE * FRAME_SKIP)
#define BUFFER_START (FRAME_SIZE * FRAME_START)
#define BUFFER_END   sizeof(BUFFER_ADDR)

    uint32_t j = BUFFER_START;

    int      frames   = 0;
    int      lrcstate = 0;
    uint32_t tstart;

    while (1) {
        // 1 frame: 40(5*8) bytes to 64 bytes(8*8)
        for (uint8_t i = 0; i < 8; i++, j += 5) {
            const uint8_t* p = &BUFFER_ADDR[j];
            // 5 byte -> 8 byte
            uint8_t image[8] = {
#if MODE
                // 0:00011111
                p[0],
                // 0:11100000 1:00000011
                p[0] >> 5 | p[1] << 3,
                // 1:01111100
                p[1] >> 2,
                // 1:10000000 2:00001111
                p[1] >> 7 | p[2] << 1,
                // 2:11110000 3:00000001
                p[2] >> 4 | p[3] << 4,
                // 3:00111110
                p[3] >> 1,
                // 3:11000000 4:00000111
                p[3] >> 6 | p[4] << 2,
                // 4:11111000
                p[4] >> 3
#else
                // 0:1111 1000
                p[0] >> 3,
                // 0:0000 0111  1:1100 0000
                ((p[0] & 0x07) << 2) | (p[1] >> 6),
                // 1:0011 1110
                (p[1] & 0x3E) >> 1,
                // 1:0000 0001  2:1111 0000
                ((p[1] & 0x01) << 4) | (p[2] >> 4),
                // 2:0000 1111  3:1000 0000
                ((p[2] & 0x0F) << 1) | (p[3] >> 7),
                // 3:0111 1100
                (p[3] & 0x7C) >> 2,
                // 3:0000 0011  4:1110 0000
                ((p[3] & 0x03) << 3) | (p[4] >> 5),
                // 4:0001 1111
                p[4] & 0x1F
#endif
            };
            LCD_CreateChar(i, image);
        }

        LCD_SetCursor(0, 0);
        for (int i = 0; i < 4; ++i)
            LCD_ShowChar(i);
        LCD_SetCursor(0, 1);
        for (int i = 4; i < 8; ++i)
            LCD_ShowChar(i);

        if (j >= BUFFER_END) {
            j = BUFFER_START;
        } else {
            j += BUFFER_STEP;
        }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
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
void Error_Handler(void)
{
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
