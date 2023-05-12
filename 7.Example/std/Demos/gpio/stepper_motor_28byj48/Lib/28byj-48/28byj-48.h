#ifndef _28BYJ_48_H
#define _28BYJ_48_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

void stepper_init(void);

void steeper_rotate(uint32_t angle, bool dir);

#ifdef __cplusplus
}
#endif

#endif
