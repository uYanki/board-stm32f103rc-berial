#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define println(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__)
#define printv(fmt, var)  printf("[ line: %d | function: %s ] %s = " fmt "\r\n", __LINE__, __FUNCTION__, #var, var)

extern uint8_t key_scan(void);
extern void    print_list(void);

uint32_t m_pItems[] = {
    (1 << 8) | (0 << 0),
    (0 << 8) | (1 << 0),
    (0 << 8) | (2 << 0),
    (0 << 8) | (3 << 0),
    (0 << 8) | (4 << 0),
    (0 << 8) | (5 << 0),
    (0 << 8) | (6 << 0),
    (1 << 8) | (6 << 0),
};

uint8_t m_nDisplayedCount = 4;
uint8_t m_nLength         = ARRAY_SIZE(m_pItems);
uint8_t m_nMaskedIndex    = 0;
uint8_t m_nSelectedIndex  = 0;

uint8_t  m_LineH = 1;
uint32_t m_MaskY = 0;
uint32_t m_ItemY = 0;

inline bool    unselectable(uint8_t index) { return (m_pItems[index] >> 8) & 0x01; }
inline bool    selectable(uint8_t index) { return !unselectable(index); }
inline uint8_t value(uint8_t index) { return m_pItems[index] & 0xFF; }

inline void move(uint32_t* s, uint32_t p) { *s -= p; }
inline void moveto(uint32_t* s, uint32_t p) { *s = p; }

void switchToPrev(void)
{
    if (m_nSelectedIndex == 0) return;

    int8_t nTargetIndex = m_nSelectedIndex;
    int8_t nDestIndex   = nTargetIndex - m_nDisplayedCount;
    if (nDestIndex < -1) nDestIndex = -1;

    while (--nTargetIndex > nDestIndex) {
        if (selectable(nTargetIndex)) break;
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

    moveto(&m_ItemY, (m_nSelectedIndex - m_nMaskedIndex) * m_LineH);
    moveto(&m_MaskY, m_nMaskedIndex * m_LineH);
}

void switchToNext(void)
{
    if (m_nSelectedIndex == m_nLength - 1) return;

    int8_t nTargetIndex = m_nSelectedIndex;
    int8_t nDestIndex   = nTargetIndex + m_nDisplayedCount;
    if (nDestIndex > m_nLength) nDestIndex = m_nLength;

    while (++nTargetIndex < nDestIndex) {
        if (selectable(nTargetIndex)) break;
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

    moveto(&m_ItemY, (m_nSelectedIndex - m_nMaskedIndex) * m_LineH);
    moveto(&m_MaskY, m_nMaskedIndex * m_LineH);
}

int main()
{
    if (unselectable(m_pItems[0])) {
        switchToNext();
        if (!selectable(m_pItems[m_nSelectedIndex]))
            switchToPrev();
    }
    while (true) {
        print_list();
        if (key_scan())
            break;
    }

    return 0;
}

///////////////

uint8_t key_scan(void)
{
    uint8_t key;
    scanf("%d", &key);
    key %= 10;
    switch (key) {
        case 1: switchToPrev(); break;
        case 2: switchToNext(); break;
        default: return key;
    }
    return 0;
}

void print_item(uint8_t index)
{
    uint8_t start = m_nSelectedIndex - m_nMaskedIndex;
    uint8_t end   = start + m_nDisplayedCount - 1;

    printf(" %c%c%c%c%d %c ",
           (start == index) ? '[' : ' ',
           (m_nSelectedIndex == index) ? 's' : ' ',                 // selected
           (((m_ItemY + m_MaskY) / m_LineH) == index) ? 'm' : ' ',  // masked
           unselectable(index) ? '#' : ' ',                         // selectable
           value(index),
           (end == index) ? ']' : ' ');
}

void print_list(void)
{
    for (uint8_t i = 0; i < m_nLength; ++i)
        print_item(i);
    println("");
}
