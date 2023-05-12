#include "DFRobot_GT30L.h"

static DFRobot_GT30L GT30L;

static uint8_t GT30L_pin_cs = 0;

int16_t GT30L_getCharacter(uint8_t* pCh, uint8_t* pBuf, uint8_t* pWidth, uint8_t* pHeight)
{
    uint16_t var1          = 0;
    uint8_t  unicodeBuf[2] = {0}, GB2312Buf[2] = {0};
    uint32_t addr = 0;
    if (*pCh < 0x20) {
        if (*pCh > 0x06 && *pCh < 0x0e) {
            *pWidth  = 8;
            *pHeight = 16;
            return 1;
        } else {
            return DISPLAY_ERR_PARAM;
        }
        // utf-8 code
    } else if ((*pCh & 0xf0) == 0xe0 && (*(pCh + 1) & 0xc0) == 0x80 && (*(pCh + 2) & 0xc0) == 0x80) {
        var1          = (*pCh & 0x0f) << 12 | (*(pCh + 1) & 0x3f) << 6 | (*(pCh + 2) & 0x3f);
        unicodeBuf[0] = var1 >> 8;
        unicodeBuf[1] = var1;
        GT30L.unicodeToGB2312(unicodeBuf, GB2312Buf);
        addr = GT30L.getGTAddr(GB2312Buf, eCHARACTER_HEIGHT_16);
        GT30L.readBytes(addr, 32, pBuf);
        for (var1 = 0; var1 < 32; var1++) {
            dataLoopUpDown_8(pBuf + var1);
        }
        *pWidth  = 16;
        *pHeight = 16;
        return 3;
        // unicode code
    } else if ((*pCh & 0x80) == 0x80) {
        GB2312Buf[0] = *pCh;
        GB2312Buf[1] = *(pCh + 1);
        addr         = GT30L.getGTAddr(GB2312Buf, eCHARACTER_HEIGHT_16);
        GT30L.readBytes(addr, 32, pBuf);
        for (var1 = 0; var1 < 32; var1++) {
            dataLoopUpDown_8(pBuf + var1);
        }
        *pWidth  = 16;
        *pHeight = 16;
        return 2;
        // char code
    } else {
        addr = GT30L.getGTAddr(pCh, eCHARACTER_HEIGHT_16);
        GT30L.readBytes(addr, 16, pBuf);
        for (var1 = 0; var1 < 16; var1++) {
            dataLoopUpDown_8(pBuf + var1);
        }
        *pWidth  = 8;
        *pHeight = 16;
        return 1;
    }
}

void GT30L_setCSPin(uint8_t pin_cs)
{
    GT30L_pin_cs = pin_cs;
    pinMode(pin_cs, OUTPUT);
    digitalWrite(pin_cs, 1);
}

void DFRobot_GT30L::readBytes(uint32_t addr, uint8_t count, uint8_t* pBuf)
{
    SPI.begin();
    digitalWrite(GT30L_pin_cs, 0);
    SPI.transfer(0x0b);
    SPI.transfer(addr >> 16);
    SPI.transfer(addr >> 8);
    SPI.transfer(addr);
    SPI.transfer(0x00);
    while (count--) {
        *pBuf = SPI.transfer(0x00);
        pBuf++;
    }
    digitalWrite(GT30L_pin_cs, 1);
}

void DFRobot_GT30L::unicodeToGB2312(uint8_t* pUnicode, uint8_t* pGB2312)
{
    uint32_t addr;
    uint8_t  result = 0;
    uint32_t h;
    uint32_t code;
    code = *pUnicode++;
    code = (code << 8) + *pUnicode;
    if (code < 0xa0)
        result = 1;
    else if (code <= 0xf7)
        h = code - 160 - 1281;  //-1441
    else if (code < 0x2c7)
        result = 1;
    else if (code <= 0x2c9)
        h = code - 160 - 1281 - 463;  //-1904
    else if (code < 0x2010)
        result = 1;
    else if (code <= 0x2642)
        h = code - 160 - 463 - 7494 - 333 - 948;  //-9398
    else if (code < 0x3000)
        result = 1;
    else if (code <= 0x3017)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 948;  //-11891
    else if (code < 0x3220)
        result = 1;
    else if (code <= 0x3229)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 428;  //-11571
    else if (code < 0x4e00)
        result = 1;
    else if (code <= 0x9fbf)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126;  //-18589
    else if (code == 0xe76c)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379 - 22699;  //-60278
    else if (code <= 0xe774)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379;  //-37579
    else if (code < 0xff00)
        result = 1;
    else if (code <= 0xff5f)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379 - 6027 + 666;  //-42940
    else if (code < 0xffe0)
        result = 1;
    else if (code < 0xffe5)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379 - 6027 - 128 + 666;  //-43068
    else if (code == 0xffe5)
        h = code - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379 - 6027 - 128 + 812631;  //+768897
    else
        result = 1;
    if (result == 0) {
        addr = UNICODETOGB2312_ADDR + (h << 1);  // +AAA;
        // Get the gb2312 encoding from the address
        readBytes(addr, 2, pGB2312);
    } else {
        pGB2312[0] = 0xa1;
        pGB2312[1] = 0xa1;
    }
}

uint32_t DFRobot_GT30L::getGTAddr(uint8_t* pGB2312, eCHARACTER_HEIGHT type)
{
    uint32_t temp = 0;
    if ((pGB2312[0] & 0x80) != 0x80) {
        if (pGB2312[0] >= ' ')
            temp = pGB2312[0] - ' ';
        if (type == eCHARACTER_HEIGHT_8)
            temp = temp * 8 + ASC0808D2HZ_ADDR;  // 7*8 ascii code
        else if (type == eCHARACTER_HEIGHT_12)
            temp = temp * 12 + ASC0812M2ZF_ADDR;  // 6*12 ascii code
        else if (type == eCHARACTER_HEIGHT_16)
            temp = temp * 16 + GBEX0816ZF_ADDR;  // 8*16 ascii code
    } else {
        if (pGB2312[0] >= 0xA4 && pGB2312[0] <= 0Xa8 && pGB2312[1] >= 0xA1)
            temp = JFLS1516HZ_ADDR;
        else if (pGB2312[0] >= 0xA1 && pGB2312[0] <= 0Xa9 && pGB2312[1] >= 0xA1)
            temp = ((pGB2312[0] - 0xA1) * 94 + (pGB2312[1] - 0xA1)) * 32 + ADDR_CCP;
        else if (pGB2312[0] >= 0xB0 && pGB2312[0] <= 0xF7 && pGB2312[1] >= 0xA1)
            temp = ((pGB2312[0] - 0xB0) * 94 + (pGB2312[1] - 0xA1) + 846) * 32 + JFLS1516HZ_ADDR;
    }
    return temp;
}
