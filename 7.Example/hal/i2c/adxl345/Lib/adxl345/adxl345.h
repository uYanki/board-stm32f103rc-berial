#ifndef __ADXL345_H__
#define __ADXL345_H__

// IR temperature sensor

#include "i2cdev/i2cdev.h"
#include <stdbool.h>

#define ADXL345_ADDR_LOW  0x53
#define ADXL345_ADDR_HIGH 0x1D

#define ADXL345_DEV       (ADXL345_ADDR_LOW << 1)  // <---
#define ADXL345_I2C       hi2c2                    // <---

#define ADXL345_DEBUG     1

#if ADXL345_DEBUG
#include "math.h"
#define RAD_TO_DEG 57.2958
#endif

//////////////////////////////////////////// register

// 3轴数字加速度计（I2C/SPI）
// - CS 上拉 -> I2C 模式
// - CS 下拉 -> SPI 模式下 ( CPOL = CPHA = 1 ), 最大时钟 5 MHz

/******************************************************************************/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |十六进制|十进制|     名称     |类型 |  复位值  |  描述                   | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x00  |   0  |    DEVID     | R   | 11100101 | 器件ID                  | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x1D  |  29  |  THRESH_TAP  | R/W | 00000000 | 敲击阈值                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x1E  |  30  |     OFSX     | R/W | 00000000 | X轴偏移                 | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x1F  |  31  |     OFSY     | R/W | 00000000 | Y轴偏移                 | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x20  |  32  |     OFSZ     | R/W | 00000000 | Z轴偏移                 | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x21  |  33  |     DUR      | R/W | 00000000 | 敲击持续时间            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x22  |  34  |    Latent    | R/W | 00000000 | 敲击延迟                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x23  |  35  |    Window    | R/W | 00000000 | 敲击窗口                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x24  |  36  |  THRESH_ACT  | R/W | 00000000 | 活动阈值                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x25  |  37  | THRESH_INACT | R/W | 00000000 | 静止阈值                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x26  |  38  |  TIME_INACT  | R/W | 00000000 | 静止时间                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x27  |  39  | ACT_INACT_CTL| R/W | 00000000 | 轴使能控制活动和静止检测| **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x28  |  40  |   THRESH_FF  | R/W | 00000000 | 自由落体阈值            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x29  |  41  |   TIME_FF    | R/W | 00000000 | 自由落体时间            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x2A  |  42  |   TAP_AXES   | R/W | 00000000 | 单击/双击轴控制         | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x2B  |  43  |ACT_TAP_STATUS| R   | 00000000 | 单击/双击源             | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x2C  |  44  |   BW_RATE    | R/W | 00001010 | 数据速率及功率模式控制  | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x2D  |  45  |  POWER_CTL   | R/W | 00000000 | 省电特性控制            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x2E  |  46  |  INT_ENABLE  | R/W | 00000000 | 中断使能控制            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x2F  |  47  |   INT_MAP    | R/W | 00000000 | 中断映射控制            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x30  |  48  |   INT_SOURCE | R   | 00000010 | 中断源                  | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x31  |  49  | DATA_FORMAT  | R/W | 00000000 | 数据格式控制            | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x32  |  50  |    DATAX0    | R   | 00000000 | X轴数据0                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x33  |  51  |    DATAX1    | R   | 00000000 | X轴数据1                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x34  |  52  |    DATAY0    | R   | 00000000 | Y轴数据0                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x35  |  53  |    DATAY1    | R   | 00000000 | Y轴数据1                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x36  |  54  |    DATAZ0    | R   | 00000000 | Z轴数据0                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x37  |  55  |    DATAZ1    | R   | 00000000 | Z轴数据1                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x38  |  56  |   FIFO_CTL   | R/W | 00000000 | FIFO控制                | **/
/**  +--------+------+--------------+-----+----------+-------------------------+ **/
/**  |  0x39  |  57  |  FIFO_STATUS | R   | 00000000 | FIFO状态                | **/
/**  +--------+------+--------------------+----------+-------------------------+ **/
/******************************************************************************/

