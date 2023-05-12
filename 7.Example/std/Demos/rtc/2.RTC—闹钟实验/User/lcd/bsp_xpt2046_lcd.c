#include "bsp_xpt2046_lcd.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h> 
#include <string.h>



/******************************* 声明 XPT2046 相关的静态函数 ***************************/
static void                   XPT2046_EXTI_Config                   ( void );
static void                   XPT2046_EXTI_NVIC_Config              ( void );
static void                   XPT2046_GPIO_SPI_Config               ( void );

static void                   XPT2046_DelayUS                       ( __IO uint32_t ulCount );
static void                   XPT2046_WriteCMD                      ( uint8_t ucCmd );
static uint16_t               XPT2046_ReadCMD                       ( void );
static uint16_t               XPT2046_ReadAdc                       ( uint8_t ucChannel );
static void                   XPT2046_ReadAdc_XY                    ( int16_t * sX_Ad, int16_t * sY_Ad );
static uint8_t                XPT2046_ReadAdc_Smooth_XY             ( strType_XPT2046_Coordinate * pScreenCoordinate );
static uint8_t                XPT2046_Calculate_CalibrationFactor   ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_Coordinate * pScreenSample, strType_XPT2046_Calibration * pCalibrationFactor );
static void                   ILI9341_DrawCross                     ( uint16_t usX, uint16_t usY );



/******************************* 定义 XPT2046 全局变量 ***************************/
strType_XPT2046_TouchPara strXPT2046_TouchPara = { 0.085958, -0.001073, -4.979353, -0.001750, 0.065168, -13.318824 };  //初始值为扫描方式2下的一组校准系数，不校准触摸屏时可使用该组默认系数
                                              // { 0.001030, 0.064188, -10.804098, -0.085584, 0.001420, 324.127036 };  //初始值为扫描方式1下的一组校准系数，不校准触摸屏时可使用该组默认系数

volatile uint8_t ucXPT2046_TouchFlag = 0;



/**
  * @brief  XPT2046 初始化函数
  * @param  无
  * @retval 无
  */	
void XPT2046_Init ( void )
{
	XPT2046_GPIO_SPI_Config ();
	
	XPT2046_EXTI_Config ();
		
}


/**
  * @brief  配置 XPT2046 外部中断优先级
  * @param  无
  * @retval 无
  */	
static void XPT2046_EXTI_NVIC_Config ( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = macXPT2046_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}


/**
  * @brief  配置 XPT2046 外部中断
  * @param  无
  * @retval 无
  */	
static void XPT2046_EXTI_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	
	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd ( macXPT2046_EXTI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE );
												
	/* config the NVIC */
	XPT2046_EXTI_NVIC_Config ();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = macXPT2046_EXTI_GPIO_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(macXPT2046_EXTI_GPIO_PORT, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(macXPT2046_EXTI_SOURCE_PORT, macXPT2046_EXTI_SOURCE_PIN); 
  EXTI_InitStructure.EXTI_Line = macXPT2046_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
  EXTI_Init(&EXTI_InitStructure); 
	
}


/**
  * @brief  配置 XPT2046 的模拟SPI
  * @param  无
  * @retval 无
  */	
static void XPT2046_GPIO_SPI_Config ( void ) 
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
	

  /* 开启GPIO时钟 */
  RCC_APB2PeriphClockCmd ( macXPT2046_SPI_GPIO_CLK, ENABLE );

  /* 模拟SPI GPIO初始化 */          
  GPIO_InitStructure.GPIO_Pin=macXPT2046_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(macXPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = macXPT2046_SPI_MOSI_PIN;
  GPIO_Init(macXPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = macXPT2046_SPI_MF103-霸道_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
  GPIO_Init(macXPT2046_SPI_MF103-霸道_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = macXPT2046_SPI_CS_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
  GPIO_Init(macXPT2046_SPI_CS_PORT, &GPIO_InitStructure); 
   
  /* 拉低片选，选择XPT2046 */
  macXPT2046_CS_DISABLE();

}




/**
  * @brief  用于 XPT2046 的简单微秒级延时函数
  * @param  nCount ：延时计数值，单位为微妙
  * @retval 无
  */	
static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;


	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  
	      
		while ( uc -- );     //延1微秒	

	}
	
}



