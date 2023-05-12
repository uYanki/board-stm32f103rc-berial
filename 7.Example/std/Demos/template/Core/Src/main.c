#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

#if USE_UART

    println("hello world");

    LOGE("error");
    LOGW("warning");
    LOGI("info");
    LOGD("debug");

#endif

    while (1) {
        // #undef USE_KEY

#if USE_PINCTRL && USE_LED && USE_KEY

        LED = KEY;

#if USE_IRQ_U1RX

        if (uart1_rxflg) {
            println("recvice: %s", uart1_rxbuf);
            uart1_rxflg = 0;  // clear flag
        }

#endif

#elif USE_LED

#if USE_PINCTRL

        // 闪灯

        LED = 0;
        delay_ms(1e3);  // 1s
        LED = 1;
        delay_ms(1e3);

#endif

        GPIO_ResetBits(LED_GPIO_Port, LED_GPIO_Pin);
        delay_us(1e6);  // 1s
        GPIO_SetBits(LED_GPIO_Port, LED_GPIO_Pin);
        delay_us(1e6);

#if USE_UART

        // 可开启 XCOM 的时间戳查看时间间隔
        LOGD("debug");

#endif

#else

#endif
    }
}
