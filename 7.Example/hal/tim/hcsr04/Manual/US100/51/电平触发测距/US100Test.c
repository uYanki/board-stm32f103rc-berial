
#include "US100Test.h"

void Delay1Ms(void);
void Delay5Ms(void);
void Delay400Ms(void);

void UART_ISR(void);
void UARTSnd(unsigned char dat);
void UART_init();
void init(void);

sbit RCK_STMM = P2^7;
sbit DIO_DS	  = P2^6;
sbit SCK_SHBit = P2^5;

sbit Trig_Mode = P2^0;
sbit UART_Mode = P2^1;
sbit UART_TEMP  = P2^2;

sbit Trig_Pin = P1^7;
sbit Echo_Pin = P1^6;

void InitIOPort()
{	
//	P0M1 = 0XFF //p0口设为输入。
	P0M1 = 0;//P0口设为双向口，可输入输出
	P0M2 = 0;//P0口设为双向口，可输入输出

	P2M1 &= 0X0F;//P2口的P2.5，P2.6，P2.7设为双向口，可输入输出
	P2M2 &= 0X0F;//P2口的P2.5，P2.6，P2.7设为双向口，可输入输出
}


void DisplayLED(unsigned char LEDid, unsigned char Value, unsigned char Isdot)
{
	unsigned char tempa, tempb;
	unsigned char tempc, tempd;
	const unsigned char NumValue[11] = {0X40, 0X79, 0X24, 0X30, 0X19, 0X12, 0X02, 0X78, 0X00, 0X10, 0X3F};//0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -.
	
	tempa = 1;
	tempa <<= (LEDid-1);
	if(Value >= 11)//value should little than 10
		Value = 0;
	tempb = NumValue[Value];
	if(Isdot == 0)
		tempb += 0X80;

	//begin output
	tempd = 0X80;
	for(tempc = 0; tempc < 8; tempc++)
	{
		SCK_SHBit = 0;
		SCK_SHBit = 0;
		DIO_DS  = (tempa&tempd);
		SCK_SHBit = 1;
		SCK_SHBit = 1;
		tempd >>= 1;
	}

	tempd = 0X80;
	for(tempc = 0; tempc < 8; tempc++)
	{
		SCK_SHBit = 0;
		SCK_SHBit = 0;
		DIO_DS  = (tempb&tempd);
		SCK_SHBit = 1;
		SCK_SHBit = 1;
		tempd >>= 1;
	}

	RCK_STMM = 0;
	RCK_STMM = 0;
	RCK_STMM = 0;
	RCK_STMM = 0;
	RCK_STMM = 1;
	RCK_STMM = 1;
	RCK_STMM = 1;
	RCK_STMM = 1;
	//Delay1Ms();
}

void DisplayLength(unsigned long number)
{
	unsigned char Thousand, Hundred, Decade, Unit;
	if(number > 9999)
		number = 0;
	Thousand = number/1000;

	number = number%1000;
	Hundred = number/100;

	number = number%100;
	Decade = number/10;

	Unit = number%10;

	DisplayLED(1, Thousand, 0);
	DisplayLED(2, Hundred, 0);
	DisplayLED(3, Decade, 0);
	DisplayLED(4, Unit, 0);
}


unsigned long MeterByTrig()
{
	unsigned char tempa;
	unsigned char IsOverFlow;
	unsigned long PresentTime, tempb;
	IsOverFlow = 0;

	//Delay1Ms();

	P1M1 &= 0X7F;
	P1M2 &= 0X7F;

	P1M1 |= 0X40;
	P1M2 &= 0XBF;


	//Timer Init
	TMOD = 0X11;
	TAMOD = 0X00;
	TCON = 0X00;
	TH1 = 0;
	TL1 = 0;

	Trig_Pin = 0;
	for(tempa = 0; tempa < 10; tempa++)
	{
		Trig_Pin = 1;
	}
	Trig_Pin = 0;

	while(1)
	{
		if(Echo_Pin == 1)
		{//Start Timer;
			TCON = 0X40;//start timer
			break;
		}
		if(TCON&0X80)
		{
			IsOverFlow = 1;
			break;//overflow;
		}
	}

	while(1)
	{
		if(Echo_Pin == 0)
		{//Stop Timer;
			TCON = 0X00;
			break;
		}
		if(TCON&0X80)
		{
			IsOverFlow = 1;
			break;//overflow;
		}
	}

//	unsigned long PresentTime, tempb;
	PresentTime = TH1;
	PresentTime <<= 0X08;
	PresentTime += TL1;

	tempb = PresentTime/137;
	PresentTime /= 7;//*3

	PresentTime -= tempb;//time, us

	tempb = PresentTime/150;
	PresentTime /= 3;
	PresentTime += tempb;
	//Delay1Ms();
	return PresentTime;
}

void main(void)
{
	unsigned char tempa;
	unsigned long PreLength;

	InitIOPort();

	while(1)
	{
		PreLength = MeterByTrig();
		for(tempa = 0; tempa < 200; tempa++)
		{
			DisplayLength(PreLength);
		}
	}
}


//1ms延时
void Delay1Ms(void)
{
 unsigned long TempCyc = 90;//36865;//5552;
 while(TempCyc--);
}
//5ms延时
void Delay5Ms(void)
{
 unsigned long TempCyc = 460;//36865;//5552;
 while(TempCyc--);
}

//400ms延时
void Delay400Ms(void)
{
 unsigned char TempCycA = 80;//5;
 unsigned long TempCycB;
 while(TempCycA--)
 {
  TempCycB=460;//50000; //7269;
  while(TempCycB--);
 };

}


