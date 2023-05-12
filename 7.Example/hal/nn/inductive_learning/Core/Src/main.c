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

// 归纳学习

#define OK 1
#define NG 0
#define SETSIZE 100   // 学习数据集的大小
#define CNO 10        // 学习数据的位数（10）
#define GENMAX 10000  // 生成候选解的次数
#define SEED 32767    // 随机数的seed

// 读入学习数据集
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE]);

// 返回0、1、2的随机函数
int rand012();

// 计算候选解模式的得分（0～SETSIZE）
int calcscore(int data[SETSIZE][CNO], int teacher[SETSIZE], int answer[CNO]);

// 候选解模式的得分（0～SETSIZE）的计算
int calcscore(int data[SETSIZE][CNO], int teacher[SETSIZE], int answer[CNO]) {
    int score = 0;  // 得分（0～SETSIZE）
    int point;      // 相一致的位数(0～CNO)
    int i, j;

    for (i = 0; i < SETSIZE; ++i) {
        // 计算一致程度
        point = 0;
        for (j = 0; j < CNO; ++j) {
            if (answer[j] == 2)
                ++point;  // 通配符
            else if (answer[j] == data[i][j])
                ++point;  // 相一致
        }

        if ((point == CNO) && (teacher[i] == 1)) {
            ++score;
        } else if ((point != CNO) && (teacher[i] == 0)) {
            ++score;
        }
    }
    return score;
}

// 读入学习数据集
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE]) {
    int i, j;
    for (i = 0; i < SETSIZE; ++i) {
        for (j = 0; j < CNO; ++j)
            scanf("%d", &data[i][j]);
        scanf("%d", &teacher[i]);
    }
}

// 返回0、1、2的随机函数
int rand012() {
    int rnd;

    // 去除随机数的最大值
    while ((rnd = rand()) == RAND_MAX)
        ;
    // 计算随机数
    return (double)rnd / RAND_MAX * 3;
}

// 学习数据集
int data[SETSIZE][CNO] = {
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 0, 0, 0, 1, 1, 0, 1, 0},
    {1, 0, 0, 1, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 1, 1, 1, 0, 0, 1, 1, 0, 1},
    {0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {0, 0, 1, 1, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 1},
    {0, 1, 1, 0, 1, 0, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 1, 0},
    {1, 0, 1, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 1, 1, 0},
    {1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {0, 1, 1, 1, 1, 0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 1, 1, 1, 1, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 0, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {0, 1, 1, 0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 0, 1, 1, 1, 0, 1, 1},
    {0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 1},
    {0, 1, 0, 1, 1, 1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0, 0, 1, 0, 1, 1},
    {1, 1, 1, 0, 0, 1, 0, 1, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 1, 0, 1, 0, 1},
    {0, 0, 0, 1, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 1, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 0, 0, 0, 1, 1},
    {0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 0, 0, 1},
    {0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 1, 1, 0, 1, 1, 1},
    {0, 0, 1, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 1}};

// 教师数据
int teacher[SETSIZE] = {1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};

void dsp_test(void) {
    int i, j;
    int score = 0;    // 得分（0～SETSIZE）
    int answer[CNO];  // 候选解

    int bestscore = 0;    // 得分的最优值
    int bestanswer[CNO];  // 搜索过程中的最优解

    srand(SEED);  // 随机数的初始化

    // 读入学习数据集
    // readdata(data, teacher);

    // 生成候选解并检查
    for (i = 0; i < GENMAX; ++i) {
        // 生成候选解
        for (j = 0; j < CNO; ++j)
            answer[j] = rand012();

        // 检查
        score = calcscore(data, teacher, answer);

        // 最优得分的更新
        if (score > bestscore) {  // 如果是到目前为止的最优值则更新
            for (j = 0; j < CNO; ++j)
                bestanswer[j] = answer[j];
            bestscore = score;
            for (j = 0; j < CNO; ++j)
                printf("%1d ", bestanswer[j]);
            printf(":score=%d\r\n", bestscore);
        }
    }
    // 输出最优解
    printf("\r\n最优解\r\n");
    for (j = 0; j < CNO; ++j)
        printf("%1d ", bestanswer[j]);
    printf(":score=%d\r\n", bestscore);
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
