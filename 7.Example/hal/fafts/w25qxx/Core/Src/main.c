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
#include "fatfs.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <string.h>
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
FATFS    fs;
FATFS*   pfs;
FIL      fil;
FRESULT  fres;
DWORD    fre_clust;
uint32_t totalSpace, freeSpace;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void FAFTS_Error_Handler(char* file, int line) {
    printf("%s:%d\r\n", file, line);
    while (1) {}
}

UINT work[4096];  // 工作缓冲区

char rx[100], tx[100] = {"hello uyk"};
UINT count;

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
    MX_SPI3_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */

    // 挂载设备
    switch (f_mount(&fs, "", 1)) {
        case FR_OK: break;

        // 没有文件系统
        case FR_NO_FILESYSTEM: {
            // 格式化
            if (f_mkfs("", 0, work) != FR_OK)
                FAFTS_Error_Handler(__FILE__, __LINE__);
            // 重新挂载
            f_mount(NULL, "", 1);
            f_mount(&fs, "", 1);
            break;
        }

        default: {
            FAFTS_Error_Handler(__FILE__, __LINE__);
            break;
        }
    }

    // 获取容量

    if (f_getfree("", &fre_clust, &pfs) != FR_OK)  // 获取磁盘信息及空闲簇数量
        FAFTS_Error_Handler(__FILE__, __LINE__);

    // 扇区大小 4096k
    totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize) * 4;  // 总扇区数
    freeSpace  = (uint32_t)(fre_clust * pfs->csize) * 4;            // 空闲扇区数

    printf("free space: %.2f MB\r\n", freeSpace / 1024.f);
    printf("total space: %.2f MB\r\n", totalSpace / 1024.f);

    // 读写文件

    if (f_open(&fil, "1:file.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ) != FR_OK)  // 打开/创建文件
        FAFTS_Error_Handler(__FILE__, __LINE__);

    if (f_write(&fil, tx, sizeof(tx), &count) != FR_OK)  // 写
        FAFTS_Error_Handler(__FILE__, __LINE__);

    printf("count of buffer (write): %d\r\n", count);

    if (f_lseek(&fil, 0) != FR_OK)  // 设置光标位置
        FAFTS_Error_Handler(__FILE__, __LINE__);

    if (f_read(&fil, rx, f_size(&fil), &count) != FR_OK)  // 读
        FAFTS_Error_Handler(__FILE__, __LINE__);

    printf("count of buffer (read): %d\r\n", count);
    printf("%s", rx);

    if (f_close(&fil) != FR_OK)  // 关闭文件
        FAFTS_Error_Handler(__FILE__, __LINE__);

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
