#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#if 1
#include "ks108x/hwspi/ks108x.h"
#else
#include "ks108x/swspi/ks108x.h"
#endif

int main(void)
{
    board_init();

    ks108x_init();

#if 0
    ks108x_reset();
#endif
    ks108x_set_ch1(KS108X_CHANNEL1_POWER_UP, KS108X_GAIN1_9, KS108X_GAIN2_80);
    ks108x_set_ch1(KS108X_CHANNEL1_POWER_UP, KS108X_GAIN1_5, KS108X_GAIN2_40);

    while (1) {
    }
}
