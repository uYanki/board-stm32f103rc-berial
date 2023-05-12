#ifndef DFROBOT_GT30L_H
#define DFROBOT_GT30L_H

#include <Arduino.h>
#include <SPI.h>
#include "DFRobot_Types.h"

#define USE_GT30L

#define ADDR_CCP ((uint32_t)0x22242c)  // character chinese punctuation

#define ASC0808D2HZ_ADDR 0x66c0
#define ASC0812M2ZF_ADDR 0x66d40
#define GBEX0816ZF_ADDR  0x27BFAA  // 8*16 ASCII character
#define ZF1112B2ZF_ADDR  0x3cf80
#define HZ1112B2HZ_ADDR  (0x3cf80 + 376 * 24)
#define CUTS1516ZF_ADDR  0x00
#define JFLS1516HZ_ADDR  0x21E72C  // 16*16 chinese character

#define ASCII0507ZF_ADDR     245696
#define ARIAL_16B0_ADDR      246464
#define ARIAL_12B0_ADDR      422720
#define SPAC1616_ADDR        425264
#define GB2312TOUNICODE_ADDR 12032
#define UNICODETOGB2312_ADDR 0x267B06  // 267B06  425328

class DFRobot_GT30L {
public:
    void     unicodeToGB2312(uint8_t* pUnicode, uint8_t* pGB2312);
    uint32_t getGTAddr(uint8_t* pGB2312, eCHARACTER_HEIGHT type);
    void     readBytes(uint32_t addr, uint8_t count, uint8_t* pBuf);
};

int16_t GT30L_getCharacter(uint8_t* pCh, uint8_t* pBuf, uint8_t* pWidth, uint8_t* pHeight);
void    GT30L_setCSPin(uint8_t pin_cs);

#endif
