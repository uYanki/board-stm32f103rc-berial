#ifndef __SWITCH1_H__
#define __SWITCH1_H__

#include "item.hpp"

class i_switch : public item {
public:
    enum style {
        MaskTitle = 0 << 0,
        MaskBox   = 1 << 0,
    };

protected:
    bool m_state;  // true:on, false: off

    rect<> m_rcMaskOn;
    rect<> m_rcMaskOff;

    uint8_t m_style : 1;

public:
    i_switch(const char* title, bool state = false, uint8_t style = style ::MaskTitle)
        : item(title, item::style::Default), m_state(state), m_style(style) {}
    ~i_switch() {}

protected:
    void init(const vec2<>& size)
    {
        item::init(size);

        m_rcMaskOn.w  = _str_w_("[on]") + PADDING_X;
        m_rcMaskOff.w = _str_w_("[off]") + PADDING_X;
        m_rcMaskOn.h = m_rcMaskOff.h = FONTSIZE + PADDING_Y * 2;
        m_rcMaskOn.y = m_rcMaskOff.y = 0;
        if (m_style & style ::MaskBox) {
            m_rcMaskOn.x  = size.x - m_rcMaskOn.w - PADDING_X / 2;
            m_rcMaskOff.x = size.x - m_rcMaskOff.w - PADDING_X / 2;
        } else {
            m_rcMaskOn.x = m_rcMaskOff.x = PADDING_X / 2;
        }
    }

    bool render(void)
    {
        _draw_str(m_ptText.x, m_ptText.y, m_title);

        if (m_state) {
            _draw_str(m_size.w - m_rcMaskOn.w, m_ptText.y, "[on]");
        } else {
            _draw_str(m_size.w - m_rcMaskOff.w, m_ptText.y, "[off]");
        }

        return false;
    }

    bool event(key_id_t key)
    {
        if (key == KEY_ID_OK) {
            m_state = !m_state;
            item::event(key);
            return true;
        }
        return false;
    }

    rect<> mask(void) const
    {
        if (m_style & style ::MaskBox)
            return m_state ? m_rcMaskOn : m_rcMaskOff;
        return m_rcMaskText;
    }
};

#endif
