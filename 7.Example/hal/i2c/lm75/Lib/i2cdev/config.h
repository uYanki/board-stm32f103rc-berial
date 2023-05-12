#ifndef __I2C_DEV_CFG_H__
#define __I2C_DEV_CFG_H__

#include "utils.h"

#define CONFIG_USING_I2C 1

#if CONFIG_USING_I2C

#define CONFIG_USING_SOFTWARE_I2C 0  // software i2c
#define CONFIG_USING_HARDWARE_I2C 1  // hardware i2c

#endif

#endif