/**
 * @file MPU.h
 * MPU library main file. Declare MPU functions.
 *
 * @attention
 *  MPU library requires I2Cbus or SPIbus library.
 *  Select the communication protocol in `menuconfig`
 *  and include the corresponding library to project components.
 *
 * @note
 *  The following is taken in the code:
 *  - MPU9250 is the same as MPU6500 + AK8963
 *  - MPU9150 is the same as MPU6050 + AK8975
 *  - MPU6000 code equals MPU6050
 *  - MPU6555 code equals MPU6500
 *  - MPU9255 code equals MPU9250
 * */

#ifndef _MPU_H_
#define _MPU_H_

#include <stdint.h>
#include "config.h"
#include "types.h"

#ifdef CONFIG_MPU_I2C
#include "i2c.h"
typedef I2C_HandleTypeDef mpu_bus_t;
#elif CONFIG_MPU_SPI
#include "spi.h"
typedef SPI_HandleTypeDef mpu_bus_t;
#endif

// Setup

mpu_err_t MPU_Initialize();
mpu_err_t MPU_Reset();
mpu_err_t MPU_SetSleep(bool enable);
mpu_err_t MPU_TestConnection();
mpu_err_t MPU_SelfTest(selftest_t* result);
mpu_err_t MPU_ResetSignalPath();
uint8_t   MPU_WhoAmI();
bool      MPU_GetSleep();

// Main configurations

mpu_err_t   MPU_SetSampleRate(uint16_t rate);
mpu_err_t   MPU_SetClockSource(clock_src_t clockSrc);
mpu_err_t   MPU_SetDigitalLowPassFilter(dlpf_t dlpf);
uint16_t    MPU_GetSampleRate();
clock_src_t MPU_GetClockSource();
dlpf_t      MPU_GetDigitalLowPassFilter();

// Power management

mpu_err_t       MPU_SetLowPowerAccelMode(bool enable);
mpu_err_t       MPU_SetLowPowerAccelRate(lp_accel_rate_t rate);
lp_accel_rate_t MPU_GetLowPowerAccelRate();
bool            MPU_GetLowPowerAccelMode();
mpu_err_t       MPU_SetStandbyMode(stby_en_t mask);
stby_en_t       MPU_GetStandbyMode();

// Full-Scale Range

mpu_err_t  MPU_SetGyroFullScale(gyro_fs_t fsr);
mpu_err_t  MPU_SetAccelFullScale(accel_fs_t fsr);
gyro_fs_t  MPU_GetGyroFullScale();
accel_fs_t MPU_GetAccelFullScale();

// Offset / Bias

mpu_err_t  MPU_SetGyroOffset(raw_axes_t bias);
mpu_err_t  MPU_SetAccelOffset(raw_axes_t bias);
raw_axes_t MPU_GetGyroOffset();
raw_axes_t MPU_GetAccelOffset();
mpu_err_t  MPU_ComputeOffsets(raw_axes_t* accel, raw_axes_t* gyro);

// Interrupt

mpu_err_t    MPU_SetInterruptConfig(int_config_t config);
mpu_err_t    MPU_SetInterruptEnabled(int_en_t mask);
int_stat_t   MPU_GetInterruptStatus();
int_config_t MPU_GetInterruptConfig();
int_en_t     MPU_GetInterruptEnabled();

// FIFO

mpu_err_t     MPU_SetFIFOMode(fifo_mode_t mode);
mpu_err_t     MPU_SetFIFOConfig(fifo_config_t config);
mpu_err_t     MPU_SetFIFOEnabled(bool enable);
mpu_err_t     MPU_ResetFIFO();
uint16_t      MPU_GetFIFOCount();
mpu_err_t     MPU_ReadFIFO(size_t length, uint8_t* data);
mpu_err_t     MPU_WriteFIFO(size_t length, uint8_t* data);
fifo_mode_t   MPU_GetFIFOMode();
fifo_config_t MPU_GetFIFOConfig();
bool          MPU_GetFIFOEnabled();

// Auxiliary I2C Master

mpu_err_t           MPU_SetAuxI2CConfig(const auxi2c_config_t* config);
mpu_err_t           MPU_SetAuxI2CEnabled(bool enable);
mpu_err_t           MPU_SetAuxI2CSlaveConfig(const auxi2c_slv_config_t* config);
mpu_err_t           MPU_SetAuxI2CSlaveEnabled(auxi2c_slv_t slave, bool enable);
mpu_err_t           MPU_SetAuxI2CBypass(bool enable);
mpu_err_t           MPU_ReadAuxI2CRxData(size_t length, uint8_t* data, size_t skip /* 0 */);
mpu_err_t           MPU_RestartAuxI2C();
auxi2c_stat_t       MPU_GetAuxI2CStatus();
auxi2c_config_t     MPU_GetAuxI2CConfig();
auxi2c_slv_config_t MPU_GetAuxI2CSlaveConfig(auxi2c_slv_t slave);
bool                MPU_GetAuxI2CEnabled();
bool                MPU_GetAuxI2CSlaveEnabled(auxi2c_slv_t slave);
bool                MPU_GetAuxI2CBypass();
mpu_err_t           MPU_AuxI2CWriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
mpu_err_t           MPU_AuxI2CReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data);

