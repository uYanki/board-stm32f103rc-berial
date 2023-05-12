/**
  ******************************************************************************
  * @file    bsp_mcooutput.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   配置RCC输出
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32  开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "bsp_mcooutput.h"
#include "stm32f10x_rcc.h"

/*
 * 初始化MCO引脚PA8
 * 在F1系列中MCO引脚只有一个，即PA8，在F4系列中，MCO引脚会有两个
 */
void MCO_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 选择GPIO8引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	
	//设置为复用功能推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	//设置IO的翻转速率为50M
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 初始化GPIOA8
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
