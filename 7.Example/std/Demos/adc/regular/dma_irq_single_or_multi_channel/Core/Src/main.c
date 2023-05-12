#include "board.h"

#define TAG         "main"
#define DBG_LVL     SDK_DEBUG

#define CONFIG_MODE 0  // 0:多通道 1:单通道

#if CONFIG_MODE
uint16_t adc_value = 0;
#else
uint16_t adc_value[4] = {0};
#endif

void adc_init_single_channel(void);
void adc_init_mulit_channel(void);

int main(void)
{
    board_init();

#if CONFIG_MODE
    adc_init_single_channel();
#else
    adc_init_mulit_channel();
#endif

    for (;;) {}
}

#if CONFIG_MODE

void adc_init_single_channel(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        // adc1/2/3 _ ch10
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    {
        ADC_InitTypeDef ADC_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // <14MHz

        ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;                    // 扫描模式(多通道才要,单通道不需要)
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                     // 连续转换
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  // 软件触发
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(ADC1, &ADC_InitStructure);
        ADC_Cmd(ADC1, ENABLE);

        // 转换顺序及采样时间
        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);  // PC0

        // ADC 校准
        ADC_ResetCalibration(ADC1);
        while (ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (ADC_GetCalibrationStatus(ADC1)) {}
    }
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

        NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    {
        DMA_InitTypeDef DMA_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

        DMA_DeInit(DMA1_Channel1);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&adc_value;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = 1;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel1, &DMA_InitStructure);
        DMA_Cmd(DMA1_Channel1, ENABLE);

        DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    }

    ADC_DMACmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

#else

void adc_init_mulit_channel(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        // adc1/2/3 _ ch10/11/12/13
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

        NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    {
        ADC_InitTypeDef ADC_InitStructure;

        RCC_ADCCLKConfig(RCC_PCLK2_Div6);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode       = ENABLE;  // 扫描模式
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 4;
        ADC_Init(ADC1, &ADC_InitStructure);
        ADC_Cmd(ADC1, ENABLE);

        // 转换顺序及采样时间
        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);  // PC0
        ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);  // PC1
        ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);  // PC2
        ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5);  // PC3

        // ADC 校准
        ADC_ResetCalibration(ADC1);
        while (ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (ADC_GetCalibrationStatus(ADC1)) {}
    }
    {
        DMA_InitTypeDef DMA_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

        DMA_DeInit(DMA1_Channel1);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)adc_value;  // <--
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = ARRAY_SIZE(adc_value);  // <--
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;  // <--
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel1, &DMA_InitStructure);
			
        DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
			
				DMA_Cmd(DMA1_Channel1, ENABLE);
    }

    ADC_DMACmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

#endif

void DMA1_Channel1_IRQHandler(void)
{
#if 1

    if (DMA_GetITStatus(DMA1_IT_TC1)) {
#if CONFIG_MODE
        println("%d", adc_value);
#else
        println("%d,%d,%d,%d", adc_value[0], adc_value[1], adc_value[2], adc_value[3]);
#endif
        DMA_ClearITPendingBit(DMA1_IT_GL1);
    }

#else

    if (DMA_GetFlagStatus(DMA1_FLAG_TC1)) {
#if CONFIG_MODE
        println("%d", adc_value);
#else
        println("%d,%d,%d,%d", adc_value[0], adc_value[1], adc_value[2], adc_value[3]);
#endif
        DMA_ClearITPendingBit(DMA1_FLAG_TC1);
    }

#endif
}
