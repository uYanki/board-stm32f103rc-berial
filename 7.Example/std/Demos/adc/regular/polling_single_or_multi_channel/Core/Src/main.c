#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void     adc_init(void);
uint16_t adc_read(uint8_t ch);

int main(void)
{
    board_init();
    adc_init();

    for (;;) {
        println("%d,%d,%d,%d",
                adc_read(ADC_Channel_10),
                adc_read(ADC_Channel_11),
                adc_read(ADC_Channel_12),
                adc_read(ADC_Channel_13));
    }
}

void adc_init(void)
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
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;  // 扫描模式
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 连续转换
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(ADC1, &ADC_InitStructure);
        ADC_Cmd(ADC1, ENABLE);

        // ADC 校准
        ADC_ResetCalibration(ADC1);
        while (ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (ADC_GetCalibrationStatus(ADC1)) {}
    }
}

uint16_t adc_read(uint8_t ch)
{
    uint16_t adc_value;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_55Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // 触发转换

    while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {}

    adc_value = ADC_GetConversionValue(ADC1);

    return adc_value;
}
