#ifndef __BASE_H__
#define __BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/////////// gui

#include "oled/oled.h"

#include "u8g2/u8g2.h"
#include "u8g2/u8x8.h"
#include "u8g2/mui.h"
#include "u8g2/mui_u8g2.h"

extern u8g2_t u8g2;

extern uint8_t* buf_ptr;
extern uint16_t buf_len;

static char _str_[16];

#define _str_w_(str) u8g2_GetStrWidth(&u8g2, str)  // string width

#define _draw_str(x, y, str)      u8g2_DrawStr(&u8g2, x, y, str)
#define _draw_num(x, y, fmt, num) sprintf(_str_, fmt, num), _draw_str(x, y, _str_)

#define _draw_pixel(x, y)          u8g2_DrawPixel(&u8g2, x, y);
#define _draw_hline(x, y, w)       u8g2_DrawHLine(&u8g2, x, y, w)
#define _draw_vline(x, y, h)       u8g2_DrawVLine(&u8g2, x, y, h)
#define _draw_line(x1, y1, x2, y2) u8g2_DrawLine(&u8g2, x1, y1, x2, y2)

#define _draw_rect(x, y, w, h)            u8g2_DrawBox(&u8g2, x, y, w, h)
#define _draw_rounded_rect(x, y, w, h, r) u8g2_DrawRBox(&u8g2, x, y, w, h, r)

#define _fiil_rect(x, y, w, h)            u8g2_DrawFrame(&u8g2, x, y, w, h)
#define _fiil_rounded_rect(x, y, w, h, r) u8g2_DrawRFrame(&u8g2, x, y, w, h, r)

#define _draw_circle(x, y, r, md)                   u8g2_DrawCircle(&u8g2, x, y, r, md);
#define _draw_circle_full(x, y, r)                  u8g2_DrawCircle(&u8g2, x, y, r, U8G2_DRAW_ALL);
#define _draw_circle_full_at_screen_center(x, y, r) _draw_circle_full(64 + x, 32 + y, r);

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
#include "easing/easing.h"

typedef void (*cbk_t)(void*);

// 派生类+虚函数

typedef struct {
    bool bRepaint;
    // callback
    cbk_t lpfnPainter;
    cbk_t lpfnHandler;
} page_t;

#define page_new(lpfnPainter, lpfnHandler) \
    {                                      \
        true, lpfnPainter, lpfnHandler     \
    }

#ifdef __cplusplus
}
#endif

#endif