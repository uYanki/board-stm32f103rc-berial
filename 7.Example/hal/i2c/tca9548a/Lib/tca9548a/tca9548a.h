#ifndef __TCA9584A_H__
#define __TCA9584A_H__

#include "i2cdev/i2cdev.h"
#include <stdbool.h>

typedef enum {                         //  A0  |  A1  | A2
    TCA9548A_ADDRESS_0 = 0x70 | 0x00,  // LOW  | LOW  | LOW
    TCA9548A_ADDRESS_1 = 0x70 | 0x01,  // HIGH | LOW  | LOW
    TCA9548A_ADDRESS_2 = 0x70 | 0x02,  // LOW  | HIGH | LOW
    TCA9548A_ADDRESS_3 = 0x70 | 0x03,  // HIGH | HIGH | LOW
    TCA9548A_ADDRESS_4 = 0x70 | 0x04,  // LOW  | LOW  | HIGH
    TCA9548A_ADDRESS_5 = 0x70 | 0x05,  // HIGH | LOW  | HIGH
    TCA9548A_ADDRESS_6 = 0x70 | 0x06,  // LOW  | HIGH | HIGH
    TCA9548A_ADDRESS_7 = 0x70 | 0x07,  // HIGH | HIGH | HIGH
} tca9584a_channel_t;

static struct i2c_cli tca9548a = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = TCA9548A_ADDRESS_0 << 1,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

//////////////////////////////////////////// types

typedef enum {
    TCA9548A_CHANNEL_0 = 1 << 0,
    TCA9548A_CHANNEL_1 = 1 << 1,
    TCA9548A_CHANNEL_2 = 1 << 2,
    TCA9548A_CHANNEL_3 = 1 << 3,
    TCA9548A_CHANNEL_4 = 1 << 4,
    TCA9548A_CHANNEL_5 = 1 << 5,
    TCA9548A_CHANNEL_6 = 1 << 6,
    TCA9548A_CHANNEL_7 = 1 << 7,
} tca9548a_channel_t;

//////////////////////////////////////////// function

void tca9548a_select(tca9548a_channel_t ch);
void tca9548a_scan(void);

#endif
