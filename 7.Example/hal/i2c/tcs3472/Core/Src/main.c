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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcs3472x/tcs3472x.h"
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
    static uint32_t t = 0;
    if (t) println("interval = %f s", (HAL_GetTick() - t) / 1000.f);
    t = HAL_GetTick();  // 计算中断间隔时间

    uint16_t rgb565;
    uint32_t rgb888;
    uint16_t red, green, blue, clear;
    if (tcs3472x_read_rgbc(&red, &green, &blue, &clear)) {
        tcs3472x_get_rgb565_rgb888(red, green, blue, &rgb565, &rgb888);
        println("%d\t%d\t%d", rgb888 >> 16, (rgb888 >> 8) & 0xFF, rgb888 & 0xFF);
    } else {
        println("fail to read rgbc");
    }
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
    MX_I2C2_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

#define PRINT_FREQ 0  // 频率
#define PRINT_LUX  0  // 流名
#define PRINT_CCT  1  // 色温
#define PRINT_RGB  0  //
#define PRINT_IRQ  0  // 中断

    uint16_t red, green, blue, clear;
		
    if (!tcs3472x_init()) {
        i2cdrv_detector(tcs3472x.bus, tcs3472x.drv);
        println("can't find tcs3472x");
        while (1) {}
    }

#if PRINT_FREQ
    uint32_t t = HAL_GetTick(), cnt = 0;
    // 采样间隔 = 等待时间 + 积分时间
    tcs3472x_set_wait_enable(true);
    tcs3472x_set_wait_time(TCS3472X_WAIT_TIME_204MS);
    tcs3472x_set_integration_time(TCS3472X_INTEGRATION_TIME_154MS);
#endif

#if PRINT_RGB
    tcs3472x_set_gain(TCS3472X_GAIN_16X);
#endif
#if PRINT_IRQ
    tcs3472x_set_wait_enable(true);
    // tcs3472x_set_wait_enable(false);
    tcs3472x_set_wait_time(TCS3472X_WAIT_TIME_2450MS);
#if 1
    tcs3472x_set_interrupt_mode(TCS3472X_PERS_NONE);
#else
    tcs3472x_set_interrupt_mode(TCS3472X_PERS_2_CYCLE);
    tcs3472x_set_clear_low_limit(2000);
    tcs3472x_set_clear_high_limit(8000);
#endif
    tcs3472x_set_integration_time(TCS3472X_INTEGRATION_TIME_700MS);
    tcs3472x_set_interrupt_enable(true);
    if (tcs3472x_read_rgbc(&red, &green, &blue, &clear)) {
    } else {
        println("fail to read rgbc");
    }

    while (1) {}
#endif
    while (1) {
        if (tcs3472x_read_rgbc(&red, &green, &blue, &clear)) {
#if PRINT_FREQ
            // freq = 1 / (integration time + wait time)
            println("%.4f", ++cnt / ((HAL_GetTick() - t) / 1000.0));  // hz
#endif
#if PRINT_LUX
            uint16_t lux = tcs3472x_calculate_lux(red, green, blue, clear);
            println("%d", lux);  // println("lux:%d", lux);
#endif
#if PRINT_CCT
            uint16_t cct1 = calculate_color_temperature(red, green, blue);
            uint16_t cct2 = tcs3472x_calculate_color_temperature(red, green, blue, clear);
            uint16_t cct3 = tcs3472x_calculate_color_temperature_ex(red, green, blue);
            println("R:%d\tG:%d\tB:%d\tC:%d\tT:%d\tT:%d\tT:%d", red, green, blue, clear, cct1, cct2, cct3);
#endif
#if PRINT_RGB
            uint16_t rgb565;
            uint32_t rgb888;
            tcs3472x_get_rgb565_rgb888(red, green, blue, &rgb565, &rgb888);
            // 可用 python/app.py 查看颜色
            println("%d\t%d\t%d", rgb888 >> 16, (rgb888 >> 8) & 0xFF, rgb888 & 0xFF);
#endif
        } else {
            println("fail to read rgbc");
        }
#if 1
        tcs3472x_delay_integration_time();
        tcs3472x_delay_wait_time();
#endif
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
