#ifndef __MLX90614_H__
#define __MLX90614_H__

// IR temperature sensor

#include "i2cdev/i2cdev.h"
#include <stdbool.h>

#define MLX90614_ADDR 0x66

static struct i2c_cli mlx90614 = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = MLX90614_ADDR << 1,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

//////////////////////////////////////////// command

// 操作码

#define COMMAND_READ_FLAGS      0xF0  // read flags
#define COMMAND_ENTER_SLEEP     0xFF  // enter sleep
#define COMMAND_RAM_ACCESS      0x00
#define COMMAND_EEPROM_ACCESS   0x20

//////////////////////////////////////////// register

// < RAM > 0x00 (不可写)
#define MLX90614_REG_RAWIR1     (COMMAND_RAM_ACCESS | 0x04)  // raw data ir channel 1
#define MLX90614_REG_RAWIR2     (COMMAND_RAM_ACCESS | 0x05)  // raw data ir channel 2
#define MLX90614_REG_TA         (COMMAND_RAM_ACCESS | 0x06)  // ta （temperature）
#define MLX90614_REG_TOBJ1      (COMMAND_RAM_ACCESS | 0x07)  // tobj1（temperature）
#define MLX90614_REG_TOBJ2      (COMMAND_RAM_ACCESS | 0x08)  // tobj2（temperature）
// < EEPROM > 0x20
#define MLX90614_REG_TO_MAX     (COMMAND_EEPROM_ACCESS | 0x00)  // to max
#define MLX90614_REG_TO_MIN     (COMMAND_EEPROM_ACCESS | 0x01)  // to min
#define MLX90614_REG_PWM_CTRL   (COMMAND_EEPROM_ACCESS | 0x02)  // pwm ctrl
#define MLX90614_REG_TA_RANGE   (COMMAND_EEPROM_ACCESS | 0x03)  // ta range
#define MLX90614_REG_EMISSIVITY (COMMAND_EEPROM_ACCESS | 0x04)  // emissivity
#define MLX90614_REG_CONFIG1    (COMMAND_EEPROM_ACCESS | 0x05)  // config1
#define MLX90614_REG_ADDRESS    (COMMAND_EEPROM_ACCESS | 0x0E)  // address
#define MLX90614_REG_ID1        (COMMAND_EEPROM_ACCESS | 0x1C)  // id1 number
#define MLX90614_REG_ID2        (COMMAND_EEPROM_ACCESS | 0x1D)  // id2 number
#define MLX90614_REG_ID3        (COMMAND_EEPROM_ACCESS | 0x1E)  // id3 number
#define MLX90614_REG_ID4        (COMMAND_EEPROM_ACCESS | 0x1F)  // id4 number

//////////////////////////////////////////// types

typedef enum {
    MLX90614_TEMP_UNIT_RAW,         // 原始数据
    MLX90614_TEMP_UNIT_KELVIN,      // 开氏度（Kelvin）
    MLX90614_TEMP_UNIT_CENTIGRADE,  // 摄氏度（Centigrade）
    MLX90614_TEMP_UNIT_FAHRENHEIT,  // 华氏度（Fahrenheit）
} mlx90614_temp_unit;

typedef enum {
    MLX90614_FLAG_EE_BUSY = (1 << 7),  // the previous write/erase eeprom access is still in progress, high active
    MLX90614_FLAG_EE_DEAD = (1 << 5),  // eeprom double error has occurred, high active
    MLX90614_FLAG_INIT    = (1 << 4),  // por initialization routine is still ongoing, low active
} mlx90614_flag_t;

typedef enum {
    MLX90614_MODE_TA_TOBJ1    = 0x00,  // ta and tobj1 mode
    MLX90614_MODE_TA_TOBJ2    = 0x01,  // ta and tobj2 mode
    MLX90614_MODE_TOBJ2       = 0x02,  // tobj2 mode
    MLX90614_MODE_TOBJ1_TOBJ2 = 0x03,  // tobj1 and tobj2 mode
} mlx90614_mode_t;

typedef enum {
    MLX90614_GAIN_1    = 0x00,  // gain 1
    MLX90614_GAIN_3    = 0x01,  // gain 3
    MLX90614_GAIN_6    = 0x02,  // gain 6
    MLX90614_GAIN_12P5 = 0x03,  // gain 12.5
    MLX90614_GAIN_25   = 0x04,  // gain 25
    MLX90614_GAIN_50   = 0x05,  // gain 50
    MLX90614_GAIN_100  = 0x06,  // gain 100
} mlx90614_gain_t;

typedef enum {
    MLX90614_KT2_POSITIVE = 0x00,  // positive
    MLX90614_KT2_NEGATIVE = 0x01,  // negative
} mlx90614_kt2_t;

typedef enum {
    MLX90614_KS_POSITIVE = 0x00,  // positive
    MLX90614_KS_NEGATIVE = 0x01,  // negative
} mlx90614_ks_t;

typedef enum {
    MLX90614_IR_SENSOR_SINGLE = 0x00,  // single ir sensor
    MLX90614_IR_SENSOR_DUAL   = 0x01,  // dual ir sensor
} mlx90614_ir_sensor_t;

