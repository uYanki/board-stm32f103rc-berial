
/**
 * @file mpu/math.h
 * @brief MPU Math helper file
 */

#ifndef _MPU_MATH_H_
#define _MPU_MATH_H_

#include "types.h"

#define M_PI 3.1415926

static inline uint8_t AccelFSRvalue(const accel_fs_t fs) {
    return 2 << fs;
}

static inline uint16_t GyroFSRvalue(const gyro_fs_t fs) {
    return 250 << fs;
}

static inline uint16_t AccelSensitivity(const accel_fs_t fs) {
    return 16384 >> fs;
}

static inline float GyroSensitivity(const gyro_fs_t fs) {
    return 131.f / (1 << fs);
}

static inline float AccelResolution(const accel_fs_t fs) {
    return (float)(AccelFSRvalue(fs)) / INT16_MAX;
}

static inline float GyroResolution(const gyro_fs_t fs) {
    return (float)(GyroFSRvalue(fs)) / INT16_MAX;
}

static inline float AccelGravity(const int16_t axis, const accel_fs_t fs) {
    return axis * AccelResolution(fs);
}

static inline float_axes_t AccelGravityEx(const raw_axes_t* raw_axes, const accel_fs_t fs) {
    float_axes_t axes;
    axes.x = raw_axes->x * AccelResolution(fs);
    axes.y = raw_axes->y * AccelResolution(fs);
    axes.z = raw_axes->z * AccelResolution(fs);
    return axes;
}

static inline float GyroDegPerSec(const int16_t axis, const gyro_fs_t fs) {
    return axis * GyroResolution(fs);
}

static inline float_axes_t GyroDegPerSecEx(const raw_axes_t* raw_axes, const gyro_fs_t fs) {
    float_axes_t axes;
    axes.x = raw_axes->x * GyroResolution(fs);
    axes.y = raw_axes->y * GyroResolution(fs);
    axes.z = raw_axes->z * GyroResolution(fs);
    return axes;
}

static inline float GyroRadPerSec(const int16_t axis, const gyro_fs_t fs) {
    return (M_PI / 180) * GyroDegPerSec(axis, fs);
}

static inline float_axes_t GyroRadPerSecEx(const raw_axes_t* raw_axes, const gyro_fs_t fs) {
    float_axes_t axes;
    axes.x = (M_PI / 180) * GyroDegPerSec(raw_axes->x, fs);
    axes.y = (M_PI / 180) * GyroDegPerSec(raw_axes->y, fs);
    axes.z = (M_PI / 180) * GyroDegPerSec(raw_axes->z, fs);
    return axes;
}

#if defined CONFIG_MPU6500 || defined CONFIG_MPU9250

#define kRoomTempOffset 0         // LSB(int16_t)
#define kCelsiusOffset 21.f       // ºC(float)
#define kTempSensitivity 333.87f  // LSB/ºC(float)

#elif defined CONFIG_MPU6000 || defined CONFIG_MPU6050 || defined CONFIG_MPU9150

#define kRoomTempOffset -521    // LSB
#define kCelsiusOffset 35.f     // ºC
#define kTempSensitivity 340.f  // LSB/ºC

#endif

static const float kTempResolution   = 98.67f / INT16_MAX;
static const float kFahrenheitOffset = kCelsiusOffset * 1.8f + 32;  // ºF

static inline float TempCelsius(const int16_t temp) {
    // TEMP_degC = ((TEMP_OUT – RoomTemp_Offset)/Temp_Sensitivity) + DegreesCelsius_Offset
    return (temp - kRoomTempOffset) * kTempResolution + kCelsiusOffset;
}

static inline float TempFahrenheit(const int16_t temp) {
    return (temp - kRoomTempOffset) * kTempResolution * 1.8f + kFahrenheitOffset;
}

#if defined CONFIG_MPU_AK89xx

static inline int16_t MagAdjust(const int16_t axis, const uint8_t adjValue) {
    // Hadj = H * ((((ASA - 128) * 0.5) / 128) + 1)
    // return axis * ((((adjValue - 128) * 0.5f) / 128) + 1);
    const float factor = 0.5f / 128;
    return axis * ((adjValue - 128) * factor + 1);
}
#endif

#endif /* end of include guard: _MPU_MATH_H_ */
