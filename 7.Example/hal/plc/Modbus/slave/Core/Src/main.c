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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mbslave.h"
#include <math.h> // for test
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
    MX_TIM7_Init();
    /* USER CODE BEGIN 2 */

    // eMBInit(MB_RTU, 0x01, 2, 19200, MB_PAR_EVEN);
    eMBInit(MB_RTU, 0x0A, 2, 115200, MB_PAR_NONE);

    const UCHAR ucSlaveID[] = {0xAA, 0xBB, 0xCC};
    eMBSetSlaveID(0x33, FALSE, ucSlaveID, 3);

    // test data

    usRegInputBuf[0]   = 0x1234;
    usRegInputBuf[1]   = 0x2345;
    usRegInputBuf[2]   = 0x3456;
    usRegInputBuf[3]   = 0x4567;
    usRegInputBuf[4]   = 0x0000;
		usRegInputBuf[5]   = 0x0000;

    usRegHoldingBuf[0] = 0x0000;
    usRegHoldingBuf[1] = 0xFFFF;
    usRegHoldingBuf[2] = 0x0000;
    usRegHoldingBuf[3] = 0xFFFF;
    usRegHoldingBuf[4] = 0x0000;
    usRegHoldingBuf[5] = 0xFFFF;
    usRegHoldingBuf[6] = 0x0000;
    usRegHoldingBuf[7] = 0xFFFF;

    ucDiscInBuf[0]     = 0b11001100;
    ucDiscInBuf[1]     = 0b00110011;

    ucCoilBuf[0]       = 0b10101010;
    ucCoilBuf[1]       = 0b01010101;
    ucCoilBuf[2]       = 0b11001100;
    ucCoilBuf[3]       = 0b00110011;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    eMBEnable();
    while (1) {
        eMBPoll();
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
    RCC_OscInitStruct.OscillatorType     = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState           = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue     = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState           = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState       = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource      = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL         = RCC_PLL_MUL9;
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

/**************************************************/

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM7) {
        pxMBPortCBTimerExpired();

        // test data
        ++usRegInputBuf[4];
			  usRegInputBuf[5] = (cos((usRegInputBuf[4]%360)/57.3f) + 1)*1000;
    }
}

/** Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call xMBPortSerialPutByte( ) to send the character.
 */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART2) {
        pxMBFrameCBTransmitterEmpty();
    }
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the character.
 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART2) {
        pxMBFrameCBByteReceived();
    }
}

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
