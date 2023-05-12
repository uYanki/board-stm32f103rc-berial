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

// 和Visual Studio的互换性保证
#define _CRT_SECURE_NO_WARNINGS

// 基于GA搜索背包问题的最优解

// 符号常数的定义

#define MAXVALUE    100                 // 重量和价值的最大值
#define N           30                  // 行李个数
#define WEIGHTLIMIT (N * MAXVALUE / 4)  // 重量限制
#define POOLSIZE    30                  // 染色体个数
#define LASTG       50                  // 终止后代
#define MRATE       0.01                // 突变的概率
#define SEED        32767               // 随机数的seed
#define YES         1                   // yes对应的整数值
#define NO          0                   // no对应的整数值

// 行李的初始化
void initparcel();

// 计算适应度
int evalfit(int gene[]);

// 交叉
void mating(int pool[POOLSIZE][N], int ngpool[POOLSIZE * 2][N]);

// 突变
void mutation(int ngpool[POOLSIZE * 2][N]);

// 输出结果
void printp(int pool[POOLSIZE][N]);

// 生成初始染色体集合
void initpool(int pool[POOLSIZE][N]);

// 生成n之下的随机数
int rndn(int l);
// 真值的反转
int notval(int v);

// 选择父代
int selectp(int roulette[POOLSIZE], int totalfitness);

// 两个特定染色体的交叉
void crossing(int m[], int p[], int c1[], int c2[]);

void selectng(int ngpool[POOLSIZE * 2][N], int pool[POOLSIZE][N]);  // 选择下一代

// 行李数据 weight,value
int parcel[N][2] = {
    {65, 27},
    {39, 82},
    {9, 85},
    {72, 71},
    {87, 91},
    {91, 28},
    {34, 92},
    {58, 79},
    {3, 27},
    {12, 82},
    {92, 15},
    {39, 49},
    {83, 54},
    {76, 43},
    {6, 26},
    {77, 2},
    {68, 6},
    {24, 60},
    {60, 47},
    {6, 40},
    {91, 58},
    {44, 68},
    {50, 33},
    {91, 92},
    {57, 62},
    {97, 49},
    {96, 68},
    {39, 77},
    {89, 6},
    {24, 97}};  // 行李

// 行李的初始化
void initparcel()
{
    // int i = 0;
    // while ((i < N) && (scanf("%d %d", &parcel[i][0], &parcel[i][1]) != EOF)) ++i;
}

// 选择下一代
void selectng(int ngpool[POOLSIZE * 2][N], int pool[POOLSIZE][N])
{
    int i, j, c;                 // 循环控制参数
    int totalfitness = 0;        // 适应度的总计值
    int roulette[POOLSIZE * 2];  // 存放适应度
    int ball;                    // 球（选择位置的数值）
    int acc = 0;                 // 适应度的累积值

    // 循环进行选择
    for (i = 0; i < POOLSIZE; ++i) {
        // 生成轮盘
        totalfitness = 0;
        for (c = 0; c < POOLSIZE * 2; ++c) {
            roulette[c] = evalfit(ngpool[c]);
            // 计算适应度的总计值
            totalfitness += roulette[c];
        }
        // 选择一个染色体
        ball = rndn(totalfitness);
        acc  = 0;
        for (c = 0; c < POOLSIZE * 2; ++c) {
            acc += roulette[c];     // 累积评价值
            if (acc > ball) break;  // 对应的基因
        }

        // 染色体的复制
        for (j = 0; j < N; ++j)
            pool[i][j] = ngpool[c][j];
    }
}

// 父代的选择
int selectp(int roulette[POOLSIZE], int totalfitness)
{
    int i;        // 循环的控制变量
    int ball;     // 球（选择位置的数值）
    int acc = 0;  // 评价值的累积值

    ball = rndn(totalfitness);
    for (i = 0; i < POOLSIZE; ++i) {
        acc += roulette[i];     // 评价值的累积
        if (acc > ball) break;  // 对应的基因
    }
    return i;
}

