#include "Led_Key.h"

void Delay(unsigned long nCount)
{
	while(nCount--)
	{
	}
}

void Led_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//PA0--KEY1  input
void Key_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //修改的地方
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int Key_Scan(GPIO_TypeDef* GPIOx, unsigned int Pin)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		Delay(1000);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
			return KEY_ON;
		}
		return KEY_OFF;
	}
	return KEY_OFF;
}

