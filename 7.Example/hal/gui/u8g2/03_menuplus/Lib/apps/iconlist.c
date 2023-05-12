// #include "iconlist.h"

// #define FRAMECOUNT 6

// iconlist_t iconlist_initialize(item_icon_t pItems[], uint8_t nSize, cbk_t lpfnPainter, cbk_t lpfnHandler)
// {
//     if (nSize == 0) {
//         iconlist_t m = {0};
//         return m;
//     }

//     uint8_t nWidth  = CONFIG_SCREEN_WIDTH;
//     uint8_t nHeight = CONFIG_SCREEN_HEIGHT;

//     iconlist_t m = {
//         .bRepaint = true,

//         .lpfnPainter = lpfnPainter == 0 ? iconlist_callback_default_painter : lpfnPainter,
//         .lpfnHandler = lpfnHandler == 0 ? iconlist_callback_default_handler : lpfnHandler,

//         .nWidth  = nWidth,
//         .nHeight = nHeight,

//         .pItems = pItems,
//         .nSize  = nSize,

//         .hTitle = nHeight / 4,

//         .nSelectedIndex = 0,

//         .nIconSpace = 48,

//         .xIconOffset        = easing_create(EASING_MODE_DEFAULT, _easing_calc_OutCubic, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
//         .yTitleOffset       = easing_create(EASING_MODE_DEFAULT, _easing_calc_Linear, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
//         .xIconShakingOffset = easing_create(EASING_TIMES_SINGLE | EASING_DIR_BACKANDFORTH, _easing_calc_InOutCirc, 0, 4, EASING_INTERVAL_NONE),
//         .yIconUpDownOffset  = easing_create(EASING_MODE_NTIMES(2) | EASING_DIR_BACKANDFORTH, _easing_calc_OutQuad, 0, 4, EASING_INTERVAL_NONE),

//     };

//     return m;
// }

// void iconlist_callback_default_handler(iconlist_t* p)
// {
//     switch (key_scan()) {
//         case KEY_ID_NONE: break;
//         case KEY_ID_PREV: iconlist_callback_handler_switch_prev(p); break;
//         case KEY_ID_NEXT: iconlist_callback_handler_switch_next(p); break;
//         default: {
//             println("%s", p->pItems[p->nSelectedIndex].title);
//             iconlist_callback_handler_shake_selected(p);
//             break;
//         }
//     }
// }

// void iconlist_callback_handler_switch_prev(iconlist_t* p)
// {
//     if (p->nSelectedIndex > 0) {
//         // decrease selected index
//         --p->nSelectedIndex;
//         // decrease offset
//         easing_start_relative(&p->xIconOffset, -p->nIconSpace);
//         easing_start_relative(&p->yTitleOffset, -p->hTitle);
//         // stop animation
//         easing_stop(&p->xIconShakingOffset, 0);
//         easing_stop(&p->yIconUpDownOffset, 0);
//         p->nShakingTimes = 0;
//         // repaint
//         p->bRepaint = true;
//     }
// }

// void iconlist_callback_handler_switch_next(iconlist_t* p)
// {
//     if (p->nSelectedIndex < p->nSize - 1) {
//         // increase selected index
//         ++p->nSelectedIndex;
//         // increase offset
//         easing_start_relative(&p->xIconOffset, p->nIconSpace);
//         easing_start_relative(&p->yTitleOffset, p->hTitle);
//         // stop animation
//         easing_stop(&p->xIconShakingOffset, 0);
//         easing_stop(&p->yIconUpDownOffset, 0);
//         p->nShakingTimes = 0;
//         // repaint
//         p->bRepaint = true;
//     }
// }

// void iconlist_callback_handler_shake_selected(iconlist_t* p)
// {
//     p->nShakingTimes = 3;
//     p->bRepaint      = true;
// }

// void iconlist_callback_handler_updown_selected(iconlist_t* p)
// {
//     easing_start_absolute(&p->yIconUpDownOffset, 0, -p->nIconSpace / 8);
//     p->bRepaint = true;
// }

// void iconlist_callback_default_painter(iconlist_t* p)
// {
//     // easing

//     easing_update(&p->yTitleOffset);
//     easing_update(&p->xIconOffset);
//     easing_update(&p->xIconShakingOffset);
//     easing_update(&p->yIconUpDownOffset);

//     if (p->nShakingTimes > 0 && easing_isok(&p->xIconShakingOffset)) {
//         switch (p->nShakingTimes % 2) {
//             case 0: easing_start_absolute(&p->xIconShakingOffset, 0, p->nIconSpace / 8); break;
//             case 1: easing_start_absolute(&p->xIconShakingOffset, 0, -p->nIconSpace / 8); break;
//         }
//         --p->nShakingTimes;
//     }

//     p->bRepaint =
//         !(easing_isok(&p->yTitleOffset) &&
//           easing_isok(&p->xIconOffset) &&
//           easing_isok(&p->xIconShakingOffset) &&
//           easing_isok(&p->yIconUpDownOffset));

//     // new frame

//     _clear();

//     // draw

//     uint8_t index = easing_curpos(&p->xIconOffset) / p->nIconSpace;
//     if (index > 0) --index;
//     int8_t offset = index * p->nIconSpace - easing_curpos(&p->xIconOffset);
//     while (index < p->nSize && offset < p->nWidth) {
//         // icon
//         if (index == p->nSelectedIndex) {
//             u8g2_DrawXBMP(&u8g2, 46 + p->xIconShakingOffset.nCurrent + offset, p->yIconUpDownOffset.nCurrent + (p->nHeight - p->nIconSpace) / 2, p->pItems[index].nHeight, p->pItems[index].nWidth, p->pItems[index].buff);
//         } else {
//             u8g2_DrawXBMP(&u8g2, 46 + offset, (p->nHeight - p->nIconSpace) / 2 + 5, p->pItems[index].nHeight, p->pItems[index].nWidth, p->pItems[index].buff);
//         }
//         // title
//         u8g2_SetClipWindow(&u8g2, 0, 48, p->nWidth, p->nHeight);
//         _draw_str((p->nWidth - _str_w_(p->pItems[index].title)) / 2, 62 - p->yTitleOffset.nCurrent + index * 16, p->pItems[index].title);
//         u8g2_SetMaxClipWindow(&u8g2);

//         ++index, offset += p->nIconSpace;
//     }

//     // update screen

//     _update();
// }
