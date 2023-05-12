#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void init_adc(void);
void init_timer(void);

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
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 定时器触发, 不用连续转换
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T3_TRGO;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(ADC1, &ADC_InitStructure);

        ADC_Cmd(ADC1, ENABLE);

        // 规则通道
        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);  // PC0

        // ADC 校准
        ADC_ResetCalibration(ADC1);
        while (ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (ADC_GetCalibrationStatus(ADC1)) {}

        // 使能转换完成中断
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

        // 启用外部触发
        ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    }
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

        TIM_TimeBaseStructure.TIM_Period        = 40;  // 250Hz
        TIM_TimeBaseStructure.TIM_Prescaler     = 7200 - 1;
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

        TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
        TIM_Cmd(TIM3, ENABLE);
    }
}

void ADC1_2_IRQHandler(void)
{
    if (SET == ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
        println("%d", ADC_GetConversionValue(ADC1));
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}
