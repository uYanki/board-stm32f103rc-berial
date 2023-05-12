/**
 ******************************************************************************
 * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"
#include "string.h"
/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
}
////////////////
void EXTI9_5_IRQHandler(void)
{
}

//////////////
void USART1_IRQHandler(void)
{
    static uint8_t i = 0, rebuf[30] = {0};
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  // 判断接收标志
    {
        rebuf[i++] = USART_ReceiveData(USART1);  // 读取串口数据，同时清接收标志
        if (rebuf[0] != add)                     // 帧头不对
            i = 0;
        else if ((i == 2) && (rebuf[1] != 0x03))  // 帧头不对
            i = 0;
        else if ((i == 3) && (rebuf[2] > 16))  // 帧头不对
            i = 0;
        else if ((i == 4) && (rebuf[3] > 16))  // 帧头不对
            i = 0;
        if (i > 3)  // i等于4时，已经接收到数据量字节rebuf[3]
        {
            if (i < 21) {
                if (i == (rebuf[3] + 5))  // 判断是否接收一帧数据完毕
                {
                    if (!Receive_ok)  // 当数据处理完成后才接收新的数据
                    {
                        memcpy(usart_rx_data, rebuf, i);  // 拷贝接收到的数据
                        Receive_ok = 1;                   // 接收完成标志
                    }
                    i = 0;  // 缓存清0
                }
            } else
                i = 0;
        }
    }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
