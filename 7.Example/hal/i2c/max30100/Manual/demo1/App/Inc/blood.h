#ifndef __BLOOD_H
#define __BLOOD_H

#include "stm32f1xx_hal.h"
typedef enum {
    BLD_NORMAL,  // 正常
    BLD_ERROR,   // 侦测错误

} BloodState;  // 血液状态

typedef struct
{
    uint16_t   heart;    // 心率数据
    float      SpO2;     // 血氧数据
    BloodState state;    // 状态
    uint8_t    update;   // 信息更新标志位
    uint8_t    display;  // 数据更新标志位
} BloodData;

void blood_Setup(void);
void blood_Loop(void);
void blood_Interrupt(void);

#endif /*__BLOOD_H*/
