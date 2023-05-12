#ifndef DFROBOT_DISPLAY_TYPE_H
#define DFROBOT_DISPLAY_TYPE_H

#include "stdint.h"
#include "stdlib.h"

#define _DEBUG 0

#if _DEBUG
#define _DEBUG_PRINT(x)       Serial.print(x)
#define _DEBUG_PRINTVAR(x, y) Serial.print(x, y)
#define _DEBUG_CODE(x)        x
#else
#define _DEBUG_PRINT(x)
#define _DEBUG_PRINTVAR(x, y)
#define _DEBUG_CODE(x)
#endif

#define DISPLAY_ERR_OK         0
#define DISPLAY_ERR            -1
#define DISPLAY_ERR_PARAM      -2
#define DISPLAY_ERR_NOTSUPPORT -3
#define DISPLAY_ERR_MEMOVER    -4

#define DISPLAY_WAR_OUTRANGE 1

#define BEGIN_ERR_OK  0
#define BEGIN_ERR_ERR -1

#define BEGIN_WAR_NOTEST 1

typedef int16_t (*pfCharacterFont_t)(uint8_t* pCh, uint8_t* pBuf, uint8_t* width, uint8_t* height);
typedef int16_t (*pfReadSDFile_t)(const char* filePath, uint32_t lenth, uint8_t* pBuf, uint8_t param);

enum eROTATION {
    eROTATION_0,
    eROTATION_90,
    eROTATION_180,
    eROTATION_270
};

enum eCHARACTER_BYTEWIDTH {
    eCHARACTER_BYTEWIDTH_1,
    eCHARACTER_BYTEWIDTH_2,
    eCHARACTER_BYTEWIDTH_3
};

enum eCHARACTER_HEIGHT {
    eCHARACTER_HEIGHT_8,
    eCHARACTER_HEIGHT_12,
    eCHARACTER_HEIGHT_16
};

enum eINVERT {
    eINVERT_ON,
    eINVERT_OFF
};

enum eSHAPE {
    eSHAPE_RECT,
    eSHAPE_CIRCLE,
    eSHAPE_TRIANGLE,
    eSHAPE_SECTOR,
    eSHAPE_HEART,
    eSHAPE_STAR
};

enum eCOLORFORMAT {
    eCOLORFORMAT_565 = 2,
    eCOLORFORMAT_888 = 3
};

enum eRELATIVITY {
    eRELATIVITY_NO,
    eRELATIVITY_YES
};

#ifdef __cplusplus
extern "C" {
#endif

void dataLoopUpDown_8(uint8_t* pDat);
void color888To565(uint8_t r, uint8_t g, uint8_t b, uint16_t* pColorBuf);

#ifdef __cplusplus
}
#endif

#endif