/**
  * @brief  XPT2046 的写入命令
  * @param  ucCmd ：命令
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 无
  */
static void XPT2046_WriteCMD ( uint8_t ucCmd ) 
{
	uint8_t i;


	macXPT2046_MOSI_0();
	
	macXPT2046_CLK_LOW();

	for ( i = 0; i < 8; i ++ ) 
	{
		( ( ucCmd >> ( 7 - i ) ) & 0x01 ) ? macXPT2046_MOSI_1() : macXPT2046_MOSI_0();
		
	  XPT2046_DelayUS ( 5 );
		
		macXPT2046_CLK_HIGH();

	  XPT2046_DelayUS ( 5 );

		macXPT2046_CLK_LOW();
	}
	
}


/**
  * @brief  XPT2046 的读取命令
  * @param  无
  * @retval 读取到的数据
  */
static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;
	


	macXPT2046_MOSI_0();

	macXPT2046_CLK_HIGH();

	for ( i=0;i<12;i++ ) 
	{
		macXPT2046_CLK_LOW();    
	
		usTemp = macXPT2046_MF103-霸道();
		
		usBuf |= usTemp << ( 11 - i );
	
		macXPT2046_CLK_HIGH();
		
	}
	
	return usBuf;

}


/**
  * @brief  对 XPT2046 选择一个模拟通道后，启动ADC，并返回ADC采样结果
  * @param  ucChannel
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 该通道的ADC采样结果
  */
static uint16_t XPT2046_ReadAdc ( uint8_t ucChannel )
{
	XPT2046_WriteCMD ( ucChannel );

  return 	XPT2046_ReadCMD ();
	
}


/**
  * @brief  读取 XPT2046 的X通道和Y通道的AD值（12 bit，最大是4096）
  * @param  sX_Ad ：存放X通道AD值的地址
  * @param  sY_Ad ：存放Y通道AD值的地址
  * @retval 无
  */
static void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad )  
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 

	
	
	sX_Ad_Temp = XPT2046_ReadAdc ( macXPT2046_CHANNEL_X );

	XPT2046_DelayUS ( 1 ); 

	sY_Ad_Temp = XPT2046_ReadAdc ( macXPT2046_CHANNEL_Y ); 
	
	
	* sX_Ad = sX_Ad_Temp; 
	* sY_Ad = sY_Ad_Temp; 
	
	
}

 
/**
  * @brief  在触摸 XPT2046 屏幕时获取一组坐标的AD值，并对该坐标进行滤波
  * @param  无
  * @retval 滤波之后的坐标AD值
  */
