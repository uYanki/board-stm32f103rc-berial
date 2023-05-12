#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "core_cm3.h"
#include "stm32f10x.h"

void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);

#endif


