#ifndef __BH1750_H
#define __BH1750_H

/*
    光照度 lx
    勒克斯 (Lux) 被光均匀照射的物体，距离该光源1米处，在1m2面积上得到的光通量是1lm时，它的照度是1lux

    光照度可用照度计直接测量。光照度的单位是勒克斯，是英文lux的音译，也可写为lx。
    被光均匀照射的物体，在1平方米面积上得到的光通量是1流明时，它的照度是1勒克斯。有时为了充分利用光源，
    常在光源上附加一个反射装置，使得某些方向能够得到比较多的光通量，以增加这一被照面上的照度。例如
    汽车前灯、手电筒、摄影灯等。

    以下是各种环境照度值：单位lux
    黑夜：0.001—0.02；
    月夜：0.02—0.3；
    阴天室内：5—50；
    阴天室外：50—500；
    晴天室内：100—1000；
    夏季中午太阳光下的照度：约为10*9
    阅读书刊时所需的照度：50—60；
    家用摄像机标准照度：1400

    BH1750FVI 为光照度测量芯片。测量量程可通过命令进行调节。
    最小 0.11 lux， 最大 100000 lux
*/

#include "i2c.h"

#define BH1750_ADDR_LOW  0X23
#define BH1750_ADDR_HIGH 0X5C

#define BH1750_I2C hi2c2                    // <----
#define BH1750_DEV (BH1750_ADDR_HIGH << 1)  // <----

/*** CMD 指令集 ***/

#define BH1750_POWER_DOWN   0x00  // 掉电，芯片上电后缺省就是 PowerOff 模式
#define BH1750_POWER_ON     0x01  // 上电，等待测量命令
#define BH1750_MODULE_RESET 0x07  // 清零数据寄存器 (在 PowerOff 模式下无效)

/**
 * 不同模式下分辨率不同，即精度不同
 * 高分辨率模式2：分辨率是0.5lx
 * 高分辨率模式：分辨率1lx
 * 低分辨率模式：分辨率4lx
 * 不同模式只是精度不一样，对于计算没有区别
 */

// 连续高分辨率测量模式（测量时间 120ms，最大 180ms）
#define BH1750_CONTINUE_H_MODE  0x10
#define BH1750_CONTINUE_H_MODE2 0x11
// 连续低分辨率测量模式（测量时间 16ms）
#define BH1750_CONTINUE_L_MODE 0x13

// 单次高分辨率测量模式（测量完成后自动进入 PowerOff）
#define BH1750_ONE_TIME_H_MODE  0x20
#define BH1750_ONE_TIME_H_MODE2 0x21
// 单次低分辨率测量模式（测量完成后进入 PowerOff）
#define BH1750_ONE_TIME_L_MODE 0x23

void     BH1750_WriteCmd(uint8_t data);
uint16_t BH1750_ReadData(void);

void  BH1750_Init(void);
void  BH1750_Reset(void);
void  BH1750_PowerOn(void);
void  BH1750_PowerOff(void);
float BH1750_GetLux(void);
void  BH1750_AdjustSensitivity(uint8_t sen);
void  BH1750_ChageMode(uint8_t mode);

#endif
