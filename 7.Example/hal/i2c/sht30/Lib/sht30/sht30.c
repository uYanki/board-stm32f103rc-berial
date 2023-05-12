#include "sht30.h"

HAL_StatusTypeDef SHT3x_WriteCmd(uint16_t cmd)
{
    uint8_t buff[2] = {cmd >> 8, cmd & 0xFF};
    return HAL_I2C_Master_Transmit(&SHT3X_I2C, SHT3X_DEV, buff, 2, 0XFFFF);
}

HAL_StatusTypeDef SHT3x_ReadRaw(uint8_t* buff)
{
    SHT3x_WriteCmd(READOUT_FOR_PERIODIC_MODE);
    return HAL_I2C_Master_Receive(&SHT3X_I2C, SHT3X_DEV | 1, buff, 6, 0xFFFF);
    // 6个值：温度(高八位,低八位,CRC位), 湿度(高八位,低八位,CRC位)
}

void SHT3x_Init(void)
{
    SHT3x_WriteCmd(SOFT_RESET_CMD);
    HAL_Delay(20);
    SHT3x_WriteCmd(PERI_MEDIUM_2_CMD);
}

#define CRC8_POLYNOMIAL 0x31

uint8_t SHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{  // CRC校验
    uint8_t crc = 0xFF, bit = 0, byteCtr;
    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) {
        crc ^= (data[byteCtr]);
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc != checksum;  // 值同没问题, 值不同则有问题
}

uint8_t SHT3x_Read(float* temperature, float* humidity)
{
    uint16_t data = 0;
    uint8_t  buff[6];
    SHT3x_ReadRaw(buff);
    if (SHT3x_CheckCrc(buff, 2, buff[2])) return 1;
    data         = ((uint16_t)buff[0] << 8) | buff[1];
    *temperature = -45 + 175 * ((float)data / 65535);
    if (SHT3x_CheckCrc(buff + 3, 2, buff[5])) return 2;
    data      = ((uint16_t)buff[3] << 8) | buff[4];
    *humidity = 100 * ((float)data / 65535);
    return 0;
}