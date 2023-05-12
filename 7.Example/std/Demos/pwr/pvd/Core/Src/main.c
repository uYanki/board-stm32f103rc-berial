#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void PVD_IRQHandler(void)
{
    if (SET == PWR_GetFlagStatus(PWR_FLAG_PVDO)) {
        LOGD("PVD IRQ");
    }

    EXTI_ClearITPendingBit(EXTI_Line16);
}

void PVD_Config(uint32_t level)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        NVIC_InitStructure.NVIC_IRQChannel                   = PVD_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

        NVIC_Init(&NVIC_InitStructure);
    }

    {
        EXTI_InitTypeDef EXTI_InitStructure;

        EXTI_ClearITPendingBit(EXTI_Line16);

        EXTI_InitStructure.EXTI_Line    = EXTI_Line16;  // LINE16 -> PVD
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;

        EXTI_Init(&EXTI_InitStructure);
    }

    // 设置阈值电压
    PWR_PVDLevelConfig(level);

    PWR_PVDCmd(ENABLE);
}

int main(void)
{
    board_init();

    PVD_Config(PWR_PVDLevel_2V6);

    for (;;)
        ;
}
