/******************************************************************************/
/** 模块名称：扩展外设驱动                                                   **/
/** 文件名称：fm24xxxfunction.c                                              **/
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

#include "stddef.h"
#include "fm24xxx.h"

static uint16_t devAddMask[FM24Number] = {0xFC, 0xF0, 0xFE, 0xFC, 0xF0, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xFE};

/*从FM24XXX读取单个字节,从随机地址读取*/
uint8_t ReadByteFromFM24xxx(FM24ObjectType* fram, uint32_t regAddress)
{
    uint8_t  wData[2];
    uint16_t wSize = 0;
    uint8_t  rData;
    uint8_t  temp;

    if (fram->memAddLength == FM248BitMemAdd) {
        wData[wSize++] = (uint8_t)regAddress;
        temp           = (uint8_t)(regAddress >> 8);
    } else {
        wData[wSize++] = (uint8_t)(regAddress >> 8);
        wData[wSize++] = (uint8_t)regAddress;
        temp           = (uint8_t)(regAddress >> 16);
    }
    temp             = (temp & (~(devAddMask[fram->mode] >> 1))) << 1;
    fram->devAddress = (fram->devAddress & devAddMask[fram->mode]) | temp;

    fram->Read(fram, wData, wSize, &rData, 1);

    return rData;
}

/*向FM24XXX写入单个字节*/
void WriteByteToFM24xxx(FM24ObjectType* fram, uint32_t regAddress, uint8_t data)
{
    uint8_t  temp;
    uint8_t  tData[3];
    uint16_t tSize = 0;

    if (fram->memAddLength == FM248BitMemAdd) {
        tData[tSize++] = (uint8_t)(regAddress & 0xFF);
        temp           = (uint8_t)(regAddress >> 8);
    } else {
        tData[tSize++] = (uint8_t)(regAddress >> 8);
        tData[tSize++] = (uint8_t)(regAddress & 0xFF);
        temp           = (uint8_t)(regAddress >> 16);
    }
    temp             = (temp & (~(devAddMask[fram->mode] >> 1))) << 1;
    fram->devAddress = (fram->devAddress & devAddMask[fram->mode]) | temp;

    tData[tSize++] = data;

    fram->WP(FM24WP_Disable);
    fram->Write(fram, tData, tSize);
    fram->WP(FM24WP_Enable);
}

/*从FM24XXX读取多个字节，从指定地址最多到所在页的结尾*/
void ReadBytesFromFM24xxx(FM24ObjectType* fram, uint32_t regAddress, uint8_t* rData, uint16_t rSize)
{
    uint8_t  temp;
    uint8_t  wData[2];
    uint16_t wSize = 0;

    if (fram->memAddLength == FM248BitMemAdd) {
        wData[wSize++] = (uint8_t)regAddress;
        temp           = (uint8_t)(regAddress >> 8);
    } else {
        wData[wSize++] = (uint8_t)(regAddress >> 8);
        wData[wSize++] = (uint8_t)regAddress;
        temp           = (uint8_t)(regAddress >> 16);
    }
    temp             = (temp & (~(devAddMask[fram->mode] >> 1))) << 1;
    fram->devAddress = (fram->devAddress & devAddMask[fram->mode]) | temp;

    fram->Read(fram, wData, wSize, rData, rSize);
}

/*向FM24XXX写入多个字节，从指定地址最多到所在页的结尾*/
void WriteBytesToFM24xxx(FM24ObjectType* fram, uint32_t regAddress, uint8_t* wData, uint16_t wSize)
{
    uint8_t  temp;
    uint8_t  tData[128];
    uint16_t tSize = 0;

    if (fram->memAddLength == FM248BitMemAdd) {
        tData[tSize++] = (uint8_t)(regAddress & 0xFF);
        temp           = (uint8_t)(regAddress >> 8);
    } else {
        tData[tSize++] = (uint8_t)(regAddress >> 8);
        tData[tSize++] = (uint8_t)(regAddress & 0xFF);
        temp           = (uint8_t)(regAddress >> 16);
    }
    temp             = (temp & (~(devAddMask[fram->mode] >> 1))) << 1;
    fram->devAddress = (fram->devAddress & devAddMask[fram->mode]) | temp;

    for (int i = 0; i < wSize; i++) {
        tData[tSize++] = wData[i];
    }

    fram->WP(FM24WP_Disable);
    fram->Write(fram, tData, tSize);
    fram->WP(FM24WP_Enable);
}

/*FM24XXX对象进入休眠模式*/
void FM24xxxEnterSleepMode(FM24ObjectType* fram)
{
    uint8_t wData[2];

    wData[0]         = fram->devAddress;
    fram->devAddress = 0xF8;
    wData[1]         = 0x86;

    fram->Write(fram, wData, 2);

    fram->devAddress = wData[0];
}

/*读取FM24XXX对象ID*/
uint32_t ReadFM24xxxDeviceID(FM24ObjectType* fram)
{
    uint8_t  wData[2];
    uint8_t  rData[3];
    uint32_t deviceID = 0;
    wData[0]          = fram->devAddress;
    fram->devAddress  = 0xF8;

    fram->Read(fram, wData, 1, rData, 3);

    fram->devAddress = wData[0];

    deviceID = rData[0];
    deviceID = (deviceID << 8) + rData[1];
    deviceID = (deviceID << 8) + rData[2];

    return deviceID;
}

/*FM24XXX对象初始化*/
void Fm24cxxInitialization(FM24ObjectType* fram,        // FM24xxx对象实体
                           uint8_t         devAddress,  // FM24xxx设备地址
                           FM24ModeType    mode,        // FM24xxx设备类型
                           Fm24WP          wp,          // FM24xxx写保护
                           Fm24Read        read,        // 读FM24xxx对象操作指针
                           Fm24Write       write,       // 写FM24xxx对象操作指针
                           Fm24Delayms     delayms      // 延时操作指针
)
{
    if ((fram == NULL) || (read == NULL) || (write == NULL) || (delayms == NULL)) {
        return;
    }
    fram->Read    = read;
    fram->Write   = write;
    fram->Delayms = delayms;

    if ((0xA0 <= devAddress) && (devAddress <= 0xAE)) {
        fram->devAddress = devAddress;
    } else if ((0x50 <= devAddress) && (devAddress <= 0x57)) {
        fram->devAddress = (devAddress << 1);
    } else {
        fram->devAddress = 0x00;
    }

    if (mode >= FM24Number) {
        return;
    }
    fram->mode = mode;

    if ((mode < FM24CL64B) && (mode != FM24C64B)) {
        fram->memAddLength = FM248BitMemAdd;
    } else {
        fram->memAddLength = FM2416BitMemAdd;
    }

    fram->WP = wp;
}

/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/
