#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"
#include "bsp_iwdg.h"
#include "bsp_wwdg.h"
#include "bsp_uart.h"
#include "bsp_dma.h"
#include "bsp_adc.h"
#include "bsp_tim2.h"
#include "bsp_rtc.h"

int main(void)
{
	unsigned int flag;
	
	SysTick_Configuration();
	
	flag = RTC_Configuration();
	
	Uart1_Configuration();
	
	if(flag == 0)  //没有配置过RTC时间，需要设置为当前时间
			Set_Time();
	
	while(1)
	{
		Time_Display(RTC_GetCounter());
		Delay_us(1000000);
	}
}

