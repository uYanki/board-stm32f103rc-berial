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

#define F_COUNT 512
#define M       16

/* xn:输入的信号序列（列向量）
 * itr:迭代次数，标量，默认为xn的长度，M<itr<sizeof(xn)
 * en:误差序列（itr*1）列向量
 * dn:所期望的响应序列（列向量）
 * M:滤波器的阶数（标量）
 * mu:收敛因子（步长）标量
 * W:滤波器权值矩阵，大小为M*itr
 * yn:实际输出序列（列向量）*/

// float* LMS_Filter(int itr, const float *xn, const float *dn, float mu, int length);
void LMS_Filter(int itr, const float* xn, const float* dn, float mu, int length, float* yn)
{
    static int   i = 0;
    static int   k = 0;
    static float y = 0.0;
    static float en[F_COUNT];
    static float W[M][F_COUNT];
    static float x[M];

    /*创建一个en全零矩阵，en(k)表示第k次迭代时预期输出与实际输入的误差*/
    for (i = 0; i < itr; i++) {
        en[i] = 0;
    }

    /*创建一个W全零矩阵，每一行代表一个加权参量，每一列代表一次迭代*/
    for (i = 0; i < M; i++)
        for (k = 0; k < itr; k++)
            W[i][k] = 0;

    /*创建一个x全零矩阵*/
    for (i = 0; i < M; i++)
        x[i] = 0;

    /*迭代计算*/
    for (k = M; k <= itr; k++) {
        /* 滤波器M个抽头的输入：从xn第k-1个值倒序取出M个样点的值放入x
         * y为滤波器输出：W的第K-2列与x的积的和*/
        for (i = 0; i < M; i++) {
            x[i] = xn[k - i - 1];
            y += W[i][k - 2] * x[i];
        }

        en[k - 1] = dn[k - 1] - y;  // 第k次迭代的误差

        /*滤波器权值计算的迭代式*/
        for (i = 0; i < M; i++) {
            W[i][k - 1] = W[i][k - 2] + 2 * mu * en[k - 1] * x[i];
        }

        y = 0.0;
    }

    /*创建一个yn全无穷大矩阵，维数与xn一样*/
    for (i = 0; i < itr; i++) {
        yn[i] = 0.0;
    }

    /*求最优时滤波器的输出序列*/
    for (k = M; k <= length; k++) {
        for (i = 0; i < M; i++) {
            x[i] = xn[k - i - 1];
            y += W[i][itr - 1] * x[i];
        }

        yn[k - 1] = y;
        y         = 0.0;
    }
}

float xn[512] = {0.}, dn[512] = {0.}, yn[512] = {0.};

void dsp_test(void)
{
    uint16_t i = 0;
    for (i = 0; i < 512; i++)
        xn[i] = 1.5f * arm_sin_f32(100 * PI * i / 6400) + 0.5f * arm_sin_f32(900 * PI * i / 6400);

    for (i = 0; i < 512; i++)
        dn[i] = 1.5f * arm_sin_f32(100 * PI * i / 6400);

    LMS_Filter(128, xn, dn, 0.0008, 512, yn);

    for (i = 0; i < 512; i++)
        printf("%f	%f	%f\r\n", xn[i], dn[i], yn[i]);  // 原始数据,理想数据,滤波后的数据
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
