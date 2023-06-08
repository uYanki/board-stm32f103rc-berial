//**** 声明 ********************************************************************
/*******************************************************************************
 * 下面来自互联开源程序，由深圳市大夏龙雀科技有限公司收集
 * 方便用户参考学习，本公司不提供任何技术支持
 * 程序仅供测试参考，不能应用在实际工程中，不一定能通过编译
 * 公司网站 http://www.szdx-smart.com/
 * 淘宝网址 https://shop184598174.taobao.com/?spm=a1z10.5-c-s.w12096189-21564973333.3.547b1176WCCDxR&scene=taobao_shop
*******************************************************************************/



//***************************************************************************//
//                                                                           //
//                       引脚功能定义                                        //
//                                                                           //
//***************************************************************************//

// P1 引脚定义
#define KP_INT                 BIT0 // 键盘中断 
#define J60_INT                BIT1 // ENC28J60中断
#define BL_CTR                 BIT2 // TFT背光控制
#define TP_INT                 BIT3 // 触摸屏中断
#define DIG5                   BIT4 // 数码管第5位
#define DIG6                   BIT5 // 数码管第6位
#define DIG7                   BIT6 // 数码管第7位
#define DIG8                   BIT7 // 数码管第8位

// P2 引脚定义
#define	DB10   	 	       BIT0 // TFT数据总线高8位         
#define	DB11   	 	       BIT1           
#define	DB12   	 	       BIT2          
#define	DB13   	 	       BIT3           
#define	DB14   	 	       BIT4           
#define	DB15   	 	       BIT5           
#define	DB16   	 	       BIT6           
#define	DB17   	 	       BIT7  

// P3 引脚定义
#define	NCS25  	 	       BIT0  // SST25V片选
#define SI25                   BIT1  // SST25V数据输入
#define	LCD_NRD	               BIT1  // LCD读信号  
#define SO25                   BIT2  // SST25V数据输出
#define	LCD_NRS 	       BIT2  // LCD寄存器选择  
#define	LCD_NWR   	       BIT3  // LCD写信号
#define SC25                   BIT3  // SST25V数据时钟
#define	TP_CS  	 	       BIT4  // XTP2046片选
#define	TP_BUSY	 	       BIT5  // XTP2046忙
#define	LCD_NCS                BIT6  // LCD片选
#define LCD_NRST   	       BIT7  // LCD复位

// P4 引脚定义
#define	DB00   	 	       BIT0  // TFT数据总线低8位        
#define	DB01   	 	       BIT1           
#define	DB02   	 	       BIT2          
#define	DB03   	 	       BIT3           
#define	DB04   	 	       BIT4           
#define	DB05   	 	       BIT5           
#define	DB06   	 	       BIT6           
#define	DB07   	 	       BIT7           

// P5 引脚定义
#define	NWOL   	               BIT0  // 
#define	J60_CS                 BIT1  // ENC28J60片选
#define	XT2IN	               BIT2  // XT2输入端
#define	XT2OUT	               BIT3  // XT2输出端
#define POWER 	               BIT4  // 外设电源控制端
#define	TXD_U                  BIT6  // USB串口输出
#define	RXD_U                  BIT7  // USB串口输入

// P6 引脚定义
#define	KPR0 	 	       BIT0  // 键盘0行
#define	KPR1   	               BIT1  // 键盘1行
#define	KPR2	               BIT2  // 键盘2行
#define	KPR3	               BIT3  // 键盘3行
#define KPC0   	 	       BIT4  // 键盘0列
#define	KPC1  	 	       BIT5  // 键盘1列
#define	KPC2  	               BIT6  // 键盘2列
#define	KPC3                   BIT7  // 键盘3列

// P7 引脚定义
#define	XT1IN   	       BIT0  // XT1输入
#define	XT1OUT                 BIT1  // XT1输出  
#define	LED_PWR                BIT3  // LED电源控制
#define DE_485    	       BIT4  // 485数据控制端
#define	TVBTM	               BIT5  // 主电源电压检测输入  
#define	TVBTB                  BIT6  // 备用电池电压检测输入  
#define	BUZZER                 BIT7  // 蜂鸣器控制端

// P8 引脚定义
#define	SEGA    	       BIT0  // 数码管段选
#define	SEGB    	       BIT1  
#define	SEGC    	       BIT2  
#define	SEGD    	       BIT3  
#define	SEGE    	       BIT4  
#define	SEGF    	       BIT5  
#define	SEGG    	       BIT6  
#define	SEGDP    	       BIT7  

