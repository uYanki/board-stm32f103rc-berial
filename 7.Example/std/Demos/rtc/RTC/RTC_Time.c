/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               RTC_Time.c
** Descriptions:            The RTC application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/*******************************************************************************
* 本文件实现基于RTC的日期功能，提供年月日的读写。（基于ANSI-C的time.h）
* 

* RTC中保存的时间格式，是UNIX时间戳格式的。即一个32bit的time_t变量（实为u32）
*
* ANSI-C的标准库中，提供了两种表示时间的数据  型：
* time_t:   	UNIX时间戳（从1970-1-1起到某时间经过的秒数）
* 	typedef unsigned int time_t;
* 
* struct tm:	Calendar格式（年月日形式）
*   tm结构如下：
*   struct tm {
*   	int tm_sec;   // 秒 seconds after the minute, 0 to 60
*   					 (0 - 60 allows for the occasional leap second)
*   	int tm_min;   // 分 minutes after the hour, 0 to 59
*		int tm_hour;  // 时 hours since midnight, 0 to 23
*		int tm_mday;  // 日 day of the month, 1 to 31
*		int tm_mon;   // 月 months since January, 0 to 11
*		int tm_year;  // 年 years since 1900
*		int tm_wday;  // 星期 days since Sunday, 0 to 6
*		int tm_yday;  // 从元旦起的天数 days since January 1, 0 to 365
* 		int tm_isdst; // 夏令时？？Daylight Savings Time flag
* 		...
* 	}
* 	其中wday，yday可以自动产生，软件直接读取
* 	mon的取值为0-11
*	***注意***：
*	tm_year:在time.h库中定义为1900年起的年份，即2008年应表示为2008-1900=108
* 	这种表示方法对用户来说不是十分友好，与现实有较大差异。
* 	所以在本文件中，屏蔽了这种差异。
* 	即外部调用本文件的函数时，tm结构体类型的日期，tm_year即为2008
* 	注意：若要调用系统库time.c中的函数，需要自行将tm_year-=1900
* 
* 成员函数说明：
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	输入一个Unix时间戳（time_t），返回Calendar格式日期
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	输入一个Calendar格式日期，返回Unix时间戳（time_t）
* time_t Time_GetUnixTime(void);
* 	从RTC取当前时间的Unix时间戳值
* struct tm Time_GetCalendarTime(void);
* 	从RTC取当前时间的日历时间
* void Time_SetUnixTime(time_t);
* 	输入UNIX时间戳格式时间，设置为当前RTC时间
* void Time_SetCalendarTime(struct tm t);
* 	输入Calendar格式时间，设置为当前RTC时间
* 
* 外部调用实例：
* 定义一个Calendar格式的日期变量：
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12月
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* 获取当前日期时间：
* tm_now = Time_GetCalendarTime();
* 然后可以直接读tm_now.tm_wday获取星期数
* 
* 设置时间：
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* 计算两个时间的差
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt就是两个时间差的秒数
* dt_tm = mktime(dt);	//注意dt的年份匹配，ansi库中函数为相对年份，注意超限
* 另可以参考相关资料，调用ansi-c库的格式化输出等功能，ctime，strftime等
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "RTC_Time.h" 

/* Private define ------------------------------------------------------------*/
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */  

/* Private function prototypes -----------------------------------------------*/
void Time_Set(u32 t);

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar
* Description    : 转换UNIX时间戳为日历时间
* Input          : - t: 当前时间的UNIX时间戳
* Output         : None
* Return         : struct tm
* Attention		 : None
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	/* localtime转换结果的tm_year是相对值，需要转成绝对值 */
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix
* Description    : 写入RTC时钟当前时间
* Input          : - t: struct tm
* Output         : None
* Return         : time_t
* Attention		 : None
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  /* 外部tm结构体存储的年份为2008格式	*/
						/* 而time.h中定义的年份格式为1900年开始的年份 */
						/* 所以，在日期转换时要考虑到这个因素。*/
	return mktime(&t);
}


