
#include "MAX30100.h"

void MAX30100_WriteByte(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&MAX30100_I2C, MAX30100_DEV, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

uint8_t MAX30100_ReadByte(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&MAX30100_I2C, MAX30100_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}

void MAX30100_ReadBytes(uint8_t reg, uint8_t* buff, uint8_t len)
{
    HAL_I2C_Mem_Read(&MAX30100_I2C, MAX30100_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, len, 0xFF);
}

////////////////////////////////////////////////////////////////////////

uint16_t rawIRValue, rawRedValue;

void MAX30100_Init()
{
    MAX30100_SetMode(DEFAULT_MODE);
    // MAX30100_SetMode(MAX30100_MODE_SPO2_HR);
    MAX30100_SetLedsPulseWidth(DEFAULT_PULSE_WIDTH);
    MAX30100_SetSamplingRate(DEFAULT_SAMPLING_RATE);
    MAX30100_SetLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
    // MAX30100_SetLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_27_1MA);
    MAX30100_SetHighresModeEnabled(1);
}

void MAX30100_SetMode(Mode mode)
{
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100_SetLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
    uint8_t previous = MAX30100_ReadByte(MAX30100_REG_SPO2_CONFIGURATION);
    MAX30100_WriteByte(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

void MAX30100_SetSamplingRate(SamplingRate samplingRate)
{
    uint8_t previous = MAX30100_ReadByte(MAX30100_REG_SPO2_CONFIGURATION);
    MAX30100_WriteByte(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

void MAX30100_SetLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
    MAX30100_WriteByte(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

void MAX30100_SetHighresModeEnabled(uint8_t enable)
{
    uint8_t previous = MAX30100_ReadByte(MAX30100_REG_SPO2_CONFIGURATION);
    MAX30100_WriteByte(MAX30100_REG_SPO2_CONFIGURATION, enable ? (previous | MAX30100_SPC_SPO2_HI_RES_EN) : (previous & ~MAX30100_SPC_SPO2_HI_RES_EN));
}

void MAX30100_ReadFIFO(void)
{
    uint8_t buff[4];
    MAX30100_ReadBytes(MAX30100_REG_FIFO_DATA, buff, 4);
    rawIRValue  = (buff[0] << 8) | buff[1];
    rawRedValue = (buff[2] << 8) | buff[3];
}

void MAX30100_ResetFIFO()
{
    MAX30100_WriteByte(MAX30100_REG_FIFO_WRITE_POINTER, 0);
    MAX30100_WriteByte(MAX30100_REG_FIFO_READ_POINTER, 0);
    MAX30100_WriteByte(MAX30100_REG_FIFO_OVERFLOW_COUNTER, 0);
}

uint8_t MAX30100_GetPartId()
{
    return MAX30100_ReadByte(0xff);
}

void MAX30100_StartTemperatureSampling()
{
    uint8_t config = MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) | MAX30100_MC_TEMP_EN;
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, config);
}

uint8_t MAX30100_IsTemperatureReady()
{
    return !(MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) & MAX30100_MC_TEMP_EN);
}

float MAX30100_RetrieveTemperature()
{
    int8_t tempInteger = MAX30100_ReadByte(MAX30100_REG_TEMPERATURE_DATA_INT);
    float  tempFrac    = MAX30100_ReadByte(MAX30100_REG_TEMPERATURE_DATA_FRAC);
    return tempFrac * 0.0625 + tempInteger;
}

void MAX30100_Shutdown()
{
    uint8_t config = MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) | MAX30100_MC_SHDN;
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, config);
}

void MAX30100_Resume()
{
    uint8_t config = MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) & ~MAX30100_MC_SHDN;
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, config);
}