#if   0                 //注意：校正较精准，但是相对复杂，速度较慢
static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate )
{
	uint8_t ucCount = 0;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 9 ] = { { 0 }, { 0 } };  //坐标X和Y进行9次采样

	int32_t lAverage  [ 3 ], lDifference [ 3 ];
	

	do
	{		   
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++; 
			 
	} while ( ( macXPT2046_EXTI_Read() == macXPT2046_EXTI_ActiveLevel ) && ( ucCount < 9 ) ); 	//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<9*/
	 
	
	/*如果触笔弹起*/
	if ( macXPT2046_EXTI_Read() != macXPT2046_EXTI_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//触摸中断标志复位		

	
	/* 如果成功采样9次,进行滤波 */ 
	if ( ucCount == 9 )   								
	{  
		/* 为减少运算量,分别分3组取平均值 */
		lAverage  [ 0 ] = ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + sBufferArray [ 0 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] ) / 3;
		
		/* 计算3组数据的差值 */
		lDifference [ 0 ] = lAverage  [ 0 ]-lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ]-lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ]-lAverage  [ 0 ];
		
		/* 对上述差值取绝对值 */
		lDifference [ 0 ] = lDifference [ 0 ]>0?lDifference [ 0 ]: ( -lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ]>0?lDifference [ 1 ]: ( -lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ]>0?lDifference [ 2 ]: ( -lDifference [ 2 ] );
		
		
		/* 判断绝对差值是否都超过差值门限，如果这3个绝对差值都超过门限值，则判定这次采样点为野点,抛弃采样点，差值门限取为2 */
		if (  lDifference [ 0 ] > macXPT2046_THRESHOLD_CalDiff  &&  lDifference [ 1 ] > macXPT2046_THRESHOLD_CalDiff  &&  lDifference [ 2 ] > macXPT2046_THRESHOLD_CalDiff  ) 
			return 0;
		
		
		/* 计算它们的平均值，同时赋值给strScreenCoordinate */ 
		if ( lDifference [ 0 ] < lDifference [ 1 ] )
		{
			if ( lDifference [ 2 ] < lDifference [ 0 ] ) 
				pScreenCoordinate ->x = ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
			else 
				pScreenCoordinate ->x = ( lAverage  [ 0 ] + lAverage  [ 1 ] ) / 2;	
		}
		
		else if ( lDifference [ 2 ] < lDifference [ 1 ] ) 
			pScreenCoordinate -> x = ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
		
		else 
			pScreenCoordinate ->x = ( lAverage  [ 1 ] + lAverage  [ 2 ] ) / 2;
		
		
		/* 同上，计算Y的平均值 */
		lAverage  [ 0 ] = ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + sBufferArray [ 1 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] ) / 3;
		
		lDifference [ 0 ] = lAverage  [ 0 ] - lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ] - lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ] - lAverage  [ 0 ];
		
		/* 取绝对值 */
		lDifference [ 0 ] = lDifference [ 0 ] > 0 ? lDifference [ 0 ] : ( - lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ] > 0 ? lDifference [ 1 ] : ( - lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ] > 0 ? lDifference [ 2 ] : ( - lDifference [ 2 ] );
		
		
		if ( lDifference [ 0 ] > macXPT2046_THRESHOLD_CalDiff && lDifference [ 1 ] > macXPT2046_THRESHOLD_CalDiff && lDifference [ 2 ] > macXPT2046_THRESHOLD_CalDiff ) 
			return 0;
		
		if ( lDifference [ 0 ] < lDifference [ 1 ] )
		{
			if ( lDifference [ 2 ] < lDifference [ 0 ] ) 
				pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
			else 
				pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 1 ] ) / 2;	
		}
		else if ( lDifference [ 2 ] < lDifference [ 1 ] ) 
			pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
		else
			pScreenCoordinate ->y =  ( lAverage  [ 1 ] + lAverage  [ 2 ] ) / 2;
		
		
		return 1;
		
		
	}
	
	else if ( ucCount > 1 )
	{
		pScreenCoordinate ->x = sBufferArray [ 0 ] [ 0 ];
		pScreenCoordinate ->y = sBufferArray [ 1 ] [ 0 ];
	
		return 0;
		
	}  
	
	
	return 0; 
	
	
}


#else     //注意：画板应用实例专用,不是很精准，但是简单，速度比较快   
static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate )
{
	uint8_t ucCount = 0, i;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //坐标X和Y进行多次采样
	
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;


	/* 循环采样10次 */ 
	do					       				
	{		  
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );  
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++;  
		
	}	while ( ( macXPT2046_EXTI_Read() == macXPT2046_EXTI_ActiveLevel ) && ( ucCount < 10 ) );//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<10
	
	
	/*如果触笔弹起*/
	if ( macXPT2046_EXTI_Read() != macXPT2046_EXTI_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//中断标志复位

	
	/*如果成功采样10个样本*/
	if ( ucCount ==10 )		 					
	{
		lX_Max = lX_Min = sBufferArray [ 0 ] [ 0 ];
		lY_Max = lY_Min = sBufferArray [ 1 ] [ 0 ];       
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray [ 0 ] [ i ] < lX_Min )
				lX_Min = sBufferArray [ 0 ] [ i ];
			
			else if ( sBufferArray [ 0 ] [ i ] > lX_Max )
				lX_Max = sBufferArray [ 0 ] [ i ];

		}
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray [ 1 ] [ i ] < lY_Min )
				lY_Min = sBufferArray [ 1 ] [ i ];
			
			else if ( sBufferArray [ 1 ] [ i ] > lY_Max )
				lY_Max = sBufferArray [ 1 ] [ i ];

		}
		
		
		/*去除最小值和最大值之后求平均值*/
		pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
		                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;
		
		pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
		                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
		
		
		return 1;
		
		
	}   
	
	
	return 0;    
	
	
}


