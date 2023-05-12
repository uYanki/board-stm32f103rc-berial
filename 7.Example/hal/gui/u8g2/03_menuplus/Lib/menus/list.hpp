#ifndef __LIST_H__
#define __LIST_H__

#include "header.hpp"

class list {
public:
    enum {
        LIST_HEIGHT = CONFIG_SCREEN_HEIGHT,
        LIST_WIDTH  = CONFIG_SCREEN_WIDTH,
        // [0,LIST_WIDTH-3]:items
        // [LIST_WIDTH-2,LIST_WIDTH-1]:scrollbar
    };

private:
    // items
    item** m_pItems;
    int8_t m_nLength;

    // index of selected item
    int8_t m_nMaskedIndex;    // [0, min(nDisplayedCount,nSize)), index of the masked item in page
    int8_t m_nSelectedIndex;  // [0, nLength), current selected item index in all

    // config
    int8_t m_nDisplayedCount;  // number of items can be displayed per page

    // easing processor
    easing_core m_EasingCore;
    easing_pos  m_EasingPos[7];

    // item pos and size
    easing_pos& m_ItemX = m_EasingPos[0];
    easing_pos& m_ItemY = m_EasingPos[1];
    easing_type m_ItemW = LIST_WIDTH - 3;
    easing_type m_ItemH;  // height of item = nHeight / nDisplayedCount

    // mask pos and size
    easing_pos& m_MaskX = m_EasingPos[2];
    easing_pos& m_MaskY = m_EasingPos[3];
    easing_pos& m_MaskW = m_EasingPos[4];
    easing_pos& m_MaskH = m_EasingPos[5];

    // scrollbar pos and size
    easing_type m_ScrollX = LIST_WIDTH - 2;
    easing_type m_SliderH;  // height of scroll = nHeight / nLength
    easing_pos& m_SliderY = m_EasingPos[6];

    // repaint flag
    bool m_bRepaint : 1;

private:
    void startEasing(void);
    void updateEasing(void);
    bool checkEasing(void);
    void adjustMask(void);
    void renderItems(void);
    void renderMask(void);
    void renderScrollbar(void);

public:
    list(item* pItems[], uint8_t nLength, uint8_t nDisplayedCount = 4);
    ~list() {}

    void render(void);

    void switchToPrevItem(void);
    void switchToNextItem(void);
    void event(key_id_t id);

    item* operator[](uint8_t idx) { return m_pItems[idx]; }
};

#endif

////

void doit(void* arg)
{
    i_label    item00 = i_label("hello0", item::style::Selectable);
    i_label    item01 = i_label("hello0#", item::style::Unselectable);
    i_switch   item02 = i_switch("switch", true, i_switch::style::MaskTitle);
    i_switch   item03 = i_switch("switch", false, i_switch::style::MaskBox);
    i_checkbox item04 = i_checkbox("checkbox", i_checkbox::state::Unchecked, i_checkbox::style::LeftBoxRightTitle | i_checkbox::style::MaskTitle);
    i_checkbox item05 = i_checkbox("checkbox", i_checkbox::state::PartiallyChecked, i_checkbox::style::LeftTitleRightBox | i_checkbox::style::MaskTitle);
    i_checkbox item06 = i_checkbox("checkbox", i_checkbox::state::Checked, i_checkbox::style::LeftBoxRightTitle | i_checkbox::style::MaskBox);
    i_checkbox item07 = i_checkbox("checkbox", i_checkbox::state::Unchecked, i_checkbox::style::LeftTitleRightBox | i_checkbox::style::MaskBox);
    i_label    item08 = i_label("hello93#");
    i_label    item09 = i_label("hello3666666");
    i_switch   item10 = i_switch("switch");
    i_label    item11 = i_label("hello93#");
    i_switch   item12 = i_switch("switch");
    i_switch   item13 = i_switch("switch");
    i_label    item14 = i_label("hello94#");
    i_label    item15 = i_label("hello95#", item::style::Unselectable);

    item* items[] = {
        &item00,
        &item01,
        &item02,
        &item03,
        &item04,
        &item05,
        &item06,
        &item07,
        &item08,
        &item09,
        &item10,
        &item11,
        &item12,
        &item13,
        &item14,
        &item15,
    };

    list menulist(items, ARRAY_SIZE(items));

    while (1) {
        menulist.event(key_scan());
        menulist.render();
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void app_entry(void* arg)
{
    doit(arg);
}

#ifdef __cplusplus
}
#endif
