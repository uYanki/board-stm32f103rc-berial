
#include "vl53l0x.h"

uint8_t VL53L0X_DEV = VL53L0X_DEF_I2C_ADDR;

uint8_t  mode;
uint8_t  precision;
uint16_t ambientCount;
uint16_t signalCount;
uint16_t distance, _distance;
uint8_t  status;

void I2C_WriteByte(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(&VL53L0X_I2C, VL53L0X_DEV, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 0xFF);
}
void I2C_WriteBytes(uint8_t reg, uint8_t* src, uint8_t count)
{
    HAL_I2C_Mem_Write(&VL53L0X_I2C, VL53L0X_DEV, reg, I2C_MEMADD_SIZE_8BIT, src, count, 0xFF);
}
uint8_t I2C_ReadByte(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&VL53L0X_I2C, VL53L0X_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}
void I2C_ReadBytes(uint8_t reg, uint8_t* dst, uint8_t count)
{
    HAL_I2C_Mem_Read(&VL53L0X_I2C, VL53L0X_DEV, reg, I2C_MEMADD_SIZE_8BIT, dst, count, 0xFF);
}

void VL53L0X_Init()
{
#ifdef ESD_2V8
    uint8_t data;
    data = I2C_ReadByte(VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV);
    I2C_WriteByte(VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, (data & 0xFE) | 0x01);
#endif
    I2C_WriteByte(0x88, 0x00);
    I2C_WriteByte(0x80, 0x01);
    I2C_WriteByte(0xFF, 0x01);
    I2C_WriteByte(0x00, 0x00);
    I2C_ReadByte(0x91);
    I2C_WriteByte(0x91, 0x3c);
    I2C_WriteByte(0x00, 0x01);
    I2C_WriteByte(0xFF, 0x00);
    I2C_WriteByte(0x80, 0x00);

    printf("Revision ID: 0x%2x\r\n", I2C_ReadByte(VL53L0X_REG_IDENTIFICATION_REVISION_ID));
    printf("Device ID: 0x%2x\r\n", I2C_ReadByte(VL53L0X_REG_IDENTIFICATION_MODEL_ID));
}

void VL53L0X_SetDeviceAddress(uint8_t newDevAddr)
{
    VL53L0X_DEV = (newDevAddr & 0x7F) << 1;
    I2C_WriteByte(VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, VL53L0X_DEV);
}

void VL53L0X_HighPrecisionEnable(uint8_t enable)
{
    I2C_WriteByte(VL53L0X_REG_SYSTEM_RANGE_CONFIG, enable ? 1 : 0);
}

void VL53L0X_ReadData()
{
    uint8_t buff[12];
    I2C_ReadBytes(VL53L0X_REG_RESULT_RANGE_STATUS, buff, 12);
    ambientCount = ((buff[6] & 0xFF) << 8) | (buff[7] & 0xFF);
    signalCount  = ((buff[8] & 0xFF) << 8) | (buff[9] & 0xFF);
    distance     = ((buff[10] & 0xFF) << 8) | (buff[11] & 0xFF);
    status       = ((buff[0] & 0x78) >> 3);
}

void VL53L0X_Start()
{
    uint8_t  Byte;
    uint8_t  StartStopByte = VL53L0X_REG_SYSRANGE_MODE_START_STOP;
    uint32_t LoopNb;

    I2C_WriteByte(0x80, 0x01);
    I2C_WriteByte(0xFF, 0x01);
    I2C_WriteByte(0x00, 0x00);
    I2C_WriteByte(0x91, 0x3c);
    I2C_WriteByte(0x00, 0x01);
    I2C_WriteByte(0xFF, 0x00);
    I2C_WriteByte(0x80, 0x00);

    switch (mode) {  // DeviceMode
        case VL53L0X_DEVICEMODE_SINGLE_RANGING:
            I2C_WriteByte(VL53L0X_REG_SYSRANGE_START, 0x01);
            Byte = StartStopByte;
            /* Wait until start bit has been cleared */
            LoopNb = 0;
            do {
                if (LoopNb > 0) Byte = I2C_ReadByte(VL53L0X_REG_SYSRANGE_START);
                LoopNb = LoopNb + 1;
            } while (((Byte & StartStopByte) == StartStopByte) &&
                     (LoopNb < VL53L0X_DEFAULT_MAX_LOOP));
            break;
        case VL53L0X_DEVICEMODE_CONTINUOUS_RANGING:
            /* Back-to-back mode */
            /* Check if need to apply interrupt settings */
            // VL53L0X_CheckAndLoadInterruptSettings(Dev, 1);中断检查?
            I2C_WriteByte(VL53L0X_REG_SYSRANGE_START, VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK);
            break;
        default:
            /* Selected mode not supported */
            printf("---Selected mode not supported---");
    }
}

void VL53L0X_Stop()
{
    I2C_WriteByte(VL53L0X_REG_SYSRANGE_START, VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT);
    I2C_WriteByte(0xFF, 0x01);
    I2C_WriteByte(0x00, 0x00);
    I2C_WriteByte(0x91, 0x00);
    I2C_WriteByte(0x00, 0x01);
    I2C_WriteByte(0xFF, 0x00);
}

void VL53L0X_SetMode(uint8_t _mode /*0:single,1:continuous*/, uint8_t _precision /*0:high,1:low*/)
{
    mode      = _mode ? 1 : 0;
    precision = _precision ? 1 : 0;
    VL53L0X_HighPrecisionEnable(!precision);
}

uint16_t VL53L0X_GetAmbientCount()
{
    return ambientCount;
}

uint16_t VL53L0X_GetSignalCount()
{
    return signalCount;
}

uint8_t VL53L0X_GetStatus()
{
    return status;
}

float VL53L0X_GetDistance()
{
    (distance == 20) ? (distance = _distance) : (_distance = distance);
    return precision ? distance : (distance / 4.f);
}