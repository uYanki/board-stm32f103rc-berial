#ifndef __xmc5883l_H_
#define __xmc5883l_H_

#include <math.h>
#include <stdbool.h>

#include "i2cdev/i2cdev.h"
#include "i2c.h"

// 注：VMC5883L 未进行实际测试

#define CONFIG_HMC5883L  1
// #define CONFIG_QMC5883L 1
// #define CONFIG_VMC5883L 1

// device i2c address
#define HMC5883L_ADDRESS 0x1E
#define QMC5883L_ADDRESS 0x0D
#define VCM5883L_ADDRESS 0x0C

static struct i2c_cli xmc5883l = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
#if defined CONFIG_HMC5883L
    .dev = HMC5883L_ADDRESS << 1,
#elif defined CONFIG_QMC5883L
    .dev = QMC5883L_ADDRESS << 1,
#elif defined CONFIG_VMC5883L
    .dev = VCM5883L_ADDRESS << 1,
#endif
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

// registers

#if defined CONFIG_HMC5883L
#define HMC5883L_REG_CONFIG_A 0x00
#define HMC5883L_REG_CONFIG_B 0x01
#define HMC5883L_REG_MODE     0x02
#define HMC5883L_REG_OUT_X_M  0x03
#define HMC5883L_REG_OUT_X_L  0x04
#define HMC5883L_REG_OUT_Z_M  0x05
#define HMC5883L_REG_OUT_Z_L  0x06
#define HMC5883L_REG_OUT_Y_M  0x07
#define HMC5883L_REG_OUT_Y_L  0x08
#define HMC5883L_REG_STATUS   0x09
#define HMC5883L_REG_IDENT_A  0x0A
#define HMC5883L_REG_IDENT_B  0x0B
#define HMC5883L_REG_IDENT_C  0x0C
#elif defined CONFIG_QMC5883L
#define QMC5883L_REG_OUT_X_M  0x01
#define QMC5883L_REG_OUT_X_L  0x00
#define QMC5883L_REG_OUT_Z_M  0x05
#define QMC5883L_REG_OUT_Z_L  0x04
#define QMC5883L_REG_OUT_Y_M  0x03
#define QMC5883L_REG_OUT_Y_L  0x02
#define QMC5883L_REG_STATUS   0x06
#define QMC5883L_REG_CONFIG_1 0x09
#define QMC5883L_REG_CONFIG_2 0x0A
#define QMC5883L_REG_IDENT_B  0x0B
#define QMC5883L_REG_IDENT_C  0x20
#define QMC5883L_REG_IDENT_D  0x21
#elif defined CONFIG_VMC5883L
#define VCM5883L_REG_OUT_X_L 0x00
#define VCM5883L_REG_OUT_X_H 0x01
#define VCM5883L_REG_OUT_Y_L 0x02
#define VCM5883L_REG_OUT_Y_H 0x03
#define VCM5883L_REG_OUT_Z_L 0x04
#define VCM5883L_REG_OUT_Z_H 0x05
#define VCM5883L_CTR_REG1    0x0B
#define VCM5883L_CTR_REG2    0x0A
#endif

// configs

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_SAMPLES_8 = 0b11,
    HMC5883L_SAMPLES_4 = 0b10,
    HMC5883L_SAMPLES_2 = 0b01,
    HMC5883L_SAMPLES_1 = 0b00,
#elif defined CONFIG_QMC5883L || defined CONFIG_VMC5883
    QMC5883L_SAMPLES_8      = 0b11,
    QMC5883L_SAMPLES_4      = 0b10,
    QMC5883L_SAMPLES_2      = 0b01,
    QMC5883L_SAMPLES_1      = 0b00
#endif
} xmc5883l_samples_t;

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_DATARATE_75HZ    = 0b110,
    HMC5883L_DATARATE_30HZ    = 0b101,
    HMC5883L_DATARATE_15HZ    = 0b100,
    HMC5883L_DATARATE_7_5HZ   = 0b011,
    HMC5883L_DATARATE_3HZ     = 0b010,
    HMC5883L_DATARATE_1_5HZ   = 0b001,
    HMC5883L_DATARATE_0_75_HZ = 0b000,