// Motion Detection Interrupt

mpu_err_t    MPU_SetMotionDetectConfig(mot_config_t* config);
mot_config_t MPU_GetMotionDetectConfig();
mpu_err_t    MPU_SetMotionFeatureEnabled(bool enable);
bool         MPU_GetMotionFeatureEnabled();

#if defined CONFIG_MPU6000 || defined CONFIG_MPU6050 || defined CONFIG_MPU9150

mpu_err_t      MPU_SetZeroMotionConfig(zrmot_config_t* config);
zrmot_config_t MPU_GetZeroMotionConfig();
mpu_err_t      MPU_SetFreeFallConfig(ff_config_t* config);
ff_config_t    MPU_GetFreeFallConfig();
mot_stat_t     MPU_GetMotionDetectStatus();

#endif

// Compass | Magnetometer

#if defined CONFIG_MPU_AK89xx

mpu_err_t  MPU_CompassInit();
mpu_err_t  MPU_CompassTestConnection();
mpu_err_t  MPU_CompassSetMode(mag_mode_t mode);
mpu_err_t  MPU_CompassGetAdjustment(uint8_t* x, uint8_t* y, uint8_t* z);
mag_mode_t MPU_CompassGetMode();
uint8_t    MPU_CompassWhoAmI();
uint8_t    MPU_CompassGetInfo();
mpu_err_t  MPU_CompassReadByte(uint8_t regAddr, uint8_t* data);
mpu_err_t  MPU_CompassWriteByte(uint8_t regAddr, uint8_t data);
bool       MPU_CompassSelfTest(raw_axes_t* result /* nullptr */);

#endif

#if defined CONFIG_MPU_AK8963

mpu_err_t   MPU_CompassReset();
mpu_err_t   MPU_CompassSetSensitivity(mag_sensy_t sensy);
mag_sensy_t MPU_CompassGetSensitivity();

#endif

// Miscellaneous

mpu_err_t MPU_SetFsyncConfig(int_lvl_t level);
mpu_err_t MPU_SetFsyncEnabled(bool enable);
int_lvl_t MPU_GetFsyncConfig();
bool      MPU_GetFsyncEnabled();

#if defined CONFIG_MPU6500 || defined CONFIG_MPU9250

mpu_err_t MPU_SetFchoice(fchoice_t fchoice);
fchoice_t MPU_GetFchoice();

#endif

#if defined CONFIG_MPU9150 || (defined CONFIG_MPU6050 && !defined CONFIG_MPU6000)

mpu_err_t      MPU_SetAuxVDDIOLevel(auxvddio_lvl_t level);
auxvddio_lvl_t MPU_GetAuxVDDIOLevel();

#endif

// Read / Write
//! Functions to perform direct read or write MPU_Operation(s) to registers.

// mpu_err_t MPU_ReadBit(uint8_t regAddr, uint8_t bitNum, uint8_t* data);
// mpu_err_t MPU_ReadBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t* data);
// mpu_err_t MPU_ReadByte(uint8_t regAddr, uint8_t* data);
// mpu_err_t MPU_ReadBytes(uint8_t regAddr, size_t length, uint8_t* data);
// mpu_err_t MPU_WriteBit(uint8_t regAddr, uint8_t bitNum, uint8_t data);
// mpu_err_t MPU_WriteBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
// mpu_err_t MPU_WriteByte(uint8_t regAddr, uint8_t data);
// mpu_err_t MPU_WriteBytes(uint8_t regAddr, size_t length, uint8_t* data);
mpu_err_t MPU_RegisterDump(uint8_t start /* 0x0 */, uint8_t end /* 0x7F */);

// Sensor readings

mpu_err_t MPU_Acceleration(raw_axes_t* accel);
mpu_err_t MPU_AccelerationEx(int16_t* x, int16_t* y, int16_t* z);
mpu_err_t MPU_Rotation(raw_axes_t* gyro);
mpu_err_t MPU_RotationEx(int16_t* x, int16_t* y, int16_t* z);
mpu_err_t MPU_Temperature(int16_t* temp);
mpu_err_t MPU_Motion(raw_axes_t* accel, raw_axes_t* gyro);

#if defined CONFIG_MPU_AK89xx

mpu_err_t MPU_Heading(raw_axes_t* mag);
mpu_err_t MPU_HeadingEx(int16_t* x, int16_t* y, int16_t* z);
mpu_err_t MPU_MotionEx(raw_axes_t* accel, raw_axes_t* gyro, raw_axes_t* mag);

#endif

mpu_err_t MPU_Sensors(raw_axes_t* accel, raw_axes_t* gyro, int16_t* temp);
mpu_err_t MPU_SensorsEx(sensors_t* sensors, size_t extsens_len /* 0 */);

mpu_err_t MPU_AccelSelfTest(raw_axes_t* regularBias, raw_axes_t* selfTestBias, uint8_t* result);
mpu_err_t MPU_GyroSelfTest(raw_axes_t* regularBias, raw_axes_t* selfTestBias, uint8_t* result);
mpu_err_t MPU_GetBiases(accel_fs_t accelFS, gyro_fs_t gyroFS, raw_axes_t* accelBias, raw_axes_t* gyroBias, bool selftest);

#endif /* end of include guard: _MPU_H_ */
