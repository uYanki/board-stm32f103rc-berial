#ifndef __I2C_DEV_H__
#define __I2C_DEV_H__

// i2c master

#if 0
#include "config.h"
#else
#include "i2cdev/config.h"
#endif

#if CONFIG_USING_I2C

#if CONFIG_USING_SOFTWARE_I2C

#include "gpio.h"

#undef SDA_Pin
#undef SCL_Pin

typedef struct {
    // gpio
    GPIO_TypeDef* SDA_Port;
    uint32_t      SDA_Pin;
    GPIO_TypeDef* SCL_Port;
    uint32_t      SCL_Pin;
    // delay_us
    uint32_t      Interval;
} soft_i2c_t;

#endif

#if CONFIG_USING_HARDWARE_I2C

#include "i2c.h"
typedef I2C_HandleTypeDef hard_i2c_t;

#endif

// 字节序, byte order
enum {
    ORDER_FIRST_HIGH = 0,  // litte endian
    ORDER_FIRST_LOW  = 1,  // big endian
};

enum {
    UINT_FORMAT_BIN = 0,  // binary
    UINT_FORMAT_DEC = 1,  // decimal
    UINT_FORMAT_HEX = 2,  // hexadecimal
    UINT_FORMAT_BCD = 3,
};

typedef void*    i2c_bus_t;
typedef uint16_t i2c_dev_t;

// operations
typedef enum {
    /* response */
    I2C_NACK      = 1 << 0,
    // - at WRITE mode: ignore NACK
    // - at READ mode: don't ACK

    /* device address size */
    I2C_DEV_7BIT  = 0 << 1,
    I2C_DEV_10BIT = 1 << 1,

    /* register address size */
    I2C_REG_8BIT  = 0 << 2,
    I2C_REG_16BIT = 1 << 2,
} i2c_ops_t;

// driver

typedef bool (*i2cdrv_init)(i2c_bus_t bus);
typedef bool (*i2cdrv_check)(i2c_bus_t bus, i2c_dev_t dev);
typedef bool (*i2cdrv_recv)(i2c_bus_t bus, i2c_dev_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops);
typedef bool (*i2cdrv_send)(i2c_bus_t bus, i2c_dev_t dev, const uint8_t* dat, uint16_t len, i2c_ops_t ops);
typedef bool (*i2cdrv_read)(i2c_bus_t bus, i2c_dev_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops);
typedef bool (*i2cdrv_write)(i2c_bus_t bus, i2c_dev_t dev, uint16_t reg, const uint8_t* dat, uint16_t len, i2c_ops_t ops);

typedef struct i2c_drv {
    i2cdrv_init  init;
    i2cdrv_check check;
    /* xfer-ops */
    i2cdrv_recv  recv;
    i2cdrv_send  send;
    /* mem-ops */
    i2cdrv_read  read;
    i2cdrv_write write;
}* i2c_drv_t;

// client
typedef struct i2c_cli {
    i2c_bus_t bus;
    i2c_drv_t drv;
    i2c_dev_t dev;
    i2c_ops_t ops;
}* i2c_cli_t;

//======================================// soft i2c

#if CONFIG_USING_SOFTWARE_I2C

// !! 10-bit address is not supported

bool soft_i2c_init(soft_i2c_t* bus);
bool soft_i2c_check(soft_i2c_t* bus, uint8_t dev);
bool soft_i2c_receive(soft_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops);
bool soft_i2c_transmit(soft_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops);
bool soft_i2c_read_mem(soft_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops);
bool soft_i2c_write_mem(soft_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops);

/* examples:

   static soft_i2c_t swi2c_bus = {
        .SCL_Port = GPIOB,
        .SCL_Pin  = GPIO_PIN_10,
        .SDA_Port = GPIOB,
        .SDA_Pin  = GPIO_PIN_11,
        .Interval = 6,
    };

    struct i2c_cli bme280 = {
        .bus = &swi2c_bus,
        .drv = &swi2c_drv,
        .dev = 0x76 << 1,
        .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
    };

    swi2c_drv.init(&swi2c_bus);

    i2cdrv_detector(&swi2c_bus, &swi2c_drv);

*/

static struct i2c_drv swi2c_drv = {
    (i2cdrv_init)soft_i2c_init,
    (i2cdrv_check)soft_i2c_check,
    (i2cdrv_recv)soft_i2c_receive,
    (i2cdrv_send)soft_i2c_transmit,
    (i2cdrv_read)soft_i2c_read_mem,
    (i2cdrv_write)soft_i2c_write_mem,
};

#endif

//======================================// hard i2c

#if CONFIG_USING_HARDWARE_I2C

