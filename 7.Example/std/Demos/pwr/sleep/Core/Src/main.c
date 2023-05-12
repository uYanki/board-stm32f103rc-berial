#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        LOGD("key press (PA0)");
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

int main(void)
{
    RCC_ClocksTypeDef clock_status;
    uint8_t           clock_source;

    board_init();

    LOGD("hello world");

    for (;;) {
        if (uart2_rxflg) {
            uart2_rxflg = 0;

            LOGD("enter sleep mode");

            // 进入睡眠模式, 等待中断唤醒(可通过串口唤醒)
            __WFI();

            // 此处有输出, 串口正常运行
            LOGD("[is uart ok?]");

            clock_source = RCC_GetSYSCLKSource();
            RCC_GetClocksFreq(&clock_status);

            LOGD("SYSCLK:%d", clock_status.SYSCLK_Frequency);
            LOGD("HCLK:%d", clock_status.HCLK_Frequency);
            LOGD("PCLK1:%d", clock_status.PCLK1_Frequency);
            LOGD("PCLK2:%d", clock_status.PCLK2_Frequency);
            LOGD("CLKSRC:%d", clock_source);  // 0:HSI,8:PLLCLK

            LOGD("exit sleep mode");
        }
    }
}
