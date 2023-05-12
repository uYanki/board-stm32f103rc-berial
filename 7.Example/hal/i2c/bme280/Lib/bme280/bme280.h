
#ifndef __BME280_H__
#define __BME280_H__

#include "i2cdev/i2cdev.h"

#include <math.h>

#define BME280_ADDR_LOW  0x76
#define BME280_ADDR_HIGH 0x77

static struct i2c_cli bme280 = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = BME280_ADDR_LOW << 1,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

#define BME280_DEBUG 1

// #define BME280_64BIT_ENABLE

#ifndef BME280_64BIT_ENABLE
#ifndef BME280_32BIT_ENABLE
#ifndef BME280_FLOAT_ENABLE
#define BME280_FLOAT_ENABLE
#endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////////// registers

/******************************************************************************/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |寄存器名  |地址|bit7|bit6|bit5|bit4|bit3|bit2|bit1|bit0|复位状态|读写| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |hum_lsb   |0xFE|               hum_lsb<7:0>            |  0x00  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |hum_msb   |0xFD|               hum_msb<7:0>            |  0x80  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |temp_xlsb |0xFC|    temp_xlsb<7:4> | 0  | 0  | 0  | 0  |  0x00  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |temp_lsb  |0xFB|              temp_lsb<7:0>            |  0x00  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |temp_msb  |0xFA|              temp_msb<7:0>            |  0x80  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |press_xlsb|0xF9|   press_xlsb<7:4> | 0  | 0  | 0  | 0  |  0x00  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |press_lsb |0xF8|             press_lsb<7:0>            |  0x00  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |press_msb |0xF7|             press_msb<7:0>            |  0x80  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |config    |0xF5|   t_sb[2:0]  |  filter[2:0] |    |e[0]|  0x00  |读写| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |ctrl_meas |0xF4|  osrs_t[2:0] |  osrs_p[2:0] |mode[1:0]|  0x00  |读写| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  | status   |0xF3|                   |m[0]|         |u[0]|  0x00  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |ctrl_hum  |0xF2|                        |  osrs_h[2:0] |  0x00  |读写| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |calib41   |0xF0|        calibration data[共41]         |出厂设定|只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |calib26   |0xE1|        calibration data[共41]         |出厂设定|只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  | reset    |0xE0|             reset[7:0]                |  0x00  |只写| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |   id     |0xD0|             chip_id[7:0]              |  0x60  |只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |calib25   |0xA1|        calibration data[共41]         |出厂设定|只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  |calib00   |0x88|        calibration data[共41]         |出厂设定|只读| **/
/**  +----------+----+----+----+----+----+----+----+----+----+--------+----+ **/
/**  控制测量寄存器压力测量和温度测量的配置：                                **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |osrs_p[2:0]|倍数|    精度    |    |osrs_t[2:0]|倍数|     精度    |   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |   000     |  - |      -     |    |   000     |  - |     -       |   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |   001     | x1 |16bit/2.62Pa|    |   001     | x1 |16bit/0.0050C|   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |   010     | x2 |17bit/1.31Pa|    |   010     | x2 |17bit/0.0025C|   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |   011     | x4 |18bit/0.66Pa|    |   011     | x4 |18bit/0.0012C|   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |   100     | x8 |19bit/0.33Pa|    |   100     | x8 |19bit/0.0006C|   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/**    |101,110,111| x16|20bit/0.16Pa|    |101,110,111| x16|20bit/0.0003C|   **/
/**    +-----------+----+------------+    +-----------+----+-------------+   **/
/******************************************************************************/

#define BME280_REG_HUMI_MSB   0xFD  // 湿度值
#define BME280_REG_HUMI_LSB   0xFE
#define BME280_REG_TEMP_MSB   0xFA  // 温度值
#define BME280_REG_TEMP_LSB   0xFB
#define BME280_REG_TEMP_XLSB  0xFC
#define BME280_REG_PRESS_MSB  0xF7  // 压力值
#define BME280_REG_PRESS_LSB  0xF8
#define BME280_REG_PRESS_XLSB 0xF9

#define BME280_REG_CTRL_HUM   0xF2  // 湿度测量控制
#define BME280_REG_STATUS     0xF3  // 状态
#define BME280_REG_CTRL_MEAS  0xF4  // 模式/温度/气压测量控制
#define BME280_REG_CONFIG     0xF5  // 配置

#define BME280_REG_CHIP_ID    0xD0  // 设备ID (0x60)
#define BME280_REG_VERSION    0xD1

#define BME280_REG_RESET      0xE0  // 复位

#define BME280_REG_CAL26      0xE1

