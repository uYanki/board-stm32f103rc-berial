#include "bsp_SysTick.h"

unsigned int TimingDelay;

void SysTick_Configuration(void)
{
	while(SysTick_Config(72));
	
	SysTick->CTRL &= ~(1<<0);   //暂时初始化后先关闭定时器，使用前开启
}

void Delay_us(unsigned int n)
{
	TimingDelay = n;
	SysTick->CTRL |= (1<<0);   //开启Sys_Tick
	while(TimingDelay);
	SysTick->CTRL &= ~(1<<0);
}
