#ifndef __LM75_H__
#define __LM75_H__

#include "i2cdev/i2cdev.h"

// lm75 address (A2A1A0)
enum {
    LM75_ADDRESS_A000 = 0x48,
    LM75_ADDRESS_A001,
    LM75_ADDRESS_A010,
    LM75_ADDRESS_A011,
    LM75_ADDRESS_A100,
    LM75_ADDRESS_A101,
    LM75_ADDRESS_A110,
    LM75_ADDRESS_A111,
};

static struct i2c_cli lm75 = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = LM75_ADDRESS_A000 << 1,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

// chip register
#define LM75_REG_CONF  0x01  // configure
#define LM75_REG_TEMP  0x00  // temperature
#define LM75_REG_TOS   0x03  // overtemperature shutdown, Tth(ots) , default = 80°C
#define LM75_REG_THYST 0x02  // hysteresis, Thys , default = 75°C

// fault queue
typedef enum {
    LM75_FAULT_QUEUE_1 = 0,
    LM75_FAULT_QUEUE_2 = 1,
    LM75_FAULT_QUEUE_3 = 2,
    LM75_FAULT_QUEUE_6 = 3,
} lm75_fault_queue_t;

// os polarity / os active level
typedef enum {
    LM75_OS_POLARITY_LOW  = 0,
    LM75_OS_POLARITY_HIGH = 1,
} lm75_os_polarity_t;

// mode
typedef enum {
    LM75_MODE_NORMAL   = 0,
    LM75_MODE_SHUTDOWN = 1,
} lm75_device_mode_t;

// os operation
typedef enum {
    LM75_OS_OPERATION_COMPARATOR = 0,  // 比较器
    LM75_OS_OPERATION_INTERRUPT  = 1,  // 中断
} lm75_os_operation_mode_t;

////////////////////////////////////////////

#define lm75_read_word(reg)                    i2cdev_get_word(&lm75, reg, 0, ORDER_FIRST_HIGH)
#define lm75_write_word(reg, data)             i2cdev_write_word(&lm75, reg, data, ORDER_FIRST_HIGH)
#define lm75_read_bit(reg, index)              i2cdev_get_bit(&lm75, reg, index, 0)
#define lm75_write_bit(reg, index, data)       i2cdev_write_bit(&lm75, reg, index, data)
#define lm75_read_bits(reg, start, len)        i2cdev_get_bits(&lm75, reg, start, len, 0)
#define lm75_write_bits(reg, start, len, data) i2cdev_write_bits(&lm75, reg, start, len, data)

////////////////////////////////////////////

float lm75_get_limit(uint8_t reg);
void  lm75_set_limit(uint8_t reg, float value);

// limits

#define lm75_get_hysteresis()                          lm75_get_limit(LM75_REG_THYST)
#define lm75_set_hysteresis(hysteresis)                lm75_set_limit(LM75_REG_THYST, hysteresis)

#define lm75_get_over_temperature_threshold()          lm75_get_limit(LM75_REG_TOS)
#define lm75_set_over_temperature_threshold(threshold) lm75_set_limit(LM75_REG_TOS, threshold)

// bit[0]
#define lm75_get_device_mode()                         lm75_read_bit(LM75_REG_CONF, 0)
#define lm75_set_device_mode(mode)                     lm75_write_bit(LM75_REG_CONF, 0, mode)

// bit[1]
#define lm75_get_operation_mode()                      lm75_read_bit(LM75_REG_CONF, 1)
#define lm75_set_operation_mode(mode)                  lm75_write_bit(LM75_REG_CONF, 1, mode)

// bit[2]
#define lm75_get_os_polarity()                         lm75_read_bit(LM75_REG_CONF, 2)
#define lm75_set_os_polarity(mode)                     lm75_write_bit(LM75_REG_CONF, 2, mode)

// bit[4,3]
#define lm75_get_fault_queue()                         lm75_read_bits(LM75_REG_CONF, 3, 2);
#define lm75_set_fault_queue(fault_queue)              lm75_write_bits(LM75_REG_CONF, 3, 2, fault_queue);

// temp

float lm75_read_temp(void);

#endif
