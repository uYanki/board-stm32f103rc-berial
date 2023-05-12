#include "board.h"

#define TAG      "main"
#define DBG_LVL  SDK_DEBUG

// 芯片容量
#define CHIP_CAP defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) || defined(STM32F10X_HD_VL)

void awdg_init(void);

int main(void)
{
    board_init();
    awdg_init();
    for (;;)
        ;
}

void awdg_init(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;

        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    {
        NVIC_InitTypeDef NVIC_InitStructure;

#if CHIP_CAP
        NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
#else
        NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
#endif
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    {
        ADC_InitTypeDef ADC_InitStructure;

        RCC_ADCCLKConfig(RCC_PCLK2_Div6);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(ADC1, &ADC_InitStructure);

        ADC_Cmd(ADC1, ENABLE);

        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
        ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0B00, 0x0300);  // thresholds, 0.62V~2.26V
        ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_10);
        ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

        ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

        ADC_ResetCalibration(ADC1);
        while (ADC_GetResetCalibrationStatus(ADC1)) {}
        ADC_StartCalibration(ADC1);
        while (ADC_GetCalibrationStatus(ADC1)) {}

        ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // run
    }
}

#if CHIP_CAP
void ADC1_IRQHandler(void)
#else
void ADC1_2_IRQHandler(void)
#endif
{
    // 只要电压不在模拟看门狗的窗口内就会触发该中断

    println("%.2f", ADC_GetConversionValue(ADC1) * 3.3f / 4095);
    ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
}
