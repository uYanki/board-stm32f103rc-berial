#include "stm32f10x.h" 

int main(void)
{
	// 在启动文件中已将系统时钟配置为72M了
	
	while(1)
	{
		LED(OFF);
		SysTick_Delay_ms(500);
		LED(ON);
		SysTick_Delay_ms(500);
	}
}





