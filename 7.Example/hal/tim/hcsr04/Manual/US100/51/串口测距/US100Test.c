
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

unsigned char LastRcvData;
unsigned char GLengthHigh, GLengthLow, GTempera;
unsigned char RcvIndex;

void UART_ISR(void) interrupt 4
{
	RI = 0;							// clear receive interrupt flag
	LastRcvData=SBUF;						// copy input bufffer to ch
	if(RcvIndex == 0)
	{
		GLengthHigh = LastRcvData;
		RcvIndex++;
	}
	else if(RcvIndex == 1)
	{
		GLengthLow = LastRcvData;
		RcvIndex = 150;
	}
	else if(RcvIndex == 100)
	{
		GTempera = LastRcvData;
		RcvIndex++;
	}
}

void UARTSnd(unsigned char dat)
{

	TI=0;						// clear transmit interrupt flag
	SBUF=dat;				// start sending one byte
	while (!TI);				// wait until sent

}

 
void UART_init()
{	
	SCON=0x50;				// select BRG as UART Baud Rate Gen
	SSTAT=0x60;				// separate Rx / Tx interrupts
	BRGR0=0xF0;				// setup BRG for 9600 baud @ 7.373MHz internal RC oscillator
	BRGR1=0x02;
	//BRGR0=0x70;				// setup BRG for 9600 baud @ 12MHz external Crystal
//	BRGR1=0x04;
	BRGCON = 0x03;     		// enable BRG

	P1M1 = 0x00;     //push pull except RX
	P1M2 = 0xFD;
	ES = 1;			// enable UART interrupt
	EA = 1;

	RcvIndex = 0;
}


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


void MeterLenByUART()
{
	RcvIndex = 0;
	UARTSnd(0X55);
}

void MeterTempByUART()
{
	RcvIndex = 100;
	UARTSnd(0X50);
}

void main(void)
{
	unsigned char tempa;
	unsigned long PreLength;

	InitIOPort();
	UART_init();
	
	GLengthHigh = 0;
	GLengthLow = 0;
	while(1)
	{
		MeterLenByUART();// meter length, and set PreLength.
		for(tempa = 0; tempa < 100; tempa++)//display Length
		{
			PreLength = GLengthHigh;
			PreLength <<= 8;
			PreLength += GLengthLow;
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


