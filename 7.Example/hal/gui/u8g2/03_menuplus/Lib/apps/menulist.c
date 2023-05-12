#include "menulist.h"

// #define FRAMECOUNT 4

// #define CORRECT(v) ((v - (uint8_t)v) > 0.5f)

// menulist_t menulist_initialize(item_menu_t pItems[], uint8_t nSize, uint8_t nDisplyedCount, cbk_t lpfnPainter, cbk_t lpfnHandler)
// {
//     if (nSize == 0) {
//         menulist_t m = {0};
//         return m;
//     }

//     if (nDisplyedCount == 0) nDisplyedCount = 1;

//     uint8_t nWidth  = CONFIG_SCREEN_WIDTH;
//     uint8_t nHeight = CONFIG_SCREEN_HEIGHT;

//     menulist_t m = {
//         .bRepaint = true,

//         .lpfnPainter = lpfnPainter == 0 ? menulist_callback_default_painter : lpfnPainter,
//         .lpfnHandler = lpfnHandler == 0 ? menulist_callback_default_handler : lpfnHandler,

//         .nWidth  = nWidth,
//         .nHeight = nHeight,

//         .pItems = pItems,
//         .nSize  = nSize,

//         .nDisplyedCount = nDisplyedCount,

//         .hLine   = (easing_pos_t)nHeight / nDisplyedCount,
//         .hSlider = (easing_pos_t)nHeight / nSize,

//         .nMaskedIndex   = 0,
//         .nSelectedIndex = 0,

//         .xPadding = 4,
//         .yPadding = 8,  // fontsize ?

//         .xSlider = nWidth - 2,

//         .wMask = easing_create(EASING_MODE_DEFAULT, _easing_calc_OutCubic, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
//         .yMask = easing_create(EASING_MODE_DEFAULT, _easing_calc_OutCubic, 0, FRAMECOUNT, EASING_INTERVAL_NONE),

//         .ySlider      = easing_create(EASING_MODE_DEFAULT, _easing_calc_OutCubic, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
//         .yTitleOffset = easing_create(EASING_MODE_DEFAULT, _easing_calc_Linear, 0, FRAMECOUNT, EASING_INTERVAL_NONE),

//     };

//     m.wMask.nCurrent = _str_w_(pItems[0].title);

//     m.yPadding += (m.hLine - m.yPadding) / 2;

//     return m;
// }

// void menulist_callback_default_handler(menulist_t* p)
// {
//     switch (key_scan()) {
//         case KEY_ID_NONE: break;
//         case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
//         case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
//         default: {
//             println("%s", p->pItems[p->nSelectedIndex].title);
//             break;
//         }
//     }
// }

// void menulist_callback_handler_switch_prev(menulist_t* p)
// {
//     if (p->nSelectedIndex > 0) {
//         // decrease selected index
//         --p->nSelectedIndex;
//         // check if masked index at top
//         if (p->nMaskedIndex > 0) {
//             // decrease masked index
//             --p->nMaskedIndex;
//             // decrease mask ypos
//             easing_start_relative(&p->yMask, -p->hLine);
//         } else {
//             // decrease text yoffset
//             easing_start_relative(&p->yTitleOffset, -p->hLine);
//         }
//         // decrease scroll ypos
//         easing_start_relative(&p->ySlider, -p->hSlider);
//         // change mask width
//         easing_start_absolute(&p->wMask, p->wMask.nCurrent, _str_w_(p->pItems[p->nSelectedIndex].title));
//         // repaint
//         p->bRepaint = true;
//     }
// }

// void menulist_callback_handler_switch_next(menulist_t* p)
// {
//     if (p->nSelectedIndex < p->nSize - 1) {
//         // increase selected index
//         ++p->nSelectedIndex;
//         // check if masked index at bottom
//         if (p->nMaskedIndex < p->nDisplyedCount - 1) {
//             // increase masked index
//             ++p->nMaskedIndex;
//             // increase mask ypos
//             easing_start_relative(&p->yMask, p->hLine);
//         } else {
//             // increase text yoffset
//             easing_start_relative(&p->yTitleOffset, p->hLine);
//         }
//         // increase scroll ypos
//         easing_start_relative(&p->ySlider, p->hSlider);
//         // change mask width
//         easing_start_absolute(&p->wMask, p->wMask.nCurrent, _str_w_(p->pItems[p->nSelectedIndex].title));
//         // repaint
//         p->bRepaint = true;
//     }
// }

// void menulist_callback_painter_update_easing(menulist_t* p)
// {
//     easing_update(&p->wMask);
//     easing_update(&p->yMask);
//     easing_update(&p->yTitleOffset);
//     easing_update(&p->ySlider);

//     p->bRepaint =
//         !(easing_isok(&p->wMask) &&
//           easing_isok(&p->yMask) &&
//           easing_isok(&p->yTitleOffset) &&
//           easing_isok(&p->ySlider));
// }

// void menulist_callback_painter_draw_items(menulist_t* p)
// {
//     // 虚表绘制:
//     uint8_t      index    = easing_curpos(&p->yTitleOffset) / p->hLine;
//     easing_pos_t y_offset = index * p->hLine - easing_curpos(&p->yTitleOffset);
//     while (index < p->nSize && y_offset < p->nHeight) {
//         _draw_str(p->xPadding, y_offset + p->yPadding, p->pItems[index].title);
//         ++index, y_offset += p->hLine;
//     }
// }

// void menulist_callback_painter_draw_scroll(menulist_t* p)
// {
//     _draw_vline(p->xSlider, 0, p->nHeight);  // 轴

//     easing_pos_t y = 0;

//     for (uint8_t i = 0; i < p->nSize; ++i) {  // 刻度
//         _draw_pixel(p->xSlider - 1, y);
//         _draw_pixel(p->xSlider + 1, y);
//         y += p->hSlider;
//     }
//     _draw_pixel(p->xSlider - 1, p->nHeight - 1);
//     _draw_pixel(p->xSlider + 1, p->nHeight - 1);

//     easing_pos_t h = p->hSlider + CORRECT(easing_curpos(&p->ySlider));
//     _draw_vline(p->xSlider - 1, easing_curpos(&p->ySlider), h);  // 滑块
//     _draw_vline(p->xSlider + 1, easing_curpos(&p->ySlider), h);
// }

// void menulist_callback_painter_draw_item_mask(menulist_t* p)
// {
//     _set_color(2);
//     _draw_rounded_rect(0, easing_curpos(&p->yMask), easing_curpos(&p->wMask) + p->xPadding * 2, p->hLine + CORRECT(easing_curpos(&p->yMask)), 1);
//     _set_color(1);
// }

// void menulist_callback_default_painter(menulist_t* p)
// {
//     // easing

//     menulist_callback_painter_update_easing(p);

//     // new frame

//     _clear();

//     // title

//     menulist_callback_painter_draw_items(p);

//     // scroll

//     menulist_callback_painter_draw_scroll(p);

//     // mask

//     menulist_callback_painter_draw_item_mask(p);

//     // update screen

//     _update();
// }
