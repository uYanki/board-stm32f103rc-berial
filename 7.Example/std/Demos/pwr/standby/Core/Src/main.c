#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    if (SET == PWR_GetFlagStatus(PWR_FLAG_WU)) {
        LOGD("wake up from standby mode");
    } else {
        LOGD("hello, world");
    }

    for (;;) {
        if (uart2_rxflg) { // 收到换行符则进入待机模式
            uart2_rxflg = 0;

            LOGD("enter standby mode");
            PWR_ClearFlag(PWR_FLAG_WU);  // clear wake up flag
            PWR_WakeUpPinCmd(ENABLE);    // enable wake up pin (PA0)
            PWR_EnterSTANDBYMode();
        }
    }
}
