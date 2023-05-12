/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : calendar.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 10/1/2009
* Description      : 超强的日历，支持农历，24节气几乎所有日历的功能
                          日历时间以1970年为元年，用32bit的时间寄存器可以运
                          行到2100年左右
*******************************************************************************/

#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "stm32f10x.h"


u8 GetMoonDay(u8 month_p,unsigned short table_addr);
u8 GetChinaCalendar(u16  year,u8 month,u8 day,u8 *p);
void GetSkyEarth(u16 year,u8 *p);
void StrCopy(u8 *target,u8 const *source,u8 no);
void GetChinaCalendarStr(u16 year,u8 month,u8 day,u8 *str);
u8 GetJieQi(u16 year,u8 month,u8 day,u8 *JQdate);
u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str);
#endif 
