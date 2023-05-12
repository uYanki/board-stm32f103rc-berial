#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t   width;
    uint8_t         height;
    const uint16_t* data;
} FontDef;

#define ENABLE_FONT_7X10  1
#define ENABLE_FONT_11X18 1
#define ENABLE_FONT_16X26 1

#if ENABLE_FONT_7X10
extern FontDef Font_7x10;
#endif
#if ENABLE_FONT_11X18
extern FontDef Font_11x18;
#endif
#if ENABLE_FONT_16X26
extern FontDef Font_16x26;
#endif

#endif
