
/**
 * @file mpu/log.h
 * @brief MPU Log System
 *
 * @attention
 *  This header is intended to be used ONLY inside the library itself
 *  Do not include this header in your application.
 * */

#ifndef _MPU_LOG_H_
#define _MPU_LOG_H_

#include <stdint.h>
#include <stdio.h>

// Note: declare TAG before include this header
// Note: include only in .c files from this library

#define MPU_LOGEMSG(msg, format, ...) printf("%s()-> %s" format "\r\n", __FUNCTION__, msg, ##__VA_ARGS__)  // Error
#define MPU_LOGWMSG(msg, format, ...) printf("%s()-> %s" format "\r\n", __FUNCTION__, msg, ##__VA_ARGS__)  // Warning
#define MPU_LOGIMSG(msg, format, ...) printf("%s()-> %s" format "\r\n", __FUNCTION__, msg, ##__VA_ARGS__)  // Info
#define MPU_LOGDMSG(msg, format, ...) printf("%s()-> %s" format "\r\n", __FUNCTION__, msg, ##__VA_ARGS__)  // Debug
#define MPU_LOGVMSG(msg, format, ...) printf("%s()-> %s" format "\r\n", __FUNCTION__, msg, ##__VA_ARGS__)  // Verbose

#define MPU_LOGE(format, ...) MPU_LOGEMSG("", format, ##__VA_ARGS__)
#define MPU_LOGW(format, ...) MPU_LOGWMSG("", format, ##__VA_ARGS__)
#define MPU_LOGI(format, ...) MPU_LOGIMSG("", format, ##__VA_ARGS__)
#define MPU_LOGD(format, ...) MPU_LOGDMSG("", format, ##__VA_ARGS__)
#define MPU_LOGV(format, ...) MPU_LOGVMSG("", format, ##__VA_ARGS__)

#define MSG_INVALID_ARG           "Invalid Argument"
#define MSG_INVALID_STATE         "Invalid State"
#define MSG_INVALID_LENGTH        "Invalid length"
#define MSG_INVALID_FIFO_RATE     "Invalid FIFO rate"
#define MSG_INVALID_SAMPLE_RATE   "Invalid Sample rate"
#define MSG_INVALID_TAP_THRESH    "Invalid Tap threshold"
#define MSG_DMP_LOAD_FAIL         "Failed to load DMP firmware"
#define MSG_DMP_NOT_LOADED        "DMP firmware has not been loaded"
#define MSG_UNKNOWN_DMP_CFG_STATE "Unknown DMP config state"
#define MSG_NO_AXIS_PASSED        "No Axis passed"
#define MSG_BANK_BOUNDARIES       "Bank boundaries overpass"
#define MSG_FIFO_CORRUPTION       "FIFO Corruption. Quaternion data outside of the acceptable threshold"
#define MSG_AUX_I2C_DISABLED      "Auxiliary I2C is disabled"
#define MSG_AUX_I2C_SLAVE_NACK    "Auxiliary I2C Slave NACK"
#define MSG_AUX_I2C_LOST_ARB      "Auxiliary I2C Master loose abitraion of the bus"
#define MSG_COMPASS_DISABLED      "Compass is disabled"
#define MSG_NOT_SUPPORTED         "Not supported"
#define MSG_TIMEOUT               "Timeout"
#define MSG_EMPTY                 ""

#endif /* end of include guard: _MPU_LOG_H_ */
