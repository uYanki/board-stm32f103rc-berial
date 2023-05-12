#ifndef __PCA9685_H__
#define __PCA9685_H__

#include <math.h>
#include "i2c.h"

// https://blog.csdn.net/czhzasui/article/details/80332474

#define PCA9685_ONE_DEV 0x40  // 单从机地址
#define PCA9685_ALL_DEV 0x70  // 通用地址

#define PCA9685_I2C hi2c2                   // <----
#define PCA9685_DEV (PCA9685_ONE_DEV << 1)  // <----

////////////////////////////

#define REG_MODE1    0x0
#define REG_MODE2    0x1
#define REG_SUBADR1  0x2
#define REG_SUBADR2  0x3
#define REG_SUBADR3  0x4
#define REG_PRESCALE 0xFE

#define REG_LED0_ON_L  0x6
#define REG_LED0_ON_H  0x7
#define REG_LED0_OFF_L 0x8
#define REG_LED0_OFF_H 0x9

#define REG_ALLLED_ON_L  0xFA
#define REG_ALLLED_ON_H  0xFB
#define REG_ALLLED_OFF_L 0xFC
#define REG_ALLLED_OFF_H 0xFD

////////////////////////////

uint8_t PCA9685_ReadReg(uint8_t reg);
void    PCA9685_WriteReg(uint8_t reg, uint8_t data);

void PCA9685_Init();
// duty of pwm
void PCA9685_SetDuty(uint8_t channel, uint16_t on, uint16_t off);
void PCA9685_SetAllDuty(uint16_t on, uint16_t off);
// freq of pwm
void PCA9685_SetFreq(float freq);

// angle for servo
void PCA9685_SetAngle(uint8_t channel, uint16_t angle);
void PCA9685_SetAllAngle(uint16_t angle);

#endif