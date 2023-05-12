#include "delay.h"

/////////////////////////////////////////////////////////////////////////////////// systick

void delay_us(uint32_t us)
{
    // v3.0 SystemFrequency
    // v3.5 SystemCoreClock

    // SysTick 到达计数周期时, 其 CTRL 寄存器的 bit16 会被置 1, 同时也会触发 SysTick_Handler() 中断

    // 中断频率 f = SystemCoreClock / ( SystemCoreClock / n ) = n Hz

    SysTick_Config(SystemCoreClock / 1e6);  // 1MHz -> 1us

    // SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 已在 SysTick_Config 中使能

    while (us--) {
        while (!(SysTick->CTRL & 0x10000))  // bit16
            ;
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // disable
}

void delay_ms(uint32_t ms)
{
    SysTick_Config(SystemCoreClock / 1e3);  // 1kHz -> 1ms

    while (ms--) {
        while (!(SysTick->CTRL & 0x10000))
            ;
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/////////////////////////////////////////////////////////////////////////////////// timer

static bool tim2_state = false;

void timer_init(uint8_t id, uint16_t prescaler, uint16_t period)
{
    switch (id) {
        case 2: {
            // init NVIC
            {
                NVIC_InitTypeDef NVIC_InitStructure;

                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

                NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;  // 中断源
                NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 主优先级
                NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;  // 抢占优先级

                NVIC_Init(&NVIC_InitStructure);
            }
            // init TIM
            {
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

                TIM_TimeBaseInitStructure.TIM_Prescaler         = prescaler - 1;
                TIM_TimeBaseInitStructure.TIM_Period            = period - 1;
                TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
                TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
                TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;

                TIM_DeInit(TIM2);
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

                TIM_ClearFlag(TIM2, TIM_FLAG_Update);
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

                TIM_Cmd(TIM2, ENABLE);
            }
            break;
        }

        default:

            break;
    }
}

bool timer_check(uint8_t id)
{
    switch (id) {
        case 2: {
            if (tim2_state) {
                tim2_state = false;
                return true;
            }
        } break;

        default:
            break;
    }

    return false;
}

void TIM2_IRQHandler(void)
{
    if (SET == TIM_GetITStatus(TIM2, TIM_IT_Update)) {
        tim2_state = true;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
