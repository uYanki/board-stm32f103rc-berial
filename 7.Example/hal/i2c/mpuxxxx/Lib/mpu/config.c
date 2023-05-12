#include "config.h"

#if defined CONFIG_MPU_I2C

mpu_err_t MPU_ReadBit(uint8_t reg, uint8_t index, uint8_t* data) {
    uint8_t buff;
    if (MPU_ReadByte(reg, &buff))
        return *data = 0, MPU_ERR;
    *data = (buff >> index) & 0x01;
    return MPU_OK;
}
mpu_err_t MPU_ReadBits(uint8_t reg, uint8_t start, uint8_t length, uint8_t* data) {
    uint8_t buff;
    if (MPU_ReadByte(reg, &buff))
        return *data = 0, MPU_ERR;
    *data = buff & (0xFF >> (8 - length) << start);
    return MPU_OK;
}

mpu_err_t MPU_ReadByte(uint8_t reg, uint8_t* data) {
    return HAL_I2C_Mem_Read(&mpu_bus, mpu_addr, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 0xFF);
}

mpu_err_t MPU_ReadBytes(uint8_t reg, size_t length, uint8_t* data) {
    return HAL_I2C_Mem_Read(&mpu_bus, mpu_addr, reg, I2C_MEMADD_SIZE_8BIT, data, length, 0xFF);
}

mpu_err_t MPU_WriteBit(uint8_t reg, uint8_t index, uint8_t data) {
    uint8_t buff;
    if (MPU_ReadByte(reg, &buff))
        return MPU_ERR;
    index &= 0x07;
    if (data) {
        buff |= (1 << index);
    } else {
        buff &= ~(1 << index);
    }
    return MPU_WriteByte(reg, buff);
}

mpu_err_t MPU_WriteBits(uint8_t reg, uint8_t start, uint8_t length, uint8_t data) {
    uint8_t buff, mask;
    if (MPU_ReadByte(reg, &buff))
        return MPU_ERR;
    mask = 0xFF >> (8 - length) << start;
    buff &= ~mask;
    buff |= mask & data;
    return MPU_WriteByte(reg, buff);
}

mpu_err_t MPU_WriteByte(uint8_t reg, uint8_t data) {
    return HAL_I2C_Mem_Write(&mpu_bus, mpu_addr, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

mpu_err_t MPU_WriteBytes(uint8_t reg, size_t length, uint8_t* data) {
    return HAL_I2C_Mem_Write(&mpu_bus, mpu_addr, reg, I2C_MEMADD_SIZE_8BIT, data, length, 0xFF);
}

#if defined CONFIG_MPU_AK89xx

mpu_err_t MAG_ReadByte(uint8_t addr, uint8_t reg, uint8_t* data) {
    return HAL_I2C_Mem_Read(&mpu_bus, addr, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 0xFF);
}
mpu_err_t MAG_WriteByte(uint8_t addr, uint8_t reg, uint8_t data) {
    return HAL_I2C_Mem_Write(&mpu_bus, mpu_addr, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

#endif

#elif defined CONFIG_MPU_SPI

mpu_err_t MPU_ReadBit(uint8_t reg, uint8_t index, uint8_t* data) {
    uint8_t buff;
    if (MPU_ReadByte(reg, &buff))
        return *data = 0, MPU_ERR;
    *data = (buff >> index) & 0x01;
    return MPU_OK;
}
mpu_err_t MPU_ReadBits(uint8_t reg, uint8_t start, uint8_t length, uint8_t* data) {
    uint8_t buff;
    if (MPU_ReadByte(reg, &buff))
        return *data = 0, MPU_ERR;
    *data = buff & (0xFF >> (8 - length) << start);
    return MPU_OK;
}

mpu_err_t MPU_ReadByte(uint8_t reg, uint8_t* data) {
    MPU_CS_ENABLE();
    reg |= 0x80;
    HAL_SPI_Transmit(&mpu_bus, &reg, 1, 0xFF);
    HAL_SPI_Receive(&mpu_bus, data, 1, 0xFF);
    MPU_CS_DISABLE();
    return MPU_OK;
}

mpu_err_t MPU_ReadBytes(uint8_t reg, size_t length, uint8_t* data) {
    MPU_CS_ENABLE();
    reg |= 0x80;
    HAL_SPI_Transmit(&mpu_bus, &reg, 1, 0xFF);
    HAL_SPI_Receive(&mpu_bus, data, length, 0xFF);
    MPU_CS_DISABLE();
    return MPU_OK;
}

mpu_err_t MPU_WriteBit(uint8_t reg, uint8_t index, uint8_t data) {
    uint8_t buff;
    if (MPU_ReadByte(reg, &buff))
        return MPU_ERR;
    index &= 0x07;
    if (data) {
        buff |= (1 << index);
    } else {
        buff &= ~(1 << index);
    }
    return MPU_WriteByte(reg, buff);
}

mpu_err_t MPU_WriteBits(uint8_t reg, uint8_t start, uint8_t length, uint8_t data) {
    uint8_t buff, mask;
    if (MPU_ReadByte(reg, &buff))
        return MPU_ERR;
    mask = 0xFF >> (8 - length) << start;
    buff &= ~mask;
    buff |= mask & data;
    return MPU_WriteByte(reg, buff);
}

mpu_err_t MPU_WriteByte(uint8_t reg, uint8_t data) {
    MPU_CS_ENABLE();
    HAL_SPI_Transmit(&mpu_bus, &reg, 1, 0xFF);
    HAL_SPI_Transmit(&mpu_bus, &data, 1, 0xFF);
    MPU_CS_DISABLE();
    return MPU_OK;
}

mpu_err_t MPU_WriteBytes(uint8_t reg, size_t length, uint8_t* data) {
    MPU_CS_ENABLE();
    HAL_SPI_Transmit(&mpu_bus, &reg, 1, 0xFF);
    HAL_SPI_Transmit(&mpu_bus, data, length, 0xFF);
    MPU_CS_DISABLE();
    return MPU_OK;
}

#endif