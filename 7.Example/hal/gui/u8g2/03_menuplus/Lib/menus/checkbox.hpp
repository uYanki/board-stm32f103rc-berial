#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "item.hpp"

class i_checkbox : public item {
public:
    enum state {
        Unchecked,
        PartiallyChecked,
        Checked,
    };

    enum style {
        MaskTitle         = 0 << 0,
        MaskBox           = 1 << 0,
        LeftTitleRightBox = 0 << 1,
        LeftBoxRightTitle = 1 << 1,
    };

private:
    state   m_state;
    uint8_t m_style : 2;

    // box
    rect<> m_rcOuter;
    rect<> m_rcInner;

    vec2<> m_ptYes[3];

    // mask
    rect<> m_rcMaskBox;

public:
    i_checkbox(const char* title, state state = state::Unchecked, uint8_t style = style::MaskTitle | style::LeftTitleRightBox)
        : item(title, item::style::Default), m_state(state), m_style(style) {}
    ~i_checkbox() {}

protected:
    void init(const vec2<>& size)
    {
        item::init(size);

        m_rcOuter.y = PADDING_Y / 2;
        m_rcOuter.w = m_rcOuter.h = size.h - m_rcOuter.y * 2;

        if (m_style & style::LeftBoxRightTitle) {
            m_rcOuter.x = PADDING_X;
            m_ptText.x += PADDING_X * 2 + m_rcOuter.w;
            m_rcMaskText.x += PADDING_X * 2 + m_rcOuter.w;
        } else {
            m_rcOuter.x = size.w - m_rcOuter.w - PADDING_X;
        }

        m_rcInner   = m_rcOuter.expand(-PADDING_X / 2);
        m_rcMaskBox = m_rcOuter.expand(PADDING_X / 2);

        m_ptYes[0].x = m_rcInner.x;
        m_ptYes[0].y = m_rcInner.y + m_rcInner.h * 3 / 4;
        m_ptYes[1].x = m_rcInner.x + m_rcInner.w / 2;
        m_ptYes[1].y = m_rcInner.bottom();
        m_ptYes[2].x = m_rcInner.right();
        m_ptYes[2].y = m_rcInner.y;
    }

    rect<> mask(void) const { return (m_style & style::MaskBox) ? m_rcMaskBox : m_rcMaskText; }

    bool render(void)
    {
        _draw_str(m_ptText.x, m_ptText.y, m_title);
        _draw_rounded_rect(m_rcOuter.x, m_rcOuter.y, m_rcOuter.w, m_rcOuter.h, 1);
        if (m_state == state::PartiallyChecked) {
            _fill_rounded_rect(m_rcInner.x, m_rcInner.y, m_rcInner.w, m_rcInner.h, 1);
        } else if (m_state == state::Checked) {
            _draw_line(m_ptYes[0].x, m_ptYes[0].y, m_ptYes[1].x, m_ptYes[1].y);
            _draw_line(m_ptYes[1].x, m_ptYes[1].y, m_ptYes[2].x, m_ptYes[2].y);
        }
        return false;
    }

    bool event(key_id_t key)
    {
        if (key == KEY_ID_OK) {
            switch (m_state) {
                case state::Unchecked: m_state = state::PartiallyChecked; break;
                case state::PartiallyChecked: m_state = state::Checked; break;
                case state::Checked: m_state = state::Unchecked; break;
            }
            item::event(key);
            return true;
        }
        return false;
    }
};

#endif
