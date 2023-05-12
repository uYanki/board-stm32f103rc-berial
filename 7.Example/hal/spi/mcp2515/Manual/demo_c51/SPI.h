#ifndef __SPI_h__
#define __SPI_h__

#include <REG51.h>
#include <string.h>
#include <intrins.h>
#include <stdio.h>
#include "Function.h"

//管脚配置
sbit SCK=P2^3;
sbit SI=P2^2;
sbit SO=P2^1;
sbit CS=P2^0;


//功能说明：接收一个字节
unsigned char ReadSPI(void)
{
  unsigned char n=8;//从MISO线上读取一个数据字节
  unsigned char tdata;
  SCK=0;
  while(n--)
  {
    _nop_();
    _nop_();
    //delay1ms(1);//待修改
    SCK=1;
    //delay1ms(1);//同上
    tdata=tdata<<1;//左移一位
    if(SO==1)
    {
      tdata=tdata|0x01;//若收到的位为1,则数据的最后一位置1
    }
    else
    {
      tdata=tdata&0xfe;//否则数据的最后一位置0
    }
    
    SCK=0;
  }
  return tdata;
}

//功能函数：发送一个字节
void WriteSPI(unsigned char ch)
{
  unsigned char n=8;//向SI上发送一位数据字节，共八位
  SCK=0;
  while(n--)
  {
    _nop_();
    SCK=0;
    if((ch&0x80)==0x80)//若要发送的数据最高位为1，则发送1
    {
      SI=1;
    }
    else
    {
      SI=0;
    }
    _nop_();
    SCK=1;
    _nop_();
    //delay1ms(1);
    ch=ch<<1;//数据左移1位
  }
}

//向2515指定地址addr写一个字节数据value
void SPIByteWrite(unsigned char addr,unsigned char value)
{
  CS=0;
  WriteSPI(CAN_WRITE);
  WriteSPI(addr);
  WriteSPI(value);
  CS=1;
}

// 从2515指定地址addr读取一个字节数据value
unsigned char SPIByteRead(unsigned char addr)
{
  unsigned char tempdata;

  CS=0;
  WriteSPI(CAN_READ);
  WriteSPI(addr);
  tempdata=ReadSPI();
  CS=1;
  return tempdata;
}

//SPI复位
void SPIReset(void)
{
  CS=0;
  WriteSPI(CAN_RESET);
  CS=1;
}

#endif
