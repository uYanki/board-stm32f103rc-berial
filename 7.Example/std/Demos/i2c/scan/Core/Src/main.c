#include "board.h"

#include "i2cdev/i2cdev.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    struct hard_i2c hwi2c_bus1;
    struct hard_i2c hwi2c_bus2;

    board_init();

    hwi2c_bus1.I2C     = I2C1;
    hwi2c_bus1.RemapIO = true;
    hwi2c_bus1.Speed   = 100000;
    hwi2c_bus1.Timeout = 0xFF;

    hwi2c_bus2.I2C     = I2C2;
    hwi2c_bus2.RemapIO = false;
    hwi2c_bus2.Speed   = 100000;
    hwi2c_bus2.Timeout = 0xFF;

    hwi2c_drv.init(&hwi2c_bus1);
    hwi2c_drv.init(&hwi2c_bus2);

    while (1) {
        i2cdrv_detector(&hwi2c_bus1, &hwi2c_drv);
        i2cdrv_detector(&hwi2c_bus2, &hwi2c_drv);
        delay_ms(2000);
    }
}