// 温度校准系数
#define BME280_REG_DIG_T1     0x88  // u16
#define BME280_REG_DIG_T2     0x8A  // s16
#define BME280_REG_DIG_T3     0x8C  // s16
// 压力校准系数
#define BME280_REG_DIG_P1     0x8E  // u16
#define BME280_REG_DIG_P2     0x90  // s16
#define BME280_REG_DIG_P3     0x92  // s16
#define BME280_REG_DIG_P4     0x94  // s16
#define BME280_REG_DIG_P5     0x95  // s16
#define BME280_REG_DIG_P6     0x98  // s16
#define BME280_REG_DIG_P7     0x9A  // s16
#define BME280_REG_DIG_P8     0x9C  // s16
#define BME280_REG_DIG_P9     0x9E  // s16
// 湿度校准系数
#define BME280_REG_DIG_H1     0xA1  // u8
#define BME280_REG_DIG_H2     0xE1  // s16
#define BME280_REG_DIG_H3     0xE3  // u8
#define BME280_REG_DIG_H4     0xE4  // s16
#define BME280_REG_DIG_H5     0xE5  // s16
#define BME280_REG_DIG_H6     0xE7  // s8

#define BME280_CHIP_ID        0x60

////////////////////////////////////////////////////////////////////////////////

// oversampling
typedef enum {
    BME280_SAMPLE_X0  = 0,  // skipped
    BME280_SAMPLE_X1  = 1,
    BME280_SAMPLE_X2  = 2,
    BME280_SAMPLE_X4  = 3,
    BME280_SAMPLE_X8  = 4,
    BME280_SAMPLE_X16 = 5
} bme280_sample_t;

// 电源模式 power mode
typedef enum {
    BME280_MODE_SLEEP  = 0x00,  // 睡眠模式
    BME280_MODE_FORCED = 0x01,  // 强制模式(单次测量模式)
    BME280_MODE_NORMAL = 0x03   // 正常模式(连续测量模式)
} bme280_mode_t;

// 待机持续时间 standby duration
typedef enum {
    BME280_STANDBY_0P5_MS  = 0,  // 0.5ms
    BME280_STANDBY_10_MS   = 6,  // 10ms
    BME280_STANDBY_20_MS   = 7,  // 20ms
    BME280_STANDBY_62P5_MS = 1,  // 62.5ms
    BME280_STANDBY_125_MS  = 2,  // 125ms
    BME280_STANDBY_250_MS  = 3,  // 250ms
    BME280_STANDBY_500_MS  = 4,  // 500ms
    BME280_STANDBY_1000_MS = 5   // 1000ms
} bme280_standby_t;

// IIR 滤波系数
typedef enum {
    BME280_FILTER_OFF      = 0,  // 关闭滤波器
    BME280_FILTER_COEFF_2  = 1,  // 2
    BME280_FILTER_COEFF_4  = 2,  // 4
    BME280_FILTER_COEFF_8  = 3,  // 8
    BME280_FILTER_COEFF_16 = 4   // 16
} bme280_filter_t;

typedef enum {
    BME280_SPI3W_DISABLE = 0x00,  // 不启用3线SPI
    BME280_SPI3W_ENABLE  = 0x01   // 启用3线SPI
} bme280_use_spi3w_t;

//////////////////////////////////////////////////////////////////////////////// read / write

#define bme280_delay_ms(n) HAL_Delay(n)
#define bme280_millis(n)   HAL_GetTick(n)

////////////////////////////////////////////////////////////////////////////////

// 修正系数
typedef struct {
    // temperature
    uint16_t t1;
    int16_t  t2;
    int16_t  t3;
    // pressure
    uint16_t p1;
    int16_t  p2;
    int16_t  p3;
    int16_t  p4;
    int16_t  p5;
    int16_t  p6;
    int16_t  p7;
    int16_t  p8;
    int16_t  p9;
    // humidity
    uint8_t  h1;
    int16_t  h2;
    uint8_t  h3;
    int16_t  h4;
    int16_t  h5;
    int8_t   h6;
    int32_t  t_fine;
} bme280_calib_t;

typedef struct {
#ifdef BME280_FLOAT_ENABLE
    double pressure;
    double temperature;
    double humidity;
#else
    uint32_t pressure;
    int32_t  temperature;
    uint32_t humidity;
#endif
} bme280_data_t;

static bme280_calib_t calib;

void bme280_get_data(float* humidity, float* temperature, float* pressure);

bool bme280_init(void);

/*
   bme280_set_mode(
        BME280_MODE_NORMAL,
        BME280_SAMPLE_X16,
        BME280_SAMPLE_X16,
        BME280_SAMPLE_X16,
        BME280_FILTER_OFF,
        BME280_STANDBY_10_MS);
*/
void bme280_set_mode(
    bme280_mode_t    mode,
    bme280_sample_t  humidity,
    bme280_sample_t  temperature,
    bme280_sample_t  pressure,
    bme280_filter_t  filter,
    bme280_standby_t duration);

bool bme280_force_measure(void);

// form Seeed bme280
float bme280_get_altitude(float pressure);

// from Adafruit bme280
float bme280_get_altitude_with_sealevel(float seaLevel, float pressure);
float bme280_sealevel_for_altitude(float altitude, float atmospheric);

#endif
