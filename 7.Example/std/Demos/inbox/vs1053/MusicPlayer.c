#include "MusicPlayer.h"
#include "VS1053.h"	  
#include "ff.h"   
#include "lcd.h"
#include "lcd_init.h"
//#include "bsp_usart1.h"

uint8_t 	databuf[4096]={0};	
FIL 		fmp3;
uint8_t *name;

//任务优先级
#define song_TASK_PRIO		2
//任务堆栈大小	
#define song_STK_SIZE 		128
//任务句柄
TaskHandle_t SongTask_Handler;
//任务函数
void song_task(void *pvParameters);

//播放一曲指定的歌曲				     	   									    	 
void mp3_play_song(uint8_t *pname)
{	 
  name=pname;
	if(SongTask_Handler!=NULL)
	vTaskDelete(SongTask_Handler);//删除任务
	xTaskCreate((TaskFunction_t )song_task,            //任务函数
							(const char*    )"song_task",          //任务名称
							(uint16_t       )song_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                  //传递给任务函数的参数
							(UBaseType_t    )song_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&SongTask_Handler);   //任务句柄 
}

//task任务函数
void song_task(void *pvParameters)
{
	uint16_t 	br;
	uint8_t 	res;	 	   
	uint16_t 	i=0;  	
	
	VS_Restart_Play();  					//重启播放 
	VS_Set_All();        					//设置音量等信息 			 
	VS_Reset_DecodeTime();					//复位解码时间 	
	
//	VS_Set_Vol(254);//设置为最大音量

	res=f_open(&fmp3,(const TCHAR*)name,FA_OPEN_EXISTING|FA_READ);//打开文件	
	
	if(res==0)//打开成功.
	{ 
		VS_SPI_SpeedHigh();	//高速						   
		while(1)
		{
			res=f_read(&fmp3,databuf,4096,(UINT*)&br);//读出4096个字节  
			i=0;
			do//主播放循环
			{  	
				if(VS_Send_MusicData(databuf+i)==0)//给VS1053发送音频数据
				{
					i+=32;
				}   	    
			}while(i<4096);//循环发送4096个字节 				
			if(br!=4096||res!=0)
			{
				break;//读完了.		  
			} 							 
		}		
	}  
	f_close(&fmp3);
	Show_Str(10,180,100,24,"播放结束",24,0);
	vTaskDelete(SongTask_Handler);//删除任务
}


























