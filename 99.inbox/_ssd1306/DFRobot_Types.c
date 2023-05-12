#include "DFRobot_Types.h"

void dataLoopUpDown_8(uint8_t* pDat)
{
    uint8_t i    = 0;
    uint8_t var1 = *pDat;
    *pDat        = 0;
    for (i = 0; i < 8; i++) {
        if (var1 & (0x80 >> i)) {
            *pDat |= 0x01 << i;
        }
    }
}

void color888To565(uint8_t r, uint8_t g, uint8_t b, uint16_t* pColorBuf)
{
    *pColorBuf = (((uint16_t)r & 0xf8) << 11) | (((uint16_t)g & 0xfc) << 5) | (((uint16_t)b & 0xf8) >> 3);
}
