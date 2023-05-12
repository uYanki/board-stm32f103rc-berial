
#ifndef __TCS3472X_H__
#define __TCS3472X_H__

#include "i2cdev/i2cdev.h"
#include <stdbool.h>

// 带红外阻挡滤波器的红绿蓝(rgb)和清晰光感应值(clear)光传感器

#define TCS34721_I2C_ADDRESS 0x39
#define TCS34723_I2C_ADDRESS 0x39
#define TCS34725_I2C_ADDRESS 0x29
#define TCS34727_I2C_ADDRESS 0x29

static struct i2c_cli tcs3472x = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = TCS34725_I2C_ADDRESS << 1,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

#define TCS347X_ID_34721_34725 0x44
#define TCS347X_ID_34723_34727 0x4D

// 每次 I2C 读写的 bit[7]=1, bit[6,5]=type, bit[4,0]=目标寄存器地址/特殊功能寄存器
#define TCS3472X_BIT_CMD       0x80

// bit[7] command. must be 1
// bit[6,5] type
typedef enum {
    TCS3472X_CMD_TYPE_REPEATED_BYTE    = 0b00 << 5,  // Repeated byte (byte protocol), will repeatedly read the same register with each data access. 地址不自增（连续读取同1寄存器,以防干扰导致数据错误）
    TCS3472X_CMD_TYPE_AUTO_INCREMENT   = 0b10 << 5,  // Auto-increment (block protocol), will provide auto-increment function to read successive block. 地址自增（用于连读和连写）
    TCS3472X_CMD_TYPE_SPECIAL_FUNCTION = 0b11 << 5,  // Special function. 特殊功能寄存器
} tcs347x_cmd_type;

// bit[4,0] addr/sf

#define TCS3472X_CMD_CLEAR   (TCS3472X_BIT_CMD | TCS3472X_CMD_TYPE_SPECIAL_FUNCTION | 0b00110)

#define TCS3472X_REG_ENABLE  0x00  // enable
// bit[4] RGBC Interrupt Enable.
// bit[3] Wait enable.
// bit[1] RGBC Enable.
// bit[0] Power ON.

#define TCS3472X_REG_ATIME   0x01  // integration time
// Max RGBC Count = (256 − ATIME) × 1024 up to a maximum of 65535.

#define TCS3472X_REG_WTIME   0x03  // wait time (if TCS3472X_ENABLE_WEN is asserted)
#define TCS3472X_REG_AILTL   0x04  // clear channel lower interrupt threshold
#define TCS3472X_REG_AILTH   0x05
#define TCS3472X_REG_AIHTL   0x06  // clear channel upper interrupt threshold
#define TCS3472X_REG_AIHTH   0x07  // aihtl
#define TCS3472X_REG_PERS    0x0C  // persistence
// bit[3,0] controls the filtering interrupt capabilities of the device, tcs3472x_persistence_t

#define TCS3472X_REG_CONFIG  0x0D  // config
// bit[1] if it set 1, wait time = TCS3472X_REG_WTIME * 12

#define TCS3472X_REG_CONTROL 0x0F  // control
// bit[1:0] gain

#define TCS3472X_REG_ID      0x12  //
// id, 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727

#define TCS3472X_REG_STATUS  0x13  // status
// bit[4] rgbc clean channel interrupt
// bit[0] indicates that the RGBC channels have completed an integration cycle

#define TCS3472X_REG_CDATAL  0x14  // clear channel data
#define TCS3472X_REG_CDATAH  0x15
#define TCS3472X_REG_RDATAL  0x16  // red channel data
#define TCS3472X_REG_RDATAH  0x17
#define TCS3472X_REG_GDATAL  0x18  // green channel data
#define TCS3472X_REG_GDATAH  0x19
#define TCS3472X_REG_BDATAL  0x1A  // blue channel data
#define TCS3472X_REG_BDATAH  0x1B

