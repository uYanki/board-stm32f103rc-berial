
//晶振=8M
//MCU=STC10F04XE
//P0.0-P0.6共阳数码管引脚
//Trig  = P1^0
//Echo  = P3^2
#include <reg52.h>     //包括一个52标准内核的头文件
#define uchar unsigned char //定义一下方便使用
#define uint  unsigned int
#define ulong unsigned long
//***********************************************
sfr  CLK_DIV = 0x97; //为STC单片机定义,系统时钟分频
                     //为STC单片机的IO口设置地址定义
sfr   P0M1   = 0X93;
sfr   P0M0   = 0X94;
sfr   P1M1   = 0X91;
sfr   P1M0   = 0X92;
sfr	P2M1   = 0X95;
sfr	P2M0   = 0X96;
//***********************************************
sbit Trig  = P1^0; //产生脉冲引脚
sbit Echo  = P3^2; //回波引脚
sbit test  = P1^1; //测试用引脚

uchar code SEG7[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//数码管0-9
uint distance[4];  //测距接收缓冲区
uchar ge,shi,bai,temp,flag,outcomeH,outcomeL,i;  //自定义寄存器
bit succeed_flag;  //测量成功标志
//********函数声明
void conversion(uint temp_data);
void delay_20us();
//void pai_xu();

void main(void)   // 主程序
{  uint distance_data,a,b;
   uchar CONT_1;   
   CLK_DIV=0X03; //系统时钟为1/8晶振（pdf-45页） 
     P0M1 = 0;   //将io口设置为推挽输出
     P1M1 = 0;
     P2M1 = 0;
     P0M0 = 0XFF;
     P1M0 = 0XFF;
     P2M0 = 0XFF;
   i=0;
   flag=0;
	test =0;
	Trig=0;       //首先拉低脉冲输入引脚
	TMOD=0x11;    //定时器0，定时器1，16位工作方式
	TR0=1;	     //启动定时器0
   IT0=0;        //由高电平变低电平，触发外部中断
	ET0=1;        //打开定时器0中断
 //ET1=1;        //打开定时器1中断
	EX0=0;        //关闭外部中断
	EA=1;         //打开总中断0	
  
	
while(1)         //程序循环
	{
  EA=0;
	     Trig=1;
        delay_20us();
        Trig=0;         //产生一个20us的脉冲，在Trig引脚  
        while(Echo==0); //等待Echo回波引脚变高电平
	     succeed_flag=0; //清测量成功标志
	     EX0=1;          //打开外部中断
	 	  TH1=0;          //定时器1清零
        TL1=0;          //定时器1清零
	     TF1=0;          //
        TR1=1;          //启动定时器1
   EA=1;

      while(TH1 < 30);//等待测量的结果，周期65.535毫秒（可用中断实现）  
		  TR1=0;          //关闭定时器1
        EX0=0;          //关闭外部中断

    if(succeed_flag==1)
	     { 	
		   distance_data=outcomeH;                //测量结果的高8位
           distance_data<<=8;                   //放入16位的高8位
		     distance_data=distance_data|outcomeL;//与低8位合并成为16位结果数据
            distance_data*=12;                  //因为定时器默认为12分频
           distance_data/=58;                   //微秒的单位除以58等于厘米
         }                                      //为什么除以58等于厘米，  Y米=（X秒*344）/2
			                                       // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58 
    if(succeed_flag==0)
		   {
            distance_data=0;                    //没有回波则清零
		   	test = !test;                       //测试灯变化
           }

     ///       distance[i]=distance_data; //将测量结果的数据放入缓冲区
     ///        i++;
  	  ///	 if(i==3)
	  ///	     {
	  ///	       distance_data=(distance[0]+distance[1]+distance[2]+distance[3])/4;
     ///        pai_xu();
     ///        distance_data=distance[1];

      
	   a=distance_data;
       if(b==a) CONT_1=0;
       if(b!=a) CONT_1++;
       if(CONT_1>=3)
		   { CONT_1=0;
			  b=a;
			  conversion(b);
			}       
	  ///		 i=0;
 	  ///		}	     
	 }
}
//***************************************************************
//外部中断0，用做判断回波电平
INTO_()  interrupt 0   // 外部中断是0号
 {    
     outcomeH =TH1;    //取出定时器的值
     outcomeL =TL1;    //取出定时器的值
     succeed_flag=1;   //至成功测量的标志
     EX0=0;            //关闭外部中断
  }
//****************************************************************
//定时器0中断,用做显示
timer0() interrupt 1  // 定时器0中断是1号
   {
 	 TH0=0xfd; //写入定时器0初始值
	 TL0=0x77;	 	
	 switch(flag)   
      {case 0x00:P0=ge; P2=0xfd;flag++;break;
	    case 0x01:P0=shi;P2=0xfe;flag++;break;
	    case 0x02:P0=bai;P2=0xfb;flag=0;break;
      }
   }
//*****************************************************************
/*
//定时器1中断,用做超声波测距计时
timer1() interrupt 3  // 定时器0中断是1号
    {
TH1=0;
TL1=0;
     }
*/
//******************************************************************
//显示数据转换程序
void conversion(uint temp_data)  
 {  
    uchar ge_data,shi_data,bai_data ;
    bai_data=temp_data/100 ;
    temp_data=temp_data%100;   //取余运算
    shi_data=temp_data/10 ;
    temp_data=temp_data%10;   //取余运算
    ge_data=temp_data;

    bai_data=SEG7[bai_data];
    shi_data=SEG7[shi_data];
    ge_data =SEG7[ge_data];

    EA=0;
    bai = bai_data;
    shi = shi_data;
    ge  = ge_data ; 
	 EA=1;
 }
//******************************************************************
void delay_20us()
 {  uchar bt ;
    for(bt=0;bt<100;bt++);
 }
/*
void pai_xu()
  {  uint t;
  if (distance[0]>distance[1])
    {t=distance[0];distance[0]=distance[1];distance[1]=t;} /*交换值
  if(distance[0]>distance[2])
    {t=distance[2];distance[2]=distance[0];distance[0]=t;} /*交换值
  if(distance[1]>distance[2])
    {t=distance[1];distance[1]=distance[2];distance[2]=t;} /*交换值	 
    }
*/