#define ADXL345_REG_DEVID          0x00  // R   Device ID. 器件ID 0xE5
#define ADXL345_REG_THRESH_TAP     0x1D  // R/W Tap threshold. 敲击阈值
#define ADXL345_REG_OFSX           0x1E  // R/W X-axis offset. X轴偏移
#define ADXL345_REG_OFSY           0x1F  // R/W Y-axis offset. Y轴偏移
#define ADXL345_REG_OFSZ           0x20  // R/W Z-axis offset. Z轴偏移
#define ADXL345_REG_DUR            0x21  // R/W Tap duration. 敲击持续时间
#define ADXL345_REG_LATENT         0x22  // R/W Tap latency. 敲击延迟
#define ADXL345_REG_WINDOW         0x23  // R/W Tap window. 敲击窗口
#define ADXL345_REG_THRESH_ACT     0x24  // R/W Activity threshold. 活动阈值
#define ADXL345_REG_THRESH_INACT   0x25  // R/W Inactivity threshold. 静止阈值
#define ADXL345_REG_TIME_INACT     0x26  // R/W Inactivity time. 静止时间
#define ADXL345_REG_ACT_INACT_CTL  0x27  // R/W Axis enable control for activity. 轴使能控制活动和静止
#define ADXL345_REG_THRESH_FF      0x28  // R/W Free-fall threshold. 自由落体阈值
#define ADXL345_REG_TIME_FF        0x29  // R/W Free-fall time. 自由落体时间
#define ADXL345_REG_TAP_AXES       0x2A  // R/W Axis control for tap/double tap. 单击/双击轴控制
#define ADXL345_REG_ACT_TAP_STATUS 0x2B  // R   Source of tap/double tap. 单击/双击源
#define ADXL345_REG_BW_RATE        0x2C  // R/W Data rate and power mode control. 数据速率及功率模式控
#define ADXL345_REG_POWER_CTL      0x2D  // R/W Power saving features control. 省电特性控制
#define ADXL345_REG_INT_ENABLE     0x2E  // R/W Interrupt enable control. 中断使能控制
#define ADXL345_REG_INT_MAP        0x2F  // R/W Interrupt mapping control. 中断映射控制
#define ADXL345_REG_INT_SOURCE     0x30  // R   Source of interrupts. 中断源
#define ADXL345_REG_DATA_FORMAT    0x31  // R/W Data format control. 数据格式控制
#define ADXL345_REG_DATAX0         0x32  // R   X-Axis Data 0. X轴数据0
#define ADXL345_REG_DATAX1         0x33  // R   X-Axis Data 1. X轴数据1
#define ADXL345_REG_DATAY0         0x34  // R   Y-Axis Data 0. Y轴数据0
#define ADXL345_REG_DATAY1         0x35  // R   Y-Axis Data 1. Y轴数据1
#define ADXL345_REG_DATAZ0         0x36  // R   Z-Axis Data 0. Z轴数据0
#define ADXL345_REG_DATAZ1         0x37  // R   Z-Axis Data 1. Z轴数据1
#define ADXL345_REG_FIFO_CTL       0x38  // R/W FIFO control. FIFO控制
#define ADXL345_REG_FIFO_STATUS    0x39  // R   FIFO status. FIFO状态

//////////////////////////////////////////// definitions

#define ADXL345_DEVICE_ID          0xE5
#define ADXL345_SCALE_FACTOR       0.0039f

//////////////////////////////////////////// types

// ADXL345_REG_ACT_INACT_CTL
typedef enum {
    ADXL345_ACT_ACDC   = 1 << 7,
    ADXL345_ACT_X_EN   = 1 << 6,
    ADXL345_ACT_Y_EN   = 1 << 5,
    ADXL345_ACT_Z_EN   = 1 << 4,
    ADXL345_INACT_ACDC = 1 << 3,
    ADXL345_INACT_X_EN = 1 << 2,
    ADXL345_INACT_Y_EN = 1 << 1,
    ADXL345_INACT_Z_EN = 1 << 0,
} adxl345_enable_t;

// ADXL345_REG_TAP_AXES
typedef enum {
    ADXL345_SUPPRESS = 1 << 3,
    ADXL345_TAP_X_EN = 1 << 2,
    ADXL345_TAP_Y_EN = 1 << 1,
    ADXL345_TAP_Z_EN = 1 << 0,
} adxl345_tap_axes_t;

