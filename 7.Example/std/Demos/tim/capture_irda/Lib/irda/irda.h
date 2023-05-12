#ifndef __IRM_H__
#define __IRM_H__

#include "board.h"

// 红外遥控识别码(ID), 大部分相同
#define REMOTE_ID 0

// 按键按下的次数
extern uint8_t irda_cnt;

void    irda_init(void);
uint8_t irda_scan(void);

#endif
