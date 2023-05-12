#ifndef __EASING_H__
#define __EASING_H__

#ifdef __cplusplus

#include "epos.hpp"

static inline easing_type correct(easing_type v) { return (v - (uint8_t)v) > 0.5f; }

class easing_core {
public:
    // mode
    enum mode {
        MODE_BITCNT = 3,
        MODE_MASK   = (1 << MODE_BITCNT) - 1,

        // 次数:
        TIMES_CUSTOM   = 0 << 0,  // 有限
        TIMES_INFINITE = 1 << 0,  // 无限

        TIMES_SET = MODE_BITCNT,

        // 方向:
        DIR_FORWARD      = 0 << 0,  // 正向(default)
        DIR_REVERSE      = 1 << 1,  // 反向
        DIR_BACKANDFORTH = 1 << 2,  // 往返
    };

#define EASING_MODE_NTIMES(times, dir) \
    (easing_core::mode)(easing_core::mode::TIMES_CUSTOM | (times << easing_core::mode::TIMES_SET) | dir)

public:
    easing_core(
        easing_pos    pItems[],
        uint32_t      nLength,
        uint32_t      nFrameCount = 2,  // greater than or equal to 2
        mode          dwMode      = EASING_MODE_NTIMES(1, DIR_FORWARD),
        easing_calc_t lpfnCalc    = _easing_calc_Linear,
        uint32_t      nInterval   = 0);
    ~easing_core() {}

    void start(void);
    void update(void);

    // 0:start 1:stop 2:delta 3:current 4:offset 5:curpos
    void print(uint8_t type);

    void stop(void) { m_nTimes = 0; }

    bool isok(void) { return m_nTimes == 0; }

    easing_type curpos(uint32_t index) { return m_pItems[index].curpos(); }

    float progress(void) { return m_fProgress; }
    float coefficient(void) { return m_fCoefficient; }

    uint32_t frameIndex(void) { return m_nFrameIndex; }
    uint32_t frameCount(void) { return m_nFrameCount; }

    // operator
    easing_pos& operator[](uint32_t index) { return m_pItems[index]; }

private:
    mode m_dwMode;

    // positions
    easing_pos* m_pItems;
    uint32_t    m_nLength;

    easing_calc_t m_lpfnCalc;
    // calc type: _easing_calc_xxx

    // range: [2,n]
    // nFrameCount = 2 -> nCurrent = nStart or nStop
    // nFrameCount = n -> nCurrent = nStart...nStop
    uint32_t m_nFrameCount;

    // current frame. range: [0, nFrameCount],
    // nFrameIndex = 0 -> nCurrent = nStart/nStop;
    // nFrameIndex = nFrameCount -> nCurrent = nStop/nStart;
    uint32_t m_nFrameIndex;

    // current progress. range: [0,1]
    float m_fProgress;

    // current coefficient = _easing_calc_xxx(progress)
    float m_fCoefficient;

    // <0: infinite times
    // =0: easing ok
    // >0: remaining  times
    int32_t m_nTimes;

    // true: reverse, false：forward
    bool m_bDirection;

#ifdef easing_mills
    // record the start time of the next frame
    uint32_t m_nMills;

    // minimum time interval per frame (ms)
    uint32_t m_nInterval;
#endif
};

class easing_fast : public easing_core, public easing_pos {
public:
    easing_fast(easing_type   nCurrent    = 0,
                easing_type   nOffset     = 0,
                uint32_t      nFrameCount = 2,  // greater than or equal to 2
                mode          dwMode      = EASING_MODE_NTIMES(1, DIR_FORWARD),
                easing_calc_t lpfnCalc    = _easing_calc_Linear,
                uint32_t      nInterval   = 0)
        : easing_pos(nCurrent, nOffset), easing_core(this, 1, nFrameCount, dwMode, lpfnCalc, nInterval)
    {
    }
    ~easing_fast() {}

    easing_type curpos(void) { return easing_pos::curpos(); }
};

#endif

#endif