#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "lcd.h"
#include "stmflash.h"   
#include "iap.h"	 
//ALIENTEK MiniSTM32开发板实验32
//串口IAP实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

//FLASH分区: 0X0800 0000~0x0800 8000,给IAP使用,共32K字节,FLASH APP还有256-32=224KB可用.
// SRAM分区: 0X2000 1000处开始,用于存放SRAM IAP代码,共44K字节可用,用户可以自行对44K空间进行ROM和RAM区的分配
//           特别注意，SRAM APP的ROM占用量不能大于41K字节，因为本例程最大是一次接收41K字节，不能超过这个限制。

int main(void)
 { 
	u8 t;
	u8 key;
	u16 oldcount=0;	//老的串口接收数据值
	u16 applenth=0;	//接收到的app代码长度
	u8 clearflag=0; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
	uart_init(256000);	 	//串口初始化为256000
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD 
 	KEY_Init();				//按键初始化 	
 
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	LCD_ShowString(60,70,200,16,16,"IAP TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2014/3/15");  
	LCD_ShowString(60,130,200,16,16,"WK_UP:Copy APP2FLASH"); 
	LCD_ShowString(60,150,200,16,16,"KEY0:Run SRAM APP");
	LCD_ShowString(60,170,200,16,16,"KEY1:Run FLASH APP"); 
	POINT_COLOR=BLUE;//设置字体为蓝色	  
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0;
			t=0;
			if(clearflag)
			{
				clearflag--;
				if(clearflag==0)LCD_Fill(60,210,240,210+16,WHITE);//清除显示
			}
		}	  	 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)			//WK_UP按键按下
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
				LCD_ShowString(60,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					LCD_ShowString(60,210,200,16,16,"Copy APP Successed!!");
					printf("固件更新完成!\r\n");	
				}else 
				{
					LCD_ShowString(60,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
				LCD_ShowString(60,210,200,16,16,"No APP!");
			}
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示									 
		} 
		if(key==KEY1_PRES)
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");
				LCD_ShowString(60,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	  
		}
		if(key==KEY0_PRES)
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM地址
			}else 
			{
				printf("非SRAM应用程序,无法执行!\r\n");
				LCD_ShowString(60,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	 
		}				   
		 
	}   	   
}












