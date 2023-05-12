#include "lis3dsh.h"

void LIS3DSH_WriteByte(uint8_t mem_addr, uint8_t data)
{
    HAL_I2C_Mem_Write(&LIS3DSH_I2C, LIS3DSH_DEV, mem_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

uint8_t LIS3DSH_ReadByte(uint8_t mem_addr)
{
    uint8_t buff = 0;
    HAL_I2C_Mem_Read(&LIS3DSH_I2C, LIS3DSH_DEV, mem_addr, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}

void LIS3DSH_ReadBytes(uint8_t mem_addr, uint8_t* buff, uint8_t len)
{
    HAL_I2C_Mem_Read(&LIS3DSH_I2C, LIS3DSH_DEV, mem_addr, I2C_MEMADD_SIZE_8BIT, buff, len, 0xFF);
}

void LIS3DSH_Init(void)
{
LIS3DSH_RESET:
    if (LIS3DSH_ReadByte(LIS3DSH_WHO_AM_I_REG) != LIS3DSH_WHO_AM_I) {
        HAL_Delay(2000);
        printf("fail to int lis3dh");
        goto LIS3DSH_RESET;
    }
    LIS3DSH_WriteByte(LIS3DSH_CTRL_REG1, 0x47);  // 50HZ
    LIS3DSH_WriteByte(LIS3DSH_CTRL_REG2, 0x00);
    LIS3DSH_WriteByte(LIS3DSH_CTRL_REG3, 0x00);
    LIS3DSH_WriteByte(LIS3DSH_CTRL_REG4, 0x00);  // ±2g
    LIS3DSH_WriteByte(LIS3DSH_CTRL_REG5, 0x00);
}

int toInt(uint8_t firstbyte, uint8_t secondbyte)
{
    return (firstbyte & 0x80) ? (-(((firstbyte ^ 255) << 8) | (secondbyte ^ 255) + 1)) : (firstbyte << 8 | secondbyte);
}

void LIS3DSH_ReadAccel(float* x, float* y, float* z)
{
    uint8_t buff[6] = {0};
    // https://blog.csdn.net/hxiaohai/article/details/104060560
    // https://www.st.com/zh/mems-and-sensors/lis3dh.html#documentation
    LIS3DSH_ReadBytes(LIS3DSH_OUT_X_L | 0x80, buff, 6);  // 连读
#define divider (float)16380
    *x = toInt(buff[1], buff[0]) * divider / STANDARD_GRAVITY;
    *y = toInt(buff[3], buff[2]) * divider / STANDARD_GRAVITY;
    *z = toInt(buff[5], buff[4]) * divider / STANDARD_GRAVITY;
#if 1
    float pitch = atan2(-*y, *z) * RAD_TO_DEG;
    float roll  = atan2(*x, *z) * RAD_TO_DEG;
    printf("pitch:%f roll:%f\r\n", pitch, roll);
#endif
}