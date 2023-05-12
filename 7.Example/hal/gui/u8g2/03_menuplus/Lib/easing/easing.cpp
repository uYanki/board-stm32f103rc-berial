
#include "easing.hpp"

easing_core::easing_core(
    easing_pos    pItems[],
    uint32_t      nLength,
    uint32_t      nFrameCount,
    mode          dwMode,
    easing_calc_t lpfnCalc,
    uint32_t      nInterval)
{
    assert_param(pItems);
    assert_param(nLength);
    assert_param(lpfnCalc);
    assert_param(nFrameCount >= 2);

    m_dwMode = dwMode;

    m_lpfnCalc = lpfnCalc;

    m_pItems  = pItems;
    m_nLength = nLength;

    m_nFrameIndex = 0;
    m_nFrameCount = nFrameCount;

    m_fProgress    = 0.f;
    m_fCoefficient = 0.f;

    m_nTimes = 0;

    m_bDirection = dwMode & mode::DIR_REVERSE;
#ifdef easing_mills
    m_nInterval = nInterval;
#endif
}

void easing_core::start(void)
{
    m_nFrameIndex = 0;  // first frame is nStart
    m_fProgress   = 0.f;

    m_bDirection = m_dwMode & mode::DIR_REVERSE;

    if (m_dwMode & mode::TIMES_INFINITE) {
        m_nTimes = -1;
    } else {
        m_nTimes = m_dwMode >> mode::TIMES_SET;
        if (m_nTimes == 0) m_nTimes = 1;
        if (m_dwMode & mode::DIR_BACKANDFORTH) m_nTimes *= 2;
    }

#ifdef easing_mills
    m_nMills = easing_mills();
#endif
}

// update easing position
void easing_core::update(void)
{
    // isok
    if (isok()) return;

#ifdef easing_mills
    if (m_nInterval > 0) {
        if (easing_mills() < m_nMills) return;
        m_nMills = easing_mills() + m_nInterval;
    }
#endif

    // next frame
    ++m_nFrameIndex;

    if (m_nFrameIndex > m_nFrameCount) {
        if (m_dwMode & mode::DIR_BACKANDFORTH) {
            // reverse direction
            m_bDirection = !m_bDirection;
            // skip once nStart/nStop pos
            m_nFrameIndex = 2;
        } else {
            // at first frame
            m_nFrameIndex = 1;
        }
    }

    easing_pos* src  = m_pItems;
    easing_pos* dest = m_pItems + m_nLength;

    if (m_nFrameIndex == m_nFrameCount) {
        // at last frame
        m_fProgress    = 1.f;
        m_fCoefficient = 1.f;
        while (src < dest) {
            if (src->m_bEnable) src->m_nCurrent = m_bDirection ? src->m_nStart : src->m_nStop;
            ++src;
        }
        // decrease times
        if (!(m_dwMode & mode::TIMES_INFINITE)) {
            if (--m_nTimes == 0) {
                src = m_pItems;
                while (src < dest)
                    src++->m_bEnable = false;
            }
        }
    } else {
        // calculate progress
        m_fProgress = (float)(m_nFrameIndex - 1) / (m_nFrameCount - 1);
        // calculate position
        m_fCoefficient = m_lpfnCalc(m_fProgress);
        while (src < dest) {
            if (src->m_bEnable) {
                src->m_nCurrent = m_bDirection ?
                                      (src->m_nStop - m_fCoefficient * src->m_nDelta) :
                                      (src->m_nStart + m_fCoefficient * src->m_nDelta);
            }
            ++src;
        }
    }
}

void easing_core::print(uint8_t type)
{
    easing_pos* src  = m_pItems;
    easing_pos* dest = m_pItems + m_nLength;
    switch (type) {
        case 0:
            while (src < dest) printf("%f\t", src++->m_nStart);
            break;
        case 1:
            while (src < dest) printf("%f\t", src++->m_nStop);
            break;
        case 2:
            while (src < dest) printf("%f\t", src++->m_nDelta);
            break;
        case 3:
            while (src < dest) printf("%f\t", src++->m_nCurrent);
            break;
        case 4:
            while (src < dest) printf("%f\t", src++->m_nOffset);
            break;
        case 5:
            while (src < dest) printf("%f\t", src++->curpos());
            break;
        default:
            break;
    }

    printf("\r\n");
}

