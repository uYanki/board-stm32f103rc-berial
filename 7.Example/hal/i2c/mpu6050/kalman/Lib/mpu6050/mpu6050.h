/*
 * mpu6050.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Bulanov Konstantin
 */

#ifndef INC_GY521_H_
#define INC_GY521_H_

#endif /* INC_GY521_H_ */

#include <stdint.h>
#include "i2c.h"

// MPU6050 structure
typedef struct
{
    // Raw
    int16_t AccelX, AccelY, AccelZ;
    int16_t GyroX, GyroY, GyroZ;

    // State
    double Ax, Ay, Az;
    double Gx, Gy, Gz;
    float  Temperature;

    // Kalman
    double AngleX;  // roll
    double AngleY;  // pitch
} MPU6050_t;

// Kalman structure
typedef struct
{
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;

uint8_t MPU6050_Init(I2C_HandleTypeDef* I2Cx);

void MPU6050_Read_Accel(I2C_HandleTypeDef* I2Cx, MPU6050_t* DataStruct);

void MPU6050_Read_Gyro(I2C_HandleTypeDef* I2Cx, MPU6050_t* DataStruct);

void MPU6050_Read_Temp(I2C_HandleTypeDef* I2Cx, MPU6050_t* DataStruct);

void MPU6050_Read_All(I2C_HandleTypeDef* I2Cx, MPU6050_t* DataStruct);

double Kalman_getAngle(Kalman_t* Kalman, double newAngle, double newRate, double dt);
