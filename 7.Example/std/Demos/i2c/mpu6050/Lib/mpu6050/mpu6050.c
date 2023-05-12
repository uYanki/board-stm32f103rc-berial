#include "mpu6050.h"

void mpu6050_init(i2c_cli_t cli, mpu6050_data_t dat)
{
reset:
    i2cdev_write_byte(cli, MPU6050_PWR_MGMT_1, 0x80);  // 复位
    delay_ms(100);
    i2cdev_write_byte(cli, MPU6050_PWR_MGMT_1, 0x00);  // 唤醒
    if (i2cdev_get_byte(cli, MPU6050_WHO_AM_I, 0x00) != 0x68) {
        println("fail to init mpu6050");
        delay_ms(1000);
        goto reset;
    }
    i2cdev_write_byte(cli, MPU6050_SMPLRT_DIV, 0x00);         // 采样率分频:不分频
    i2cdev_write_byte(cli, MPU6050_CONFIG, 0x00);             // 低通滤波器:256Hz~260Hz
    i2cdev_write_byte(cli, MPU6050_GYRO_CONFIG, 0x00 << 3);   // 陀螺仪量程:±250°
    i2cdev_write_byte(cli, MPU6050_ACCEL_CONFIG, 0x00 << 3);  // 加速度计量程:±2g
    // set other params
    i2cdev_write_byte(cli, MPU6050_FIFO_EN, 0x00);     // disable fifo
    i2cdev_write_byte(cli, MPU6050_INT_ENABLE, 0x00);  // disable interrupt
    i2cdev_write_byte(cli, MPU6050_USER_CTRL, 0x00);   // disable i2c master
    i2cdev_write_byte(cli, MPU6050_PWR_MGMT_2, 0x00);  // enable accel & gyro

    cli->dat = dat;
}

int toInt(uint8_t firstbyte, uint8_t secondbyte)
{
    return (firstbyte & 0x80) ? (-(((firstbyte ^ 255) << 8) | (secondbyte ^ 255) + 1)) : (firstbyte << 8 | secondbyte);
}

bool mpu6050_read(i2c_cli_t cli)
{
    mpu6050_data_t mpu = (mpu6050_data_t)cli->dat;

    uint8_t data[14] = {0};

    if (i2cdev_read_block(cli, MPU6050_ACCEL_XOUT, data, 14)) {
        mpu->ax = toInt(data[0], data[1]) / MPU6050_ACCEL_SENSITIVE;
        mpu->ay = toInt(data[2], data[3]) / MPU6050_ACCEL_SENSITIVE;
        mpu->az = toInt(data[4], data[5]) / MPU6050_ACCEL_SENSITIVE;

        mpu->temp = toInt(data[6], data[7]) / MPU6050_TEMP_SENSITIVE + MPU6050_TEMP_OFFSET;

        mpu->gx = toInt(data[8], data[9]) / MPU6050_GYRO_SENSITIVE;
        mpu->gy = toInt(data[10], data[11]) / MPU6050_GYRO_SENSITIVE;
        mpu->gz = toInt(data[12], data[13]) / MPU6050_GYRO_SENSITIVE;

#if 0  // temp
    println("temp:%f", mpu->temp);
#endif
#if 0  // accle,gyro
    println("ax:%f\t,ay:%f\t,az:%f", mpu->ax, mpu->ay, mpu->az);
    println("gx:%f\t,gy:%f\t,gz:%f", mpu->gx, mpu->gy, mpu->gz);
#endif
#if 1  // angle
        println("pitch:%f\t,roll:%f", atan2(mpu->ax, mpu->az) * RAD_TO_DEG, atan2(mpu->ay, mpu->az) * RAD_TO_DEG);
#endif

        return true;
    }

    return false;
}
