#ifndef __BASE_H__
#define __BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "math.h"

/////////// gui

#include "oled/oled.h"

#include "u8g2/u8g2.h"
#include "u8g2/u8x8.h"
#include "u8g2/mui.h"
#include "u8g2/mui_u8g2.h"

// U8G 是第一代库，包含了液晶底层（HAL）、二级显存、逻辑绘图（类似于Windows 的 WinGDI）
// U8G2 是第二代库，在第一代基础上增加了 UTF8 支持，直接内置中文字库（当然FLASH也往死里用）
// U8X8 是第二代库的子集，不带缓存直接调用底层 API，用来加速操作，这个效果类似于大家直接开发的液晶接口。

extern u8g2_t u8g2;

extern uint8_t* buf_ptr;
extern uint16_t buf_len;

static char _str_[16];

static float _offset_x_, _offset_y_;

#define _set_offset_(x, y) _offset_x_ = x, _offset_y_ = y
#define _set_offset_x(x)   _offset_x_ = x
#define _set_offset_y(y)   _offset_y_ = y
#define _reset_offset()    _offset_x_ = _offset_y_ = 0

#define _str_w_(str) u8g2_GetStrWidth(&u8g2, str)  // string width_offset_y_+

#define _draw_str(x, y, str)      u8g2_DrawStr(&u8g2, _offset_x_ + x, _offset_y_ + y, str)
#define _draw_num(x, y, fmt, num) sprintf(_str_, fmt, num), _draw_str(_offset_x_ + x, _offset_y_ + y, _str_)

#define _draw_pixel(x, y)          u8g2_DrawPixel(&u8g2, _offset_x_ + x, _offset_y_ + y);
#define _draw_hline(x, y, w)       u8g2_DrawHLine(&u8g2, _offset_x_ + x, _offset_y_ + y, w)
#define _draw_vline(x, y, h)       u8g2_DrawVLine(&u8g2, _offset_x_ + x, _offset_y_ + y, h)
#define _draw_line(x1, y1, x2, y2) u8g2_DrawLine(&u8g2, _offset_x_ + x1, _offset_y_ + y1, _offset_x_ + x2, _offset_y_ + y2)

#define _draw_rect(x, y, w, h)            u8g2_DrawFrame(&u8g2, _offset_x_ + x, _offset_y_ + y, w, h)
#define _draw_rounded_rect(x, y, w, h, r) u8g2_DrawRFrame(&u8g2, _offset_x_ + x, _offset_y_ + y, w, h, r)

#define _fill_rect(x, y, w, h)            u8g2_DrawBox(&u8g2, _offset_x_ + x, _offset_y_ + y, w, h)
#define _fill_rounded_rect(x, y, w, h, r) u8g2_DrawRBox(&u8g2, _offset_x_ + x, _offset_y_ + y, w, h, r)

#define _draw_circle(x, y, r, md)                   u8g2_DrawCircle(&u8g2, _offset_x_ + x, _offset_y_ + y, r, md);
#define _draw_circle_full(x, y, r)                  u8g2_DrawCircle(&u8g2, _offset_x_ + x, _offset_y_ + y, r, U8G2_DRAW_ALL);
#define _draw_circle_full_at_screen_center(x, y, r) _draw_circle_full(64 + x, 32 + y, r);

#define _clip_set_rect(x1, y1, x2, y2) u8g2_SetClipWindow(&u8g2, x1, y1, x2, y2);
#define _clip_reset()                  u8g2_SetMaxClipWindow(&u8g2)

static void _draw_rotated_rect(float x0 /*xcenter*/, float y0 /*ycenter*/, float w, float h, float rad)
{
    float c = cos(rad), s = sin(rad);
    float ws = w * s, wc = w * c, hs = h * s, hc = h * c;
    u8g2_ClearPolygonXY();
    u8g2_AddPolygonXY(&u8g2, -wc + hs + x0, -ws - hc + y0);
    u8g2_AddPolygonXY(&u8g2, +wc + hs + x0, +ws - hc + y0);
    u8g2_AddPolygonXY(&u8g2, +wc - hs + x0, +ws + hc + y0);
    u8g2_AddPolygonXY(&u8g2, -wc - hs + x0, -ws + hc + y0);
    u8g2_DrawPolygon(&u8g2);
}

#define _set_color(c) u8g2_SetDrawColor(&u8g2, c);

#define _clear()  u8g2_ClearBuffer(&u8g2)
#define _update() u8g2_SendBuffer(&u8g2)

void u8g2_init();
void effect_disappear();

///////////

#include "key/key.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "easing/easing.hpp"
#endif

#endif