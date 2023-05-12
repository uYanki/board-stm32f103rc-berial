#ifndef	__CAN_H__
#define	__CAN_H__
//	write your header here
#include "2515.h"
#include "SPI.h"
#include "reg51.h"
#include "Function.h"

#define CAN_10Kbps 0x31
#define CAN_25Kbps 0x13
#define CAN_50Kbps 0x09
#define CAN_100Kbps 0x04
#define CAN_125Kbps 0x03
#define CAN_250Kbps 0x01
#define CAN_500Kbps 0x00



//void SPIReset(void);
//void SPIByteWrite(unsigned char ,unsigned char );
//unsigned char SPIByteRead(unsigned char );
unsigned char dummy;
 
void Init2515(unsigned int IDF)
{
	SPIReset();
	delay_ms(1);
	
	//SPIByteWrite(CANCTRL,0x80);//CAN工作在配置模式
	
    SPIByteWrite(RXM0SIDH,0x00);
	SPIByteWrite(RXM0SIDL,0x00);
	SPIByteWrite(RXF0SIDH,0x00);
	SPIByteWrite(RXF0SIDL,0x00);
	
	SPIByteWrite(RXM1SIDH,0x00);
	SPIByteWrite(RXM1SIDL,0x00);
	SPIByteWrite(RXF2SIDH,0x00);
	SPIByteWrite(RXF2SIDL,0x00);
	
	//设置波特率为10Kbps
	//set CNF1,SJW=00,长度为1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/8M=12.5us
	SPIByteWrite(CNF1,CAN_125Kbps);
	//set CNF2,SAM=0,在采样点对总线进行一次采样，PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ
	SPIByteWrite(CNF2,0x80|PHSEG1_3TQ|PRSEG_1TQ);
	//set CNF3,PHSEG2=(2+1)TQ=3TQ,同时当CANCTRL.CLKEN=1时设定CLKOUT引脚为时间输出使能位
	SPIByteWrite(CNF3,PHSEG2_3TQ);
	
	//set TXB0，设置发送缓冲器0的标识符和发送的数据，以及发送的数据长度
	 SPIByteWrite(TXB0CTRL,0x03);//设置发送缓冲器0控制寄存器清零TXREQ,发送优先级最高
	//BitModiMcp2515(TXB0CTRL,0x08,0x00);
	
	SPIByteWrite(TXB0SIDH,IDF);//设置发送缓冲器0的标准标识符，待修改***
	SPIByteWrite(TXB0SIDL,0xE0);//用到标准标识符
	SPIByteWrite(TXB0DLC,DLC_8);//设置发送数据的长度为8个字节
	//SPIByteWrite(TXB0D0,0x1E);//有待修改及确定是否使用
	//SPIByteWrite(TXB0D1,0x10)；//有待修改及确定是否使用
		
	/*set TXB1
	SPIByteWrite(TXB1SIDH,0x50);    //Set TXB0 SIDH
 	SPIByteWrite(TXB1SIDL,0x00);    //Set TXB0 SIDL
 	SPIByteWrite(TXB1DLC,0x40 | DLC_8);    //Set DLC = 3 bytes and RTR bit*/
 	
 	//设置接收缓冲器0的标识符和初始化数据
 	//SPIByteWrite(RXB0SIDH,0x55);//设置接收缓冲器0的标准标识符，待修改***
 	//SPIByteWrite(RXB0SIDL,0xE0);//用到标准标识符
 	SPIByteWrite(RXB0CTRL,0x20);//仅仅接收标准标识符的有效信息，FIILHIT0=0表示RXB0 ，采用FILHIT0
 	SPIByteWrite(RXB0DLC,DLC_8);//设置接收数据的长度为8个字节
 	
    SPIByteWrite(RXF0SIDH,0xE0);//初始化接收滤波器0，待修改***
 	SPIByteWrite(RXF0SIDL,0xE0);
 	SPIByteWrite(RXM0SIDH,0xFF);//初始化接收屏蔽器0，待修改***
 	SPIByteWrite(RXM0SIDL,0xE0);            
 	
 	//设置接收缓冲器1的标识符和初始化数据
 	 //SPIByteWrite(RXB1SIDH,IDF); //设置接收缓冲器0的标准标识符，待修改***
     //SPIByteWrite(RXB1SIDL,0xE0);//用到标准标识符
 	 SPIByteWrite(RXB1CTRL,0x20);//仅仅接收标准标识符的有效信息，FIILHIT0=0表示RXB0 ，采用FILHIT0
 	 SPIByteWrite(RXB1DLC,DLC_8);//设置接收数据的长度为8个字节
 	
 	 SPIByteWrite(RXF2SIDH,0x00);//初始化接收滤波器2，待修改***
 	 SPIByteWrite(RXF2SIDL,0xE0);
 	 
 	 SPIByteWrite(RXF3SIDH,0x01);//初始化接收滤波器3，待修改***
 	 SPIByteWrite(RXF3SIDL,0xE0);
 	 
 	 SPIByteWrite(RXF4SIDH,0x02);//初始化接收滤波器4，待修改***
 	 SPIByteWrite(RXF4SIDL,0xE0);
 	 
 	 SPIByteWrite(RXF5SIDH,0x56);//初始化接收滤波器5，待修改***
 	 SPIByteWrite(RXF5SIDL,0xE0);
 	 
 	 SPIByteWrite(RXM1SIDH,0xFF);//初始化接收屏蔽器1，待修改***
 	 SPIByteWrite(RXM1SIDL,0xE0);
 	
 	//设置接收缓冲器0中断
 	SPIByteWrite(CANINTF,0x00);//接收完一次必须对中断标志位清0
 	SPIByteWrite(CANINTE,0x02);//接收缓冲器0满中断使能位***修改成01-02
 	//设置单触发模式1 使能，报文尝试发送一次；0禁止。如有需要报文会重发
 	//BitModiMcp2515(CANCTRL,0x08,0x00);
 	SPIByteWrite(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);//设置正常模式
 	BitModiMcp2515(CANCTRL,0xE7,0x04);
  	dummy=SPIByteRead(CANSTAT);
		if (OPMODE_NORMAL != (dummy && 0xE0))
	   SPIByteWrite(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);//判断进入正常工作模式
	  //SPIByteWrite(CANCTRL,0x00);//选定正常工作模式
	 
}



