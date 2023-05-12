
#include "as5600.h"

uint16_t _rawStartAngle = 0;
uint16_t _zPosition     = 0;
uint16_t _rawEndAngle   = 0;
uint16_t _mPosition     = 0;
uint16_t _maxAngle      = 0;

uint8_t AS5600_ReadByte(uint8_t reg) {
    uint8_t buff = 0;
    HAL_I2C_Mem_Read(&AS5600_I2C, AS5600_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}

uint16_t AS5600_ReadTwoByte(uint8_t reg_high, uint8_t reg_low) {
    return ((uint16_t)AS5600_ReadByte(reg_high) << 8) | AS5600_ReadByte(reg_low);
}

uint8_t AS5600_DetectMagnet(void) {
    uint8_t retVal = AS5600_ReadByte(_stat);
    if (retVal & 0x20) retVal = 1;
    return retVal;
}

// 读取传感器角度值, @return = 400 无磁铁
float AS5600_ReadAngle(void) {
    float data = 0, degress = 0;
    if (AS5600_DetectMagnet()) {
        data = AS5600_ReadTwoByte(_raw_ang_hi, _raw_ang_lo);
        /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
        degress = data * 360 / 4095;
        if (degress < 0) degress = degress + 360.0f;
        return degress;
    }
    return 400;
}