/*******************************************************************************
* Function Name  : Time_GetUnixTime
* Description    : 从RTC取当前时间的Unix时间戳值
* Input          : None
* Output         : None
* Return         : time_t
* Attention		 : None
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime
* Description    : 从RTC取当前时间的日历时间（struct tm）
* Input          : None
* Output         : None
* Return         : struct tm
* Attention		 : None
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime
* Description    : 将给定的Unix时间戳写入RTC
* Input          : - t: time_t 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
* Input          : - t: struct tm
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : USART_Scanf
* Description    : USART Receive
* Input          : - min_value: 
*                  - max_value:
*                  - lenght:
* Output         : None
* Return         : uint8_t
* Attention		 : None
*******************************************************************************/
static uint16_t USART_Scanf(uint32_t min_value,uint32_t max_value,uint8_t lenght)
{
  uint16_t index = 0;
  uint32_t tmp[4] = {0, 0, 0, 0};

  while (index < lenght)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(USART1));

	if( tmp[index - 1] == 0x0D ) { index--; continue; }

    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("Please enter valid number between 0 and 9\r\n");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  if( lenght ==2 )
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10 );

  else  /* lenght ==4 */
  index = (tmp[3] - 0x30) + ((tmp[2] - 0x30) * 10 ) + ((tmp[1] - 0x30) * 100 ) + ((tmp[0] - 0x30) * 1000 );
  /* Checks */
  if (index > max_value || index < min_value)
  {
	printf("Please enter valid number between %d and %d\r\n", min_value, max_value);
    return 0;
  }


  return index;
}

/*******************************************************************************
* Function Name  : Time_Regulate
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_Regulate(void)
{
  struct tm time;

  memset(&time, 0 , sizeof(time) );	/* 清空结构体 */

  printf("=======================Time Settings==========================\r\n");
  printf("Please Set Years between 1970 to 2037\r\n");

  while ( time.tm_year>2037  || time.tm_year<1970)
  {   
    time.tm_year = USART_Scanf(1970,2037,4);
  }
  printf("Set Years:  %d\r\n", time.tm_year);

  printf("Please Set Months between 01 to 12\r\n");
  while (time.tm_mon >12 || time.tm_mon < 1 )
  {
    time.tm_mon= USART_Scanf(1,12,2)-1;
  }
  printf("Set Months:  %d\r\n", time.tm_mon);

  printf("Please Set Days between 01 to 31\r\n");
  while (time.tm_mday >31 ||time.tm_mday <1 )
  {
    time.tm_mday = USART_Scanf(1,31,2);
  }
  printf("Set Days:  %d\r\n", time.tm_mday);

  printf("Please Set Hours between 01 to 23\r\n");
  while (time.tm_hour >23 ||time.tm_hour <1 )
  {
    time.tm_hour = USART_Scanf(1,23,2);
  }
  printf("Set Hours:  %d\r\n", time.tm_hour);

  printf("Please Set Minutes between 01 to 59\r\n");
  while (time.tm_min >59 || time.tm_min <1 )
  {
    time.tm_min = USART_Scanf(1,59,2);
  }
  printf("Set Minutes:  %d\r\n", time.tm_min);

  printf("Please Set Seconds between 01 to 59\r\n");
  while (time.tm_sec >59 || time.tm_sec <1 )
  {
    time.tm_sec = USART_Scanf(1,59,2);
  }
  printf("Set Seconds:  %d\r\n", time.tm_sec);
  /* Return the value to store in RTC counter register */
  Time_SetCalendarTime(time);  
}

/*******************************************************************************
* Function Name  : RTC_Init
* Description    : RTC Initialization
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None	
*******************************************************************************/
void RTC_Init(void)
{

  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("RTC not yet configured....\r\n");

    /* RTC Configuration */
    RTC_Configuration();

	Time_Regulate();

	/* Adjust time by values entred by the user on the hyperterminal */

    printf("RTC configured....\r\n");

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("Power On Reset occurred....\r\n");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("External Reset occurred....\r\n");
    }

    printf("No need to configure RTC....\r\n");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }

   /* NVIC configuration */
   NVIC_Configuration();

#ifdef RTCClockOutput_Enable
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

   /* Clear reset flags */
  RCC_ClearFlag();
  return;
}

/*******************************************************************************
* Function Name  : Time_Display
* Description    : Printf Time
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_Display(void)
{
   struct tm time;
   time = Time_GetCalendarTime();
   printf("Time: %d-%d-%d   %02d:%02d:%02d \r\n", time.tm_year, \
                   time.tm_mon+1, time.tm_mday,\
                   time.tm_hour, time.tm_min, time.tm_sec);
}
