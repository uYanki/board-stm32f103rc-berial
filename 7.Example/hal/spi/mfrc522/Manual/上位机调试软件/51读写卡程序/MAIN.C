#include "reg52.h"
#include "main.h"
#include "mfrc522.h"	
#include <string.h>
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data2[4]  = {0x12,0,0,0};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
unsigned char g_ucTempbuf[20];    
unsigned char idata MLastSelectedSnr[4];
unsigned char idata RevBuffer[30];  
unsigned char data SerBuffer[20]; 
bit CmdValid; 			   

void iccardcode()
{	     
  	unsigned char cmd;
	unsigned char status;
	
	cmd = RevBuffer[0];
	switch(cmd)
 	{
		case 1:     // Halt the card     //终止卡的操作
			status= PcdHalt();;			
			RevBuffer[0]=1;
			RevBuffer[1]=status;
			break;			
		case 2:     // Request,Anticoll,Select,return CardType(2 bytes)+CardSerialNo(4 bytes)
			        // 寻卡，防冲突，选择卡    返回卡类型（2 bytes）+ 卡系列号(4 bytes)
			status= PcdRequest(RevBuffer[1],&RevBuffer[2]);
			if(status!=0)
			{
				status= PcdRequest(RevBuffer[1],&RevBuffer[2]);
				if(status!=0)				
				{
					RevBuffer[0]=1;	
					RevBuffer[1]=status;
					break;
				}
			}  
			RevBuffer[0]=3;	
			RevBuffer[1]=status;
			break;
			
		case 3:                         // 防冲突 读卡的系列号 MLastSelectedSnr
			status = PcdAnticoll(&RevBuffer[2]);
			if(status!=0)
			{
				RevBuffer[0]=1;	
				RevBuffer[1]=status;
				break;
			}
			memcpy(MLastSelectedSnr,&RevBuffer[2],4);
			RevBuffer[0]=5;
			RevBuffer[1]=status;
			break;	
		case 4:		                    // 选择卡 Select Card
			status=PcdSelect(MLastSelectedSnr);
			if(status!=MI_OK)
			{
				RevBuffer[0]=1;	
				RevBuffer[1]=status;
				break;
			}
			RevBuffer[0]=3;
			RevBuffer[1]=status;			
			break;
		case 5:	    // Key loading into the MF RC500's EEPROM
            status = PcdAuthState(RevBuffer[1], RevBuffer[3], DefaultKey, MLastSelectedSnr);// 校验卡密码
			RevBuffer[0]=1;
			RevBuffer[1]=status;			
			break;							
		case 6: 
			RevBuffer[0]=1;
			RevBuffer[1]=status;			
			break;				
		case 7:     
    		RevBuffer[0]=1;
			RevBuffer[1]=status;			
			break;
		case 8:     // Read the mifare card
		            // 读卡
			status=PcdRead(RevBuffer[1],&RevBuffer[2]);
			if(status==0)
			{RevBuffer[0]=17;}
			else
			{RevBuffer[0]=1;}
			RevBuffer[1]=status;			
			break;
		case 9:     // Write the mifare card
		            // 写卡  下载密码
			status=PcdWrite(RevBuffer[1],&RevBuffer[2]);
			RevBuffer[0]=1;
			RevBuffer[1]=status;			
			break;
		case 10:
            PcdValue(RevBuffer[1],RevBuffer[2],&RevBuffer[3]);
			RevBuffer[0]=1;	
			RevBuffer[1]=status;
			break;
		case 12:    // 参数设置
		    PcdBakValue(RevBuffer[1], RevBuffer[2]);
			RevBuffer[0]=1;	//contact
			RevBuffer[1]=0;
			break;		
	}






	}
/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
    P0 = 0xFF; P1 = 0xFF; P3 = 0xFF;



 	ET2 = 0; 	
	T2CON = 0x04;		
    PCON = 0x80;                
    SCON = 0x70;                 
    TMOD = 0x21;        //TMOD = 0x22;

	TH1   = BAUD_9600;
	TL1   = TH1;
	TR1   = 1;             // 波特率发生器

	TH0 = 0x60;
    TL0 = 0x60;
    TR0 = 0;
    
    ET0=0;
	ET1=0;
	EA=1;
	EX0=1;
	IT0 = 1;	
	TR2=0;
 	ES = 1;    
	CmdValid=0; 

     LED_GREEN = 1;
     PcdReset();
     PcdAntennaOff(); 
     PcdAntennaOn();  
	 M500PcdConfigISOType( 'A' );
     LED_GREEN = 0;
 	 delay_10ms(10);	
     LED_GREEN = 1;
 	 delay_10ms(10);
     LED_GREEN = 0;
}

/////////////////////////////////////////////////////////////////////
//用T2做延时子程序
/////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// 接收和发送中断
///////////////////////////////////////////////////////////////////////
void isr_UART(void) interrupt 4 using 1
{
    unsigned char len, i;
  	unsigned int j=0;
  	
  	if(RI)
	{		
		len=SBUF;
		RI=0;	
		for(i=0;i<len;i++)
		{
			while(!RI)
			{
				j++;
				if(j>1000)
				{
				    break;
				}
			}
			if(j<1000)
			{
				RevBuffer[i]=SBUF;
				RI=0;
				j=0;
			}
			else
			{
			    break;
			}
		}
		if(i==len)
		{
			REN=0;
			CmdValid=1;
		}
	}
	else if(!RI && TI)
	{
		TI=0;
		len=RevBuffer[0];
		for(i=0;i<len+1;i++)
		{
			SBUF=RevBuffer[i];
			while(!TI);
			TI=0;			
		}
		REN=1;
	}
}



  void main( )
{    
    unsigned char baud;
     InitializeSystem( );
    while (1)
    {	
 	    if (CmdValid)
	    {
		    CmdValid = FALSE;
		    if(RevBuffer[0]==11)	    //为了加快相应速度 测试风鸣器
		    {
		    	RevBuffer[2]=RevBuffer[1];
	  	    	RevBuffer[0]=1;         //contact
		    	RevBuffer[1]=0;
		    	CALL_isr_UART();		
                LED_GREEN = 1;
		    	delay_10ms(RevBuffer[2]);
                LED_GREEN = 0;
		    }
	    	else if(RevBuffer[0]==13)   //设置通讯波特率
	    	{
    			switch(RevBuffer[1])
    			{
    				case 0:
	    				baud=BAUD_9600;
	    				break;
	    			case 1:
	    				baud=BAUD_14400;
		    			break;
	    			case 2:
	    				baud=BAUD_19200;
	    				break;
	    			case 3:
	    				baud=BAUD_28800;
	    				break;
	    			case 4:
	    				baud=BAUD_38400;
	    				break;
	    			case 5:
	    				baud=BAUD_57600;
	    				break;
	    			case 6:
	    				baud=BAUD_115200;
	    				break;
	    			default:
	    				baud=BAUD_9600;
	    				break;
	    		}				
	    		RevBuffer[0]=1;	        //contact
	    		RevBuffer[1]=0;
	    		CALL_isr_UART();
	    		delay_10ms(5);			
	    		TR1   = 0;
	    		TH1   = baud;
	    		TL1   = TH1;
	    		delay_10ms(2);
	    		TR1   = TRUE;
	    	}

       		else
    		{
	            iccardcode();
    			CALL_isr_UART();
    		}



	 }
	 }
}


