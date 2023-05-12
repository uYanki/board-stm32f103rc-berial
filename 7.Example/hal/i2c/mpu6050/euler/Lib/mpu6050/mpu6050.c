#include "mpu6050.h"

void MPU6050_WriteByte(uint8_t mem_addr, uint8_t data)
{
    HAL_I2C_Mem_Write(&MPU6050_I2C, MPU6050_DEV, mem_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

uint8_t MPU6050_ReadByte(uint8_t mem_addr)
{
    uint8_t buff = 0;
    HAL_I2C_Mem_Read(&MPU6050_I2C, MPU6050_DEV, mem_addr, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}

void MPU6050_ReadBytes(uint8_t mem_addr, uint8_t* buff, uint8_t len)
{
    HAL_I2C_Mem_Read(&MPU6050_I2C, MPU6050_DEV, mem_addr, I2C_MEMADD_SIZE_8BIT, buff, len, 0xFF);
}

void MPU6050_Init(void)
{
MPU6050_RESET:
    MPU6050_WriteByte(MPU6050_PWR_MGMT_1, 0x80);  // 复位
    HAL_Delay(100);
    MPU6050_WriteByte(MPU6050_PWR_MGMT_1, 0x00);  // 唤醒
    if (MPU6050_ReadByte(MPU6050_WHO_AM_I) != 0x68) {
        printf("fail to init mpu6050\r\n");
        HAL_Delay(1000);
        goto MPU6050_RESET;
    }
    MPU6050_WriteByte(MPU6050_SMPLRT_DIV, 0x00);         // 采样率分频:不分频
    MPU6050_WriteByte(MPU6050_CONFIG, 0x00);             // 低通滤波器:256Hz~260Hz
    MPU6050_WriteByte(MPU6050_GYRO_CONFIG, 0x00 << 3);   // 陀螺仪量程:±250°
    MPU6050_WriteByte(MPU6050_ACCEL_CONFIG, 0x00 << 3);  // 加速度计量程:±2g
    // set other params
    MPU6050_WriteByte(MPU6050_FIFO_EN, 0x00);     // disable fifo
    MPU6050_WriteByte(MPU6050_INT_ENABLE, 0x00);  // disable interrupt
    MPU6050_WriteByte(MPU6050_USER_CTRL, 0x00);   // disable i2c master
    MPU6050_WriteByte(MPU6050_PWR_MGMT_2, 0x00);  // enable accel & gyro
}

int toInt(uint8_t firstbyte, uint8_t secondbyte)
{
    return (firstbyte & 0x80) ? (-(((firstbyte ^ 255) << 8) | (secondbyte ^ 255) + 1)) : (firstbyte << 8 | secondbyte);
}

void MPU6050_Read(MPU6050_DATA* buff)
{
    uint8_t data[14] = {0};

    MPU6050_ReadBytes(MPU6050_ACCEL_XOUT, data, 14);

    buff->ax = toInt(data[0], data[1]) / MPU6050_ACCEL_SENSITIVE;
    buff->ay = toInt(data[2], data[3]) / MPU6050_ACCEL_SENSITIVE;
    buff->az = toInt(data[4], data[5]) / MPU6050_ACCEL_SENSITIVE;

    buff->temp = toInt(data[6], data[7]) / MPU6050_TEMP_SENSITIVE + MPU6050_TEMP_OFFSET;

    buff->gx = toInt(data[8], data[9]) / MPU6050_GYRO_SENSITIVE;
    buff->gy = toInt(data[10], data[11]) / MPU6050_GYRO_SENSITIVE;
    buff->gz = toInt(data[12], data[13]) / MPU6050_GYRO_SENSITIVE;

#if 0  // temp
    printf("temp:%f\r\n", buff->temp);
#endif
#if 0  // accle,gyro
    printf("ax:%f\t,ay:%f\t,az:%f\r\n", buff->ax, buff->ay, buff->az);
    printf("gx:%f\t,gy:%f\t,gz:%f\r\n", buff->gx, buff->gy, buff->gz);
#endif
#if 1  // angle
    printf("pitch:%f\t,roll:%f\r\n", atan2(buff->ax, buff->az) * RAD_TO_DEG, atan2(buff->ay, buff->az) * RAD_TO_DEG);
#endif
}
