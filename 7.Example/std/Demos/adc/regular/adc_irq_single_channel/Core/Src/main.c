#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

    init_adc();

    for (;;) {}
}

void init_adc(void)
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
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

        NVIC_InitStructure.NVIC_IRQChannel                   = ADC1_2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
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

        // 使能转换完成中断 END Of CONV
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    }

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC1_2_IRQHandler(void)
{
    // 注：ADC_GetConversionValue() 只能获取最后1次的转换值, 因此该方法不支持多通道采样

    if (SET == ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
        println("%d", ADC_GetConversionValue(ADC1));
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}