// fir length
typedef enum {
    MLX90614_FIR_LENGTH_8    = 0x00,  // length 8
    MLX90614_FIR_LENGTH_16   = 0x01,  // length 16
    MLX90614_FIR_LENGTH_32   = 0x02,  // length 32
    MLX90614_FIR_LENGTH_64   = 0x03,  // length 64
    MLX90614_FIR_LENGTH_128  = 0x04,  // length 128
    MLX90614_FIR_LENGTH_256  = 0x05,  // length 256
    MLX90614_FIR_LENGTH_512  = 0x06,  // length 512
    MLX90614_FIR_LENGTH_1024 = 0x07,  // length 1024
} mlx90614_fir_length_t;

// iir
typedef enum {
    MLX90614_IIR_A1_0P5_B1_0P5     = 0x00,  // a1 = 0.5 b1 = 0.5
    MLX90614_IIR_A1_0P25_B1_0P75   = 0x01,  // a1 = 0.25 b1 = 0.75
    MLX90614_IIR_A1_0P166_B1_0P83  = 0x02,  // a1 = 0.166 b1 = 0.83
    MLX90614_IIR_A1_0P125_B1_0P875 = 0x03,  // a1 = 0.125 b1 = 0.875
    MLX90614_IIR_A1_1_B1_0         = 0x04,  // a1 = 1 b1 = 0
    MLX90614_IIR_A1_0P8_B1_0P2     = 0x05,  // a1 = 0.8 b1 = 0.2
    MLX90614_IIR_A1_0P666_B1_0P333 = 0x06,  // a1 = 0.666 b1 = 0.333
    MLX90614_IIR_A1_0P571_B1_0P428 = 0x07,  // a1 = 0.571 b1 = 0.428
} mlx90614_iir_t;

//////////////////////////////////////////// functions

bool mlx90614_read_word_crc(uint8_t reg, uint16_t* data);
bool mlx90614_write_word_crc(uint8_t reg, uint16_t data);
bool mlx90614_write_bit_crc(uint8_t reg, uint8_t bit, uint8_t data);
bool mlx90614_read_bit_crc(uint8_t reg, uint8_t bit, uint8_t* data);
bool mlx90614_read_bits_crc(uint8_t reg, uint8_t start, uint8_t len, uint8_t* data);
bool mlx90614_write_bits_crc(uint8_t reg, uint8_t start, uint8_t len, uint8_t data);

// write function for eeprom（register address above 0x20）
bool mlx90614_write_eeprom(uint8_t reg, uint16_t data);  // be uesd when reg & 0x20 ≠ 0

////////////////////////////////////////////

// get / set 7-bit I2C address
bool mlx90614_get_address(uint8_t* address);
bool mlx90614_set_address(uint8_t address);

bool mlx90614_get_chip_id(uint16_t ID[4]);

////////////////////////////////////////////

bool mlx90614_get_iir(mlx90614_iir_t* iir);
bool mlx90614_set_iir(mlx90614_iir_t iir);

bool mlx90614_get_repeat_sensor_test(bool* enable);
bool mlx90614_set_repeat_sensor_test(bool enable);

bool mlx90614_get_mode(mlx90614_mode_t* mode);
bool mlx90614_set_mode(mlx90614_mode_t mode);

bool mlx90614_get_ir_sensor(mlx90614_ir_sensor_t* sensor);
bool mlx90614_set_ir_sensor(mlx90614_ir_sensor_t sensor);

bool mlx90614_get_ks(mlx90614_ks_t* ks);
bool mlx90614_set_ks(mlx90614_ks_t ks);

bool mlx90614_get_fir_length(mlx90614_fir_length_t* len);
bool mlx90614_set_fir_length(mlx90614_fir_length_t len);

bool mlx90614_get_gain(mlx90614_gain_t* gain);
bool mlx90614_set_gain(mlx90614_gain_t gain);

bool mlx90614_get_kt2(mlx90614_kt2_t* kt2);
bool mlx90614_set_kt2(mlx90614_kt2_t kt2);

bool mlx90614_get_sensor_test(bool* enable);
bool mlx90614_set_sensor_test(bool enable);

// the emissivity of human skin is 0.98 (辐射系数)
bool    mlx90614_get_emissivity(float* emissivity);
uint8_t mlx90614_set_emissivity(float emissivity);

bool mlx90614_get_raw_ir_channel_1(uint16_t* data);
bool mlx90614_get_raw_ir_channel_2(uint16_t* data);

////////////////////////////////////////////

float   mlx90614_convert_raw_to_temp(int16_t raw, mlx90614_temp_unit unit);
int16_t mlx90614_convert_temp_to_raw(float temp, mlx90614_temp_unit unit);

bool mlx90614_get_object1_temp(int16_t* raw);
bool mlx90614_get_object2_temp(int16_t* raw);
bool mlx90614_get_ambient_temp(int16_t* raw);

bool mlx90614_get_min_temp(int16_t* raw);
bool mlx90614_get_max_temp(int16_t* raw);
bool mlx90614_set_min_temp(int16_t raw);
bool mlx90614_set_max_temp(int16_t raw);

////////////////////////////////////////////

uint8_t mlx90614_get_flag(void);  // 状态标志
bool    mlx90614_enter_sleep_mode(void);

#endif
