/******************************************************************************/
/** 模块名称：扩展外设驱动                                                   **/
/** 文件名称：fm24xxxfunction.h                                              **/
/** 版    本：V1.0.0                                                         **/
/** 简    介：用于实现线制串行FRAM的操作，FM24XXX系列FRAM包括4K、16K、64K、  **/
/**           128K、256K、512K、1M等多种容量，不同容量其采用的设备地址位、寄 **/
/**           存器地址位存在差异每一页所包含的字节数也不一样。具体配置如下： **/
/**  +----------+-------------+--------------------+---------------+--------+**/
/**  |   型号   |  容量结构   |     设备地址位     |   寄存器地址  |每页字节|**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24C04B  |512x8(4K)    |1|0|1|0|A2|A1|P0|R/W|9位(1个寄存器) | 16字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24C16B  |2048x8(16K)  |1|0|1|0|P2|P1|P0|R/W|11位(1个寄存器)| 16字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24C64B  |8192x8(64K)  |1|0|1|0|A2|A1|A0|R/W|13位(2个寄存器)| 32字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24CL04B |512x8(4K)    |1|0|1|0|A2|A1|P0|R/W|9位(1个寄存器) | 16字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24CL16B |2048x8(16K)  |1|0|1|0|P2|P1|P0|R/W|11位(1个寄存器)| 16字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24CL64B |8192x8(64K)  |1|0|1|0|A2|A1|A0|R/W|13位(2个寄存器)| 32字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24V01A  |16384x8(128K)|1|0|1|0|A2|A1|A0|R/W|14位(2个寄存器)| 64字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24V02A  |32768x8(256K)|1|0|1|0|A2|A1|A0|R/W|15位(2个寄存器)| 64字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24V05   |65536x8(512K)|1|0|1|0|A2|A1|A0|R/W|16位(2个寄存器)| 128字节|**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24V10   |131072x8(1M) |1|0|1|0|A2|A1|P0|R/W|17位(2个寄存器)| 256字节|**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**  |FM24W256  |32768x8(256K)|1|0|1|0|A2|A1|A0|R/W|15位(2个寄存器)| 64字节 |**/
/**  +----------+-------------+-+-+-+-+--+--+--+---+---------------+--------+**/
/**           有些容量的设备地址段有些位用于寄存器地址（P）寻址，设备地址的可**/
/**           设定位数（A）决定了同一总线上所挂载的器件数量：                **/
/**               3位用于设备地址，0位用于内存寻址：可挂载8个；              **/
/**               2位用于设备地址，1位用于内存寻址：可挂载4个；              **/
/**               1位用于设备地址，2位用于内存寻址：可挂载2个；              **/
/**               0位用于设备地址，3位用于内存寻址：可挂载1个；              **/
/**--------------------------------------------------------------------------**/
/** 修改记录：                                                               **/
/**     版本      日期              作者              说明                   **/
/**     V1.0.0  2019-07-17          木南              创建文件               **/
/**                                                                          **/
/******************************************************************************/

#ifndef __FM24XXXFUNCTION_H
#define __FM24XXXFUNCTION_H

#include "stdint.h"

/*定义FM24XXX类型枚举*/
typedef enum FM24Mode {
    FM24C04B,
    FM24C16B,
    FM24C64B,
    FM24CL04B,
    FM24CL16B,
    FM24CL64B,
    FM24V01A,
    FM24V02A,
    FM24V05,
    FM24V10,
    FM24W256,
    FM24Number
} FM24ModeType;

/*定义存储器寄存器地址字节枚举*/
typedef enum Fm24MemAddLength {
    FM248BitMemAdd,
    FM2416BitMemAdd
} FM24MemAddLengthType;

/*定义写保护枚举*/
typedef enum FM24WP {
    FM24WP_Enable,
    FM24WP_Disable
} FM24WPType;

/*定义FM24XXX对象类型*/
typedef struct FM24Object {
    uint8_t              devAddress;                                                                        // FM24xxx设备地址
    FM24ModeType         mode;                                                                              // FM24xxx设备类型
    FM24MemAddLengthType memAddLength;                                                                      // 存储器地址长度
    void (*WP)(FM24WPType wp);                                                                              // 写保护操作
    void (*Read)(struct FM24Object* fram, uint8_t* wData, uint16_t wSize, uint8_t* rData, uint16_t rSize);  // 读数据操作指针
    void (*Write)(struct FM24Object* fram, uint8_t* wData, uint16_t wSize);                                 // 写数据操作指针
    void (*Delayms)(volatile uint32_t nTime);                                                               // 延时操作指针
} FM24ObjectType;

// 写保护操作
typedef void (*Fm24WP)(FM24WPType wp);
/* 定义读数据操作函数指针类型 */
typedef void (*Fm24Read)(struct FM24Object* fram, uint8_t* wData, uint16_t wSize, uint8_t* rData, uint16_t rSize);
/* 定义写数据操作函数指针类型 */
typedef void (*Fm24Write)(struct FM24Object* fram, uint8_t* wData, uint16_t wSize);
/* 定义延时操作函数指针类型 */
typedef void (*Fm24Delayms)(volatile uint32_t nTime);

/*从FM24XXX读取单个字节,从随机地址读取*/
uint8_t ReadByteFromFM24xxx(FM24ObjectType* fram, uint32_t regAddress);

/*向FM24XXX写入单个字节*/
void WriteByteToFM24xxx(FM24ObjectType* fram, uint32_t regAddress, uint8_t data);

/*从FM24XXX读取多个字节，从指定地址最多到所在页的结尾*/
void ReadBytesFromFM24xxx(FM24ObjectType* fram, uint32_t regAddress, uint8_t* rData, uint16_t rSize);

/*向FM24XXX写入多个字节，从指定地址最多到所在页的结尾*/
void WriteBytesToFM24xxx(FM24ObjectType* fram, uint32_t regAddress, uint8_t* wData, uint16_t wSize);

/*FM24C对象初始化*/
void Fm24cxxInitialization(FM24ObjectType* fram,        // FM24xxx对象实体
                           uint8_t         devAddress,  // FM24xxx设备地址
                           FM24ModeType    mode,        // FM24xxx设备类型
                           Fm24WP          wp,          // FM24xxx写保护
                           Fm24Read        read,        // 读FM24xxx对象操作指针
                           Fm24Write       write,       // 写FM24xxx对象操作指针
                           Fm24Delayms     delayms      // 延时操作指针
);

#endif
/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/
