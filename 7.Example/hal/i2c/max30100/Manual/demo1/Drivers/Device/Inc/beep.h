#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f1xx_hal.h"

//LED 控制
void Led_Control(int pl);
//变更音调
void settone(int t);
//Beep 控制
void Beep_Control(int pl);

#endif /*__BEEP_H*/