// ADXL345_REG_ACT_TAP_STATUS
typedef enum {
    ADXL345_ACT_X_SRC = 1 << 6,
    ADXL345_ACT_Y_SRC = 1 << 5,
    ADXL345_ACT_Z_SRC = 1 << 4,
    ADXL345_ASLEEP    = 1 << 3,
    ADXL345_TAP_X_SRC = 1 << 2,
    ADXL345_TAP_Y_SRC = 1 << 1,
    ADXL345_TAP_Z_SRC = 1 << 0,
} adxl345_tap_t;

// ADXL345_REG_BW_RAT
typedef enum {
    ADXL345_LOW_POWER = (1 << 4),
} adxl345_power_t;

#define ADXL345_RATE(x) ((x)&0xF),

// ADXL345_REG_POWER_CTL
typedef enum {
    ADXL345_PCTL_LINK       = (1 << 5),
    ADXL345_PCTL_AUTO_SLEEP = (1 << 4),
    ADXL345_PCTL_MEASURE    = (1 << 3),
    ADXL345_PCTL_SLEEP      = (1 << 2),
} adxl345_pctl_t;

#define ADXL345_PCTL_WAKEUP(x) ((x)&0x3)

// ADXL345_INT_ENABLE / ADXL345_INT_MAP / ADXL345_INT_SOURCE
typedef enum {
    ADXL345_DATA_READY = (1 << 7),
    ADXL345_SINGLE_TAP = (1 << 6),
    ADXL345_DOUBLE_TAP = (1 << 5),
    ADXL345_ACTIVITY   = (1 << 4),
    ADXL345_INACTIVITY = (1 << 3),
    ADXL345_FREE_FALL  = (1 << 2),
    ADXL345_WATERMARK  = (1 << 1),
    ADXL345_OVERRUN    = (1 << 0),
} adxl345_enable1_t;

// ADXL345_DATA_FORMAT
typedef enum {
    ADXL345_SELF_TEST  = (1 << 7),
    ADXL345_SPI        = (1 << 6),
    ADXL345_INT_INVERT = (1 << 5),
    ADXL345_FULL_RES   = (1 << 3),
    ADXL345_JUSTIFY    = (1 << 2),
} adxl345_enable2_t;

#define ADXL345_RANGE(x) = ((x)&0x3);

/* ADXL345_RANGE(x) options */
typedef enum {
    ADXL345_RANGE_PM_2G  = 0,
    ADXL345_RANGE_PM_4G  = 1,
    ADXL345_RANGE_PM_8G  = 2,
    ADXL345_RANGE_PM_16G = 3,
} adxl345_enable3_t;

#define ADXL345_FIFO_MODE(x) (((x)&0x3) << 6)

/* ADXL345_FIFO_CTL definition */
typedef enum {
    ADXL345_TRIGGER = (1 << 5),
} adxl345_enable4_t;

#define ADXL345_SAMPLES(x) ((x)&0x1F)

/* ADXL345_FIFO_MODE(x) options */
typedef enum {
    ADXL345_FIFO_BYPASS  = 0,
    ADXL345_FIFO_FIFO    = 1,
    ADXL345_FIFO_STREAM  = 2,
    ADXL345_FIFO_TRIGGER = 3,
} adxl345_fifo_t;

/* ADXL345_FIFO_STATUS definition */
typedef enum {
    ADXL345_FIFO_TRIG = (1 << 7),
} adxl345_enable6_t;

#define ADXL345_ENTRIES(x)                ((x)&0x3F)

/* ADXL345 Full Resolution Scale Factor */

//////////////////////////////////////////// functions

#define adxl345_read_uint8(reg)           i2c_get_uint8(&ADXL345_I2C, ADXL345_DEV, reg, 0)
#define adxl345_write_uint8(reg, data)    i2c_write_uint8(&ADXL345_I2C, ADXL345_DEV, reg, data)
#define adxl345_read_bytes(reg, dat, len) i2c_read(&ADXL345_I2C, ADXL345_DEV, reg, dat, len)

////////////////////////////////////////////

bool adxl345_init(void);
void adxl345_read_raw(int16_t* x, int16_t* y, int16_t* z);
void adxl345_read_data(float* x, float* y, float* z);

#endif
