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
#include "i2c.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "AXP173/axp173.h"
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

#define IsKeyPress() (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)  // PA0

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
    MX_I2C2_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    printf("[status:%d]\r\n", AXP173_Init());
    /* Set off time */
    AXP173_SetPowerOffTime(AXP173_POWEROFF_4S);
    /* Enable and set LDO2 voltage */
    AXP173_SetOutputEnable(AXP173_OP_LDO2, 1);
    AXP173_SetOutputVoltage(AXP173_OP_LDO2, 3000);
    /* Enable and set LDO3 voltage */
    AXP173_SetOutputEnable(AXP173_OP_LDO3, 1);
    AXP173_SetOutputVoltage(AXP173_OP_LDO3, 3100);
    /* Enable and set LDO4 voltage */
    AXP173_SetOutputEnable(AXP173_OP_LDO4, 1);
    AXP173_SetOutputVoltage(AXP173_OP_LDO4, 3200);
    /* Enable and set DCDC1 voltage */
    AXP173_SetOutputEnable(AXP173_OP_DCDC1, 1);
    AXP173_SetOutputVoltage(AXP173_OP_DCDC1, 3300);
    /* Enable and set DCDC2 voltage */
    AXP173_SetOutputEnable(AXP173_OP_DCDC2, 1);
    AXP173_SetOutputVoltage(AXP173_OP_DCDC2, 2275);
    /* Enable VBUS ADC */
    AXP173_SetADCEnable(AXP173_ADC_VBUS_V, 1);
    AXP173_SetADCEnable(AXP173_ADC_VBUS_C, 1);
    /* Enable BAT ADC */
    AXP173_SetADCEnable(AXP173_ADC_BAT_V, 1);
    AXP173_SetADCEnable(AXP173_ADC_BAT_C, 1);
    /* Enable charge */
    AXP173_SetChargeCurrent(AXP173_CHG_450mA);
    AXP173_SetChargeLedFre(AXP173_CHARGE_LED_4HZ);
    AXP173_SetChargeEnable(1);
    while (1) {
        /* Get PMU info */
        float temp         = AXP173_GetAXP173Temp();
        float vbus_voltage = AXP173_GetVBUSVoltage();
        float vbus_current = AXP173_GetVBUSCurrent();
        float bat_voltage  = AXP173_GetBatVoltage();
        float bat_current  = AXP173_GetBatCurrent();
        printf("temp :%.2f C, vbus_voltage :%.2f V, vbus_current :%.2f mA, bat_voltage :%.2f V, bat_current :%.2f mA\r\n", temp, vbus_voltage, vbus_current, bat_voltage, bat_current);
        HAL_Delay(200);

        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        if (IsKeyPress()) {
            HAL_Delay(5);
            if (IsKeyPress()) {
                AXP173_PowerOFF();
                while (IsKeyPress())
                    ;
            }
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
