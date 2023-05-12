#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"
#include <stdbool.h>

void delay_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void timer_init(uint8_t id, uint16_t prescaler, uint16_t period);
bool timer_check(uint8_t id);

// @72MHz: prescaler = 7200, period = 2e4 => 2ms

#endif
