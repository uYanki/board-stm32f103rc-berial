#ifndef __VL53L0X_H_
#define __VL53L0X_H_

#include "i2c.h"
#include "usart.h"

#define VL53L0X_I2C hi2c2  // <----

extern uint8_t VL53L0X_DEV;

#define VL53L0X_DEF_I2C_ADDR (0x29 << 1)  // 默认地址(固定的)

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID           0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID        0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD     0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD   0x70
#define VL53L0X_REG_SYSRANGE_START                    0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS           0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS               0x14
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS          0x8a
#define VL53L0X_REG_SYSTEM_RANGE_CONFIG               0x09
#define VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV 0x89
#define VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT          0x00
#define VL53L0X_REG_SYSRANGE_MODE_START_STOP          0x01
#define VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK          0x02
#define VL53L0X_REG_SYSRANGE_MODE_TIMED               0x04

#define VL53L0X_DEVICEMODE_SINGLE_RANGING           0
#define VL53L0X_DEVICEMODE_CONTINUOUS_RANGING       1
#define VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING 3
#define VL53L0X_DEFAULT_MAX_LOOP                    200

#define ESD_2V8 1

// precision
// - High precision(0.25mm)
// - Low precision(1mm)

void     VL53L0X_Init();
void     VL53L0X_SetMode(uint8_t mode, uint8_t precision);
float    VL53L0X_GetDistance();
uint16_t VL53L0X_GetAmbientCount();
uint16_t VL53L0X_GetSignalCount();
uint8_t  VL53L0X_GetStatus();
void     VL53L0X_ReadData();
void     VL53L0X_SetDeviceAddress(uint8_t newDevAddr);
void     VL53L0X_HighPrecisionEnable(uint8_t enable);

#endif
