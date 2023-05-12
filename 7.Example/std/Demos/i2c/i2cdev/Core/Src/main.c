#include "board.h"

#include "i2cdev/i2cdev.h"

#define TAG       "main"
#define DBG_LVL   SDK_DEBUG

#define USE_SWI2C 0

int main(void)
{
    uint8_t reg;

    struct i2c_cli bme280;

#if USE_SWI2C

    struct soft_i2c swi2c_bus;

    swi2c_bus.RCC_APB2 = RCC_APB2Periph_GPIOB;
    swi2c_bus.SCL_Port = GPIOB;
    swi2c_bus.SCL_Pin  = GPIO_Pin_10;
    swi2c_bus.SDA_Port = GPIOB;
    swi2c_bus.SDA_Pin  = GPIO_Pin_11;
    swi2c_bus.Interval = 6;

    bme280.bus         = &swi2c_bus;
    bme280.drv         = &swi2c_drv;

#else

    struct hard_i2c hwi2c_bus1;
    struct hard_i2c hwi2c_bus2;

    hwi2c_bus1.I2C     = I2C1;
    hwi2c_bus1.RemapIO = true;
    hwi2c_bus1.Speed   = 400000;
    hwi2c_bus1.Timeout = 0xFF;
    hwi2c_bus1.OwnAddr = 0x19 << 1;

    hwi2c_bus2.I2C     = I2C2;
    hwi2c_bus2.RemapIO = false;
    hwi2c_bus2.Speed   = 400000;
    hwi2c_bus2.Timeout = 0xFF;
    hwi2c_bus2.OwnAddr = 0x18 << 1;

    bme280.bus         = &hwi2c_bus2;
    bme280.drv         = &hwi2c_drv;

#endif

    bme280.dev = 0x76 << 1;
    bme280.ops = I2C_DEV_7BIT | I2C_REG_8BIT;

    board_init();

#if USE_SWI2C

    swi2c_drv.init(&swi2c_bus);
    i2cdrv_detector(&swi2c_bus, &swi2c_drv);

#else

    hwi2c_drv.init(&hwi2c_bus1);
		i2cdrv_detector(&hwi2c_bus1, &hwi2c_drv);
		
    hwi2c_drv.init(&hwi2c_bus2);
    i2cdrv_detector(&hwi2c_bus2, &hwi2c_drv);
		
    // 可将 I2C1 和 I2C2 相连, 并进行互相扫描，先扫描的能获取另1个I2C的地址, 但后扫描的不能获取另1个I2C的地址（未知错误?）

#endif

    i2cdev_viewer(&bme280, 0xD0, 0xD2, UINT_FORMAT_HEX);

    reg = 0xF5;

    i2cdev_write_byte(&bme280, reg, 0xA5);  // 0b10100101
    print_binary(i2cdev_get_byte(&bme280, reg, 0x00));

    i2cdev_write_bit(&bme280, reg, 3, 1);
    println("\n[%d]", i2cdev_get_bit(&bme280, reg, 3, 0));  // 1
    print_binary(i2cdev_get_byte(&bme280, reg, 0x00));     // 0b10101101

    i2cdev_write_bits(&bme280, reg, 3, 2, 2);
    println("\n[%d]", i2cdev_get_bits(&bme280, reg, 3, 2, 0));  // 2
    print_binary(i2cdev_get_byte(&bme280, reg, 0x00));         // 0b10110101

    while (1) {}

}
