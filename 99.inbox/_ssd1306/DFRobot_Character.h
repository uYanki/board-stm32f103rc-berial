#ifndef DFROBOT_CHARACTER_H
#define DFROBOT_CHARACTER_H

#include "stdint.h"
#include "stdlib.h"
#include "DFRobot_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t table_character_6_8[][6];

int16_t        DFRobot_Character_getCharacter(uint8_t* pCh, uint8_t* pBuf, uint8_t* pWidth, uint8_t* pHeight);

#ifdef __cplusplus
}
#endif

#endif


