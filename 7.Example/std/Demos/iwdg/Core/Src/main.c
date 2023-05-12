#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "iwdg/iwdg.h"

int main(void)
{
    board_init();

    LOGD("hello");

    // 溢出时间 t=64*625/40=1000ms=1s
    iwdg_init(IWDG_Prescaler_64, 625);

    for (;;) {
        if (KEY) {
            LOGD("feed dog");
            iwdg_feed();
        }
        delay_ms(500);
    }
}