#elif defined CONFIG_QMC5883L
    QMC5883L_DATARATE_10HZ  = 0b00,
    QMC5883L_DATARATE_50HZ  = 0b01,
    QMC5883L_DATARATE_100HZ = 0b10,
    QMC5883L_DATARATE_200HZ = 0b11,
#endif
} xmc5883l_data_rate_t;

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_RANGE_8_1GA  = 0b111,
    HMC5883L_RANGE_5_6GA  = 0b110,
    HMC5883L_RANGE_4_7GA  = 0b101,
    HMC5883L_RANGE_4GA    = 0b100,
    HMC5883L_RANGE_2_5GA  = 0b011,
    HMC5883L_RANGE_1_9GA  = 0b010,
    HMC5883L_RANGE_1_3GA  = 0b001,
    HMC5883L_RANGE_0_88GA = 0b000,
#elif defined CONFIG_QMC5883L
    QMC5883L_RANGE_2GA      = 0b00,
    QMC5883L_RANGE_8GA      = 0b01,
#elif defined CONFIG_VMC5883L
    VCM5883L_RANGE_8GA = 0b01,
#endif
} xmc5883l_range_t;

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_IDLE      = 0b10,
    HMC5883_SINGLE     = 0b01,
    HMC5883L_CONTINOUS = 0b00,
#elif defined CONFIG_QMC5883L
    QMC5883L_SINGLE         = 0b00,
    QMC5883L_CONTINOUS      = 0b01,
#elif defined CONFIG_VMC5883L
    VCM5883L_SINGLE    = 0b00,
    VCM5883L_CONTINOUS = 0b01,
#endif
} xmc5883l_mode_t;

#define Vector3(_type_, _name_) \
    typedef struct {            \
        _type_ x, y, z;         \
    } _name_

typedef struct {
    int16_t x, y, z;
} vector3_int16;

typedef struct {
    float_t xy, yz, xz;
} vector3_float;

// read / write function

#define xmc5883l_read_byte(reg)                   i2cdev_get_byte(&xmc5883l, reg, 0x00)
#define xmc5883l_read_word(reg)                   i2cdev_get_word(&xmc5883l, reg, 0x00, ORDER_FIRST_LOW)
#define xmc5883l_write_byte(reg, dat)             i2cdev_write_byte(&xmc5883l, reg, dat)
#define xmc5883l_read_bit(reg, bit)               i2cdev_get_bit(&xmc5883l, reg, bit, 0x00)
#define xmc5883l_write_bit(reg, bit, dat)         i2cdev_write_bit(&xmc5883l, reg, bit, dat)
#define xmc5883l_read_bits(reg, start, len)       i2cdev_get_bits(&xmc5883l, reg, start, len, 0x00)
#define xmc5883l_write_bits(reg, start, len, dat) i2cdev_write_bits(&xmc5883l, reg, start, len, dat)

// main functions

bool xmc5883l_init(void);

vector3_int16* xmc5883l_read_raw(vector3_int16* raw);                                           // return raw
vector3_int16* xmc5883l_calibrate(vector3_int16* raw, vector3_int16* min, vector3_int16* max);  // return raw
vector3_float* xmc5883l_calc_angle(vector3_int16* raw, vector3_float* angle);                   // return angle

void                 xmc5883l_set_range(xmc5883l_range_t range);
xmc5883l_range_t     xmc5883l_get_range(void);
void                 xmc5883l_set_measurement_mode(xmc5883l_mode_t mode);
xmc5883l_mode_t      xmc5883l_get_measurement_mode(void);
void                 xmc5883l_set_data_rate(xmc5883l_data_rate_t rate);
xmc5883l_data_rate_t xmc5883l_get_data_rate(void);
void                 xmc5883l_set_samples(xmc5883l_samples_t samples);
xmc5883l_samples_t   xmc5883l_get_samples(void);

float_t xmc5883l_get_heading(vector3_int16* raw, float_t declination /* 磁偏角 */);

void compass(float heading);

#endif