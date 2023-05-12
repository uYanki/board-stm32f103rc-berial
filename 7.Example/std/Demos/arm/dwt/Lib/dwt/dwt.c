#include "dwt.h"

#define DWT_CR           *(__IO uint32_t*)0xE0001000
#define DWT_CYCCNT       *(__IO uint32_t*)0xE0001004
#define DEM_CR           *(__IO uint32_t*)0xE000EDFC

#define DEM_CR_TRCENA    (1 << 24)
#define DWT_CR_CYCCNTENA (1 << 0)

void dwt_init(void)
{
    // enable dwt
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;

    // clear cyccnt
    DWT_CYCCNT = (uint32_t)0u;

    // enable cyccnt
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

void dwt_delay(uint32_t nus)
{
    uint32_t ticks, tcnt = 0;
    uint32_t t_old, t_now;

    ticks = nus * (dwt_freq() / 1000000);  // 节拍数
    t_old = dwt_read_ts();

    while (tcnt < ticks) {
        if ((t_now = dwt_read_ts()) != t_old) {
            if (t_now > t_old) {
                tcnt += t_now - t_old;
            } else {
                tcnt += UINT32_MAX + t_now - t_old;
            }
            t_old = t_now;
        }
    }
}
