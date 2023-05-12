#ifndef __HMC5883L_H_
#define __HMC5883L_H_

// https://blog.csdn.net/weixin_44457994/article/details/121591775?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-121591775-blog-52223841.pc_relevant_multi_platform_whitelistv4&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-121591775-blog-52223841.pc_relevant_multi_platform_whitelistv4&utm_relevant_index=2

// 代码与 QMC5883L 不通用

#include "i2c.h"

#define HMC5883L_ADDR_LOW 0x1E

#define HMC5883L_I2C hi2c2
#define HMC5883L_DEV (HMC5883L_ADDR_LOW << 1)

// bit[1:0] xyz是否使用偏压,默认为0正常配置
// bit[4:2] 数据输出速率, 110为最大75HZ 100为15HZ 最小000 0.75HZ
// bit[6:5] 每次采样平均数 11为8次 00为一次
#define HMC5883L_RA_CONFIG_A 0x00
// bit[7:5] 磁场增益 数据越大,增益越小 默认001
#define HMC5883L_RA_CONFIG_B 0x01

/* 模式设置 */
// bit[1:0] 00:连续测量 01:单一测量
#define HMC5883L_RA_MODE 0x02

/* 输出结果 */
#define HMC5883L_RA_DATAX_H 0x03  // X
#define HMC5883L_RA_DATAX_L 0x04
#define HMC5883L_RA_DATAZ_H 0x05  // Z
#define HMC5883L_RA_DATAZ_L 0x06
#define HMC5883L_RA_DATAY_H 0x07  // Y
#define HMC5883L_RA_DATAY_L 0x08

/* 状态寄存器 */
// bit1 数据更新时该位自动锁存, 等待用户读取, 读取到一半的时候防止数据改变
// bit0 数据已经准备好等待读取了, DRDY引脚也能用
#define HMC5883L_RA_STATUS 0x09

/* 识别寄存器 */
// 可用于检测芯片完整性
#define HMC5883L_RA_ID_A 0x0A  // 默认值：0x48
#define HMC5883L_RA_ID_B 0x0B  // 默认值：0x34
#define HMC5883L_RA_ID_C 0x0C  // 默认值：0x33

#define HMC5883L_CRA_AVERAGE_BIT    6
#define HMC5883L_CRA_AVERAGE_LENGTH 2
#define HMC5883L_CRA_RATE_BIT       4
#define HMC5883L_CRA_RATE_LENGTH    3
#define HMC5883L_CRA_BIAS_BIT       1
#define HMC5883L_CRA_BIAS_LENGTH    2

#define HMC5883L_AVERAGING_1 0x00
#define HMC5883L_AVERAGING_2 0x01
#define HMC5883L_AVERAGING_4 0x02
#define HMC5883L_AVERAGING_8 0x03

#define HMC5883L_RATE_0P75 0x00
#define HMC5883L_RATE_1P5  0x01
#define HMC5883L_RATE_3    0x02
#define HMC5883L_RATE_7P5  0x03
#define HMC5883L_RATE_15   0x04
#define HMC5883L_RATE_30   0x05
#define HMC5883L_RATE_75   0x06

#define HMC5883L_BIAS_NORMAL   0x00
#define HMC5883L_BIAS_POSITIVE 0x01
#define HMC5883L_BIAS_NEGATIVE 0x02

#define HMC5883L_CRB_GAIN_BIT    7
#define HMC5883L_CRB_GAIN_LENGTH 3

#define HMC5883L_GAIN_1370 0x00
#define HMC5883L_GAIN_1090 0x01
#define HMC5883L_GAIN_820  0x02
#define HMC5883L_GAIN_660  0x03
#define HMC5883L_GAIN_440  0x04
#define HMC5883L_GAIN_390  0x05
#define HMC5883L_GAIN_330  0x06
#define HMC5883L_GAIN_220  0x07

#define HMC5883L_MODEREG_BIT    1
#define HMC5883L_MODEREG_LENGTH 2

#define HMC5883L_MODE_CONTINUOUS 0x00
#define HMC5883L_MODE_SINGLE     0x01
#define HMC5883L_MODE_IDLE       0x02

#define HMC5883L_STATUS_LOCK_BIT  1
#define HMC5883L_STATUS_READY_BIT 0

void    HMC5883L_Initialize();
uint8_t HMC5883L_TestConnection();

// CONFIG_A register
uint8_t HMC5883L_GetSampleAveraging();
void    HMC5883L_SetSampleAveraging(uint8_t averaging);
uint8_t HMC5883L_GetDataRate();
void    HMC5883L_SetDataRate(uint8_t rate);
uint8_t HMC5883L_GetMeasurementBias();
void    HMC5883L_SetMeasurementBias(uint8_t bias);

// CONFIG_B register
uint8_t HMC5883L_GetGain();
void    HMC5883L_SetGain(uint8_t gain);

// MODE register
uint8_t HMC5883L_GetMode();
void    HMC5883L_SetMode(uint8_t mode);

// DATA* registers
void HMC5883L_GetHeading(int16_t* Mag);
// STATUS register
uint8_t HMC5883L_GetLockStatus();
uint8_t HMC5883L_GetReadyStatus();

void    HMC5883L_WriteBits(uint8_t reg, uint8_t start, uint8_t length, uint8_t data);
void    HMC5883L_WriteBit(uint8_t reg, uint8_t index, uint8_t data);
uint8_t HMC5883L_ReadBits(uint8_t reg, uint8_t start, uint8_t length);

uint8_t HMC5883L_ReadBit(uint8_t reg, uint8_t index);

uint8_t HMC5883L_ReadReg(uint8_t reg);
void    HMC5883L_WriteReg(uint8_t reg, uint8_t buff);
void    HMC5883L_ReadBytes(uint8_t reg, uint8_t* buff, uint16_t size);
#endif