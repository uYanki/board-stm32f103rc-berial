

#ifndef __GY614_H__
#define __GY614_H__

// 白SDA,黄SCL,焊上PS

#include "i2cdev/i2cdev.h"

#define GY614_ADDRESS 0xA4  // <---

static struct i2c_cli gy614 = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = GY614_ADDRESS,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

#define GY614_I2C                hi2c1  // <---

////////////////////////////////////////////

#define GY614_REG_I2C_ADDRESS    0x00
#define GY614_REG_BAUD_RATE      0x01
#define GY614_REG_SAMPLE_RATE    0x02
#define GY614_REG_OUTPUT_MODE    0x03  // 输出模式
#define GY614_REG_OUTPUT_FORMAT  0x04  // 输出格式
#define GY614_REG_CONFIG_STATE   0x05
#define GY614_REG_TEMP_OFFSET    0x06  // 额温补偿
#define GY614_REG_EMISSIVITY     0x07  // 发射率, 1~100 -> 0.01~1.00
#define GY614_REG_TEMP_OBJECT_H  0x08  // 目标温度
#define GY614_REG_TEMP_OBJECT_L  0x09
#define GY614_REG_TEMP_AMBIENT_H 0x0A  // 环境温度
#define GY614_REG_TEMP_AMBIENT_L 0x0B
#define GY614_REG_TEMP_BROW_H    0x0C  // 由额温转换为体温值后的值
#define GY614_REG_TEMP_BROW_L    0x0D
#define GY614_REG_CHIP_INFO      0x0E  // 芯片型号 0~3
#define GY614_REG_FIREWARE_INFO  0x0F  // 固件信息 0~255

////////////////////////////////////////////

// for uart
typedef enum {
    GY614_BAUD_RATE_2400   = 0,
    GY614_BAUD_RATE_4800   = 1,
    GY614_BAUD_RATE_9600   = 2,
    GY614_BAUD_RATE_19200  = 3,
    GY614_BAUD_RATE_38400  = 4,
    GY614_BAUD_RATE_57600  = 5,
    GY614_BAUD_RATE_115200 = 6,
    GY614_BAUD_RATE_230400 = 7,
} gy614_baud_rate_t;

// for uart
typedef enum {
    GY614_OUTPUT_MODE_CONTINUE = 0,  // 连续
    GY614_OUTPUT_MODE_QUERY    = 1,  // 查询
} gy614_output_mode_t;

// for uart
typedef enum {
    GY614_OUTPUT_FORMAT_HEX  = 0,  // 十六进制
    GY614_OUTPUT_FORMAT_CHAR = 1,  // 字符
} gy614_output_format_t;

typedef enum {
    GY614_SAMPLE_RATE_1HZ  = 0,
    GY614_SAMPLE_RATE_2HZ  = 1,
    GY614_SAMPLE_RATE_5HZ  = 2,
    GY614_SAMPLE_RATE_10HZ = 3,
} gy614_sample_rate_t;

typedef enum {
    GY614_CONFIG_SET   = 0x55,  // 保存当前设置
    GY614_CONFIG_RESET = 0xAA,  // 恢复出厂设置
} gy614_config_t;

////////////////////////////////////////////

#define gy614_read_byte(reg)        i2cdev_get_byte(&gy614, reg, 0x00)
#define gy614_read_word(reg)        i2cdev_get_word(&gy614, reg, 0x00, ORDER_FIRST_HIGH)
#define gy614_write_byte(reg, data) i2cdev_write_byte(&gy614, reg, data)

////////////////////////////////////////////

#define gy614_print_regs()           i2c_print_regs(&gy614, GY614_REG_I2C_ADDRESS, GY614_REG_FIREWARE_INFO, 2)

gy614_baud_rate_t gy614_get_baud_rate(void);
void              gy614_set_baud_rate(gy614_baud_rate_t rate);

gy614_sample_rate_t gy614_get_sample_rate(void);
void                gy614_set_sample_rate(gy614_sample_rate_t rate);

gy614_output_mode_t gy614_get_output_mode(void);
void                gy614_set_output_mode(gy614_output_mode_t mode);

gy614_output_format_t gy614_get_output_format(void);
void                  gy614_set_output_format(gy614_output_format_t format);

void gy614_save_config(void);
void gy614_reset_config(void);

void    gy614_set_address(uint8_t address);
uint8_t gy614_get_address(void);

void    gy614_set_emissivity(uint8_t emissivity);
uint8_t gy614_get_emissivity(void);

uint16_t gy614_get_object_temp(void);
uint16_t gy614_get_ambient_temp(void);

uint8_t  gy614_get_temp_offset(void);
void     gy614_set_temp_offset(uint8_t offset);
uint16_t gy614_get_brow_temp(void);

uint8_t gy614_get_chip_info(void);

uint8_t gy614_get_fireware_info(void);

#endif
