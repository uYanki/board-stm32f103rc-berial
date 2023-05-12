#ifndef _LIS3DSH_H_
#define _LIS3DSH_H_

#include <math.h>

#include "i2c.h"
#include "usart.h"

#define LIS3DSH_ADDR_LOW  0x18
#define LIS3DSH_ADDR_HIGH 0x19

#define LIS3DSH_DEV (LIS3DSH_ADDR_HIGH << 1)  // <------- dev
#define LIS3DSH_I2C hi2c2                     // <------- hi2c

#define LIS3DSH_WHO_AM_I 0x33

// Register addresses

#define LIS3DSH_STATUS_REG_AUX  0x07
#define LIS3DSH_OUT_ADC1_L      0x08
#define LIS3DSH_OUT_ADC1_H      0x09
#define LIS3DSH_OUT_ADC2_L      0x0A
#define LIS3DSH_OUT_ADC2_H      0x0B
#define LIS3DSH_OUT_ADC3_L      0x0C
#define LIS3DSH_OUT_ADC3_H      0x0D
#define LIS3DSH_INT_COUNTER_REG 0x0E
#define LIS3DSH_WHO_AM_I_REG    0x0F
#define LIS3DSH_TEMP_CFG_REG    0x1F
#define LIS3DSH_CTRL_REG1       0x20
#define LIS3DSH_CTRL_REG2       0x21
#define LIS3DSH_CTRL_REG3       0x22
#define LIS3DSH_CTRL_REG4       0x23
#define LIS3DSH_CTRL_REG5       0x24
#define LIS3DSH_CTRL_REG6       0x25
#define LIS3DSH_REFERENCE       0x26
#define LIS3DSH_STATUS_REG2     0x27
#define LIS3DSH_OUT_X_L         0x28
#define LIS3DSH_OUT_X_H         0x29
#define LIS3DSH_OUT_Y_L         0x2A
#define LIS3DSH_OUT_Y_H         0x2B
#define LIS3DSH_OUT_Z_L         0x2C
#define LIS3DSH_OUT_Z_H         0x2D
#define LIS3DSH_FIFO_CTRL       0x2E

#define LIS3DSH_CLICKCFG_REG    0x38
#define LIS3DSH_CLICKSRC_REG    0x39
#define LIS3DSH_CLICKTHS_REG    0x3A
#define LIS3DSH_TIMELIMIT_REG   0x3B
#define LIS3DSH_TIMELATENCY_REG 0x3C
#define LIS3DSH_TIMEWINDOW_REG  0x3D

// Register value constants:
#define LIS3DSH_RANGE_16_G              3  // ±16g, divider = 1365
#define LIS3DSH_RANGE_8_G               2  // ±8g,  divider = 4096
#define LIS3DSH_RANGE_4_G               1  // ±4g,  divider = 8190
#define LIS3DSH_RANGE_2_G               0  // ±2g,  divider = 16380
#define LIS3DSH_DATARATE_1344_HZ        9  // 1.344 KHz
#define LIS3DSH_DATARATE_400_HZ         7  // 400Hz
#define LIS3DSH_DATARATE_200_HZ         6  // 200Hz
#define LIS3DSH_DATARATE_100_HZ         5  // 100Hz
#define LIS3DSH_DATARATE_50_HZ          4  // 50Hz
#define LIS3DSH_DATARATE_25_HZ          3  // 25Hz
#define LIS3DSH_DATARATE_10_HZ          2  // 10 Hz
#define LIS3DSH_DATARATE_1_HZ           1  // 1 Hz
#define LIS3DSH_DATARATE_POWERDOWN      0
#define LIS3DSH_DATARATE_LOWPOWER_1K6HZ 8
#define LIS3DSH_DATARATE_LOWPOWER_5KHZ  9

#define STANDARD_GRAVITY 9.806
#define RAD_TO_DEG       57.29

void LIS3DSH_Init(void);
void LIS3DSH_ReadAccel(float* x, float* y, float* z);

#endif