#endif


/**
  * @brief  计算 XPT2046 触摸坐标校正系数（注意：只有在LCD和触摸屏间的误差角度非常小时,才能运用下面公式）
  * @param  pDisplayCoordinate ：屏幕人为显示的已知坐标
  * @param  pstrScreenSample ：对已知坐标点触摸时 XPT2046 产生的坐标
  * @param  pCalibrationFactor ：根据人为设定坐标和采样回来的坐标计算出来的屏幕触摸校正系数
  * @retval 计算状态
	*   该返回值为以下值之一：
  *     @arg 1 :计算成功
  *     @arg 0 :计算失败
  */
static uint8_t XPT2046_Calculate_CalibrationFactor ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_Coordinate * pScreenSample, strType_XPT2046_Calibration * pCalibrationFactor )
{
	uint8_t ucRet = 1;

	
	/* K＝ ( X0－X2 )  ( Y1－Y2 )－ ( X1－X2 )  ( Y0－Y2 ) */
	pCalibrationFactor -> Divider =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
									                 ( ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
	
	
	if (  pCalibrationFactor -> Divider == 0  )
		ucRet = 0;

	else
	{
		/* A＝ (  ( XD0－XD2 )  ( Y1－Y2 )－ ( XD1－XD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor -> An =  ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) );
		
		/* B＝ (  ( X0－X2 )  ( XD1－XD2 )－ ( XD0－XD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor -> Bn =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) );
		
		/* C＝ ( Y0 ( X2XD1－X1XD2 )+Y1 ( X0XD2－X2XD0 )+Y2 ( X1XD0－X0XD1 ) )／K */
		pCalibrationFactor -> Cn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .x - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .x ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .x - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .x ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .x - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .x ) * pScreenSample [ 2 ] .y ;
		
		/* D＝ (  ( YD0－YD2 )  ( Y1－Y2 )－ ( YD1－YD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor -> Dn =  ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
		
		/* E＝ (  ( X0－X2 )  ( YD1－YD2 )－ ( YD0－YD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor -> En =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) ) ;
		
		
		/* F＝ ( Y0 ( X2YD1－X1YD2 )+Y1 ( X0YD2－X2YD0 )+Y2 ( X1YD0－X0YD1 ) )／K */
		pCalibrationFactor -> Fn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .y - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .y ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .y - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .y ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .y - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .y ) * pScreenSample [ 2 ] .y;
			
	}
	
	
	return ucRet;
	
	
}


/**
  * @brief  在 ILI9341 上显示校正触摸时需要的十字
  * @param  usX ：在特定扫描方向下十字交叉点的X坐标
  * @param  usY ：在特定扫描方向下十字交叉点的Y坐标
  * @retval 无
  */
static void ILI9341_DrawCross ( uint16_t usX, uint16_t usY )
{
  ILI9341_Clear ( usX - 10, usY, 20, 1, macRED);
  ILI9341_Clear ( usX, usY - 10, 1, 20, macRED);
	
}


/**
  * @brief  XPT2046 触摸屏校准
  * @param  无
  * @retval 校准结果
	*   该返回值为以下值之一：
  *     @arg 1 :校准成功
  *     @arg 0 :校准失败
  */
