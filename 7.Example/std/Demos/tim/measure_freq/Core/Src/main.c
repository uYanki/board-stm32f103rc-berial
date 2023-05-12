#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    uint8_t i;

    board_init();

    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;

        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);  // map to PC6
    }

    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
        TIM_OCInitTypeDef       TIM_OCInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

        TIM_TimeBaseInitStructure.TIM_Period        = 200 - 1;
        TIM_TimeBaseInitStructure.TIM_Prescaler     = 7200 - 1;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

        TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);

        // 预装载寄存器 CCR1
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

        TIM_Cmd(TIM3, ENABLE);
    }

    // Period = 200-1, Prescaler = 7200-1 => timebase = 20ms

    /** duty = t / T
     *
     * duty = 0.5ms / 20ms = 2.5%  -> 0°
     * duty = 1.0ms / 20ms = 5%    -> 45°
     * duty = 1.5ms / 20ms = 7.5%  -> 90°
     * duty = 2.0ms / 20ms = 10%   -> 135°
     * duty = 2.5ms / 20ms = 12.5% -> 180°
     */

    // note: TIM_Period = 200
#define ANGLE_0   175  //(uint8_t)(200 * (1 - 0.125))
#define ANGLE_180 195  //(uint8_t)(200 * (1 - 0.025))

    /** SG90 舵机
     * 接线
     * - 信号线：黄
     * - 电源线5V：红
     * - 电源下GND：棕
     * 种类
     * ① 180°版本：占空比控制角度
     * ② 360°版本：占空比控制角速度
     */

    while (1) {
        for (i = ANGLE_0; i <= ANGLE_180; ++i) {
            TIM_SetCompare1(TIM3, i);
            delay_ms(25);
        }
        for (i = ANGLE_180; i >= ANGLE_0; --i) {
            TIM_SetCompare1(TIM3, i);
            delay_ms(25);
        }
    }
}