//CAN发送程序
void CAN_Send(unsigned char *CAN_TX_Buf)
{
	unsigned char tempdata;
	tempdata=SPIByteRead(CAN_RD_STATUS);
	if(tempdata&0x04)            //判断TXREQ标志位
	{
		delay_ms(1);
      //SPIByteWrite(TXB0CTRL,0);//清除TXREQ标志位
        BitModiMcp2515(TXB0CTRL,0x08,0x00);
	while(SPIByteRead(CAN_RD_STATUS)&0x04);//等待TXREQ清零
   	 }
	SPIByteWrite(TXB0D0,CAN_TX_Buf[0]);
	SPIByteWrite(TXB0D1,CAN_TX_Buf[1]);
	SPIByteWrite(TXB0D2,CAN_TX_Buf[2]);
	SPIByteWrite(TXB0D3,CAN_TX_Buf[3]);
	SPIByteWrite(TXB0D4,CAN_TX_Buf[4]);
	SPIByteWrite(TXB0D5,CAN_TX_Buf[5]);
	SPIByteWrite(TXB0D6,CAN_TX_Buf[6]);
	SPIByteWrite(TXB0D7,CAN_TX_Buf[7]);
/*	if(tempdata&0x04)            //判断TXREQ标志位
	{
		delay_ms(2);
      //SPIByteWrite(TXB0CTRL,0);//清除TXREQ标志位
        BitModiMcp2515(TXB0CTRL,0x08,0x00);
		while(SPIByteRead(CAN_RD_STATUS)&0x04);//等待TXREQ清零
	}*/
	CS=LOW;
	//WriteSPI(CAN_RTS_TXB0);//发送缓冲器0请求发送
	BitModiMcp2515(TXB0CTRL,0x08,0xFF);
	CS=HIGH;
}

//CAN接收程序
/*void CAN_Receive(unsigned char *CAN_RX_Buf)
{
	CAN_RX_Buf[0]=SPIByteRead(RXB0D0);
	CAN_RX_Buf[1]=SPIByteRead(RXB0D1);
	CAN_RX_Buf[2]=SPIByteRead(RXB0D2);
	CAN_RX_Buf[3]=SPIByteRead(RXB0D3);
	CAN_RX_Buf[4]=SPIByteRead(RXB0D4);
	CAN_RX_Buf[5]=SPIByteRead(RXB0D5);
	CAN_RX_Buf[6]=SPIByteRead(RXB0D6);
	CAN_RX_Buf[7]=SPIByteRead(RXB0D7);
	SPIByteWrite(CANINTF,0x00);//清空接收中断标志位
	//打开接收缓冲器接收新信息，该句如何写
}*/
void CAN_Receive(unsigned char *CAN_RX_Buf)
{
	BitModiMcp2515(CANINTF,0x02,0x00);//清空接收中断标志位RX1IF
	
	CAN_RX_Buf[0]=SPIByteRead(RXB1D0);
	CAN_RX_Buf[1]=SPIByteRead(RXB1D1);
	CAN_RX_Buf[2]=SPIByteRead(RXB1D2);
	CAN_RX_Buf[3]=SPIByteRead(RXB1D3);
	CAN_RX_Buf[4]=SPIByteRead(RXB1D4);
	CAN_RX_Buf[5]=SPIByteRead(RXB1D5);
	CAN_RX_Buf[6]=SPIByteRead(RXB1D6);
	CAN_RX_Buf[7]=SPIByteRead(RXB1D7);
	//SPIByteWrite(CANINTF,0x00);     //清空接收中断标志位
	//BitModiMcp2515(CANINTF,0x02,0x00);//清空接收中断标志位RX1IF
	                                  //打开接收缓冲器接收新信息，该句如何写
}
#endif
