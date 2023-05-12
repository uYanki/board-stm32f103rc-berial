//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC522
sbit     MF522_RST  =    P1^4;                   //RC500片选
sbit     MF522_NSS  =    P1^2;
sbit     MF522_SCK  =    P1^1;
sbit     MF522_SI   =    P1^0;
sbit     MF522_SO   =    P1^3;
//指示灯
sbit     LED_GREEN  =    P1^5;     
/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem();                                   
#define BAUD_115200             256 - (OSC_FREQ/192L)/115200L   // 255
#define BAUD_57600              256 - (OSC_FREQ/192L)/57600L    // 254
#define BAUD_38400              256 - (OSC_FREQ/192L)/38400L    // 253
#define BAUD_28800              256 - (OSC_FREQ/192L)/28800L    // 252
#define BAUD_19200              256 - (OSC_FREQ/192L)/19200L    // 250
#define BAUD_14400              256 - (OSC_FREQ/192L)/14400L    // 248
#define BAUD_9600               256 - (OSC_FREQ/192L)/9600L     // 244                                   
#define OSC_FREQ                22118400L
//#define  OSC_FREQ        7372800L
#define OSC_FREQ          22118400L
#define  RCAP2_50us      65536L - OSC_FREQ/40417L
#define  RCAP2_1ms       65536L - OSC_FREQ/2000L
#define  RCAP2_10ms      65536L - OSC_FREQ/1200L
#define  TIME0_500us     65536L - OSC_FREQ/8000L
#define  TIME0_10ms      65536L - OSC_FREQ/200





#define CALL_isr_UART()         TI = 1
#define TRUE 1
#define FALSE 0

