#ifndef __SHT3x_H
#define __SHT3x_H

#include "i2c.h"

#define SHT30_ADDR_LOW  0x44
#define SHT30_ADDR_HIGH 0x45
#define SHT3X_I2C       hi2c2                  // <-----
#define SHT3X_DEV       (SHT30_ADDR_LOW << 1)  // <-----

#define SOFT_RESET_CMD 0x30A2  // 软复位命令

/* 单次测量模式 */
#define CLOCK_HIGH_ENABLED_CMD    0x2C06
#define CLOCK_MEDIUM_ENABLED_CMD  0x2C0D
#define CLOCK_LOW_ENABLED_CMD     0x2C10
#define CLOCK_HIGH_DISABLED_CMD   0x2400
#define CLOCK_MEDIUM_DISABLED_CMD 0x240B
#define CLOCK_LOW_DISABLED_CMD    0x2416

/* 周期测量模式	*/
#define PERI_HIGH_0_5_CMD   0x2032
#define PERI_MEDIUM_0_5_CMD 0x2024
#define PERI_LOW_0_5_CMD    0x202F

#define PERI_HIGH_1_CMD   0x2130
#define PERI_MEDIUM_1_CMD 0x2126
#define PERI_LOW_1_CMD    0x212D  // 1s测量1次（1Hz）

#define PERI_HIGH_2_CMD   0x2236
#define PERI_MEDIUM_2_CMD 0x2220
#define PERI_LOW_2_CMD    0x222B

#define PERI_HIGH_4_CMD   0x2334
#define PERI_MEDIUM_4_CMD 0x2322
#define PERI_LOW_4_CMD    0x2329

#define PERI_HIGH_10_CMD   0x2737
#define PERI_MEDIUM_10_CMD 0x2721
#define PERI_LOW_10_CMD    0x272A  // 1s测量10次(10Hz)

#define HEATER_ENABLED_CMD  0x306D  // 加热器使能
#define HEATER_DISABLED_CMD 0x3066  // 加热器失能

#define READ_STATUS_CMD  0xF32D  // 读取状态寄存器
#define CLEAR_STATUS_CMD 0x3041  // 清除状态寄存器

#define READOUT_FOR_PERIODIC_MODE 0xE000  // 周期测量模式读取数据命令

void    SHT3x_Init(void);
uint8_t SHT3x_Read(float* temperature, float* humidity);

#endif
