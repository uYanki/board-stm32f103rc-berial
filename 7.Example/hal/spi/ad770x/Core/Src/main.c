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
#include "dac.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "ad770x/ad770x.h"
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

void dac_set(uint16_t val)
{
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, val);
}

#define AD770X_CH1  1
#define AD770X_CH2  1
#define AD770X_VREF 2.5f

#define AD770X_PRINT_FREQ_ONLY 1

void ad770x_print(void)
{
    static uint32_t t   = 0;
    static uint8_t  cnt = 0;
    static float    hz  = 0;
    static uint16_t dac = 0;

#if AD770X_PRINT_FREQ_ONLY

#if AD770X_CH1
    // ad770x_wait_data_ready(AD7705_CHANNEL_AIN1P_AIN1N);
    ad770x_read_adc(AD7705_CHANNEL_AIN1P_AIN1N);
#endif
#if AD770X_CH2
    // ad770x_wait_data_ready(AD7705_CHANNEL_AIN2P_AIN2N);
    ad770x_read_adc(AD7705_CHANNEL_AIN2P_AIN2N);
#endif

    // ch1: 50hz -> 0.02s,   ch2: 60hz -> 0.016s,  t_total = t_ch1 + t_ch2 = 0.036s => 8.9hz
    // ch1: 250hz -> 0.004s, ch2: 250hz -> 0.004s, t_total = t_ch1 + t_ch2 = 0.008s => 39.64hz

#else

    printf("%.2f,%.2f,%.2f,%.2f\r\n", hz,

#if 0
           dac * 3.3f / 4095,
#else
           HAL_DAC_GetValue(&hdac, DAC_CHANNEL_2) * 3.3f / 4095,
#endif

#if AD770X_CH1
           ad770x_read_adc(AD7705_CHANNEL_AIN1P_AIN1N) * AD770X_VREF / 65535,
#else
           0.0f,
#endif

#if AD770X_CH2
           ad770x_read_adc(AD7705_CHANNEL_AIN2P_AIN2N) * AD770X_VREF / 65535
#else
           0.0f
#endif
    );

    dac += 5;
    if (dac > 4095)
        dac = 0;

    dac_set(dac);

#endif

    uint32_t ms = HAL_GetTick();

    if (t == 0) {
        t = ms;
    } else if ((ms - t) >= 1000) {
        hz  = 1000.0f * cnt / (ms - t);
        t   = ms;
        cnt = 0;
#if AD770X_PRINT_FREQ_ONLY
        printf("%.2f\r\n", hz);
#endif
    } else {
        ++cnt;
        return;
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
    MX_USART2_UART_Init();
    MX_SPI3_Init();
    MX_DAC_Init();
    /* USER CODE BEGIN 2 */

    ad770x_reset();
    ad770x_sync_spi();

#if AD770X_CH1
    ad770x_config_channel(
        AD7705_CHANNEL_AIN1P_AIN1N,
        AD770X_MODE_CALIBRATION_SELF,
        AD770X_CLOCK_4_9152_MHZ,
        AD770X_GAIN_2,
        AD770X_POLARITY_BIPOLAR,
        AD770X_UPDATE_RATE_50_HZ);
#endif
#if AD770X_CH2
    ad770x_config_channel(
        AD7705_CHANNEL_AIN2P_AIN2N,
        AD770X_MODE_CALIBRATION_SELF,
        AD770X_CLOCK_4_9152_MHZ,
        AD770X_GAIN_8,
        AD770X_POLARITY_BIPOLAR,
        AD770X_UPDATE_RATE_60_HZ);
#endif

    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        ad770x_print();
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
