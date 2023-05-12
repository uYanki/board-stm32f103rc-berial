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
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MFRC522/mfrc522.h"
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

uint8_t status;
uint8_t g_ucTempbuf[20];
uint8_t flag_loop       = 0;
uint8_t defaultKeyA[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t CompareID(uint8_t* CardID, uint8_t* CompareID) {
    uint8_t i;
    for (i = 0; i < 5; ++i)
        if (CardID[i] != CompareID[i])
            return MI_ERR;
    return MI_OK;
}

void RC522_ReadWriteTest() {
    uint8_t k = 5 /*地址*/, p = 0;
    uint8_t serNum[5], data[16];
    status = RC522_Request(PICC_REQALL, g_ucTempbuf);
    if (status != MI_OK) return;
    status = RC522_Anticoll(serNum);
    if (status != MI_OK) return;
    status = RC522_Select(serNum);
    if (status != MI_OK) return;
    status = RC522_AuthState(PICC_AUTHENT1A, k, defaultKeyA, serNum);
    if (status != MI_OK) return;
    printf("OLD[%03d] ", k);
    // 读取
    status = RC522_Read(k, data);
    if (status != MI_OK) return;
    for (p = 0; p < 16; ++p)
        printf("%02x ", data[p]);  // 输出
    printf("\r\n");
    // 写入
    for (p = 0; p < 16; ++p)
        ++data[p];
    status = RC522_Write(k, data);
    if (status != MI_OK) return;
    // 再次读取
    status = RC522_Read(k, data);
    if (status != MI_OK) return;
    printf("NEW[%03d] ", k);
    for (p = 0; p < 16; ++p)
        printf("%02x ", data[p]);  // 输出
    printf("\r\n");
    RC522_Halt();
}

void Read64Block(void) {
    uint8_t k = 0, p = 0;
    uint8_t readdata[16];
    uint8_t serNum[5];
    for (k = 0; k < 64; ++k) {
        status = RC522_Request(PICC_REQALL, g_ucTempbuf);
        if (status != MI_OK) continue;
        status = RC522_Anticoll(serNum);
        if (status != MI_OK) continue;
        status = RC522_Select(serNum);
        if (status != MI_OK) continue;
        status = RC522_AuthState(PICC_AUTHENT1A, k, defaultKeyA, serNum);
        if (status != MI_OK) continue;
        printf("[%03d] ", k);
        status = RC522_Read(k, readdata);
        if (status == MI_OK) {
            for (p = 0; p < 16; ++p)
                printf("%02x ", readdata[p]);
            printf("\r\n");
            RC522_Halt();
        }
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
    MX_SPI3_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    uint8_t MyID[5] = {0x2c, 0xff, 0xd3, 0x21};
    RC522_Reset();
    printf("RFID Reading\r\n");
    while (1) {
        status = RC522_Request(PICC_REQALL, g_ucTempbuf);
        if (status != MI_OK) {
            flag_loop = 0;
            continue;
        }
        status = RC522_Anticoll(g_ucTempbuf);
        if (status != MI_OK) {
            flag_loop = 0;
            continue;
        }
        if (flag_loop == 1) {
            RC522_Halt();
            continue;
        }
        flag_loop = 1;
        printf("\r\n[UID] %x:%x:%x:%x", g_ucTempbuf[0], g_ucTempbuf[1], g_ucTempbuf[2], g_ucTempbuf[3]);
        RC522_Halt();
        if (CompareID(g_ucTempbuf, MyID) == MI_OK) {
            printf(" [CHECK] True\r\n");
        } else {
            printf(" [CHECK] False\r\n");
        }
#if 0
        // printf("Read 64 sectors\r\n");
        // Read64Block();
#else
        printf("Read Write Block\r\n");
        RC522_ReadWriteTest();
#endif
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