bool hard_i2c_init(hard_i2c_t* bus);
bool hard_i2c_check(hard_i2c_t* bus, uint16_t dev);
bool hard_i2c_receive(hard_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops);
bool hard_i2c_transmit(hard_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops);
bool hard_i2c_read_mem(hard_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops);
bool hard_i2c_write_mem(hard_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops);

static struct i2c_drv hwi2c_drv = {
    (i2cdrv_init)hard_i2c_init,
    (i2cdrv_check)hard_i2c_check,
    (i2cdrv_recv)hard_i2c_receive,
    (i2cdrv_send)hard_i2c_transmit,
    (i2cdrv_read)hard_i2c_read_mem,
    (i2cdrv_write)hard_i2c_write_mem,
};

/*

    #define hwi2c_bus hi2c2

    static struct i2c_cli mpu6050 = {
        .bus = &hwi2c_bus,
        .drv = &hwi2c_drv,
        .dev = 0x68 << 1,
        .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
    };

    static struct i2c_cli bme280 = {
        .bus = &hi2c2,  // <----
        .drv = &hwi2c_drv,
        .dev = BME280_ADDR_LOW << 1,  // <----
        .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
    };

    hwi2c_drv.init(&hwi2c_bus);

    i2cdrv_detector(&hwi2c_bus, &hwi2c_drv);
    i2cdrv_detector(bme280.bus, bme280.drv);

*/

#endif

//==============================// client

/**
 * @brief basic-ops
 * @param dat buffer pointer
 * @param len buffer size
 */

bool i2cdev_recv_byte(i2c_cli_t cli, uint8_t* dat);
bool i2cdev_send_byte(i2c_cli_t cli, const uint8_t dat);

bool i2cdev_recv_bytes(i2c_cli_t cli, uint8_t* dat, uint16_t len);
bool i2cdev_send_bytes(i2c_cli_t cli, const uint8_t* dat, uint16_t len);

/**
 * @brief uint-ops
 * @param dat in/out data
 * @param order byte order ( 0:little endian, 1: big endian )
 */

bool i2cdev_read_block(i2c_cli_t cli, uint16_t reg, uint8_t* dat, uint16_t len);
bool i2cdev_write_block(i2c_cli_t cli, uint16_t reg, const uint8_t* dat, uint16_t len);

bool i2cdev_read_byte(i2c_cli_t cli, uint16_t reg, uint8_t* dat);
bool i2cdev_write_byte(i2c_cli_t cli, uint16_t reg, const uint8_t dat);

//  byte:8-bit, word:16-bit, dword:32-bit
bool i2cdev_read_word(i2c_cli_t cli, uint16_t reg, uint16_t* dat, uint8_t order);
bool i2cdev_write_word(i2c_cli_t cli, uint16_t reg, const uint16_t dat, uint8_t order);

/**
 * @brief bit-ops
 * @param reg register
 * @param idx start bit [0,7]
 * @param len number of bits [0,8-idx]
 * @param mask bit mask [0,255]
 */

bool i2cdev_read_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t* dat);
bool i2cdev_write_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, const uint8_t dat);

bool i2cdev_read_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, uint8_t* dat);
bool i2cdev_write_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, const uint8_t dat);

bool i2cdev_read_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, uint8_t* dat);
bool i2cdev_write_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, const uint8_t dat);

/**
 * @brief fast-read
 * @param def default value, return it when fail to read
 */

uint8_t  i2cdev_get_byte(i2c_cli_t cli, uint16_t reg, const uint8_t def);
uint16_t i2cdev_get_word(i2c_cli_t cli, uint16_t reg, const uint16_t def, uint8_t order);
uint8_t  i2cdev_get_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, const uint8_t def);
uint8_t  i2cdev_get_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, const uint8_t def);
uint8_t  i2cdev_get_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, const uint8_t def);

/**
 * @brief check whether the device exists
 */

bool i2cdev_check(i2c_cli_t cli);

/**
 * @brief device 7-bit address scanner
 * @return number of scanned devices
 */
uint8_t i2cdrv_detector(i2c_bus_t bus, i2c_drv_t drv);

/**
 * @brief from start to end  memory viewer
 * @param start begin of register
 * @param end end of register
 * @param fmt output format (UINT_FORMAT_xxx: 0:bin 1:dec 2:hex)
 */

bool i2cdev_viewer(i2c_cli_t cli, uint16_t start, uint16_t end, uint8_t fmt);

/**
 * @brief write reg-val pair
 * @param dat register(uint8_t) and value(uint8_t) pair
 */

bool i2cdev_mapset(i2c_cli_t cli, uint8_t* dat, uint16_t len);

#endif

#endif
