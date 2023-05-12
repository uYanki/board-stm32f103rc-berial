#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "rotary_encoder/rotary_encoder.h"

#define USE_ENCODER_IRQ 0  // 0: 轮询, 1: 中断

rotary_encoder_t re;

uint8_t rotary_encoder_read_a(void) { return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0); }

uint8_t rotary_encoder_read_b(void) { return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1); }

int main(void)
{
    board_init();

    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

#if USE_ENCODER_IRQ == 1
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
#endif
    }

#if USE_ENCODER_IRQ == 1
    {
        EXTI_InitTypeDef EXTI_InitStructure;

        EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;

        EXTI_Init(&EXTI_InitStructure);
    }
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

        NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
        NVIC_Init(&NVIC_InitStructure);
    }
#endif

    rotary_encoder_init(&re, ROTARY_ENCODER_MODE_0, rotary_encoder_read_a, rotary_encoder_read_b);

    for (;;) {
#if USE_ENCODER_IRQ == 0
        rotary_encoder_handler(&re);
        if (rotary_encoder_get(&re) != ROTARY_ENCODER_UNCHANGE)
            println("%d,%d", re.abs, re.diff);
#endif
    }
}

#if USE_ENCODER_IRQ == 1

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        rotary_encoder_handler(&re);
        if (rotary_encoder_get(&re) != ROTARY_ENCODER_UNCHANGE)
            println("%d,%d", re.abs, re.diff);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

#endif