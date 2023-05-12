/*
 * dht11.c
 *
 *  Created on: Jul 4, 2022
 *      Author: hejunlin
 */

#include "dht11.h"
#include "delay.h"


// 设置IO口方向
void DHT11_SetGpioMode(uint8_t pMode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(pMode == OUTPUT)
	{
	  GPIO_InitStruct.Pin = DHT11_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	}
	else // if(pMode == INPUT)
	{
	  GPIO_InitStruct.Pin = DHT11_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	}
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在
u8 DHT11_Init(void)
{
	DHT11_SetGpioMode(OUTPUT);

	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
}


//复位DHT11
void DHT11_Rst(void)
{
	DHT11_SetGpioMode(OUTPUT); 	//SET OUTPUT
    DHT_GPIO_LOW(); 	//拉低DQ
    HAL_Delay(20);    	//拉低至少18ms
    DHT_GPIO_HIGH(); 	//DQ=1
	delay_us(30);     	//主机拉高20~40us
}
//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void)
{
	u8 retry=0;
	DHT11_SetGpioMode(INPUT);//SET INPUT
    while (DHT_DAT&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT_DAT&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;
	return 0;
}
//从DHT11读取一个位
//返回值：1/0
u8 DHT11_ReadBit(void)
{
 	u8 retry=0;
	while(DHT_DAT&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT_DAT&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT_DAT)return 1;
	else return 0;
}
//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_ReadByte(void)
{
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++)
	{
   		dat<<=1;
	    dat|=DHT11_ReadBit();
    }
    return dat;
}
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_ReadData(u8 *temp,u8 *humi)
{
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_ReadByte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else return 1;
	return 0;
}