// P9 引脚定义
#define DIG1                   BIT0  // 数码管第1位
#define DIG2                   BIT1  // 数码管第2位
#define DIG3                   BIT2  // 数码管第3位
#define DIG4                   BIT3  // 数码管第4位
#define IrDA_OUT               BIT4  // 红外输出
#define IrDA_IN                BIT5  // 红外输入
#define SDA5571                BIT6  // DAC5571数据
#define SCK5571                BIT7  // DAC5571时钟

// P10 引脚定义
#define	PNSS    	       BIT0  // 
#define	PMOSI                  BIT1  // 通用IO01\模拟输入1  
#define	PMISO                  BIT2  // 通用IO02\模拟输入2  
#define	PSCK                   BIT3  // 通用IO03\模拟输入3  
#define TXD 	               BIT4  // 通用IO04\模拟输入4  
#define	RXD	               BIT5  // 通用IO05\模拟输入5  
#define	DIO_B20                BIT6  // DS18B20数据
#define	DE485                  BIT7  // DE485  重复定义，新版改

// P11 引脚定义
#define TACK                   BIT0  // ACLK测试端
#define TMCK                   BIT1  // MCLK测试端
#define TSMCK                  BIT2  // SMCLK测试端

//****************************************************************************
//                                                                           *
//                       引脚操作宏定义                                      *
//                                                                           *
//****************************************************************************

//------------------------------- 主电源开关 -------------------------------//
#define MAIN_POWER_ON          P5DIR  |= POWER;\
                               P5OUT  |= POWER
#define MAIN_POWER_OFF         P5OUT  &=~POWER

//----------------------------- TFT液晶操作宏 ------------------------------//
#define  LCD_POWER_ON          P1OUT  |= BL_CTR   // LCD背光
#define  LCD_POWER_OFF         P1OUT  &=~BL_CTR  
#define  SET_LCD_RST           P3OUT  |= LCD_NRST // LCD复位
#define  CLR_LCD_RST           P3OUT  &=~LCD_NRST
#define  SET_LCD_CS            P3OUT  |= LCD_NCS  // LCD片选
#define  CLR_LCD_CS            P3OUT  &=~LCD_NCS
#define  SET_LCD_RS            P3OUT  |= LCD_NRS  // LCD寄存器选择
#define  CLR_LCD_RS            P3OUT  &=~LCD_NRS
#define  SET_LCD_WR            P3OUT  |= LCD_NWR  // LCD写信号
#define  CLR_LCD_WR            P3OUT  &=~LCD_NWR 
#define  SET_LCD_RD            P3OUT  |= LCD_NRD  // LCD读信号
#define  CLR_LCD_RD            P3OUT  &=~LCD_NRD
#define  LOW_BYTE_OUT(data)    P4OUT   = data
#define  HIGH_BYTE_OUT(data)   P2OUT   = data
#define  BUS_IN                P2DIR   = 0x00;P4DIR   = 0x00
#define  BUS_OUT               P2DIR   = 0xFF;P4DIR   = 0xFF

//---------------------------------- SST25V 操作宏 -------------------------//
#define SET_NCS25              P3OUT  |= NCS25    // SST25VF片选
#define CLR_NCS25              P3OUT  &=~NCS25

//---------------------------------- 键盘操作宏 ----------------------------//
#define ROW_IN_COL_OUT         P4DIR   =  KPC0+KPC1+KPC2+KPC3      ;\
                               P4OUT   =~(KPC0+KPC1+KPC2+KPC3)     ;\
                               __delay_cycles(20)                      // 键盘列输出行输入                                        
#define CLO_IN_ROW_OUT         P4DIR   =  KPR0+KPR1+KPR2+KPR3      ;\
                               P4OUT   =~(KPR0+KPR1+KPR2+KPR3)     ;\
                               __delay_cycles(20)                      // 键盘行输出列输入

//---------------------------------- 键码定义-------------------------------//
#define No_key   255
#define F1        10
#define F2        11
#define F3        12
#define Delete    12
#define Backspace 13
#define Enter     14
#define OK        15
#define Up        16
#define Down      17
#define Left      18
#define Right     19
#define Esc       20
#define Cancel    20
#define Power     21
#define Power1    21
#define Power2    22
#define Light     22
#define Update   255
