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
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"

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



#define SEMIHOSTING 1  // 使能打印

// 支持向量机实例化结构体，所有参数可以通过python生成
arm_svm_polynomial_instance_f32 params;

// 通过使用scikit-learn包和一些随机输入数据对SVM分类器进行训练生成的参数
#define NB_SUPPORT_VECTORS 11  // 向量个数

// 向量空间的维数
#define VECTOR_DIMENSION 2

const float32_t dualCoefficients[NB_SUPPORT_VECTORS] =
    {-0.01628988f, -0.0971605f,
     -0.02707579f, 0.0249406f, 0.00223095f, 0.04117345f,
     0.0262687f, 0.00800358f, 0.00581823f, 0.02346904f, 0.00862162f}; /* Dual coefficients */

const float32_t supportVectors[NB_SUPPORT_VECTORS * VECTOR_DIMENSION] =
    {1.2510991f, 0.47782799f,
     -0.32711859f, -1.49880648f, -0.08905047f, 1.31907242f,
     1.14059333f, 2.63443767f, -2.62561524f, 1.02120701f,
     -1.2361353f, -2.53145187f,
     2.28308122f, -1.58185875f, 2.73955981f, 0.35759327f,
     0.56662986f, 2.79702016f,
     -2.51380816f, 1.29295364f, -0.56658669f, -2.81944734f}; /* Support vectors */

// 类A标识为“0”，类B标识为“1”
const int32_t classes[2] = {0, 1};

void dsp_test(void) {
    /* 输入数据 */
    float32_t in[VECTOR_DIMENSION];

    /* 分类器结果 */
    int32_t result;

    // 初始化支持向量机实例化结构体
    arm_svm_polynomial_init_f32(
        &params,             // 实例化结构体
        NB_SUPPORT_VECTORS,  // 向量个数
        VECTOR_DIMENSION,    // 向量维数
        -1.661719f,          // 截距
        dualCoefficients,    // 双系数
        supportVectors,      // 支持向量
        classes,             // 类ID标识
        3,                   // 多项式次数
        1.100000f,           // python scikit-learn包专用系数
        0.500000f            // python scikit-learn包专用系数
    );

    // 输入测试数据，类A
    in[0] = 0.4f;
    in[1] = 0.1f;

    arm_svm_polynomial_predict_f32(&params, in, &result);

// 根据分类标识：如果分类器结果为A，则标识必为“0”
#if defined(SEMIHOSTING)
    printf("Result = %d\r\n", result);
#endif

    // 输入测试数据，类B
    in[0] = 3.0f;
    in[1] = 0.0f;

    arm_svm_polynomial_predict_f32(&params, in, &result);

// 根据分类标识：如果分类器结果为A，则标识必为“1”
#if defined(SEMIHOSTING)
    printf("Result = %d\r\n", result);
#endif
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
    /* USER CODE BEGIN 2 */
    printf("Strat\r\n");
    dsp_test();
    printf("End\r\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
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