uint8_t XPT2046_Touch_Calibrate ( void )
{
	#if 1
		uint8_t i;
		
		char cStr [ 10 ];
		
    uint16_t usScreenWidth, usScreenHeigth;
		uint16_t usTest_x = 0, usTest_y = 0, usGap_x = 0, usGap_y = 0;
		
	  char * pStr = 0;
	
    strType_XPT2046_Coordinate strCrossCoordinate [ 4 ], strScreenSample [ 4 ];
	  
	  strType_XPT2046_Calibration CalibrationFactor;
    		

		#if ( macXPT2046_Coordinate_GramScan == 1 ) || ( macXPT2046_Coordinate_GramScan == 4 )
	    usScreenWidth = macILI9341_Default_Max_Width;
	    usScreenHeigth = macILI9341_Default_Max_Heigth;

	  #elif ( macXPT2046_Coordinate_GramScan == 2 ) || ( macXPT2046_Coordinate_GramScan == 3 )
	    usScreenWidth = macILI9341_Default_Max_Heigth;
	    usScreenHeigth = macILI9341_Default_Max_Width;
	
	  #endif
		
		
		/* 设定“十”字交叉点的坐标 */ 
		strCrossCoordinate [ 0 ] .x = usScreenWidth >> 2;
		strCrossCoordinate [ 0 ] .y = usScreenHeigth >> 2;
		
		strCrossCoordinate [ 1 ] .x = strCrossCoordinate [ 0 ] .x;
		strCrossCoordinate [ 1 ] .y = ( usScreenHeigth * 3 ) >> 2;
		
		strCrossCoordinate [ 2 ] .x = ( usScreenWidth * 3 ) >> 2;
		strCrossCoordinate [ 2 ] .y = strCrossCoordinate [ 1 ] .y;
		
		strCrossCoordinate [ 3 ] .x = strCrossCoordinate [ 2 ] .x;
		strCrossCoordinate [ 3 ] .y = strCrossCoordinate [ 0 ] .y;		
	  	
			
		ILI9341_GramScan ( macXPT2046_Coordinate_GramScan );
		
		
		for ( i = 0; i < 4; i ++ )
		{ 
			ILI9341_Clear ( 0, 0, usScreenWidth, usScreenHeigth, macBACKGROUND );       
			
			pStr = "Touch Calibrate ......";			
      ILI9341_DispString_EN ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * macWIDTH_EN_CHAR ) >> 1, usScreenHeigth >> 1, pStr, macBACKGROUND, macRED );			
		
			sprintf ( cStr, "%d", i + 1 );
			ILI9341_DispString_EN ( usScreenWidth >> 1, ( usScreenHeigth >> 1 ) - macHEIGHT_EN_CHAR, cStr, macBACKGROUND, macRED );
		
			XPT2046_DelayUS ( 100000 );		                                                   //适当的延时很有必要
			
			ILI9341_DrawCross ( strCrossCoordinate [ i ] .x, strCrossCoordinate [ i ] .y );  //显示校正用的“十”字

			while ( ! XPT2046_ReadAdc_Smooth_XY ( & strScreenSample [ i ] ) );               //读取XPT2046数据到变量pCoordinate，当ptr为空时表示没有触点被按下

		}
		
		
		XPT2046_Calculate_CalibrationFactor ( strCrossCoordinate, strScreenSample, & CalibrationFactor ) ;  	 //用原始参数计算出 原始参数与坐标的转换系数
		
		if ( CalibrationFactor .Divider == 0 ) goto Failure;
		
			
		usTest_x = ( ( CalibrationFactor .An * strScreenSample [ 3 ] .x ) + ( CalibrationFactor .Bn * strScreenSample [ 3 ] .y ) + CalibrationFactor .Cn ) / CalibrationFactor .Divider;		//取一个点计算X值	 
		usTest_y = ( ( CalibrationFactor .Dn * strScreenSample [ 3 ] .x ) + ( CalibrationFactor .En * strScreenSample [ 3 ] .y ) + CalibrationFactor .Fn ) / CalibrationFactor .Divider;    //取一个点计算Y值
		
		usGap_x = ( usTest_x > strCrossCoordinate [ 3 ] .x ) ? ( usTest_x - strCrossCoordinate [ 3 ] .x ) : ( strCrossCoordinate [ 3 ] .x - usTest_x );   //实际X坐标与计算坐标的绝对差
		usGap_y = ( usTest_y > strCrossCoordinate [ 3 ] .y ) ? ( usTest_y - strCrossCoordinate [ 3 ] .y ) : ( strCrossCoordinate [ 3 ] .y - usTest_y );   //实际Y坐标与计算坐标的绝对差
		
    if ( ( usGap_x > 10 ) || ( usGap_y > 10 ) ) goto Failure;       //可以通过修改这两个值的大小来调整精度    
		

    /* 校准系数为全局变量 */ 
		strXPT2046_TouchPara .dX_X = ( CalibrationFactor .An * 1.0 ) / CalibrationFactor .Divider;
		strXPT2046_TouchPara .dX_Y = ( CalibrationFactor .Bn * 1.0 ) / CalibrationFactor .Divider;
		strXPT2046_TouchPara .dX   = ( CalibrationFactor .Cn * 1.0 ) / CalibrationFactor .Divider;
		
		strXPT2046_TouchPara .dY_X = ( CalibrationFactor .Dn * 1.0 ) / CalibrationFactor .Divider;
		strXPT2046_TouchPara .dY_Y = ( CalibrationFactor .En * 1.0 ) / CalibrationFactor .Divider;
		strXPT2046_TouchPara .dY   = ( CalibrationFactor .Fn * 1.0 ) / CalibrationFactor .Divider;

