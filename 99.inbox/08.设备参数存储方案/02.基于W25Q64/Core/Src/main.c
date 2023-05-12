/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "i2c.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "at24c02_i2c_drv.h"
#include "bsp_w25q64.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
    //LED状态
    uint8_t led_status ;
    //电机状态
    uint8_t motor_status ;
    //蜂鸣器状态
    uint8_t buzzer_status ;
    //网络状态
    uint8_t network_status ;
    //语言版本
    uint8_t language_version ;
    //设备密码
    uint8_t device_password[4] ;
    //报警阈值1
    uint16_t alarm_threshold1 ;
    //报警阈值2
    uint16_t alarm_threshold2 ;
    //报警阈值3
    uint16_t alarm_threshold3 ;
} System_Param ;
System_Param device_system_para ;
System_Param device_system_para_cpy ;

//测试w25q64 spi flash
void test_w25q64_flash(void)
{
		uint8_t index = 0 ;
    uint16_t device_id ;
    device_id = W25QXX_ReadID();
    printf("device_id = 0x%04X\r\n\r\n", device_id);
    printf("加载之前FLASH的数据:\n");
    W25QXX_Read((uint8_t *)&device_system_para_cpy, 0, sizeof(System_Param));
    printf("开始读取系统参数\n");
    printf("led_status=%d\n", device_system_para_cpy.led_status);
    printf("motor_status=%d\n", device_system_para_cpy.motor_status);
    printf("buzzer_status=%d\n", device_system_para_cpy.buzzer_status);
    printf("network_status=%d\n", device_system_para_cpy.network_status);
    printf("language_version=%d\n", device_system_para_cpy.language_version);
    printf("language_version=%d\n", device_system_para_cpy.language_version);
    printf("password=%d%d%d%d\n", device_system_para_cpy.device_password[0], \
           device_system_para_cpy.device_password[1], device_system_para_cpy.device_password[2], \
           device_system_para_cpy.device_password[3]);
    printf("alarm_threshold1=%d\n", device_system_para_cpy.alarm_threshold1);
    printf("alarm_threshold2=%d\n", device_system_para_cpy.alarm_threshold2);
    printf("alarm_threshold3=%d\n", device_system_para_cpy.alarm_threshold3);

    /* 擦除该扇区 */
    printf("-------- erase sector 0 -----------\r\n");
    W25QXX_Erase_Sector(0);
		
		printf("加载擦除扇区后的数据:\n");
    W25QXX_Read((uint8_t *)&device_system_para_cpy, 0, sizeof(System_Param));
    printf("开始读取系统参数\n");
    printf("led_status=%d\n", device_system_para_cpy.led_status);
    printf("motor_status=%d\n", device_system_para_cpy.motor_status);
    printf("buzzer_status=%d\n", device_system_para_cpy.buzzer_status);
    printf("network_status=%d\n", device_system_para_cpy.network_status);
    printf("language_version=%d\n", device_system_para_cpy.language_version);
    printf("language_version=%d\n", device_system_para_cpy.language_version);
    printf("password=%d%d%d%d\n", device_system_para_cpy.device_password[0], \
           device_system_para_cpy.device_password[1], device_system_para_cpy.device_password[2], \
           device_system_para_cpy.device_password[3]);
    printf("alarm_threshold1=%d\n", device_system_para_cpy.alarm_threshold1);
    printf("alarm_threshold2=%d\n", device_system_para_cpy.alarm_threshold2);
    printf("alarm_threshold3=%d\n", device_system_para_cpy.alarm_threshold3);

    /* 写数据 */
    printf("开始写入数据\r\n");
		device_system_para.led_status = 0 ;
    device_system_para.motor_status = 1 ;
    device_system_para.buzzer_status = 1 ;
    device_system_para.network_status = 0 ;
    device_system_para.language_version = 1 ;

    for(index = 0 ; index < 4 ; index++)
        device_system_para.device_password[index] = 2 ;

    device_system_para.alarm_threshold1 = 1000 ;
    device_system_para.alarm_threshold2 = 1200 ;
    device_system_para.alarm_threshold3 = 1500 ;
    W25QXX_Page_Program((uint8_t *)&device_system_para, 0, sizeof(device_system_para));

    /* 再次读数据 */
    printf("加载写入扇区后的数据:\n");
    W25QXX_Read((uint8_t *)&device_system_para_cpy, 0, sizeof(System_Param));
    printf("开始读取系统参数\n");
    printf("led_status=%d\n", device_system_para_cpy.led_status);
    printf("motor_status=%d\n", device_system_para_cpy.motor_status);
    printf("buzzer_status=%d\n", device_system_para_cpy.buzzer_status);
    printf("network_status=%d\n", device_system_para_cpy.network_status);
    printf("language_version=%d\n", device_system_para_cpy.language_version);
    printf("language_version=%d\n", device_system_para_cpy.language_version);
    printf("password=%d%d%d%d\n", device_system_para_cpy.device_password[0], \
           device_system_para_cpy.device_password[1], device_system_para_cpy.device_password[2], \
           device_system_para_cpy.device_password[3]);
    printf("alarm_threshold1=%d\n", device_system_para_cpy.alarm_threshold1);
    printf("alarm_threshold2=%d\n", device_system_para_cpy.alarm_threshold2);
    printf("alarm_threshold3=%d\n", device_system_para_cpy.alarm_threshold3);
}
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
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_QUADSPI_Init();
    /* USER CODE BEGIN 2 */
		test_w25q64_flash();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
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
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
