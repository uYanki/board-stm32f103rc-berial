#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

/**
 * @brief 双ADC规则同步采样
 *
 * 在同一时刻进行数据采集，转换结果存放至 ADC1->DR（ADC1的转换值在高16位，ADC2的转换值在低16位）。
 *
 */

uint32_t adc_value = 0;

void adc_init(void);

int main(void)
{
    board_init();
    adc_init();
    for (;;) {}
}

void adc_init(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        // adc1/2/3 _ ch10/11
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // < 14MHz

    {  // ADC1

        ADC_InitTypeDef ADC_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        ADC_InitStructure.ADC_Mode               = ADC_Mode_RegSimult;  // <-
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(ADC1, &ADC_InitStructure);

        ADC_Cmd(ADC1, ENABLE);

        ADC_ResetCalibration(ADC1);
        while (ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (ADC_GetCalibrationStatus(ADC1)) {}

        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);

        ADC_DMACmd(ADC1, ENABLE);  // <--
    }
    {  // ADC2

        ADC_InitTypeDef ADC_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

        ADC_InitStructure.ADC_Mode               = ADC_Mode_RegSimult;  // <-
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(ADC2, &ADC_InitStructure);

        ADC_Cmd(ADC2, ENABLE);

        ADC_ResetCalibration(ADC2);
        while (ADC_GetResetCalibrationStatus(ADC2)) {}
        ADC_StartCalibration(ADC2);
        while (ADC_GetCalibrationStatus(ADC2)) {}

        ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);

        ADC_ExternalTrigConvCmd(ADC2, ENABLE);  // <--
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
    {  // DMA

        DMA_InitTypeDef DMA_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

        DMA_DeInit(DMA1_Channel1);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&adc_value;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = 1;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  // Word
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;      // Word
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel1, &DMA_InitStructure);
			
        DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

        DMA_Cmd(DMA1_Channel1, ENABLE);
    }

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void DMA1_Channel1_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        println("%d,%d", adc_value >> 16, adc_value & 0xFFFF);
        DMA_ClearITPendingBit(DMA1_IT_GL1);
    }
}
