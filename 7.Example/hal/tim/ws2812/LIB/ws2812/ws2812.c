#include "ws2812.h"

#include "tim.h"
#define WS2812_TIM htim2
#define WS2812_CH  TIM_CHANNEL_1
#define PIXEL_NUM  2

#define NUM (24 * PIXEL_NUM + 100)
#define WS1 64
#define WS0 36

uint16_t buffer[NUM] = {0};

// DMA 传输完成回调函数
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim)
{
    HAL_TIM_PWM_Stop_DMA(&WS2812_TIM, WS2812_CH);
}

void ws2812_show(void)
{
    HAL_TIM_PWM_Start_DMA(&WS2812_TIM, WS2812_CH, (uint32_t*)buffer, NUM);
}

void ws2812_clear(void)
{
    uint16_t i;
    for (i = 0; i < PIXEL_NUM * 24; i++)
        buffer[i] = WS0;  // 写入逻辑0的占空比
    for (i = PIXEL_NUM * 24; i < NUM; i++)
        buffer[i] = 0;  // 占空比为0，全为低电平
    ws2812_show();
}

uint32_t GetGRB(uint8_t R, uint8_t G, uint8_t B)
{
    return G << 16 | R << 8 | B;
}

void ws2812_set_all(uint8_t R, uint8_t G, uint8_t B)
{
    uint16_t i, j;
    uint8_t  dat[24];

    for (i = 0; i < 8; ++i) {
        dat[i]      = (G & 0x80) ? WS1 : WS0;
        dat[i + 8]  = (R & 0x80) ? WS1 : WS0;
        dat[i + 16] = (B & 0x80) ? WS1 : WS0;
        R <<= 1, G <<= 1, B <<= 1;
    }

    for (i = 0; i < PIXEL_NUM; i++)
        for (j = 0; j < 24; j++)
            buffer[i * 24 + j] = dat[j];

    for (i = PIXEL_NUM * 24; i < NUM; i++)
        buffer[i] = 0;  // 占空比为0，全为低电平

    ws2812_show();
}

void ws2812_set_pixel(uint16_t n, uint32_t GRB /*color*/)
{
    uint8_t i;
    if (n < PIXEL_NUM) {
        for (i = 0; i < 24; ++i)
            buffer[24 * n + i] = ((GRB << i) & 0X800000) ? WS1 : WS0;
    }
}
