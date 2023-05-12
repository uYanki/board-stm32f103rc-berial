/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include "stm32746g_qspi.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t wData[0x100];
uint8_t rData[0x100];
uint32_t i;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	QSPI_CommandTypeDef s_command;
	uint8_t pData[3];
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
	printf("W25Q128FV QuadSPI Test ....\r\n\r\n");
	
	/*##-1- Initialize W25Q128FV  ###########################################*/
	BSP_QSPI_Init();

	/*##-2-Read Device ID Test    ###########################################*/
	/* Read Manufacture/Device ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_ID_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
  s_command.Address           = 0x000000;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	printf("SPI  I/0 Read Device ID : 0x%2X 0x%2X\r\n",pData[0],pData[1]);
	
	
	/* Read Manufacture/Device ID Dual I/O*/
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = DUAL_READ_ID_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_2_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
  s_command.Address           = 0x000000;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES;
	s_command.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.AlternateBytes    = 0;
  s_command.DataMode          = QSPI_DATA_2_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 4;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	printf("Dual I/O Read Device ID : 0x%2X 0x%2X\r\n",pData[0],pData[1]);
	
	/* Read Manufacture/Device ID Quad I/O*/
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = QUAD_READ_ID_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
  s_command.Address           = 0x000000;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	s_command.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.AlternateBytes    = 0x00;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 4;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	printf("Quad I/O Read Device ID : 0x%2X 0x%2X\r\n",pData[0],pData[1]);

	/* Read JEDEC ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_JEDEC_ID_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 3;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
		Error_Handler();
  }
	printf("Read JEDEC ID :  0x%2X 0x%2X 0x%2X\r\n\r\n",pData[0],pData[1],pData[2]);
	
	
	/*##-3-QSPI Erase/Write/Read Test    ###########################################*/
	/* fill buffer */
	for(i =0;i<0x100;i ++)
	{
			wData[i] = i;
		  rData[i] = 0;
	}
	
	if(BSP_QSPI_Erase_Block(0) == QSPI_OK)
		printf(" QSPI Erase Block ok\r\n");
	else
		Error_Handler();
	
	if(BSP_QSPI_Write(wData,0x00,0x100)== QSPI_OK)
		printf(" QSPI Write ok\r\n");
	else
		Error_Handler();
	
	if(BSP_QSPI_Read(rData,0x00,0x100)== QSPI_OK)
		printf(" QSPI Read ok\r\n\r\n");
	else
		Error_Handler();
	
	printf("QSPI Read Data : \r\n");
	for(i =0;i<0x100;i++)
		printf("0x%02X  ",rData[i]);
	printf("\r\n\r\n");
	
	for(i =0;i<0x100;i++)
		if(rData[i] != wData[i])printf("0x%02X 0x%02X ",wData[i],rData[i]);
	printf("\r\n\r\n");
	/* check date */
	if(memcmp(wData,rData,0x100) == 0 ) 
		printf(" W25Q128FV QuadSPI Test OK\r\n");
	else
		printf(" W25Q128FV QuadSPI Test False\r\n");
	
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

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  HAL_PWREx_ActivateOverDrive();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
	printf("something wrong ....\r\n");
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
