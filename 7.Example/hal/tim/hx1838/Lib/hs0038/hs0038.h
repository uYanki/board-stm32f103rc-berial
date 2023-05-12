#ifndef __IR_H
#define __IR_H

#include "tim.h"

#define IR_TIM        htim3
#define IR_TIM_CHANNE TIM_CHANNEL_1

#define IR_us_LOGIC_0      560
#define IR_us_LOGIC_1      1640
#define IR_us_START        4480
#define IR_us_REPEAT_START 39350
#define IR_us_REPEAT_END   2220
#define IR_DEVIATION       100  // 误差范围

#define IR_CHECK_ADDRESS 1  // 1：检查地址位 0：取消
#define IR_CHECK_COMMAND 1  // 1：检查数据位 0：取消
#define IR_CHECK_REPEAT  1  // 1：检查重复   0：取消

void    IR_Init(void);
void    IR_CaptureCallback(void);
uint8_t IR_Scanf(uint32_t* data);

#endif

/* usage:
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    if (TIM1 == htim->Instance) IR_CaptureCallback();
}
*/
