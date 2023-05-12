#include "pca9685.h"

uint8_t PCA9685_ReadReg(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&PCA9685_I2C, PCA9685_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, HAL_MAX_DELAY);
    return buff;
}

void PCA9685_WriteReg(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&PCA9685_I2C, PCA9685_DEV, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

uint16_t pca9685_freq = 0;

void PCA9685_Init()
{
    PCA9685_WriteReg(REG_MODE1, 0x00);  // reset
}

/**
 * @brief 设置占空比 duty=(off-on)/4096
 *
 * @param on 脉冲起始时间 [0,off]
 * @param off 脉冲结束时间 [off,4095] (当on=0时, off等于脉冲长度)
 *
 */
void PCA9685_SetDuty(uint8_t channel, uint16_t on, uint16_t off)
{
    PCA9685_WriteReg(REG_LED0_ON_L + 4 * channel, on & 0xFF);
    PCA9685_WriteReg(REG_LED0_ON_H + 4 * channel, on >> 8);
    PCA9685_WriteReg(REG_LED0_OFF_L + 4 * channel, off & 0xFF);
    PCA9685_WriteReg(REG_LED0_OFF_H + 4 * channel, off >> 8);
}

void PCA9685_SetAllDuty(uint16_t on, uint16_t off)
{
    PCA9685_WriteReg(REG_ALLLED_ON_L, on & 0xFF);
    PCA9685_WriteReg(REG_ALLLED_ON_H, on >> 8);
    PCA9685_WriteReg(REG_ALLLED_OFF_L, off & 0xFF);
    PCA9685_WriteReg(REG_ALLLED_OFF_H, off >> 8);
}

void PCA9685_SetFreq(float freq)
{
    // prescaleval=round(osc_cloc/4096/freq)-1
    // Correct for overshoot in the frequency setting (see issue #11) -> freq *= 0.9 （此处没用）
    pca9685_freq        = freq;
    uint8_t prescaleval = floor((float)25000000 / 4096 / freq + 0.5);
    uint8_t oldmode     = PCA9685_ReadReg(REG_MODE1);
    // setup sleep mode, Low power mode. Oscillator off (bit4: 1-sleep, 0-normal)
    PCA9685_WriteReg(REG_MODE1, (oldmode & 0x7F) | 0x10);
    // set the prescaler
    PCA9685_WriteReg(REG_PRESCALE, prescaleval);
    // setup normal mode (bit4: 1-sleep, 0-normal)
    PCA9685_WriteReg(REG_MODE1, oldmode);
    HAL_Delay(5);
    // PCA9685_WriteReg(REG_MODE1, oldmode | 0xa1);
    PCA9685_WriteReg(REG_MODE1, oldmode | 0x80);
    // This sets the MODE1 register to turn on auto increment.
    // This is why the beginTransmission below was not working.
}

void PCA9685_SetAngle(uint8_t channel, uint16_t angle)
{
    float off  = (float)angle / 180 * 2000 + 500;  // map [0,180] to [500,2500]
    float unit = (float)1000000 / pca9685_freq;    // us
    PCA9685_SetDuty(channel, 0, 4096 * off / unit);
}

void PCA9685_SetAllAngle(uint16_t angle)
{
    float off  = (float)angle / 180 * 2000 + 500;  // map [0,180] to [500,2500]
    float unit = (float)1000000 / pca9685_freq;    // us
    PCA9685_SetAllDuty(0, 4096 * off / unit);
}
