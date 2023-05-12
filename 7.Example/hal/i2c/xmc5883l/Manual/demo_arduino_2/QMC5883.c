

/* 包含头文件 -----------------------------------------------------------------*/
#include "i2c.h"
#include "math.h"
#include "matrix.h"
#include "time.h"

#include "uart3.h"

/* 私有宏定义 -----------------------------------------------------------------*/
#define MAT_N 10
#define QMC5883_ADDR 0x1e
#define fabs(x) (x < 0 ? -x : x)
/* 私有变量 -------------------------------------------------------------------*/
int16_t MagnetRawAd[3];
int16_t X_Buff[CALIBTATE_DATA_NUM], Y_Buff[CALIBTATE_DATA_NUM],
    Z_Buff[CALIBTATE_DATA_NUM], CalibrateCnt;
/* 私有函数 -------------------------------------------------------------------*/
/******************************************************************************
 * @函数名称：void MPU6050_Init(void)
 * @函数描述：MPU6050传感器初始化
 * @输入参数：None
 * @输出参数：None
 *******************************************************************************/
uint8_t QMC5883_ReadReg(uint8_t Reg) {
    u8 RegVal = 0;
    I2C_ReadByte(&RegVal, 1, Reg, QMC5883_ADDR);
    return RegVal;
}
/******************************************************************************
 * @函数名称：void MPU6050_Init(void)
 * @函数描述：MPU6050传感器初始化
 * @输入参数：None
 * @输出参数：None
 *******************************************************************************/
void QMC5883_WriteReg(uint8_t Val, uint8_t Reg) {
    I2C_WriteByte(Val, Reg, QMC5883_ADDR);
}
/******************************************************************************
 * @函数名称：void QMC5883_Init(void)
 * @函数描述：程序入口
 * @输入参数：None
 * @输出参数：None
 *******************************************************************************/
uint8_t QMC5883_InitConfig(void) {
    uint8_t Temp;

    QMC5883_WriteReg(0x01, 0x0B);
    QMC5883_WriteReg(0x40, 0x20);
    QMC5883_WriteReg(0x01, 0x21);
    QMC5883_WriteReg(0x0D, 0x09); /****OSR=512,RNG=+/-2G,ODR=200Hz,MODE= continuous*******/
    while (!TIME_CheckTimeMs(TIME_DELAY, 2))
        ;  /// DELAY 2ms
    Temp = QMC5883_ReadReg(0x09);
    while (Temp != 0x0D) return 1;

    return 0;
}
/******************************************************************************
 * @函数名称：void QMC5883_GetData(float *Magnet)
 * @函数描述：获取地磁数据
 * @输入参数：地磁buff
 * @输出参数：None
 *******************************************************************************/
void QMC5883_GetData(float* Magnet) {
    uint8_t Buff[6], i;
    uint8_t Temp;

    Temp = QMC5883_ReadReg(0x06);
    if (Temp & 0x06) {
        // LOG("output data OVL or ERROR!")
        return;
    } else if (Temp & 0x00) {
        // LOG("output data not ready!")
        return;
    }

    I2C_ReadByte(Buff, 6, 0x00, QMC5883_ADDR);
    MagnetRawAd[0] = ((int16_t)Buff[1] << 8) | Buff[0];
    MagnetRawAd[1] = ((int16_t)Buff[3] << 8) | Buff[2];
    MagnetRawAd[2] = ((int16_t)Buff[5] << 8) | Buff[4];

    for (i = 0; i < 3; i++) {
        Magnet[i] = (float)MagnetRawAd[i] / 120.f;  //单位uT
    }
}

/* 文件结束 --------------------------------------------------------------------*/
