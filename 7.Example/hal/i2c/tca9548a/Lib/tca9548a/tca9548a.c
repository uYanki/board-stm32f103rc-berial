
#include "tca9548a.h"

void tca9548a_select(tca9548a_channel_t ch)
{
    i2cdev_send_byte(&tca9548a, (uint8_t)ch);
}

void tca9548a_scan(void)
{
    for (uint8_t i = 0; i < 7; ++i) {
        println("scanning channel %d", i);
        tca9548a_select((tca9548a_channel_t)(1 << i));
        i2cdrv_detector(tca9548a.bus, tca9548a.drv);
    }
}