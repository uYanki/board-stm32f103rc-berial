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
#include <stdlib.h>
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

// 强化学习

//  符号常数的定义
#define GENMAX 1000   // 学习的重复次数
#define NODENO 15     // Q值的节点数
#define ALPHA 0.1f    // 学习系数
#define GAMMA 0.9f    // 折扣率
#define EPSILON 0.3f  // 确定行动选择的随机性
#define SEED 32767    // 随机数的seed

//  函数原型声明
int    rand100();                           // 返回0～100的随机函数
int    rand01();                            // 返回0、1的随机函数
double rand1();                             // 返回0～1的实数的随机函数
void   printqvalue(int qvalue[NODENO]);     // 输出Q值
int    selecta(int s, int qvalue[NODENO]);  // 行动选择
int    updateq(int s, int qvalue[NODENO]);  // 更新Q值

// 更新Q值
int updateq(int s, int qvalue[NODENO]) {
    int qv;    // 要更新的Q值
    int qmax;  // Q值的最大值

    // 最末端的情形
    if (s > 6) {
        if (s == 14)  // 给予奖赏
            qv = qvalue[s] + ALPHA * (1000 - qvalue[s]);
        // 给予奖赏的节点Q值增加
        // 其他节点的Q值追加时
        // 去掉下面2行的注释
        //  else if(s==11)// 给予奖赏
        //   qv=qvalue[s]+ALPHA*(500-qvalue[s]) ;
        else  // 无奖赏
            qv = qvalue[s];
    }
    // 最末端之外
    else {
        if ((qvalue[2 * s + 1]) > (qvalue[2 * s + 2]))
            qmax = qvalue[2 * s + 1];
        else
            qmax = qvalue[2 * s + 2];
        qv = qvalue[s] + ALPHA * (GAMMA * qmax - qvalue[s]);
    }

    return qv;
}

// 行动选择
int selecta(int olds, int qvalue[NODENO]) {
    int s;

    // 基于ε-greedy方法的行动选择
    if (rand1() < EPSILON) {
        // 随机行动
        if (rand01() == 0)
            s = 2 * olds + 1;
        else
            s = 2 * olds + 2;
    } else {
        // 选择Q值的最大值
        if ((qvalue[2 * olds + 1]) > (qvalue[2 * olds + 2]))
            s = 2 * olds + 1;
        else
            s = 2 * olds + 2;
    }

    return s;
}

// 输出Q值
void printqvalue(int qvalue[NODENO]) {
    for (int i = 1; i < NODENO; ++i)
        printf("%d\t", qvalue[i]);
    printf("\r\n");
}

// 返回0～1的实数的随机函数
double rand1() {
    // 随机数的计算
    return (double)rand() / RAND_MAX;
}

// 返回0、1的随机函数
int rand01() {
    int rnd;
    // 去除随机数的最大值
    while ((rnd = rand()) == RAND_MAX) {}
    // 随机数的计算
    return (int)((double)rnd / RAND_MAX * 2);
}

// 返回0～100的随机函数
int rand100() {
    int rnd;
    // 去除随机数的最大值
    while ((rnd = rand()) == RAND_MAX) {}
    // 随机数的计算
    return (int)((double)rnd / RAND_MAX * 101);
}

void dsp_test(void) {
    int i;
    int s;               // 状态
    int t;               // 时刻
    int qvalue[NODENO];  // Q值

    srand(SEED);  // 随机数的初始化

    // Q值的初始化
    for (i = 0; i < NODENO; ++i)
        qvalue[i] = rand100();
    printqvalue(qvalue);

    // 学习的主体
    for (i = 0; i < GENMAX; ++i) {
        s = 0;  // 行动的初始状态
        for (t = 0; t < 3; ++t) {
            // 到最末端为止重复进行
            // 行动选择
            s = selecta(s, qvalue);

            // Q值的更新
            qvalue[s] = updateq(s, qvalue);
        }
        // Q值的输出
        printqvalue(qvalue);
    }
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