// 积分时间
typedef enum {
    TCS3472X_INTEGRATION_TIME_2P4MS = 0xFF,  // 2.4ms - 1 cycle    - Max Count: 1024
    TCS3472X_INTEGRATION_TIME_24MS  = 0xF6,  // 24ms  - 10 cycles  - Max Count: 10240
    TCS3472X_INTEGRATION_TIME_50MS  = 0xEB,  // 50ms  - 20 cycles  - Max Count: 20480
    TCS3472X_INTEGRATION_TIME_101MS = 0xD5,  // 154ms - 64 cycles  - Max Count: 65535
    TCS3472X_INTEGRATION_TIME_154MS = 0xC0,  // 240ms - 100 cycles - Max Count: 65535
    TCS3472X_INTEGRATION_TIME_700MS = 0x00,  // 700ms - 256 cycles - Max Count: 65535
} tcs3472x_integration_time_t;

// 增益
typedef enum {
    TCS3472X_GAIN_1X  = 0x00,
    TCS3472X_GAIN_4X  = 0x01,
    TCS3472X_GAIN_16X = 0x02,
    TCS3472X_GAIN_60X = 0x03,
} tcs3472x_gain_t;

typedef enum {
    // WLONG0
    TCS3472X_WAIT_TIME_2P4MS  = 0x0FF,  // 2.4 ms
    TCS3472X_WAIT_TIME_204MS  = 0x0AB,  // 204 ms
    TCS3472X_WAIT_TIME_614MS  = 0x000,  // 614 ms
    // WLONG1
    TCS3472X_WAIT_TIME_29MS   = 0x1FF,  // 29 ms
    TCS3472X_WAIT_TIME_2450MS = 0x1AB,  // 2450 ms
    TCS3472X_WAIT_TIME_7400MS = 0x100,  // 7400 ms
} tcs3472x_wait_time_t;

typedef enum {
    // 读取完成就触发中断
    TCS3472X_PERS_NONE     = 0x00,  // every rgbc cycle interrupt
    // CLEAR通道单次/连续多次超出阈值就触发触发:
    TCS3472X_PERS_1_CYCLE  = 0x01,  // 1 clear channel value outside of threshold range
    TCS3472X_PERS_2_CYCLE  = 0x02,  // 2 clear channel consecutive values out of range
    TCS3472X_PERS_3_CYCLE  = 0x03,  // 3 clear channel consecutive values out of range
    TCS3472X_PERS_5_CYCLE  = 0x04,  // 5 clear channel consecutive values out of range
    TCS3472X_PERS_10_CYCLE = 0x05,  // 10 clear channel consecutive values out of range
    TCS3472X_PERS_15_CYCLE = 0x06,  // 15 clear channel consecutive values out of range
    TCS3472X_PERS_20_CYCLE = 0x07,  // 20 clear channel consecutive values out of range
    TCS3472X_PERS_25_CYCLE = 0x08,  // 25 clear channel consecutive values out of range
    TCS3472X_PERS_30_CYCLE = 0x09,  // 30 clear channel consecutive values out of range
    TCS3472X_PERS_35_CYCLE = 0x0A,  // 35 clear channel consecutive values out of range
    TCS3472X_PERS_40_CYCLE = 0x0B,  // 40 clear channel consecutive values out of range
    TCS3472X_PERS_45_CYCLE = 0x0C,  // 45 clear channel consecutive values out of range
    TCS3472X_PERS_50_CYCLE = 0x0D,  // 50 clear channel consecutive values out of range
    TCS3472X_PERS_55_CYCLE = 0x0E,  // 55 clear channel consecutive values out of range
    TCS3472X_PERS_60_CYCLE = 0x0F,  // 60 clear channel consecutive values out of range
} tcs3472x_persistence_t;

#define tcs3472x_read_byte(reg)                 i2cdev_get_byte(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), 0)
#define tcs3472x_write_byte(reg, dat)           i2cdev_write_byte(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), dat)
#define tcs3472x_read_word(reg)                 i2cdev_get_word(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), 0, ORDER_FIRST_LOW)
#define tcs3472x_write_word(reg, dat)           i2cdev_write_word(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), dat, ORDER_FIRST_LOW)
#define tcs3472x_read_block(reg, dat, len)      i2cdev_read_block(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), dat, len)
#define tcs3472x_read_bit(reg, idx)             i2cdev_get_bit(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), idx, 0)
#define tcs3472x_write_bit(reg, idx, dat)       i2cdev_write_bit(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), idx, dat)
#define tcs3472x_read_bits(reg, idx, len)       i2cdev_get_bits(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), idx, len, 0)
#define tcs3472x_write_bits(reg, idx, len, dat) i2cdev_write_bits(&tcs3472x, (uint8_t)(TCS3472X_BIT_CMD | reg), idx, len, dat)

