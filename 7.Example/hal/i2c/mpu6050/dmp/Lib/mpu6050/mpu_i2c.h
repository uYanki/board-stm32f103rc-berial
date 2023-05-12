#ifndef __MPU_I2C_H_
#define __MPU_I2C_H_

#include <math.h>
#include <stdint.h>
#include "i2c.h"

#define MPU6050_I2C hi2c2

// for inv_mpu.c & inv_mpu_dmp_motion_driver.c
#define STM32_HAL
#define MPU6050
#define TRACE

#ifdef STM32_HAL
#define fabs              fabsf
#define min(a, b)         ((a < b) ? a : b)
#define delay_ms          HAL_Delay
#define get_ms(timestamp) (*timestamp = HAL_GetTick())
#define __no_operation()  (0)
static inline int reg_int_cb(struct int_param_s* int_param)
{
    UNUSED(int_param);
    return 0;
}
#endif

#ifdef TRACE
#ifdef DIAG_TRACE_H_
#define log_i trace_printf
#define log_e trace_printf
#else
#define log_i printf
#define log_e printf
#endif
#else
// log_info
#define log_i(...) \
    do {           \
    } while (0)
// log error
#define log_e(...) \
    do {           \
    } while (0)
#endif

HAL_StatusTypeDef i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t* data);
HAL_StatusTypeDef i2c_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t* data);
HAL_StatusTypeDef i2c_write_bit(uint8_t slave_addr, uint8_t reg_addr, uint8_t bitNum, uint8_t data);
HAL_StatusTypeDef i2c_write_bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t bitStart, uint8_t length, uint8_t data);

#endif
