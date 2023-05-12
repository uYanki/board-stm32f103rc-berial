#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "usmart.h"
#include "led.h"
#include "lcd.h"
#include "myiic.h"
#include "max30100.h"
void set_scl(u8 sw)
{
	if(sw)IIC_SCL_CLR;
	else IIC_SCL_SET;
}
void set_sda(u8 sw)
{
	if(sw) IIC_SDA_CLR;
	else IIC_SDA_SET;
}

int main(void)
{
//	u8 temp;
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	usmart_dev.init(84);//
	LED_Init();
	LCD_Init();
	max_begin();
	while(1)
	{
		LED0=!LED0;
		delay_ms(100);
	}
}