//    /* 打印校校准系数 */ 
//		printf ( "校准系数如下：\r\n" );
//		
//    for ( i = 0; i < 6; i ++ )
//		{
//			uint32_t ulHeadAddres = ( uint32_t ) ( & strXPT2046_TouchPara );
//			
//			
//			printf ( "%LF\r\n", * ( ( long double * ) ( ulHeadAddres + sizeof ( long double ) * i ) ) );
//			
//		}	
		

	#endif
	
	
	ILI9341_Clear ( 0, 0, usScreenWidth, usScreenHeigth, macBACKGROUND );
	
	pStr = "Calibrate Succed";			
	ILI9341_DispString_EN ( ( usScreenWidth - strlen ( pStr ) * macWIDTH_EN_CHAR ) >> 1, usScreenHeigth >> 1, pStr, macBACKGROUND, macRED );	

  XPT2046_DelayUS ( 200000 );

	return 1;    
	

	Failure:
	
	ILI9341_Clear ( 0, 0, usScreenWidth, usScreenHeigth, macBACKGROUND ); 
	
	pStr = "Calibrate fail";			
	ILI9341_DispString_EN ( ( usScreenWidth - strlen ( pStr ) * macWIDTH_EN_CHAR ) >> 1, usScreenHeigth >> 1, pStr, macBACKGROUND, macRED );	

	pStr = "try again";			
	ILI9341_DispString_EN ( ( usScreenWidth - strlen ( pStr ) * macWIDTH_EN_CHAR ) >> 1, ( usScreenHeigth >> 1 ) + macHEIGHT_EN_CHAR, pStr, macBACKGROUND, macRED );				

	XPT2046_DelayUS ( 1000000 );		
	
	return 0; 
		
		
}

   
/**
  * @brief  获取 XPT2046 触摸点（校准后）的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @param  pTouchPara：坐标校准系数
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           //若正常，则返回0
	
	strType_XPT2046_Coordinate strScreenCoordinate; 
	

  if ( XPT2046_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
  {    
		pDisplayCoordinate ->x = ( ( pTouchPara ->dX_X * strScreenCoordinate .x ) + ( pTouchPara ->dX_Y * strScreenCoordinate .y ) + pTouchPara ->dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara ->dY_X * strScreenCoordinate .x ) + ( pTouchPara ->dY_Y * strScreenCoordinate .y ) + pTouchPara ->dY );

  }
	 
	else ucRet = 0;            //如果获取的触点信息有误，则返回0
	
	
	
  return ucRet;
	
	
} 
















