#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <math.h>

#include "board.h"
#include "i2cdev/i2cdev.h"

#define MPU6050_ADDR_LOW         0xD0  // 0x68<<1
#define MPU6050_ADDR_HIGH        0XD1  // 0x69<<1

#define MPU6050_DEV              MPU6050_ADDR_LOW  // <--- 器件地址
#define MPU6050_I2C              hi2c2             // <--- 硬件I2C

#define MPU6050_ADDRESS_AD0_LOW  0x68
#define MPU6050_ADDRESS_AD0_HIGH 0x69
#define MPU6050_WHO_AM_I         0x75

#define MPU6050_PWR_MGMT_1       0x6B
#define MPU6050_PWR_MGMT_2       0x6C
#define MPU6050_SMPLRT_DIV       0x19
#define MPU6050_CONFIG           0x1A
#define MPU6050_GYRO_CONFIG      0x1B
#define MPU6050_ACCEL_CONFIG     0x1C

#define MPU6050_ACCEL_FULLSCALE  2
#define MPU6050_GYRO_FULLSCALE   250

#define MPU6050_ACCEL_XOUT       0x3B
#define MPU6050_ACCEL_YOUT       0x3D
#define MPU6050_ACCEL_ZOUT       0x3F
#define MPU6050_TEMP_OUT         0x41
#define MPU6050_GYRO_XOUT        0x43
#define MPU6050_GYRO_YOUT        0x45
#define MPU6050_GYRO_ZOUT        0x47

#define MPU6050_FIFO_EN          0x23
#define MPU6050_INT_ENABLE       0x38
#define MPU6050_USER_CTRL        0x6A

#define MPU6050_ACCEL_SENSITIVE  (float)(16384)
#define MPU6050_GYRO_SENSITIVE   (float)(131.0)

#define MPU6050_TEMP_SENSITIVE   (float)340
#define MPU6050_TEMP_OFFSET      (float)36.5

#define RAD_TO_DEG               57.2958

typedef struct mpu6050_data {
    float ax, ay, az;
    float temp;
    float gx, gy, gz;
}* mpu6050_data_t;

void mpu6050_init(i2c_cli_t cli, mpu6050_data_t dat);

bool mpu6050_read(i2c_cli_t cli);

#endif
