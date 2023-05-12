/**
  ******************************************************************************
  * @file    main
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RCC MCO输出
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32  开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */


/* 
 * 配置MCO引脚：PA8 对外提供时钟，最高频率不能超过IO口的翻转频率50MHZ
 * MCO 时钟来源可以是：PLLCLK/2 ,HSI,HSE,SYSCLK
 */
#include "stm32f10x.h"
#include "./led/bsp_led.h"
#include "bsp_clkconfig.h"
#include "bsp_mcooutput.h"

//  软件延时函数，使用不同的系统时钟，延时不一样
void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	
	// 重新设置系统时钟，根据需要修改，一般设置最高为72MH
	// SYSCLK = 8M * RCC_PLLMul_x, x:[2,3,...16]
	//HSE_SetSysClock(RCC_PLLMul_9);
	
	HSI_SetSysClock(RCC_PLLMul_16);
	// MCO 引脚初始化
	MCO_GPIO_Config();
	
	// 设置MCO引脚输出时钟，用示波器即可在PA8测量到输出的时钟信号
	// MCO引脚输出可以是HSE,HSI,PLLCLK/2,SYSCLK
	//RCC_MCOConfig(RCC_MCO_HSE);	             // 8M	        
	//RCC_MCOConfig(RCC_MCO_HSI);	           // 8M 	        
	//RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);    // 36M		
	RCC_MCOConfig(RCC_MCO_SYSCLK);		     // 72M  
	
	// LED 端口初始化
	LED_GPIO_Config();
	while (1)
	{
		LED1( ON );			  // 亮
		Delay(0x0FFFFF);
		LED1( OFF );		  // 灭 
		Delay(0x0FFFFF);		
	}

}


//  软件延时函数，使用不同的系统时钟，延时不一样
void Delay(__IO uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
