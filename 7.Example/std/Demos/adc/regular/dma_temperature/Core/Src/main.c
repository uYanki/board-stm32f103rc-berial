#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#define TIMES   10

#define USE_DMA 1

int main(void)
{
    uint16_t buffer = 0;

    double vsense, temperture;

    board_init();

#if USE_DMA
    {
        DMA_InitTypeDef DMA_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&buffer;
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
    }
#endif

    {
        ADC_InitTypeDef ADC_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 72MHz / 6 = 12MHz (需小于14MHz)

        ADC_InitStructure.ADC_Mode             = ADC_Mode_Independent;       // 独立模式
        ADC_InitStructure.ADC_ScanConvMode     = DISABLE;                    // 非扫描模式
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 禁止外部触发 -> 软件触发采样
        ADC_InitStructure.ADC_DataAlign        = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel     = 1;  // 转换序列中的通道数

#if USE_DMA
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
#else
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 非连续转换
#endif

        ADC_Init(ADC1, &ADC_InitStructure);
        ADC_Cmd(ADC1, ENABLE);

        ADC_ResetCalibration(ADC1);
        while (SET == ADC_GetResetCalibrationStatus(ADC1)){}
        ADC_StartCalibration(ADC1);  // 校准
        while (SET == ADC_GetCalibrationStatus(ADC1)){}

        // 内部温度传感器 (通道16)
        ADC_TempSensorVrefintCmd(ENABLE);

        // 配置转换序列
        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);

#if USE_DMA
        ADC_DMACmd(ADC1, ENABLE);
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);

#endif
    }

    while (1) {
        {
            uint8_t  times = 0;
            uint32_t sum   = 0;

            while (times++ < TIMES) {
#if USE_DMA
                sum += buffer;
#else
                ADC_SoftwareStartConvCmd(ADC1, ENABLE);      // 触发采样
                while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
                sum += ADC_GetConversionValue(ADC1);  // 读取采样值
#endif
                delay_ms(3);
            }

            vsense     = (double)(sum / TIMES) * (3.3 / 4095.0);
            temperture = (1.43 - vsense) / 0.0043 + 25.0;
        }

        LOGI("%.3f, %.3f", vsense, temperture);
        delay_ms(50);
    }
}
