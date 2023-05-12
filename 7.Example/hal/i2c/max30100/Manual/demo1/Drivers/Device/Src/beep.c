/**
 ******************************************************************************
 * @file    beep.c
 * @author  张东
 * @version V1.0.0
 * @date    2019-12-29
 * @brief   蜂鸣器提示音 LED闪烁 以及其他提示相关外设控制
 ******************************************************************************
 */
/*--Include-start-------------------------------------------------------------*/
#include "beep.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

/*Global data space ----------------------------------------------------------*/

const uint16_t FREQ[21] = {
    262,
    294,
    330,
    349,
    392,
    440,
    494,
    523,
    587,
    659,
    698,
    784,
    880,
    988,
    1047,
    1175,
    1319,
    1397,
    1568,
    1760,
    1776,

};  // 音阶频率表

uint16_t g_tone = 0;  // 音调 0-20

// 蜂鸣器使用通道 TIM2->CCR1
// LED使用通道   TIM2->CCR2

/**
 * @brief  PWM信号定义.
 * @param  freq pwm频率.
 * @param  duty pwm占空比.
 * @retval none.
 */
void set_pwm_value(uint16_t freq) {
    htim2.Init.Period = 1000000 / freq;
    HAL_TIM_PWM_Init(&htim2);
}

// LED 控制
void Led_Control(int pl) {
    // 范围检查
    if (pl >= 0 && pl < 100) {
        TIM2->CCR2 = 10000 / (FREQ[g_tone]) * pl;
    }
}

// Beep 控制
void Beep_Control(int pl) {
    // 范围检查
    if (pl >= 0 && pl < 100) {
        TIM2->CCR1 = 10000 / (FREQ[g_tone]) * pl;
    }
}

// 变更音调
void settone(int t) {
    // 范围检查
    if (t >= 0 && t < 21) {
        g_tone = t;
        set_pwm_value((FREQ[g_tone]));
    }
}
