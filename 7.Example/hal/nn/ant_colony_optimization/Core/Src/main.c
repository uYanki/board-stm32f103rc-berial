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

#define NOA 10        // 蚂蚁的个体数
#define ILIMIT 50     // 循环次数
#define Q 3           // 信息素更新的常数
#define RHO 0.8       // 挥发常数
#define STEPS 10      // 路程的步数
#define EPSILON 0.15  // 确定行动选择的随机性
#define SEED 32768    // 随机数的Seed

void   printp(double pheromone[2][STEPS]);                                              // 显示
void   printmSTEPS(int mSTEPS[NOA][STEPS]);                                             // 显示蚂蚁的行动
void   walk(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]);    // 使蚂蚁步行
void   update(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]);  // 更新信息素
double rand1(void);                                                                     // 返回0～1实数的随机函数
int    rand01(void);                                                                    // 返回0、1的随机函数

// 更新信息素
void update(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]) {
    int    m;   // 蚂蚁个体的号码
    int    lm;  // 蚂蚁步行的距离
    int    i, j;
    double sum_lm = 0;  // 蚂蚁步行的总体距离

    // 信息素的挥发
    for (i = 0; i < 2; ++i)
        for (j = 0; j < STEPS; ++j)
            pheromone[i][j] *= RHO;

    // 蚂蚁的覆盖
    for (m = 0; m < NOA; ++m) {
        // 个体m的移动距离lm的计算
        lm = 0;
        for (i = 0; i < STEPS; ++i)
            lm += cost[mSTEPS[m][i]][i];

        // 信息素的覆盖
        for (i = 0; i < STEPS; ++i)
            pheromone[mSTEPS[m][i]][i] += Q * (1.0 / lm);
        sum_lm += lm;
    }
    // 蚂蚁步行的平均距离的输出
    printf("%lf\n", sum_lm / NOA);
}

// 使蚂蚁步行
void walk(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]) {
    int m;  // 蚂蚁个体的号码
    int s;  // 蚂蚁现在所处的位置

    for (m = 0; m < NOA; ++m) {
        for (s = 0; s < STEPS; ++s) {
            // 基于ε-greedy的行动选择
            if ((rand1() < EPSILON) || (abs(pheromone[0][s] - pheromone[1][s]) < 1e-9)) {
                // 随机行动
                mSTEPS[m][s] = rand01();
            } else {  // 基于信息素的选择
                if (pheromone[0][s] > pheromone[1][s])
                    mSTEPS[m][s] = 0;
                else
                    mSTEPS[m][s] = 1;
            }
        }
    }
    // 输出蚂蚁的行动
    printmSTEPS(mSTEPS);
}

// 显示蚂蚁的行动
void printmSTEPS(int mSTEPS[NOA][STEPS]) {
    int i, j;

    printf("mSTEPS\r\n");
    for (i = 0; i < NOA; ++i) {
        for (j = 0; j < STEPS; ++j)
            printf("%d ", mSTEPS[i][j]);
        printf("\r\n");
    }
}

// 显示信息素
void printp(double pheromone[2][STEPS]) {
    int i, j;

    for (i = 0; i < 2; ++i) {
        for (j = 0; j < STEPS; ++j)
            printf("%4.2lf ", pheromone[i][j]);
        printf("\r\n");
    }
}

// 返回0～1的实数的随机函数
double rand1(void) {
    // 随机数的计算
    return (double)rand() / RAND_MAX;
}

// 返回0、1的随机函数
int rand01(void) {
    int rnd;
    // 去除随机数的最大值
    while ((rnd = rand()) == RAND_MAX) {}
    // 计算随机数
    return (int)((double)rnd / RAND_MAX * 2);
}

void dsp_test(void) {
    // 各步的代价（距离）
    int cost[2][STEPS] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                          {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

    double pheromone[2][STEPS] = {0};  // 各步的信息素量
    int    mSTEPS[NOA][STEPS];         // 蚂蚁走过的过程
    int    i;                          // 循环次数的控制

    // 随机数的初始化
    srand(SEED);

    // 最优化的主体
    for (i = 0; i < ILIMIT; ++i) {
        // 信息素的状态的输出
        printf("%d:\r\n", i);
        printp(pheromone);
        // 使蚂蚁步行
        walk(cost, pheromone, mSTEPS);
        // 更新信息素
        update(cost, pheromone, mSTEPS);
    }
    // 信息素状态的输出
    printf("%d:\r\n", i);
    printp(pheromone);
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
