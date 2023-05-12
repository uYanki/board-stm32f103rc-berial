#include "board.h"

#define TAG      "main"
#define DBG_LVL  SDK_DEBUG

#define WWDG_CNT 0x7F  // 0x40 ~ 0x7F

// 在计数值将要低于窗口下限值时喂狗
void WWDG_IRQHandler(void)
{
    uint8_t feed = !!!KEY;

    if (feed) WWDG_SetCounter(WWDG_CNT);  // 喂狗
    LOGD("[ISR] feed dog: %d", feed);

    WWDG_ClearFlag();
}

int main(void)
{
    board_init();

    if (SET == RCC_GetFlagStatus(RCC_FLAG_WWDGRST)) {
        LOGD("watch dog reset");  // 因没喂狗而复位
        RCC_ClearFlag();
    }

    // init NVIC
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        NVIC_InitStructure.NVIC_IRQChannel                   = WWDG_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;

        NVIC_Init(&NVIC_InitStructure);
    }
    // init WWDG
    {
        RCC_APB1PeriphClockCmd(RCC_APB1ENR_WWDGEN, ENABLE);

        /** WWDG_SetPrescaler
         * @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(36MHz)/4096)/1
         * @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(36MHz)/4096)/2
         * @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(36MHz)/4096)/4
         * @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(36MHz)/4096)/8
         * */
        WWDG_SetPrescaler(WWDG_Prescaler_8);

        WWDG_SetWindowValue(0x5f);  // 上窗口值
        WWDG_Enable(WWDG_CNT);
        WWDG_ClearFlag();
        WWDG_EnableIT();
    }

    for (;;) {
#if 1
        uint8_t
            wwdg_tr = WWDG->CR & 0X7F,   // 当前计数值
            wwdg_wr = WWDG->CFR & 0X7F;  // 窗口上限值

        // 在计数值低于窗口上限值时喂狗
        if (wwdg_tr < wwdg_wr) {
            WWDG_SetCounter(WWDG_CNT);  // 喂狗
            LOGD("[Polling] feed dog: 1");
        }
#endif
    }
}
