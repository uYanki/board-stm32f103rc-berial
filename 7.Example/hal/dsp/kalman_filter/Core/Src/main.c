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
#include "adc.h"
#include "dac.h"
#include "gpio.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "stdlib.h"
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

#define F_COUNT 512
#define M 16

// 卡尔曼滤波
typedef struct KalmanFilter {
    float x_mea;  // 测量值
    float x_est;  // 估计值
    float e_mea;  // 测量偏差
    float e_est;  // 估计偏差
    float Kk;     // 增益
} KalmanFilter;

#define Kk_calc(x, y) (x) / (x + y)

// 初始化卡尔曼滤波器
void Kalman_Init(KalmanFilter* kalmanFilter, float FirstMeaValue, float E_mea, float FirstEstValue, float E_est) {
    kalmanFilter->x_est = FirstEstValue;
    kalmanFilter->x_mea = FirstMeaValue;
    kalmanFilter->e_est = E_est;
    kalmanFilter->e_mea = E_mea;
    kalmanFilter->Kk    = Kk_calc(kalmanFilter->e_est, kalmanFilter->e_mea);
}

// 更新卡尔曼滤波器
void Kalman_Update(KalmanFilter* kalmanFilter, float newMeaValue) {
    float temp          = kalmanFilter->e_est;
    kalmanFilter->x_est = kalmanFilter->x_est + kalmanFilter->Kk * (newMeaValue - kalmanFilter->x_est);
    kalmanFilter->x_mea = newMeaValue;
    kalmanFilter->Kk    = Kk_calc(kalmanFilter->e_est, kalmanFilter->e_mea);
    kalmanFilter->e_est = (1 - kalmanFilter->Kk) * temp;
}

float RandomNumGenerator(int base, int range) {
    float k         = 0.0;
    float randomNum = 0.0;
    k               = 2 * range * 10;
    randomNum       = rand() % (int)k;
    k               = base - range + (randomNum / 10);
    return k;
}

uint16_t read_adc() {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 50);  // timeout = 50ms
    while (HAL_IS_BIT_CLR(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) {}
    return HAL_ADC_GetValue(&hadc1);
}

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
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    MX_DAC_Init();
    /* USER CODE BEGIN 2 */

    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 1024);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);

    HAL_ADCEx_Calibration_Start(&hadc1);  // ADC 校准 (必要的)

    KalmanFilter k;
    Kalman_Init(&k, 1024, 2, 1024, 2);
    // 因为真实值在 DAC 输出值附近，所以设置初值为 1024，偏差为 2

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        uint16_t val = read_adc();
        Kalman_Update(&k, val);
        printf("%d	%f	%f\r\n", val, k.x_mea, k.x_est);
        HAL_Delay(100);
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
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = {0};

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
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
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
