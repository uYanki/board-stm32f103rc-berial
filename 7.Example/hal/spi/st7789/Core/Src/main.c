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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "st7789/images.h"
#include "st7789/st7789.h"
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint8_t bl = 0;
    (bl = !bl) ? st7789_backlight_on() : st7789_backlight_off();
    printf("bl:%d\r\n", bl);
}
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
    MX_USART2_UART_Init();
    MX_SPI3_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    st7789_init();
    st7789_backlight_on();  // 开启背光（可配置PWM进行背光调节）

    uint16_t colors[] = {
        COLOR_RGB565_BLACK,
        COLOR_RGB565_WHITE,
        COLOR_RGB565_BLACK,
        COLOR_RGB565_BLUE,
        COLOR_RGB565_RED,
        COLOR_RGB565_MAGENTA,
        COLOR_RGB565_GREEN,
        COLOR_RGB565_CYAN,
        COLOR_RGB565_YELLOW,
        COLOR_RGB565_BROWN,
        COLOR_RGB565_BRRED,
        COLOR_RGB565_DARKBLUE,
        COLOR_RGB565_LIGHTBLUE,
        COLOR_RGB565_GRAYBLUE,
        COLOR_RGB565_LIGHTGREEN,
        COLOR_RGB565_LGRAY,
        COLOR_RGB565_LGRAYBLUE,
        COLOR_RGB565_LBBLUE,
    };

    uint8_t color_cnt = (sizeof(colors) / sizeof(colors[0]));

    uint8_t bInvert = 0;

    while (1) {
        for (uint8_t i = 0; i < color_cnt; ++i) {
            st7789_clear(colors[i]);  // 清屏
            HAL_Delay(300);
        }

        st7789_draw_string(0, 0, "hello, world", Font_7x10, COLOR_RGB565_RED, COLOR_RGB565_WHITE);    // 显示字符
        st7789_draw_string(0, 20, "hello, world", Font_11x18, COLOR_RGB565_RED, COLOR_RGB565_WHITE);  // 显示字符
        st7789_draw_string(0, 40, "hello, world", Font_16x26, COLOR_RGB565_RED, COLOR_RGB565_WHITE);  // 显示字符
        HAL_Delay(1000);

        int n = 50;
        while (--n) {
            st7789_fill_rect(
                rand() % ST7789_WIDTH,
                rand() % ST7789_HEIGHT,
                rand() % 40,
                rand() % 40,
                colors[rand() % color_cnt]);
            HAL_Delay(50);
        }
        HAL_Delay(1000);
        st7789_invert(bInvert = !bInvert);  // 反色
        HAL_Delay(2000);
        st7789_draw_image(20, 20, 128, 128, (uint16_t*)image_saber);  // 绘图
        HAL_Delay(1000);

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

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
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

    /** Initializes the CPU, AHB and APB buses clocks
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
