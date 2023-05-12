#include "list.hpp"

list::list(item* pItems[], uint8_t nLength, uint8_t nDisplayedCount)
    : m_nDisplayedCount(nDisplayedCount), m_pItems(pItems), m_nLength(nLength),
      m_EasingCore(m_EasingPos, ARRAY_SIZE(m_EasingPos), 4)
{
    assert_param(pItems);
    assert_param(nLength);
    assert_param(nDisplayedCount);

    m_nSelectedIndex = m_nMaskedIndex = 0;

    m_ItemX = m_ItemY = 0.f;

    m_ItemH = (easing_type)LIST_HEIGHT / m_nDisplayedCount;

    m_SliderY = 0.f;
    m_SliderH = (float)LIST_HEIGHT / m_nLength;

    for (uint8_t i = 0; i < m_nLength; ++i) {
        m_pItems[i]->init(vec2<>(m_ItemW, m_ItemH));
    }

    if (m_pItems[0]->m_style & item::style::Unselectable) {
        switchToNextItem();
        if (m_pItems[0]->m_style & item::style::Unselectable)
            switchToPrevItem();
    }

    startEasing();
}

void list::event(key_id_t id)
{
    switch (id) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: {
            println("prev");
            if (m_pItems[m_nSelectedIndex]->m_style & item::style::CaptureKeysFirst)
                if (m_pItems[m_nSelectedIndex]->event(KEY_ID_PREV)) break;
            switchToPrevItem();
            startEasing();
            break;
        }
        case KEY_ID_NEXT: {
            println("next");
            if (m_pItems[m_nSelectedIndex]->m_style & item::style::CaptureKeysFirst)
                if (m_pItems[m_nSelectedIndex]->event(KEY_ID_NEXT)) break;
            switchToNextItem();
            startEasing();
            break;
        }
        default: {
            if (m_bRepaint = m_pItems[m_nSelectedIndex]->event(id))
                startEasing();
            break;
        }
    }
}

/**
 * @brief update all easing position
 */
void list::updateEasing(void)
{
    m_EasingCore.update();
}

/**
 * @brief check all easing is ok
 */
bool list::checkEasing(void)
{
    return m_EasingCore.isok();
}

/**
 * @brief repaint objects
 */
void list::render(void)
{
    if (m_bRepaint) {
        _clear();
        if (m_pItems) {
            // println("repaint");
            m_bRepaint = !checkEasing();
            updateEasing();
            renderItems();
            renderScrollbar();
            renderMask();
        } else {
            m_bRepaint = false;
        }
        _update();
    }
}

void list::renderItems(void)
{
    easing_type yoffset = -m_ItemY.curpos();
    for (uint8_t i = 0; (i < m_nLength) && (yoffset < LIST_HEIGHT); ++i, yoffset += m_ItemH) {
        if (yoffset < 0) continue;
        _clip_set_rect(0, yoffset, m_ItemW, yoffset + m_ItemH);
        _set_offset_(0, yoffset);
        m_pItems[i]->render();
    }
    _set_offset_(0, 0);
    _clip_reset();
}

void list::renderScrollbar(void)
{
    _set_offset_x(m_ScrollX);

    _draw_vline(0, 0, LIST_HEIGHT);  // 轴

    easing_type y = 0;
    for (uint8_t i = 0; i < m_nLength; ++i) {  // 刻度
        _draw_pixel(-1, y);
        _draw_pixel(+1, y);
        y += m_SliderH;
    }
    _draw_pixel(-1, LIST_HEIGHT - 1);
    _draw_pixel(+1, LIST_HEIGHT - 1);

    easing_type h = m_SliderH + correct(m_SliderY.curpos());
    _draw_vline(-1, m_SliderY.curpos(), h);  // 滑块
    _draw_vline(+1, m_SliderY.curpos(), h);

    _set_offset_x(0);
}

void list::renderMask(void)
{
    _set_color(2);  // xor
    _fill_rounded_rect(m_MaskX.curpos(), m_MaskY.curpos(), m_MaskW.curpos(), m_MaskH.curpos(), 1);
    _set_color(1);
}

/**
 * @brief adjust mask pos and size
 */
void list::adjustMask(void)
{
    if (m_nLength == 0 || (m_pItems[m_nSelectedIndex]->m_style & item::style::Unselectable)) {
        // set mask invisible
        m_MaskX.moveto(0);
        m_MaskY.moveto(0);
        m_MaskW.moveto(0);
        m_MaskH.moveto(0);
    } else {
        rect<> mask = m_pItems[m_nSelectedIndex]->mask();
        // move
        m_MaskX.moveto(mask.x);
        m_MaskY.moveto(mask.y + m_nMaskedIndex * m_ItemH);
        // resize
        m_MaskW.moveto(mask.w);
        m_MaskH.moveto(mask.h);
    }
}

void list::startEasing(void)
{
    m_ItemY.moveto((m_nSelectedIndex - m_nMaskedIndex) * m_ItemH);
    m_SliderY.moveto(m_nSelectedIndex * m_SliderH);
    adjustMask();
    m_EasingCore.start();
    m_bRepaint = true;
}

/**
 * @brief swicth to prev selectable item
 */
void list::switchToPrevItem(void)
{
    if (m_nSelectedIndex == 0) return;

    int8_t nTargetIndex = m_nSelectedIndex;
    int8_t nDestIndex   = nTargetIndex - m_nDisplayedCount;
    if (nDestIndex < -1) nDestIndex = -1;

    while (--nTargetIndex > nDestIndex) {
        if (!(m_pItems[nTargetIndex]->m_style & item::style::Unselectable)) break;
    }

    if (nTargetIndex == -1) {
        m_nMaskedIndex = m_nSelectedIndex;
    } else {
        int8_t nDeltaIndex = m_nSelectedIndex - nTargetIndex;
        m_nSelectedIndex   = nTargetIndex;
        if (nDeltaIndex >= m_nMaskedIndex) {
            m_nMaskedIndex = 0;
        } else {
            m_nMaskedIndex -= nDeltaIndex;
        }
    }
}

/**
 * @brief swicth to next selectable item
 */
void list::switchToNextItem(void)
{
    if (m_nSelectedIndex == m_nLength - 1) return;

    int8_t nTargetIndex = m_nSelectedIndex;
    int8_t nDestIndex   = nTargetIndex + m_nDisplayedCount;
    if (nDestIndex > m_nLength) nDestIndex = m_nLength;

    while (++nTargetIndex < nDestIndex) {
        if (!(m_pItems[nTargetIndex]->m_style & item::style::Unselectable)) break;
    }

    if (nTargetIndex == m_nLength) {
        m_nMaskedIndex = m_nSelectedIndex - (m_nLength - m_nDisplayedCount);
    } else {
        int8_t nDeltaIndex     = nTargetIndex - m_nSelectedIndex;
        int8_t nRemianingCount = (m_nDisplayedCount - 1) - m_nMaskedIndex;
        m_nSelectedIndex       = nTargetIndex;
        if (nDeltaIndex >= nRemianingCount) {
            m_nMaskedIndex = m_nDisplayedCount - 1;
        } else {
            m_nMaskedIndex += nDeltaIndex;
        }
    }
}
