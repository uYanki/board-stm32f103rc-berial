#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"
#include "./font/fonts.h"


#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;

//要支持中文需要实现本函数，可参考“液晶显示中英文（字库在外部FLASH）”例程
#define      GetGBKCode( ucBuffer, usChar ) 


#endif /*end of __FONT_H    */
