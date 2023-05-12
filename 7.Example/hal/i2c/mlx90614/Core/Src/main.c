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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MLX90614/mlx90614.h"

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
    MX_I2C2_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

		i2cdrv_detector(mlx90614.bus, mlx90614.drv);

    uint16_t ID[4];
    if (mlx90614_get_chip_id(ID)) {
        printv("0x%04x", ID[0]);
        printv("0x%04x", ID[1]);
        printv("0x%04x", ID[2]);
        printv("0x%04x", ID[3]);
    }
    println("-----------------------------------");
    // set params
    uint8_t eeprom_addr;
    if (mlx90614_set_address(0x66)) {  // set device address (ecffet after restart)
        if (mlx90614_get_address(&eeprom_addr))
            printv("0x%02x", eeprom_addr);
    }
    if (mlx90614_set_mode(MLX90614_MODE_TOBJ1_TOBJ2)) {
        mlx90614_mode_t mode;
        if (mlx90614_get_mode(&mode))
            printv("%d", mode);
    }
    if (mlx90614_set_ir_sensor(MLX90614_IR_SENSOR_DUAL)) {
        mlx90614_ir_sensor_t sensor;
        if (mlx90614_get_ir_sensor(&sensor))
            printv("%d", sensor);
    }
    if (mlx90614_set_iir(MLX90614_IIR_A1_0P571_B1_0P428)) {
        mlx90614_iir_t iir;
        if (mlx90614_get_iir(&iir))
            printv("%d", iir);
    }
    if (mlx90614_set_fir_length(MLX90614_FIR_LENGTH_1024)) {
        mlx90614_fir_length_t len;
        if (mlx90614_get_fir_length(&len))
            printv("%d", len);
    }
    if (mlx90614_set_gain(MLX90614_GAIN_25)) {
        mlx90614_gain_t gain;
        if (mlx90614_get_gain(&gain))
            printv("%d", gain);
    }
    println("-----------------------------------");
    int16_t raw_min, raw_max;
    mlx90614_set_min_temp(mlx90614_convert_temp_to_raw(10, MLX90614_TEMP_UNIT_CENTIGRADE));
    mlx90614_set_max_temp(mlx90614_convert_temp_to_raw(50, MLX90614_TEMP_UNIT_CENTIGRADE));
    if (mlx90614_get_min_temp(&raw_min) && mlx90614_get_max_temp(&raw_max)) {
        float temp_min = mlx90614_convert_raw_to_temp(raw_min, MLX90614_TEMP_UNIT_CENTIGRADE);
        float temp_max = mlx90614_convert_raw_to_temp(raw_max, MLX90614_TEMP_UNIT_CENTIGRADE);
        printv("%f", temp_min);
        printv("%f", temp_max);
    }
    if (mlx90614_set_emissivity(0.98)) {  // 辐射系数
        float emissivity = mlx90614_get_emissivity(&emissivity);
        printv("%f", emissivity);
    }
    println("-----------------------------------");
    while (1) {
#define MLX90614_TEST 2
#if MLX90614_TEST == 1
        // 原始数据，不知道有什么用
        uint16_t raw_ir[2];
        mlx90614_get_raw_ir_channel_1(&raw_ir[0]);
        mlx90614_get_raw_ir_channel_2(&raw_ir[1]);
        println("%d,%d", raw_ir[0], raw_ir[1]);
        HAL_Delay(10);
#elif MLX90614_TEST == 2
        int16_t raw_obj1, raw_obj2, raw_ambient;
        if (mlx90614_get_object1_temp(&raw_obj1) &&
            mlx90614_get_object2_temp(&raw_obj2) &&
            mlx90614_get_ambient_temp(&raw_ambient)) {
            float temp_obj1    = mlx90614_convert_raw_to_temp(raw_obj1, MLX90614_TEMP_UNIT_CENTIGRADE);
            float temp_obj2    = mlx90614_convert_raw_to_temp(raw_obj2, MLX90614_TEMP_UNIT_CENTIGRADE);
            float temp_ambient = mlx90614_convert_raw_to_temp(raw_ambient, MLX90614_TEMP_UNIT_CENTIGRADE);
            printv("%f", temp_obj1);
            printv("%f", temp_obj2);
            printv("%f", temp_ambient);
            println("-----------------------------------");
        }
        HAL_Delay(500);
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
