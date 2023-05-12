#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

const uint16_t sine12bit[] = {
    2048, 2460, 2856, 3218, 3532, 3786, 3969, 4072,
    4093, 4031, 3887, 3668, 3382, 3042, 2661, 2255,
    1841, 1435, 1054, 714, 428, 209, 65, 3,
    24, 127, 310, 564, 878, 1240, 1636, 2048};

#define POINT_CNT ARRAY_SIZE(sine12bit)

uint32_t dual_sine12bit[POINT_CNT];

#define DAC_DHR12RD_ADDRESS (DAC_BASE + 0x20)

int main(void)
{
    uint8_t i = 0;

    for (i = 0; i < POINT_CNT; ++i) {  // 双通道右对齐
        dual_sine12bit[i] = (sine12bit[i] << 16) + (sine12bit[i]);
    }

    // init GPIO
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }

    // init DAC
    {
        DAC_InitTypeDef DAC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

        DAC_InitStructure.DAC_Trigger        = DAC_Trigger_T2_TRGO;       // 触发源: TIM
        DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;   // 波形发生
        DAC_InitStructure.DAC_OutputBuffer   = DAC_OutputBuffer_Disable;  // DAC 缓冲

        DAC_Init(DAC_Channel_1, &DAC_InitStructure);
        DAC_Init(DAC_Channel_2, &DAC_InitStructure);

        DAC_Cmd(DAC_Channel_1, ENABLE);
        DAC_Cmd(DAC_Channel_2, ENABLE);

        DAC_DMACmd(DAC_Channel_2, ENABLE);
    }

    // init TIM
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 72M

        // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Period        = 10 - 1;
        TIM_TimeBaseStructure.TIM_Prescaler     = 1 - 1;  // 不分频
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

        // @TIM_Prescaler
        // - TIM_Period =  2 - 1 = 1  -> T ≈ 7.1us, f ≈ 140.5kHz
        // - TIM_Period = 20 - 1 = 19 -> T ≈ 8.9us, f ≈ 112.5kHz

        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);  // 作触发源
        TIM_Cmd(TIM2, ENABLE);
    }

    // init DMA
    {
        DMA_InitTypeDef DMA_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

        DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_ADDRESS;       // 外设地址
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)dual_sine12bit;  // 内存地址
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;     // 内存到外设
        DMA_InitStructure.DMA_BufferSize         = POINT_CNT;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;  // 内存地址自增
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;  // 循环模式
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;  // 非内存到内存

        DMA_Init(DMA2_Channel4, &DMA_InitStructure);
        DMA_Cmd(DMA2_Channel4, ENABLE);
    }

    for (;;)
        ;
}
