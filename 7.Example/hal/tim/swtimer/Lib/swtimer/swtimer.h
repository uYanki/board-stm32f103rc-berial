#ifndef _SOFT_TIMER_H_
#define _SOFT_TIMER_H_

/**
 * 每次调用 timer_ticks() 时, 定时器计数加1。
 * 当计数值达到 timer.period 时，调用 timer.callback。
 * 即该定时器的时基与硬件定时器的周期有关。
 * 当然也可以把 timer_ticks() 放在主循环中，就变成了每隔多少次执行1次回调函数。
 */

#include <stdint.h>

struct timer;

typedef void (*tmr_cbk_t)(struct timer* timer);

typedef struct timer {
    struct timer* next;
    uint32_t      period;
    uint32_t      timeout;
    tmr_cbk_t     callback;
    void*         user_data;
} timer_t;

#ifdef __cplusplus
extern "C" {
#endif

void    timer_init(timer_t* timer, uint32_t period, tmr_cbk_t callback, void* user_data);
uint8_t timer_start(timer_t* timer, uint32_t delay);
void    timer_stop(timer_t* timer);
void    timer_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
