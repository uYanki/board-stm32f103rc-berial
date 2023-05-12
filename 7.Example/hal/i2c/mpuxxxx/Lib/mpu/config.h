#ifndef _MPU_CONFUG_H_
#define _MPU_CONFUG_H_

// <! Select MPU

/**
 * MPU9250 is the same as MPU6500 + AK8963
 * MPU9150 is the same as MPU6050 + AK8975
 * MPU6000 code is the same as MPU6050 code
 * MPU6555 equals MPU6500, MPU9255 equals MPU9250
 */

// #define CONFIG_MPU6000 1
// #define CONFIG_MPU6050 1
#define CONFIG_MPU6500 1
// #define CONFIG_MPU9150 1
// #define CONFIG_MPU9250 1
// #define CONFIG_MPU6555 1
// #define CONFIG_MPU9255 1

#if defined CONFIG_MPU9255
#define CONFIG_MPU9250 1
#endif

#if defined CONFIG_MPU6000 || defined CONFIG_MPU9150
#define CONFIG_MPU6050 1
#elif defined CONFIG_MPU6555 || defined CONFIG_MPU9250 || defined CONFIG_MPU9255
#define CONFIG_MPU6500 1
#endif

#if defined CONFIG_MPU9250 || defined CONFIG_MPU9255
#define CONFIG_MPU_AK8963 1
#elif defined CONFIG_MPU9150
#define CONFIG_MPU_AK8975 1
#endif

#if defined CONFIG_MPU_AK8963 || defined CONFIG_MPU_AK8975
#define CONFIG_MPU_AK89xx 1
#endif

// <! Select BUS

// #define CONFIG_MPU_I2C 1
#define CONFIG_MPU_SPI 1

#ifdef CONFIG_MPU_I2C

#include "i2c.h"

/*! MPU's possible I2C slave addresses */
typedef enum {
    MPU_I2CADDRESS_AD0_LOW  = 0x68,
    MPU_I2CADDRESS_AD0_HIGH = 0x69
} mpu_i2caddr_t;

// #define MPU_ADDR MPU_I2CADDRESS_AD0_LOW
#define MPU_ADDR MPU_I2CADDRESS_AD0_HIGH

#define mpu_bus  hi2c2  //<---
#define mpu_addr (MPU_ADDR << 1)

#elif CONFIG_MPU_SPI

#if defined CONFIG_MPU6000 || defined CONFIG_MPU6500 || defined CONFIG_MPU6555 || defined CONFIG_MPU9250 || defined CONFIG_MPU9255

#include "spi.h"
#define mpu_bus hspi1  //<---

#define MPU_CS_DISABLE() HAL_GPIO_WritePin(MPU_CS_GPIO_Port, MPU_CS_Pin, GPIO_PIN_SET)
#define MPU_CS_ENABLE()  HAL_GPIO_WritePin(MPU_CS_GPIO_Port, MPU_CS_Pin, GPIO_PIN_RESET)

#endif

#endif

static const char* TAG = "MPU";

typedef HAL_StatusTypeDef mpu_err_t;
#define MPU_OK  HAL_OK
#define MPU_ERR HAL_ERROR
#define MPU_TO  HAL_TIMEOUT

#define delay_ms HAL_Delay

// Read / Write
mpu_err_t MPU_ReadBit(uint8_t reg, uint8_t index /* bit index */, uint8_t* data);
mpu_err_t MPU_ReadBits(uint8_t reg, uint8_t start /* bit start */, uint8_t length, uint8_t* data);
mpu_err_t MPU_ReadByte(uint8_t reg, uint8_t* data);
mpu_err_t MPU_ReadBytes(uint8_t reg, size_t length, uint8_t* data);
mpu_err_t MPU_WriteBit(uint8_t reg, uint8_t index /* bit index */, uint8_t data);
mpu_err_t MPU_WriteBits(uint8_t reg, uint8_t start /* bit start */, uint8_t length, uint8_t data);
mpu_err_t MPU_WriteByte(uint8_t reg, uint8_t data);
mpu_err_t MPU_WriteBytes(uint8_t reg, size_t length, uint8_t* data);

#if defined CONFIG_MPU_AK89xx
mpu_err_t   MAG_ReadByte(uint8_t addr, uint8_t reg, uint8_t* data);
mpu_err_t   MAG_WriteByte(uint8_t addr, uint8_t reg, uint8_t data);
#endif

// <! Other Config

// #define CONFIG_MPU_COMM_PROTOCOL 1
// #define CONFIG_MPU_ENABLE_DMP 1
// #define CONFIG_MPU_FIFO_CORRUPTION_CHECK 1

#endif
