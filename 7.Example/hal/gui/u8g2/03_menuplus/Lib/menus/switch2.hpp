#ifndef __SWITCH2_H__
#define __SWITCH2_H__

#include "item.hpp"

class i_switch_r : public item {
private:
    easing_fast m_easing;

protected:
    bool m_state;  // true:on, false: off

public:
    i_switch_r(const char* title, bool state = false)
        : item(title, style::Default), m_state(state) {}
    ~i_switch_r() {}

protected:
    void init(const vec2<>& size)
    {
        item::init(size);
    }

    bool render(void)
    {
        _draw_str(m_ptText.x, m_ptText.y, m_title);

        return !m_easing.isok();
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
};

#endif
