#include "AXP173.h"

uint8_t AXP173_ReadByte(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, HAL_MAX_DELAY);
    return buff;
}

void AXP173_WriteByte(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

uint16_t AXP173_Read12Bit(uint8_t reg)
{
    uint8_t buff[2];
    HAL_I2C_Mem_Read(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, 2, HAL_MAX_DELAY);
    return (buff[0] << 4) | buff[1];
}

uint16_t AXP173_Read13Bit(uint8_t reg)
{
    uint8_t buff[2];
    HAL_I2C_Mem_Read(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, 2, HAL_MAX_DELAY);
    return (buff[0] << 5) | buff[1];
}

uint16_t AXP173_Read16Bit(uint8_t reg)
{
    uint8_t buff[2];
    HAL_I2C_Mem_Read(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, 2, HAL_MAX_DELAY);
    return (buff[0] << 8) | buff[1];
}

uint16_t AXP173_Read24Bit(uint8_t reg)
{
    uint8_t buff[3];
    HAL_I2C_Mem_Read(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, 3, HAL_MAX_DELAY);
    return (buff[0] << 16) | (buff[1] << 8) | buff[2];
}

uint16_t AXP173_Read32Bit(uint8_t reg)
{
    uint8_t buff[4];
    HAL_I2C_Mem_Read(&AXP173_I2C, AXP173_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, 4, HAL_MAX_DELAY);
    return (buff[0] << 24) | (buff[1] << 16) | (buff[2] << 8) | buff[3];
}

uint8_t AXP173_Init()
{  // check device
    return HAL_I2C_IsDeviceReady(&AXP173_I2C, AXP173_DEV, 5, 0xFFFF) == HAL_OK;
}

uint8_t AXP173_IsACINExist()
{
    return AXP173_ReadByte(0x00) & 0B10000000;
}

uint8_t AXP173_IsACINAvl()
{
    return AXP173_ReadByte(0x00) & 0B01000000;
}

uint8_t AXP173_IsVBUSExist()
{
    return AXP173_ReadByte(0x00) & 0B00100000;
}

uint8_t AXP173_IsVBUSAvl()
{
    return AXP173_ReadByte(0x00) & 0B00010000;
}

/**
 * @brief Get bat current direction
 *
 * @return true Bat charging
 * @return false Bat discharging
 */
uint8_t AXP173_GetBatCurrentDir()
{
    return AXP173_ReadByte(0x00) & 0B00000100;
}

uint8_t AXP173_IsAXP173OverTemp()
{
    return AXP173_ReadByte(0x01) & 0B10000000;
}

/**
 * @brief Get bat charging state
 *
 * @return true Charging
 * @return false Charge finished or not charging
 */
uint8_t AXP173_IsCharging()
{
    return AXP173_ReadByte(0x01) & 0B01000000;
}

uint8_t AXP173_IsBatExist()
{
    return AXP173_ReadByte(0x01) & 0B00100000;
}

uint8_t AXP173_IsChargeCsmaller()
{
    return AXP173_ReadByte(0x01) & 0B00000100;
}

/**
 * @brief Set channels' output enable or disable
 *
 * @param channel Output channel
 * @param state true:Enable, false:Disable
 */
void AXP173_SetOutputEnable(OUTPUT_CHANNEL channel, uint8_t state)
{
    uint8_t buff = AXP173_ReadByte(0x12);
    buff         = state ? (buff | (1U << channel)) : (buff & ~(1U << channel));
    AXP173_WriteByte(0x12, buff);
}

/**
 * @brief Set channels' output voltage
 *
 * @param channel Output channel
 * @param voltage DCDC1 & LDO4: 700~3500(mV), DCDC2: 700~2275(mV), LDO2 & LDO3: 1800~3300{mV}
 */
void AXP173_SetOutputVoltage(OUTPUT_CHANNEL channel, uint16_t voltage)
{
    uint8_t buff = 0;
    switch (channel) {
        case AXP173_OP_DCDC1:
            voltage = (clamp(voltage, 700, 3500) - 700) / 25;
            buff    = AXP173_ReadByte(0x26);
            buff    = (buff & 0B10000000) | (voltage & 0B01111111);
            AXP173_WriteByte(0x26, buff);
            break;
        case AXP173_OP_DCDC2:
            voltage = (clamp(voltage, 700, 2275) - 700) / 25;
            buff    = AXP173_ReadByte(0x23);
            buff    = (buff & 0B11000000) | (voltage & 0B00111111);
            AXP173_WriteByte(0x23, buff);
            break;
        case AXP173_OP_LDO2:
            voltage = (clamp(voltage, 1800, 3300) - 1800) / 100;
            buff    = AXP173_ReadByte(0x28);
            buff    = (buff & 0B00001111) | (voltage << 4);
            AXP173_WriteByte(0x28, buff);
            break;
        case AXP173_OP_LDO3:
            voltage = (clamp(voltage, 1800, 3300) - 1800) / 100;
            buff    = AXP173_ReadByte(0x28);
            buff    = (buff & 0B11110000) | (voltage);
            AXP173_WriteByte(0x28, buff);
            break;
        case AXP173_OP_LDO4:
            voltage = (clamp(voltage, 700, 3500) - 700) / 25;
            buff    = AXP173_ReadByte(0x27);
            buff    = (buff & 0B10000000) | (voltage & 0B01111111);
            AXP173_WriteByte(0x27, buff);
            break;
        default:
            break;
    }
}

void AXP173_PowerOFF()
{
    AXP173_WriteByte(0x32, (AXP173_ReadByte(0x32) | 0B10000000));
}

void AXP173_SetPowerOffTime(POWEROFF_TIME time)
{
    AXP173_WriteByte(0x36, ((AXP173_ReadByte(0x36) & 0B11111100) | time));
}

/**
 * @brief Set charge enable or disable
 *
 * @param state true:Enable, false:Disable
 */
void AXP173_SetChargeEnable(uint8_t state)
{
    if (state)
        AXP173_WriteByte(0x33, (AXP173_ReadByte(0x33) | 0B10000000));
    else
        AXP173_WriteByte(0x33, (AXP173_ReadByte(0x33) & 0B01111111));
}

void AXP173_SetChargeCurrent(CHARGE_CURRENT current)
{
    AXP173_WriteByte(0x33, (AXP173_ReadByte(0x33) & 0B11110000) | current);
}

void AXP173_SetChargeLedFre(CHARGE_LED_FRE frequency)
{
    // 使能通过寄存器控制LED
    // AXP173_WriteReg(0x32, ((AXP173_ReadReg(0x32) & 0B11001111) | ((frequency<<4)|0B00001000)));
    AXP173_WriteByte(0x32, (AXP173_ReadByte(0x32) & 0B11001111) | ((frequency << 4) | 0B00000000));
    AXP173_WriteByte(0x32, (AXP173_ReadByte(0x32) & 0B11110111) | (0B00000000));
}

/**
 * @brief Set ADC channel enable or disable
 *
 * @param channel ADC channel
 * @param state true:Enable, false:Disable
 */
void AXP173_SetADCEnable(ADC_CHANNEL channel, uint8_t state)
{
    uint8_t buff = AXP173_ReadByte(0x82);
    buff         = state ? (buff | (1U << channel)) : (buff & ~(1U << channel));
    AXP173_WriteByte(0x82, buff);
}

void AXP173_SetCoulometer(COULOMETER_CTRL option, uint8_t state)
{
    uint8_t buff = AXP173_ReadByte(0xB8);
    buff         = state ? (buff | (1U << option)) : (buff & ~(1U << option));
    AXP173_WriteByte(0xB8, buff);
}

inline uint32_t AXP173_GetCoulometerChargeData()
{
    return AXP173_Read32Bit(0xB0);
}

inline uint32_t AXP173_GetCoulometerDischargeData()
{
    return AXP173_Read32Bit(0xB4);
}

float AXP173_GetCoulometerData()
{
    uint32_t coin  = AXP173_GetCoulometerChargeData();
    uint32_t coout = AXP173_GetCoulometerDischargeData();
    // data = 65536 * current_LSB * (coin - coout) / 3600 / ADC rate
    return 65536 * 0.5 * (int32_t)(coin - coout) / 3600.0 / 25.0;
}

float AXP173_GetBatVoltage()
{
    float ADCLSB = 1.1 / 1000.0;
    return AXP173_Read12Bit(0x78) * ADCLSB;
}

float AXP173_GetBatCurrent()
{
    float    ADCLSB     = 0.5;
    uint16_t CurrentIn  = AXP173_Read13Bit(0x7A);
    uint16_t CurrentOut = AXP173_Read13Bit(0x7C);
    return (CurrentIn - CurrentOut) * ADCLSB;
}

float AXP173_GetBatLevel()
{
    const float batVoltage    = AXP173_GetBatVoltage();
    const float batPercentage = (batVoltage < 3.248088) ? (0) : (batVoltage - 3.120712) * 100;
    return (batPercentage <= 100) ? batPercentage : 100;
}

float AXP173_GetBatPower()
{
    float    VoltageLSB = 1.1;
    float    CurrentLCS = 0.5;
    uint32_t ReData     = AXP173_Read24Bit(0x70);
    return VoltageLSB * CurrentLCS * ReData / 1000.0;
}

float AXP173_GetVBUSVoltage()
{
    float    ADCLSB = 1.7 / 1000.0;
    uint16_t ReData = AXP173_Read12Bit(0x5A);
    return ReData * ADCLSB;
}

float AXP173_GetVBUSCurrent()
{
    float    ADCLSB = 0.375;
    uint16_t ReData = AXP173_Read12Bit(0x5C);
    return ReData * ADCLSB;
}

float AXP173_GetAXP173Temp()
{
    float       ADCLSB       = 0.1;
    const float OFFSET_DEG_C = -144.7;
    uint16_t    ReData       = AXP173_Read12Bit(0x5E);
    return OFFSET_DEG_C + ReData * ADCLSB;
}

float AXP173_GetTSTemp()
{
    float       ADCLSB       = 0.1;
    const float OFFSET_DEG_C = -144.7;
    uint16_t    ReData       = AXP173_Read12Bit(0x62);
    return OFFSET_DEG_C + ReData * ADCLSB;
}