// 交叉
void mating(int pool[POOLSIZE][N], int ngpool[POOLSIZE * 2][N])
{
    int i;                   // 循环的控制变量
    int totalfitness = 0;    // 评价值的总计值
    int roulette[POOLSIZE];  // 存放评价值
    int mama, papa;          // 父代的基因的号码

    // 生成轮盘
    for (i = 0; i < POOLSIZE; ++i) {
        roulette[i] = evalfit(pool[i]);
        // 计算评价值的总计值
        totalfitness += roulette[i];
    }

    // 选择和交叉的循环
    for (i = 0; i < POOLSIZE; ++i) {
        do {  // 父代的选择
            mama = selectp(roulette, totalfitness);
            papa = selectp(roulette, totalfitness);
        } while (mama == papa);  // 删除重复的

        // 特定2染色体的交叉
        crossing(pool[mama], pool[papa], ngpool[i * 2], ngpool[i * 2 + 1]);
    }
}

// 特定2染色体的交叉
void crossing(int m[], int p[], int c1[], int c2[])
{
    int j;   // 循环控制变量
    int cp;  // 交叉的点

    // 确定交叉点
    cp = rndn(N);

    // 复制前半部分
    for (j = 0; j < cp; ++j) {
        c1[j] = m[j];
        c2[j] = p[j];
    }
    // 复制后半部分
    for (; j < N; ++j) {
        c2[j] = m[j];
        c1[j] = p[j];
    }
}

// 计算评价值
int evalfit(int g[])
{
    int pos;         // 指定基因位点
    int value  = 0;  // 评价值
    int weight = 0;  // 重量

    // 调查各个基因位点计算重量和评价值
    for (pos = 0; pos < N; ++pos) {
        weight += parcel[pos][0] * g[pos];
        value += parcel[pos][1] * g[pos];
    }
    // 致死基因的处理
    if (weight >= WEIGHTLIMIT) value = 0;
    return value;
}

// 输出结果
void printp(int pool[POOLSIZE][N])
{
    int    i, j;                // 循环的控制变量
    int    fitness;             // 评价值
    double totalfitness = 0;    // 评价值的总计值
    int    elite, bestfit = 0;  // 精英基因的处理用变量

    for (i = 0; i < POOLSIZE; ++i) {
        for (j = 0; j < N; ++j)
            printf("%1d", pool[i][j]);
        fitness = evalfit(pool[i]);
        printf("\t%d\r\n", fitness);
        if (fitness > bestfit) {  // 精英解
            bestfit = fitness;
            elite   = i;
        }
        totalfitness += fitness;
    }
    // 输出精英解的评价值
    printf("elite=%d\tbestfit=%d \t", elite, bestfit);
    // 输出平均评价值
    printf("average=%lf\r\n", totalfitness / POOLSIZE);
}

// 生成初始染色体集合
void initpool(int pool[POOLSIZE][N])
{
    int i, j;  // 循环控制变量
    for (i = 0; i < POOLSIZE; ++i)
        for (j = 0; j < N; ++j)
            pool[i][j] = rndn(2);
}

// n以下随机数的生成
int rndn(int l)
{
    int rndno;
    while ((rndno = ((double)rand() / RAND_MAX) * l) == l) {}
    return rndno;
}

// 突变
void mutation(int ngpool[POOLSIZE * 2][N])
{
    for (int i = 0; i < POOLSIZE * 2; ++i)
        for (int j = 0; j < N; ++j)
            if ((double)rndn(100) / 100.0 <= MRATE)  // 反转的突变
                ngpool[i][j] = notval(ngpool[i][j]);
}

// 真值的反转
int notval(int v)
{
    return (v == YES) ? NO : YES;
}

int pool[POOLSIZE][N];        // 染色体集合
int ngpool[POOLSIZE * 2][N];  // 下一代染色体集合
int generation;               // 现在的代数

void dsp_test(void)
{
    srand(SEED);

    // 行李的初始化
    // initparcel() ;

    // 生成初始染色体集合
    initpool(pool);

    // 循环直至终止子代
    for (generation = 0; generation < LASTG; ++generation) {
        printf("[%d]\r\n", generation);  // 代数
        mating(pool, ngpool);            // 交叉
        mutation(ngpool);                // 突变
        selectng(ngpool, pool);          // 选择下一子代
        printp(pool);                    // 输出结果
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
    /* USER CODE BEGIN 2 */
    printf("Start\r\n");
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
