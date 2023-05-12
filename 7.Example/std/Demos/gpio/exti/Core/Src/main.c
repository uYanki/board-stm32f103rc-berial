#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void EXTI0_IRQHandler(void)
{
    // 此处按键没消抖（注:不要在中断里进行延时）
    if (SET == EXTI_GetITStatus(EXTI_Line0)) {
        EXTI_ClearITPendingBit(EXTI_Line0);
        println("hello");
    }
}

int main(void)
{
    board_init();

    /* PA0 - KEY */

    // init GPIO
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    // init EXTI
    {
        EXTI_InitTypeDef EXTI_InitStructure;

        EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;

        GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
        EXTI_Init(&EXTI_InitStructure);
    }
    // init NVIC
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

        NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;

        NVIC_Init(&NVIC_InitStructure);

        // EXTI0_IRQn -> void EXTI0_IRQHandler(void);
        // EXTI9_5_IRQn -> void EXTI9_5_IRQHandler(void);
        // EXTI15_10_IRQn -> void EXTI15_10_IRQHandler(void);
    }

    for (;;) {
        delay_ms(2000);
        EXTI_GenerateSWInterrupt(EXTI_Line0);  // 触发软中断
    }
}
