#include "board.h"

#include "mpu6050/mpu6050.h"

#define TAG       "main"
#define DBG_LVL   SDK_DEBUG

#define USE_SWI2C 0

int main(void)
{
    struct soft_i2c swi2c_bus;
    struct hard_i2c hwi2c_bus;

    struct i2c_cli      mpu;
    struct mpu6050_data data;

    board_init();

#if USE_SWI2C

    swi2c_bus.RCC_APB2 = RCC_APB2Periph_GPIOB;
    swi2c_bus.SCL_Port = GPIOB;
    swi2c_bus.SCL_Pin  = GPIO_Pin_8;
    swi2c_bus.SDA_Port = GPIOB;
    swi2c_bus.SDA_Pin  = GPIO_Pin_9;
    swi2c_bus.Interval = 6;

    mpu.bus = &swi2c_bus;
    mpu.drv = &swi2c_drv;

    swi2c_drv.init(&swi2c_bus);
    while (i2cdrv_detector(&swi2c_bus, &swi2c_drv) == 0) {
        delay_ms(2000);
    }

    UNUSED(hwi2c_bus);

#else

    hwi2c_bus.I2C     = I2C1;
    hwi2c_bus.RemapIO = true;
    hwi2c_bus.Speed   = 100000;
    hwi2c_bus.Timeout = 0xFF;

    mpu.bus = &hwi2c_bus;
    mpu.drv = &hwi2c_drv;

    hwi2c_drv.init(&hwi2c_bus);
    while (i2cdrv_detector(&hwi2c_bus, &hwi2c_drv) == 0) {
        delay_ms(2000);
    }

    UNUSED(swi2c_bus);

#endif

    mpu.dev = 0x68 << 1;
    mpu.ops = I2C_DEV_7BIT | I2C_REG_8BIT;

    mpu6050_init(&mpu, &data);

    while (1) {
        mpu6050_read(&mpu);
        delay_ms(50);
    }
}
