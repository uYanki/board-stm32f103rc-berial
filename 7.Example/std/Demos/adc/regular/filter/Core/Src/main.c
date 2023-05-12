#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "filter/filter.h"

#define CONFIG_MODE 0

#define bufsize     10  // 缓冲大小

#if CONFIG_MODE
#include <string.h>
uint16_t new_value[bufsize];
#else
uint16_t new_value;
#endif

void init_sampler(void);

int main(void)
{
    board_init();

    init_sampler();
    for (;;)
        ;
}

void init_sampler(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;
    }

    {
        ADC_InitTypeDef ADC_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 72MHz / 6 = 12MHz (需小于14MHz)

        ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;       // 独立模式
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;                    // 非扫描模式
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  // 禁止外部触发 -> 软件触发采样
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;       // 转换序列中的通道数
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换

        ADC_Init(ADC1, &ADC_InitStructure);
        ADC_Cmd(ADC1, ENABLE);

        // 校准
        ADC_ResetCalibration(ADC1);
        while (SET == ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (SET == ADC_GetCalibrationStatus(ADC1)) {}

        // 内部温度传感器 (通道16)
        ADC_TempSensorVrefintCmd(ENABLE);

        // 转换序列
        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);

        ADC_DMACmd(ADC1, ENABLE);
    }

    {
        DMA_InitTypeDef DMA_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

        DMA_DeInit(DMA1_Channel1);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
#if CONFIG_MODE
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)new_value;
        DMA_InitStructure.DMA_BufferSize     = bufsize;
#else
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&new_value;
        DMA_InitStructure.DMA_BufferSize     = 1;
#endif
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel1, &DMA_InitStructure);

        DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    }
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }

    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void do_filter(void)
{
#if CONFIG_MODE

    static uint16_t buffer[bufsize * 3] = {0};
    static uint16_t values[3]           = {0};

#if 1
    uint8_t  i;
    uint16_t value = new_value[0];
    for (i = 0; i < bufsize; ++i) {
        buffer[bufsize * 0 + i] = buffer[bufsize * 1 + i] = buffer[bufsize * 2 + i] = new_value[i];
    }
#endif

    values[0] = middle_value_filter(&buffer[00], bufsize);
    values[1] = arithmetical_average_value_filter(&buffer[10], bufsize);
    values[2] = middle_average_value_filter(&buffer[20], bufsize);

    println("%d,%d,%d,%d",
            value + 00,
            values[0] + 10,
            values[1] + 20,
            values[2] + 30);

#else

#define limit 3055

    static uint16_t values[5]           = {0};
    static uint16_t coeff[bufsize]      = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    static uint16_t buffer[bufsize * 5] = {0};

#if 1
    uint16_t        value               = new_value >>= 4;
#endif

    values[0] = amplitude_limiter_filter(new_value, values[00], limit);
    values[1] = glide_average_value_filter(&buffer[10], bufsize, new_value);
    values[2] = glide_middle_average_value_filter(&buffer[20], bufsize, new_value);
    values[3] = limit_glide_average_value_filter(&buffer[30], bufsize, new_value, limit);
    values[4] = weighted_glide_average_value_filter(&buffer[40], bufsize, new_value, coeff);

    println("%d,%d,%d,%d,%d,%d",
            new_value + 00,
            values[0] + 10,
            values[1] + 20,
            values[2] + 30,
            values[3] + 40,
            values[4] + 50);

#endif
}

void DMA1_Channel1_IRQHandler(void)
{
#if 1
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        do_filter();
        DMA_ClearITPendingBit(DMA1_IT_GL1);
    }
#else
    if (DMA_GetFlagStatus(DMA1_FLAG_TC1)) {
        do_filter();
        DMA_ClearFLAG(DMA1_FLAG_TC1);
    }
#endif
}
