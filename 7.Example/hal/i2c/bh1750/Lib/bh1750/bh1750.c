#include "bh1750.h"

static uint8_t BH1750_MTReg;  // 灵敏度倍率
static uint8_t BH1750_Mode;   // 测量模式

void BH1750_WriteCmd(uint8_t cmd) { HAL_I2C_Master_Transmit(&BH1750_I2C, BH1750_DEV, &cmd, 1, 0xFF); }

// 读取原始的测量数据 ,结果未转换.
// 注：在初始化完成后/连续测量模式下，需间隔 180ms 才能读到正确数据。
uint16_t BH1750_ReadData(void)
{
    uint8_t buff[2];
    HAL_I2C_Master_Receive(&BH1750_I2C, BH1750_DEV | 1, buff, 2, 0xFF);
    return (buff[0] << 8) | buff[1];
}

void BH1750_PowerOn(void) { BH1750_WriteCmd(BH1750_POWER_ON); }     // 上电
void BH1750_PowerOff(void) { BH1750_WriteCmd(BH1750_POWER_DOWN); }  // 断电

void BH1750_Reset(void) { BH1750_WriteCmd(BH1750_MODULE_RESET); }  // 数据寄存器复位

void BH1750_Init(void)
{
    BH1750_PowerOff();
    HAL_Delay(100);
    BH1750_PowerOn();
    BH1750_ChageMode(2);           // 高分辨率连续测量
    BH1750_AdjustSensitivity(69);  // 芯片缺省灵敏度倍率
}

// 设置量程倍率
void BH1750_AdjustSensitivity(uint8_t sen /* [31,254], 值越大 灵敏度越高*/)
{
    if (sen <= 31) {
        sen = 31;
    } else if (sen > 254) {
        sen = 254;
    }
    BH1750_MTReg = sen;
    BH1750_WriteCmd(0x40 + (BH1750_MTReg >> 5));   /* 更改高3bit */
    BH1750_WriteCmd(0x60 + (BH1750_MTReg & 0x1F)); /* 更改低5bit */
    /*　更改量程范围后，需要重新发送命令设置测量模式　*/
    BH1750_ChageMode(BH1750_Mode);
}

// 设置测量分辨率
void BH1750_ChageMode(uint8_t mode /*1~3*/)
{
    switch (BH1750_Mode = mode) {
        case 1: BH1750_WriteCmd(BH1750_CONTINUE_H_MODE); break;   // 分辨率 1 lux
        case 2: BH1750_WriteCmd(BH1750_CONTINUE_H_MODE2); break;  // 分辨率 0.5 lux
        case 3: BH1750_WriteCmd(BH1750_CONTINUE_L_MODE); break;   // 分辨率 4 lux
    }
}

// 读取光强度,单位[Lux]
float BH1750_GetLux(void)
{
    uint16_t data;
    float    lux;
    data = BH1750_ReadData();
    /* 光强度 = 16位寄存器值 / 1.2  * (69 / X) */
    // lux = data * (((float)1 / 1.2) * ((float)69 / BH1750_MTReg ));
    lux = (float)(data * 5 * 69) / (6 * BH1750_MTReg);
    if (BH1750_Mode == 2) lux /= 2;  // 高分辨率测量模式2
    return lux;
}