////////////////////////////////////////////////////////////////////////////////////

#ifdef TESTMODE
/**
 *

 * pos = {}:
 *
 *      0.000000        10.000000       20.000000       0.000000        0.000000        0.000000        0.000000        0.000000        0.000000
 *      2.500000        12.500000       22.500000       2.500000        25.000000       250.000000      2.500000        25.000000       250.000000
 *      5.000000        15.000000       25.000000       5.000000        50.000000       500.000000      5.000000        50.000000       500.000000
 *      7.500000        17.500000       27.500000       7.500000        75.000000       750.000000      7.500000        75.000000       750.000000
 *      10.000000       20.000000       30.000000       10.000000       100.000000      1000.000000     10.000000       100.000000      1000.000000
 *
 * pos = {
 *      easing_pos(10, 5), // absolute
 *      easing_pos(10, 10),
 *      easing_pos(20, 10),
 *      easing_pos(10, 5), // relative
 *      easing_pos(10, 10),
 *      easing_pos(20, 10),
 *      easing_pos(10, 5), // moveto
 *      easing_pos(10, 10),
 *      easing_pos(20, 10),
 * }
 *
 *      5.000000        20.000000       30.000000       15.000000       20.000000       30.000000       15.000000       20.000000       30.000000
 *      7.500000        22.500000       32.500000       17.500000       45.000000       280.000000      15.000000       42.500000       275.000000
 *      10.000000       25.000000       35.000000       20.000000       70.000000       530.000000      15.000000       65.000000       520.000000
 *      12.500000       27.500000       37.500000       22.500000       95.000000       780.000000      15.000000       87.500000       765.000000
 *      15.000000       30.000000       40.000000       25.000000       120.000000      1030.000000     15.000000       110.000000      1010.000000
 *
 */

int main()
{
#if 0  // unused

    char  buff[32];
    FILE* f = fopen("data.csv", "w");
    while (!easing_isok(&e)) {
        easing_update(&e);
        float x = easing_curpos(&e);
        sprintf(buff, "%f\n", y);
        fputs(buff, f);
    }
    fclose(f);

#endif

#if 0

    easing_pos pos[10] = {
#if 0
        // absolute
        easing_pos(10, 5),
        easing_pos(10, 10),
        easing_pos(20, 10),
        // relative
        easing_pos(10, 5),
        easing_pos(10, 10),
        easing_pos(20, 10),
        // moveto(absolute)
        easing_pos(10, 5),
        easing_pos(10, 10),
        easing_pos(20, 10),
        // disable
        easing_pos(10, 10),
#endif
    };
    pos[4] = 2000;  // set current pos

    pos[0].move(00, 10);
    pos[1].move(10, 20);
    pos[2].move(20, 30);
    pos[3].move(10);
    pos[4].move(100);
    pos[5].move(1000);
    pos[6].moveto(10);
    pos[7].moveto(100);
    pos[8].moveto(1000);
    pos[9].moveto(10);

    printf("--------------------\r\n");
    pos[8].print();  // enable
    printf("--------------------\r\n");
    pos[9].print();  // disable
    printf("--------------------\r\n");

    // const size_t len = 1;
    const size_t len = ARRAY_SIZE(pos);

    // easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_FORWARD));
    // easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_REVERSE));
    // easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_FORWARD | easing_core::mode::DIR_BACKANDFORTH));
    easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_REVERSE | easing_core::mode::DIR_BACKANDFORTH));

    easing.start();
    while (!easing.isok()) {
        easing.update();
        for (size_t i = 0; i < len; ++i) {
            printf("%f\t", easing[i].curpos());
            // printf("%f\t", pos[i].curpos());
        }
        printf("\n");
    }

#endif

#if 0
    easing_fast easing(10, 10, 5);

    easing.move(20);
    easing.start();
    while (!easing.isok()) {
        easing.update();
        printf("%f\n", easing.curpos());
    }

#endif

    return 0;
}

#endif