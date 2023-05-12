#ifndef __IWDG_H__
#define __IWDG_H__

#include "board.h"

void iwdg_init(uint8_t prescaler, uint16_t reload);
void iwdg_feed(void);

#endif