bool tcs3472x_init(void);

void tcs3472x_set_rgbc_enable(bool enable);
bool tcs3472x_get_rgbc_enable(void);

// 低功耗
void tcs3472x_set_power_on(void);
void tcs3472x_set_power_off(void);  // sleep
bool tcs3472x_get_power_mode(void);

// 积分时间
void                        tcs3472x_set_integration_time(tcs3472x_integration_time_t t);
tcs3472x_integration_time_t tcs3472x_get_integration_time(void);
void                        tcs3472x_delay_integration_time(void);

// 每次读取间隔不小于积分时间
bool tcs3472x_read_rgbc(uint16_t* red, uint16_t* green, uint16_t* blue, uint16_t* clear);

// from waveshare
void     tcs3472x_get_rgb565_rgb888(uint16_t r, uint16_t g, uint16_t b, uint16_t* rgb565, uint32_t* rgb888);
uint16_t tcs3472x_calculate_color_temperature(uint16_t r, uint16_t g, uint16_t b, uint16_t c);

uint16_t tcs3472x_calculate_lux(uint16_t r, uint16_t g, uint16_t b, uint16_t c);

// 等待时间（使用:等待和中断使能后，模式设置为TCS3472X_PERS_NONE, 则自动每隔一定时间触发采集完成中断, 就不用主控去频繁轮询/定时器判断）
void                 tcs3472x_set_wait_enable(bool enable);
bool                 tcs3472x_get_wait_enable(void);
void                 tcs3472x_set_wait_time(tcs3472x_wait_time_t t);
tcs3472x_wait_time_t tcs3472x_get_wait_time(void);
void                 tcs3472x_delay_wait_time(void);

// 增益
void            tcs3472x_set_gain(tcs3472x_gain_t gain);
tcs3472x_gain_t tcs3472x_get_gain(void);

// 中断
void                   tcs3472x_set_interrupt_enable(bool enable);
bool                   tcs3472x_get_interrupt_enable(void);
void                   tcs3472x_set_interrupt_mode(tcs3472x_persistence_t mode);
tcs3472x_persistence_t tcs3472x_get_interrupt_mode(void);

// 中断阈值（注：上限低于下限值时，会忽略上限）
void     tcs3472x_set_clear_low_limit(uint16_t threshold);
uint16_t tcs3472x_get_clear_low_limit(void);
void     tcs3472x_set_clear_high_limit(uint16_t threshold);
uint16_t tcs3472x_get_clear_high_limit(void);

// 修改积分时间后导致颜色不正常？因为积分时间决定了RGBC通道数据最大值，修改积分时间会导致颜色偏暗或者偏白。只需要增加或减少LED亮度即可。
// 输出的RGB数据全为253并且中断引脚产生中断等等?光强超出检查范围，减小增益即可

// form csdn
// https://blog.csdn.net/qq_40600537/article/details/82762871
typedef enum {
    TEMP_RANK_WARM = 0,  // 暖色
    TEMP_RANK_MIDDLE,    // 中间色
    TEMP_RANK_COLD       // 冷色
} temperature_rank_t;

double             calculate_color_temperature(uint16_t r, uint16_t g, uint16_t b);  // return cct
uint16_t           calculate_brightness(uint16_t r, uint16_t g, uint16_t b);
temperature_rank_t get_temperature_rank(double cct);

// from MH_TCS34725
uint16_t tcs3472x_calculate_color_temperature_ex(uint16_t r, uint16_t g, uint16_t b);
uint16_t tcs3472x_calculate_lux_ex(uint16_t r, uint16_t g, uint16_t b);

#endif
