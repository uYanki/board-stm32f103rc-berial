#ifndef __BSP_XPT2046_LCD_H
#define	__BSP_XPT2046_LCD_H


#include "stm32f10x.h"



/******************************* XPT2046 触摸屏中断引脚定义 ***************************/
#define             macXPT2046_EXTI_GPIO_CLK                        RCC_APB2Periph_GPIOF   
#define             macXPT2046_EXTI_GPIO_PORT                       GPIOF
#define             macXPT2046_EXTI_GPIO_PIN                        GPIO_Pin_9
#define             macXPT2046_EXTI_SOURCE_PORT                     GPIO_PortSourceGPIOF
#define             macXPT2046_EXTI_SOURCE_PIN                      GPIO_PinSource9
#define             macXPT2046_EXTI_LINE                            EXTI_Line9
#define             macXPT2046_EXTI_IRQ                             EXTI9_5_IRQn
#define             macXPT2046_EXTI_INT_FUNCTION                    EXTI9_5_IRQHandler

#define             macXPT2046_EXTI_ActiveLevel                     0
#define             macXPT2046_EXTI_Read()                          GPIO_ReadInputDataBit ( macXPT2046_EXTI_GPIO_PORT, macXPT2046_EXTI_GPIO_PIN )



/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define             macXPT2046_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG

#define             macXPT2046_SPI_CS_PIN		                        GPIO_Pin_10
#define             macXPT2046_SPI_CS_PORT		                      GPIOF

#define	            macXPT2046_SPI_CLK_PIN	                        GPIO_Pin_7
#define             macXPT2046_SPI_CLK_PORT	                        GPIOG

#define	            macXPT2046_SPI_MOSI_PIN	                        GPIO_Pin_11
#define	            macXPT2046_SPI_MOSI_PORT	                      GPIOF

#define	            macXPT2046_SPI_MF103-霸道_PIN	                        GPIO_Pin_6
#define	            macXPT2046_SPI_MF103-霸道_PORT	                      GPIOF


#define             macXPT2046_CS_ENABLE()                          GPIO_SetBits ( macXPT2046_SPI_CS_PORT, macXPT2046_SPI_CS_PIN )    
#define             macXPT2046_CS_DISABLE()                         GPIO_ResetBits ( macXPT2046_SPI_CS_PORT, macXPT2046_SPI_CS_PIN )  

#define             macXPT2046_CLK_HIGH()                           GPIO_SetBits ( macXPT2046_SPI_CLK_PORT, macXPT2046_SPI_CLK_PIN )    
#define             macXPT2046_CLK_LOW()                            GPIO_ResetBits ( macXPT2046_SPI_CLK_PORT, macXPT2046_SPI_CLK_PIN ) 

#define             macXPT2046_MOSI_1()                             GPIO_SetBits ( macXPT2046_SPI_MOSI_PORT, macXPT2046_SPI_MOSI_PIN ) 
#define             macXPT2046_MOSI_0()                             GPIO_ResetBits ( macXPT2046_SPI_MOSI_PORT, macXPT2046_SPI_MOSI_PIN )

#define             macXPT2046_MF103-霸道()                               GPIO_ReadInputDataBit ( macXPT2046_SPI_MF103-霸道_PORT, macXPT2046_SPI_MF103-霸道_PIN )



/******************************* XPT2046 触摸屏参数定义 ***************************/
#define             macXPT2046_Coordinate_GramScan                  2               //选择 XPT2046 触摸屏的坐标相对于何种扫描方向
#define             macXPT2046_THRESHOLD_CalDiff                    2               //校准触摸屏时触摸坐标的AD值相差门限 

#define	            macXPT2046_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            macXPT2046_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字



/******************************* 声明 XPT2046 相关的数据类型 ***************************/
typedef	struct          //液晶坐标结构体 
{
   uint16_t x;		
   uint16_t y;
	
} strType_XPT2046_Coordinate;   


typedef struct         //校准因子结构体 
{
	long double An,  		 //注:sizeof(long double) = 8
              Bn,     
              Cn,   
              Dn,    
              En,    
              Fn,     
              Divider;
	
} strType_XPT2046_Calibration;


typedef struct         //校准系数结构体（最终使用）
{
	long double dX_X,  			 
              dX_Y,     
              dX,   
              dY_X,    
              dY_Y,    
              dY;

} strType_XPT2046_TouchPara;



/******************************* 声明 XPT2046 相关的外部全局变量 ***************************/
extern volatile uint8_t               ucXPT2046_TouchFlag;

extern strType_XPT2046_TouchPara      strXPT2046_TouchPara;



/******************************** XPT2046 触摸屏函数声明 **********************************/
void                     XPT2046_Init                    ( void );

uint8_t                  XPT2046_Touch_Calibrate         ( void );
uint8_t                  XPT2046_Get_TouchedPoint        ( strType_XPT2046_Coordinate * displayPtr, strType_XPT2046_TouchPara * para );
 


#endif /* __BSP_TOUCH_H */

