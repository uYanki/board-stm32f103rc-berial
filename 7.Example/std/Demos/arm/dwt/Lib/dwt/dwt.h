#ifndef __DWT_H__
#define __DWT_H__

#include "debug.h"
#include "stm32f10x.h"

#define dwt_freq()    (SystemCoreClock)

// read timestamp
#define dwt_read_ts() DWT_CYCCNT

void dwt_init(void);
void dwt_delay(uint32_t nus);

#endif
