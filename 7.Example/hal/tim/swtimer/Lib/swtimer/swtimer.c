
#include "swtimer.h"

static timer_t* m_head  = 0;
static uint32_t m_ticks = 0;

void timer_init(timer_t* timer, uint32_t period, tmr_cbk_t callback, void* user_data)
{
    timer->callback  = callback;
    timer->period    = period;
    timer->user_data = user_data;
}

/**
 * @brief add timer to work linklist
 * @param [in] delay 0 = start now, otherwise = delay for a while then start
 * @return 0: already exist, 1: success
 */
uint8_t timer_start(timer_t* timer, uint32_t delay)
{
    timer_t* target = m_head;

    while (target) {
        if (target == timer) return 0;
        target = target->next;
    }

    timer->timeout = m_ticks + delay;
    timer->next    = m_head;
    m_head         = timer;
    return 1;
}

void timer_stop(timer_t* timer)
{
    timer_t** curr = &m_head;
    timer_t*  entry;

    while (*curr) {
        entry = *curr;

        if (entry == timer) {
            *curr = entry->next;
        } else
            curr = &entry->next;
    }
}

void timer_ticks(void)
{
    timer_t* timer = m_head;

    ++m_ticks;

    while (timer) {
        if (m_ticks >= timer->timeout) {
            if (timer->period == 0) {
                timer_stop(timer);
            } else {
                timer->timeout = m_ticks + timer->period;
            }
            timer->callback(timer);
        }
        timer = timer->next;
    }
}
