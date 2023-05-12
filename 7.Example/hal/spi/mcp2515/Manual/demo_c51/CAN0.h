#ifndef __CAN_H__
#define __CAN_H__


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


unsigned char DLC,dummy;

void Init2515(void)
{
  SPIReset();
  delay1ms(1);

  //SPIByteWrite(CANCTRL,0x80);//CAN工作在配置模式

  SPIByteWrite(RXM0SIDH,0x00);
  SPIByteWrite(RXM0SIDL,0x00);

  SPIByteWrite(RXF0SIDH,0x00);
  SPIByteWrite(RXF0SIDL,0x00);
  //设置波特率为10Kbps
  //set CNF1,SJW=00,长度为1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/8M=12.5us
  SPIByteWrite(CNF1,CAN_125Kbps);
  //set CNF2,SAM=0,在采样点对总线进行一次采样，PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ
  SPIByteWrite(CNF2,0x80|PHSEG1_3TQ|PRSEG_1TQ);
  //set CNF3,PHSEG2=(2+1)TQ=3TQ,同时当CANCTRL.CLKEN=1时设定CLKOUT引脚为时间输出使能位
  SPIByteWrite(CNF3,PHSEG2_3TQ);

  //set TXB0，设置发送缓冲器0的标识符和发送的数据，以及发送的数据长度
  SPIByteWrite(TXB0SIDH,0xFF);//设置发送缓冲器0的标准标识符，待修改***
  SPIByteWrite(TXB0SIDL,0xE0);//用到标准标识符

  //SPIByteWrite(TXB0D0,0x1E);//有待修改及确定是否使用
  //SPIByteWrite(TXB0D1,0x10)；//有待修改及确定是否使用

  /*set TXB1
  SPIByteWrite(TXB1SIDH,0x50);    //Set TXB0 SIDH
  SPIByteWrite(TXB1SIDL,0x00);    //Set TXB0 SIDL
  SPIByteWrite(TXB1DLC,0x40 | DLC_8);    //Set DLC = 3 bytes and RTR bit*/

  //设置接收缓冲器0的标识符和初始化数据
  SPIByteWrite(RXB0SIDH,0x00);//设置接收缓冲器0的标准标识符，待修改***
  SPIByteWrite(RXB0SIDL,0x60);//用到标准标识符
  SPIByteWrite(RXB0CTRL,0x60);//仅仅接收标准标识符的有效信息，FIILHIT0=0表示RXB0 ，采用FILHIT0
  SPIByteWrite(RXB0DLC,DLC_8);//设置接收数据的长度为8个字节

  SPIByteWrite(RXF0SIDH,0xFF);//初始化接收滤波器0，待修改***
  SPIByteWrite(RXF0SIDL,0xE0);
  SPIByteWrite(RXM0SIDH,0xFF);//初始化接收屏蔽器0，待修改***
  SPIByteWrite(RXM0SIDL,0xE0);

  //设置接收缓冲器0中断
  SPIByteWrite(CANINTF,0x00);//清空中断标志位
  SPIByteWrite(CANINTE,0x01);//接收缓冲器0满中断使能位

  SPIByteWrite(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//设置正常模式

  dummy=SPIByteRead(CANSTAT);
  if(OPMODE_NORMAL!=(dummy&&0xE0))
  {
    SPIByteWrite(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//判断进入正常工作模式
  }

}

//CAN发送任意长度字节
void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1)
{
  unsigned char tempdata,j;
  tempdata=SPIByteRead(CAN_RD_STATUS);
  SPIByteWrite(TXB0DLC,length1);
  for(j=0;j<length1;j++)
  {
    SPIByteWrite(TXB0D0+j,CAN_TX_Buf[j]);
  }

  if(tempdata&0x04)//判断TXREQ标志位
  {
    delay1ms(1);
    SPIByteWrite(TXB0CTRL,0);//清除TXREQ标志位
    while(SPIByteRead(CAN_RD_STATUS)&0x04);//等待TXREQ清零
  }
  CS=0;
  WriteSPI(CAN_RTS_TXB0);//发送缓冲器0请求发送
  CS=1;
}

//CAN接收任意长度的数据
void CAN_Receive_DLC(unsigned char *CAN_RX_Buf)
{
  unsigned char i;
  DLC=SPIByteRead(RXB0DLC);
  for(i=0;i<DLC;i++)
  {
    //把接收缓冲区里的数据，放到内部RAM缓冲区
    CAN_RX_Buf[i]=SPIByteRead(RXB0D0+i);
  }

  SPIByteWrite(CANINTF,0);
}

#endif
