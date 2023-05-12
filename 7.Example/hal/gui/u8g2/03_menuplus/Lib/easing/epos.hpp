
// https://blog.csdn.net/z2014z/article/details/120691794

#ifndef __EASING_BASE_H__
#define __EASING_BASE_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <math.h>

// #define TESTMODE

#ifdef TESTMODE  // test mode

#include <assert.h>
#include <stdio.h>

#define assert_param assert

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#else

#include "utils.h"

// whether enable interval
#define easing_mills() HAL_GetTick()

#endif

/***********************************************************************************************/  // easing_pos

// the min or max moving distance
#define EASING_MINIMUM_MOVING_DISTACE (1.5f)
// #define EASING_MAXIMUM_MOVING_DISTACE (999999.0f)

typedef float easing_type;                // datatype
typedef void (*easing_cbk_t)(void* arg);  // callback

class easing_core;
class easing_pos;

class easing_pos {
    friend easing_core;

public:
    easing_pos(easing_type nCurrent = 0, easing_type nOffset = 0)
        : m_nStart(nCurrent), m_nStop(nCurrent), m_nCurrent(nCurrent), m_nOffset(nOffset), m_bEnable(false) {}
    ~easing_pos() {}

    // absolute
    easing_pos& move(easing_type nStart, easing_type nStop)
    {
        m_nStart = nStart;
        m_nStop  = nStop;
        calc_delta();
        return *this;
    }

    // absolute
    easing_pos& moveto(easing_type nStop)
    {
#if 1
        return move(m_nCurrent, nStop);  // from current pos
#else
        return move(m_nStop, nStop);  // from last stop pos.
#endif
    }

    // relative
    easing_pos& move(easing_type nDistance)
    {
        return moveto(m_nStop + nDistance);
    }

    easing_pos& offset(easing_type nOffset)
    {
        m_nOffset = nOffset;
        return *this;
    }

    // call it after easing_pos_set_xxx if use EASING_MINIMUM_DISTANCE or EASING_MAXIMUM_DISTANCE
    easing_pos& enable(bool bEnable)
    {
        m_bEnable = bEnable;
        return *this;
    }

    easing_pos& print(void)
    {
        printv("%f", m_nStart);
        printv("%f", m_nStop);
        printv("%f", m_nCurrent);
        printv("%f", m_nOffset);
        printv("%f", m_nDelta);
        printv("%d", m_bEnable);
        printv("%f", curpos());
        return *this;
    }

    // operator

    easing_pos& operator=(easing_type nCurrent)
    {
        m_nCurrent = nCurrent;
        return *this;
    }

#ifdef easing_mills
    easing_pos& delay(uint32_t ms)
    {
        m_nDelay = ms;
        return *this;
    }
#endif

    easing_type curpos(void)
    {
        return m_nCurrent + m_nOffset;
    }

private:
    void calc_delta(void)
    {
        m_bEnable = true;
        m_nDelta  = m_nStop - m_nStart;
#ifdef EASING_MINIMUM_MOVING_DISTACE
        m_bEnable = abs(m_nDelta) >= EASING_MINIMUM_MOVING_DISTACE;
#endif
#ifdef EASING_MAXIMUM_MOVING_DISTACE
        m_bEnable = abs(m_nDelta) <= EASING_MAXIMUM_MOVING_DISTACE;
#endif
    }

private:
    // initial and final position
    easing_type m_nStart;
    easing_type m_nStop;

    // curpos = m_nOffset + m_nCurrent
    easing_type m_nOffset;

    // nDelta = nStop - nStart
    easing_type m_nDelta;

    // range: [nStart, nStop]
    easing_type m_nCurrent;

    // updatable
    bool m_bEnable;

#ifdef easing_mills
    // delay a short time before easing
    uint32_t m_nDelay;  // ms
#endif
};

/***********************************************************************************************/  // _easing_calc_xxx

typedef float (*easing_calc_t)(const float t);
// function example: static inline _easing_calc_xxx(const float t) { return t; } // 0<t<1

// linear t
// quadratic t^2
// cubic t^3
// quartic t^4
// quintic t^5
// sinusoidal 正弦 sin(t)
// exponential 指数 2^t
// circular 圆形
// elastic 衰减三次幂 (s+1)t^3 - st^2
// elastic 衰减正弦
// back 衰减反弹

#ifndef PI
#define PI 3.1415926
#endif

static const float DH  = 1. / 22.;
static const float D1  = 1. / 11.;
static const float D2  = 2. / 11.;
static const float D3  = 3. / 11.;
static const float D4  = 4. / 11.;
static const float D5  = 5. / 11.;
static const float D7  = 7. / 11.;
static const float IH  = 1. / (1. / 22.);
static const float I1  = 1. / (1. / 11.);
static const float I2  = 1. / (2. / 11.);
static const float I4D = 1. / (4. / 11.) / (4. / 11.);

