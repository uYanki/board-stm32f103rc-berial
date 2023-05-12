#ifndef _MPU9250_H
#define _MPU9250_H

#include "i2c.h"
#include <math.h>
#include <stdio.h>

#define MPU_ADDR_LOW 0x68
#define MPU_ADDR_HIGH 0x69

/* MPU ID */

// 丝印
// MP92: MPU9250
// MP65: MPU6500
// MP651: MPU6515

#define MPU6500_ID 0X70
#define MPU9250_ID 0X71
#define MPU9255_ID 0X73
#define MPU6515_ID 0X75
#define MPU6050_ID 0X68
#define MPU9150_ID 0X34
#define MPU6880_ID 0X78

// 注：MPU9XXX 里封装的 MPU6XXX 不是固定的
// MPU9150 = MPU6050 + AK8975C
// MPU9250 = MPU6515 + AK896

#define MPU9250_I2C hi2c2            // <-----
#define MPU_DEV (MPU_ADDR_LOW << 1)  // <-----
#define MPU_ID MPU6500_ID            // <-----
#define AK8963_DEV (0x0C << 1)       // 磁力计地址（固定）
#define AK8963_ID 0X48

/* AK8963 */

#define MAG_WIA 0x00  // 器件ID
#define MAG_CNTL1 0X0A
#define MAG_CNTL2 0X0B

// 磁力计
#define MAG_XOUT_L 0X03
#define MAG_XOUT_H 0X04
#define MAG_YOUT_L 0X05
#define MAG_YOUT_H 0X06
#define MAG_ZOUT_L 0X07
#define MAG_ZOUT_H 0X08

/* MPUXXXX */

#define MPU_SAMPLE_RATE 0X19  // 采样频率分频器
#define MPU_CFG 0X1A          // 器件配置
#define MPU_GYRO_CFG 0X1B     // 陀螺仪配置
#define MPU_ACCEL_CFG 0X1C    // 加速度计配置
#define MPU_MOTION_DET 0X1F   // 运动检测阀值设置
#define MPU_FIFO_EN 0X23      // FIFO使能
#define MPU_INTBP_CFG 0X37    // 中断&旁路设置
#define MPU_INT_EN 0X38       // 中断使能
#define MPU_INT_STA 0X3A      // 中断状态

// 加速度
#define MPU_ACCEL_XOUTH 0X3B
#define MPU_ACCEL_XOUTL 0X3C
#define MPU_ACCEL_YOUTH 0X3D
#define MPU_ACCEL_YOUTL 0X3E
#define MPU_ACCEL_ZOUTH 0X3F
#define MPU_ACCEL_ZOUTL 0X40
// 温度
#define MPU_TEMP_OUTH 0X41
#define MPU_TEMP_OUTL 0X42
// 陀螺仪
#define MPU_GYRO_XOUTH 0X43
#define MPU_GYRO_XOUTL 0X44
#define MPU_GYRO_YOUTH 0X45
#define MPU_GYRO_YOUTL 0X46
#define MPU_GYRO_ZOUTH 0X47
#define MPU_GYRO_ZOUTL 0X48

#define MPU_MDETECT_CTRL 0X69  // 运动检测控制
#define MPU_USER_CTRL 0X6A     // 用户控制
#define MPU_PWR_MGMT1 0X6B     // 电源管理1
#define MPU_PWR_MGMT2 0X6C     // 电源管理2

#define MPU_DEVICE_ID 0X75  // 器件ID

#define RAD_TO_DEG 57.2958

uint8_t MPU9250_Init(void);

HAL_StatusTypeDef MPU_WriteByte(uint8_t dev, uint8_t reg, uint8_t data);
uint8_t           MPU_ReadByte(uint8_t dev, uint8_t reg);
HAL_StatusTypeDef MPU_WriteBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t* buf);
HAL_StatusTypeDef MPU_ReadBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t* buf);

HAL_StatusTypeDef MPU_SetGyroFsr(uint8_t fsr);
HAL_StatusTypeDef MPU_SetAccelFsr(uint8_t fsr);
HAL_StatusTypeDef MPU_SetRate(uint16_t rate);

float             MPU_GetTemperature(void);
HAL_StatusTypeDef MPU_GetGyroscope(int16_t* gx, int16_t* gy, int16_t* gz);
HAL_StatusTypeDef MPU_GetAccelerometer(int16_t* ax, int16_t* ay, int16_t* az);
HAL_StatusTypeDef MPU_GetMagnetometer(int16_t* mx, int16_t* my, int16_t* mz);

HAL_StatusTypeDef  MPU_CalcAngle(void);

#endif
