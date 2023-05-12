#ifndef __ICONLIST_H__
#define __ICONLIST_H__

#include "res/images.h"

// 垂直滚动菜单

#include "base.h"

typedef struct {
    uint8_t     ID;
    const char* title;
    uint8_t     nWidth;
    uint8_t     nHeight;
    uint8_t*    buff;
} item_icon_t;

typedef struct {
    bool bRepaint;

    // callback
    cbk_t lpfnPainter;
    cbk_t lpfnHandler;

    // size
    uint16_t nWidth;
    uint16_t nHeight;

    // menus
    item_icon_t* pItems;
    uint8_t      nSize;

    // index
    uint8_t nSelectedIndex;  // selected index

    // pos & easing
    easing_pos_t hTitle;  // 标题最大高度

    easing_t yTitleOffset;  // 标题垂直偏移
    easing_t xIconOffset;   // 图标水平便宜

    uint8_t  nIconSpace;          // 图标间距
    uint8_t  nShakingTimes;       // 抖动次数
    easing_t xIconShakingOffset;  // 左右偏移
    easing_t yIconUpDownOffset;   // 上下偏移

} iconlist_t;

iconlist_t iconlist_initialize(
    // items
    item_icon_t items[],
    uint8_t     size,
    // cbk
    cbk_t lpfnPainter,  // default = iconlist_callback_default_painter
    cbk_t lpfnHandler   // default = iconlist_callback_default_handler
);

// lpfnPainter
void iconlist_callback_default_painter(iconlist_t* p);

// lpfnHandler
void iconlist_callback_default_handler(iconlist_t* p);
void iconlist_callback_handler_shake_selected(iconlist_t* p);
void iconlist_callback_handler_updown_selected(iconlist_t* p);
void iconlist_callback_handler_switch_prev(iconlist_t* p);
void iconlist_callback_handler_switch_next(iconlist_t* p);

#endif