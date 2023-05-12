#ifndef __MENULIST_H__
#define __MENULIST_H__

// 图标菜单

#include "base.h"

typedef struct {
    uint8_t     ID;
    const char* title;
    void*       arg;
} item_menu_t;

typedef struct {
    bool bRepaint;

    // callback
    cbk_t lpfnPainter;
    cbk_t lpfnHandler;

    // nSize
    uint16_t nWidth;
    uint16_t nHeight;

    // menus
    item_menu_t* pItems;
    uint8_t      nSize;

    // config
    uint8_t      nDisplyedCount;  // number of items can be displayed per page
    easing_pos_t hLine;           // height of item / line ( nHeight / nDisplyedCount )
    easing_pos_t hSlider;         // height of scroll ( nHeight / nSize)

    // index
    uint8_t nMaskedIndex;    // [0, min(nDisplyedCount,nSize)), index of the masked item in page
    uint8_t nSelectedIndex;  // [0, nSize), current selected item index in all

    // pos & easing
    uint8_t      xPadding;
    easing_pos_t yPadding;
    easing_t     wMask;
    easing_t     yMask;
    uint8_t      xSlider;
    easing_t     ySlider;
    easing_t     yTitleOffset;

} menulist_t;

menulist_t menulist_initialize(
    // items
    item_menu_t pItems[],
    uint8_t     nSize,
    // show
    uint8_t nDisplyedCount,  // default = 1
    // cbk
    cbk_t lpfnPainter,  // default = menulist_callback_default_painter
    cbk_t lpfnHandler   // default = menulist_callback_default_handler
);

// lpfnPainter
void menulist_callback_default_painter(menulist_t* p);

void menulist_callback_painter_update_easing(menulist_t* p);
void menulist_callback_painter_draw_items(menulist_t* p);
void menulist_callback_painter_draw_scroll(menulist_t* p);
void menulist_callback_painter_draw_item_mask(menulist_t* p);

// lpfnHandler
void menulist_callback_default_handler(menulist_t* p);

void menulist_callback_handler_switch_prev(menulist_t* p);
void menulist_callback_handler_switch_next(menulist_t* p);

#endif