static inline float _easing_calc_InBounce(const float t)
{
    float s;
    if (t < D1) {
        s = t - DH;
        s = DH - s * s * IH;
    } else if (t < D3) {
        s = t - D2;
        s = D1 - s * s * I1;
    } else if (t < D7) {
        s = t - D5;
        s = D2 - s * s * I2;
    } else {
        s = t - 1;
        s = 1 - s * s * I4D;
    }
    return s;
}

static inline float _easing_calc_OutBounce(const float t)
{
    return 1. - _easing_calc_InBounce(1. - t);
}

static inline float _easing_calc_InOutBounce(const float t)
{
    return (t < 0.5) ? _easing_calc_InBounce(t * 2.0) * 0.5 : 1 - _easing_calc_InBounce(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InCirc(const float t)
{
    return 1.0 - sqrtf(1.0 - t * t);
}

static inline float _easing_calc_OutCirc(const float t)
{
    return 1. - _easing_calc_InCirc(1. - t);
}

static inline float _easing_calc_InOutCirc(const float t)
{
    return (t < 0.5) ? _easing_calc_InCirc(t * 2.0) * 0.5 : 1 - _easing_calc_InCirc(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InCubic(const float t)
{
    return t * t * t;
}

static inline float _easing_calc_OutCubic(const float t)
{
    return 1. - _easing_calc_InCubic(1. - t);
}

static inline float _easing_calc_InOutCubic(const float t)
{
    return (t < 0.5) ? _easing_calc_InCubic(t * 2.0) * 0.5 : 1 - _easing_calc_InCubic(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_OutElastic(const float t)
{
    float s = 1 - t;
    return 1 - powf(s, 8) + sinf(t * t * 6 * PI) * s * s;
}

static inline float _easing_calc_InElastic(const float t)
{
    return 1.0 - _easing_calc_OutElastic(1.0 - t);
}

static inline float _easing_calc_InOutElastic(const float t)
{
    return (t < 0.5) ? _easing_calc_InElastic(t * 2.0) * 0.5 : 1 - _easing_calc_InElastic(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InExpo(const float t)
{
    return powf(2, 10 * (t - 1));
}

static inline float _easing_calc_OutExpo(const float t)
{
    return 1.0 - powf(2, -10 * t);
}

static inline float _easing_calc_InOutExpo(const float t)
{
    return (t < 0.5) ? _easing_calc_InExpo(t * 2.0) * 0.5 : 1 - _easing_calc_InExpo(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_Linear(const float t)
{
    return t;
}

static inline float _easing_calc_InQuad(const float t)
{
    return t * t;
}

static inline float _easing_calc_OutQuad(const float t)
{
    return 1. - _easing_calc_InQuad(1. - t);
}

static inline float _easing_calc_InOutQuad(const float t)
{
    return (t < 0.5) ? _easing_calc_InQuad(t * 2.0) * 0.5 : 1 - _easing_calc_InQuad(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InQuart(const float t)
{
    return t * t * t * t;
}

static inline float _easing_calc_OutQuart(const float t)
{
    return 1. - _easing_calc_InQuart(1. - t);
}

static inline float _easing_calc_InOutQuart(const float t)
{
    return (t < 0.5) ? _easing_calc_InQuart(t * 2.0) * 0.5 : 1 - _easing_calc_InQuart(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InQuint(const float t)
{
    return t * t * t * t * t;
}

static inline float _easing_calc_OutQuint(const float t)
{
    return 1. - _easing_calc_InQuint(1. - t);
}

static inline float _easing_calc_InOutQuint(const float t)
{
    return (t < 0.5) ? _easing_calc_InQuint(t * 2.0) * 0.5 : 1 - _easing_calc_InQuint(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InSine(const float t)
{
    return 1.0 - cosf(t * (PI / 2));
}

static inline float _easing_calc_OutSine(const float t)
{
    return 1. - _easing_calc_InSine(1. - t);
}

static inline float _easing_calc_InOutSine(const float t)
{
    return (t < 0.5) ? _easing_calc_InSine(t * 2.0) * 0.5 : 1 - _easing_calc_InSine(2.0 - t * 2.0) * 0.5;
}

static inline float _easing_calc_InBack(const float t)
{
    return 3 * t * t * t - 2 * t * t;
}

static inline float _easing_calc_OutBack(const float t)
{
    return 1. - _easing_calc_InBack(1. - t);
}

static inline float _easing_calc_InOutBack(const float t)
{
    return (t < 0.5) ? _easing_calc_InBack(t * 2.0) * 0.5 : 1 - _easing_calc_InBack(2.0 - t * 2.0) * 0.5;
}

#endif