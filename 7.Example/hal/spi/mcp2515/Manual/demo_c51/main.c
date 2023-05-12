//=================================================================================
//串口到can       4800bps
//
//sbit SCK=P2^3;
//sbit SI=P2^2;
//sbit SO=P2^1;
//sbit CS=P2^0;
//INT 接P3.3
//
//=================================================================================

#include "reg51.h"
#include "2515.h"
#include "SPI.h"
#include "Function.h"

#include "CAN0.h"

void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);


unsigned char gRXFlag=0,sRXFlag=0,yRXFlag=0;
unsigned char data Com_RecBuff[8];
unsigned char bdata flag_init;

//*************************
#define INBUF_LEN 8   //数据长度

unsigned char inbuf1[INBUF_LEN];
unsigned char checksum,count3=0;


unsigned char a[8];
unsigned char b[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};


/************************************************************************
*函数原型: void init_serialcomm(void)            *
*参数说明: 串口初始化                            *
*说明: 设值单片机的定时器1的方式选择波特率 。该子程序只能用于复位模式
************************************************************************/
void init_serialcomm(void)
{
  SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
  TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
  PCON |= 0x80;       //SMOD=1; 
  TH1   = 0xF4;       //Baud:4800  fosc=11.0592MHz  
  TL1   = 0xF4;       //
  IE   |= 0x90;       //Enable Serial Interrupt 
  TR1   = 1;          // timer 1 run
}

//定时器0初始化
void timer0initial()
{
  TMOD|=0x1;        //工作方式16位定时计数器
  TH0=0xb8;TL0=0x00;  //50MS定时初值（T0计时用）
  IE|=0x90;       //Enable Serial Interrupt 
  ET0=0;
  TR0=0;
}

//向串口发送一个字符
void send_char_com(unsigned char ch)
{
  SBUF=ch;
  while(!TI);
  TI=0;
}

//向串口发送一个字符串,strlen为该字符串长度
void send_string_com(unsigned char *str,unsigned int strlen)
{
  unsigned int k=0;
  do
  {
    send_char_com(*(str+k));
    k++;
  }while(k<strlen);
}

//定时器0中断,不够8个就在此发送
void time_intt0(void) interrupt 1 using 2
{
  ET0=0;
  TR0=0;
  TH0=0xb8;
  TL0=0x00;
  sRXFlag=1;
}

//串口接收中断函数
void serial() interrupt 4 using 1
{
  unsigned char ch;

  if(RI)
  {
    RI=0;
    ET0=1;TR0=1;
    ch=SBUF;

    inbuf1[count3]=ch;
    count3=count3+1;
    if(count3==INBUF_LEN)
    {
      ET0=0;
      TR0=0;
      TH0=0xb8;
      TL0=0x00;  //关闭定时中断，停止定时中断
      sRXFlag=1;
    }
  }
}


/****************************************************
**函数原型：  void ex1_int(void) interrupt 2 //using 1
**功    能：  外部中断1接收函数
**入口参数:   无 
**出口参数:   RevceData[]数组   
**说    明:   当MCP2515收到正确的报文时,会产生int1中断
*****************************************************/
void ex1_int(void) interrupt 2 using 1
{
  gRXFlag=1;
}


/****************************************************
**函数原型：  void Init_Cpu(void)
**功    能：  单片机初始化,开放外部中断1
**入口参数:   无 
**出口参数:    
**说    明:   单片机初始化,开放外部中断1  
*****************************************************/
void Init_Cpu(void)
{
  PX1=1;
  EX1=1;
  IT1=0;
  EA=1;
}

void main()
{

  Init2515();
  init_serialcomm();     //初始化串口
  timer0initial();       //定时器0初始化
  Init_Cpu();

  while(1)
  {
    if(sRXFlag)
    {
      sRXFlag=0;
      CAN_Send_anylength(inbuf1,count3);
      count3=0;
    }

    if(gRXFlag)
    {
      CAN_Receive_DLC(a);
      gRXFlag=0;
      send_string_com(a,DLC);
    }
  }
}
