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
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "ST7735/st7735.h"
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
void GuiTest()
{
    DisplayButtonDown(10, 10, 40, 30);
    DisplayButtonUp(10, 40, 40, 70);
    HAL_Delay(2000);
    ST7735_FillScreen(ST7735_BLACK);

    Gui_DrawBox(10, 10, 20, 20, 0);
    Gui_DrawBox(40, 10, 20, 20, 1);
    Gui_DrawBox(70, 10, 20, 20, 2);
    Gui_DrawCircle(50, 50, 20, ST7735_BLUE);
    HAL_Delay(2000);
    ST7735_FillScreen(ST7735_BLACK);
}

void HAL_Delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

float HCSR04_Measure(TIM_HandleTypeDef* htim)
{
    uint32_t tick_us;

    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    HAL_Delay_us(20);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

    __HAL_TIM_SetCounter(htim, 0);

    /* waitting for start of the high level through echo pin */
    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET) {}

    /* start the tim and enable the interrupt */
    HAL_TIM_Base_Start(htim);

    /* waitting for end of the high level through echo pin */
    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET) {}

    /* stop the tim */
    HAL_TIM_Base_Stop(htim);

    /* get the time of high level */
    tick_us = __HAL_TIM_GetCounter(htim);

    /* calc distance in unit mm */

    float distance = tick_us * .17f;

    return (distance > 450) ? 0 : distance;
}

void DrawRadarLine(double angle, int r)
{
    double x, y;
    x = 80 + r * (double)cos(angle / 180 * 3.1415926);
    y = 108 - r * (double)sin(angle / 180 * 3.1415926);
    Gui_DrawLine(80, 108, x, y, ST7735_GREEN);
}

void DrawRadarPoint(double angle, int r)
{
    double x, y;
    x = 80 + r * (double)cos(angle / 180 * 3.1415926);
    y = 100 - r * (double)sin(angle / 180 * 3.1415926);
    Gui_DrawPoint(x, y, ST7735_RED);
}

void DrawRadar()
{
    ST7735_FillScreen(ST7735_BLACK);

    Gui_DrawCircle(80, 100, 76, ST7735_GREEN);
    Gui_DrawCircle(80, 100, 57, ST7735_GREEN);
    Gui_DrawCircle(80, 100, 38, ST7735_GREEN);
    Gui_DrawCircle(80, 100, 19, ST7735_GREEN);
    DrawRadarLine(30, 85);  // radioactive rays
    DrawRadarLine(60, 85);
    DrawRadarLine(90, 85);
    DrawRadarLine(120, 85);
    DrawRadarLine(150, 85);
    Gui_FillRect(0, 100, 160, 128, ST7735_BLACK);  // clear bottom half circle
    Gui_DrawLine(0, 100, 160, 100, ST7735_GREEN);  // draw axis
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
    MX_SPI3_Init();
    MX_TIM1_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
    // while (1) GuiTest();

    char* buff[20];
    while (1) {
        DrawRadar();
        for (uint8_t i = 0; i < 180; ++i) {
            float distance_mm = HCSR04_Measure(&htim1);
            sprintf(buff, "distance: %.2f mm   ", distance_mm);
            ST7735_DrawString(0, 108, buff, Font_7x10, ST7735_WHITE, ST7735_BLACK);
            DrawRadarPoint(i, distance_mm / 8);
            HAL_Delay(20);
        }
        DrawRadar();
        for (uint8_t i = 180; i > 0; --i) {
            float distance_mm = HCSR04_Measure(&htim1);
            sprintf(buff, "distance: %.2f mm   ", distance_mm);
            ST7735_DrawString(0, 108, buff, Font_7x10, ST7735_WHITE, ST7735_BLACK);
            DrawRadarPoint(i, distance_mm / 8);
            HAL_Delay(20);
        }
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
