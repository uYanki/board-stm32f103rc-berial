/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @version V3.0
  * @date    2015-xx-xx
  * @brief   蜂鸣器应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./beep/bsp_beep.h"   

 /**
  * @brief  初始化控制蜂鸣器的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启控制蜂鸣器的GPIO的端口时钟*/
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE); 

		/*选择要控制蜂鸣器的GPIO*/															   
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		/*设置GPIO模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置GPIO速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化控制蜂鸣器的GPIO*/
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 
    
    /* 关闭蜂鸣器*/
		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);	 
}
/*********************************************END OF FILE**********************/
