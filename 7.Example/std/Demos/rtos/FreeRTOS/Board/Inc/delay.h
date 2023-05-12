#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif
