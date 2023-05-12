#ifndef __ITEM_H__
#define __ITEM_H__

#ifdef __cplusplus

#include "apps/base.h"

template <typename T = easing_type>
struct vec2 {
    union {
        T x;
        T w;
    };
    union {
        T y;
        T h;
    };

    vec2(T v = 0) : x(v), y(v) {}
    vec2(T x_, T y_) : x(x_), y(y_) {}
};

///////

enum {
    PADDING_X = 4,
    PADDING_Y = 4,
    FONTSIZE  = 8,
};

template <typename T = uint8_t>
struct rect {
    T x, y, w, h;

#if 1
    rect(T left = 0, T top = 0, T width = 0, T height = 0) : x(left), y(top), w(width), h(height)
#else
    rect(T left = 0, T top = 0, T right = 0, T bottom = 0) : x(left), y(top), w(right - left), h(bottom - top)
#endif
    {
        assert_param(width >= 0);
        assert_param(height >= 0);
    }

    ~rect() {}

    rect expand(int8_t d) { return rect(x - d, y - d, w + 2 * d, h + 2 * d); }

    // offset
    rect operator-(T d) { return rect(x - d, y - d, w, h); }
    rect operator+(T d) { return rect(x + d, y + d, w, h); }

    T left() { return x; }
    T top() { return y; }
    T right() { return x + w; }
    T bottom() { return y + h; }
    T width() { return w; }
    T height() { return h; }
};

static inline rect<uint8_t> rcMaskStr(const char* str, uint8_t x = 0, uint8_t y = 0, uint8_t padding_x = PADDING_X, uint8_t padding_y = PADDING_Y, uint8_t fontsize = FONTSIZE)
{
    return rect<uint8_t>(x, y, _str_w_(str) + padding_x * 2, fontsize + padding_y * 2);
}

/*********************************************************************************************/  // label

class list;

typedef class item {
    friend list;

public:
    // callback
    typedef bool (*cbk_t)(item* item, void* arg1, void* arg2);

    // style
    enum style {
        Selectable       = 0 << 0,
        Unselectable     = 1 << 0,
        CaptureKeysFirst = 1 << 1,  // 优先响应按键

        Default = Selectable,
    };

protected:
    // property
    const char* m_title;
    vec2<>      m_size;  // item w & h, set by list
    uint8_t     m_style : 2;

    vec2<uint8_t> m_ptText;

    // callback
    cbk_t m_cbk;
    void* m_arg1;
    void* m_arg2;

    // mask
    rect<> m_rcMaskText;

public:
    item(const char* title, style style = style::Default)
        : m_title(title), m_style(style), m_cbk(0), m_arg1(0), m_arg2(0) {}
    ~item() {}

    void cbk()
    {
        if (m_cbk) m_cbk(this, m_arg1, m_arg2);
    }

    void cbk(cbk_t cbk, void* arg1 = nullptr, void* arg2 = nullptr)
    {
        m_cbk  = cbk;
        m_arg1 = arg1;
        m_arg2 = arg2;
    }

protected:
    /**
     * @brief called by list, set item size, and you can do other things
     */
    virtual void init(const vec2<>& size)
    {
        m_size   = size;
        m_ptText = vec2<uint8_t>(PADDING_X, PADDING_Y + FONTSIZE);

        m_rcMaskText.x = 0;
        m_rcMaskText.y = 0;
        m_rcMaskText.w = PADDING_X * 2 + _str_w_(m_title);
        m_rcMaskText.h = PADDING_Y * 2 + FONTSIZE;

        m_ptText.x = m_rcMaskText.x + PADDING_X;
        m_ptText.y = m_rcMaskText.y + PADDING_Y + FONTSIZE;
    }

    virtual rect<> mask(void) const { return m_rcMaskText; }

    /**
     * @brief render item
     * @return true: need to redraw again
     */
    virtual bool render(void)
    {
        if (m_style & style::Unselectable)
            _draw_vline(0, m_size.h / 4, m_size.h / 2);
        _draw_str(PADDING_X, PADDING_Y + FONTSIZE, m_title);
        return false;
    }

    /**
     * @return true: notify list to repaint
     */
    virtual bool event(key_id_t key)
    {
        println("%s", m_title);
        if (m_cbk) return m_cbk(this, m_arg1, m_arg2);
        return false;
    }

} i_label;

#endif

#endif