
#include "hs0038.h"

uint16_t CaptureBuf    = 0;    // 存放捕获缓存
uint8_t  CaptureFlag   = 1;    // 捕获标志位
uint8_t  IRData[4]     = {0};  // 存放数据
uint8_t  IRFlag        = 0;    // 红外接收状态
uint8_t  IRReceiveFlag = 0;    // 完成接收标志

#define CHECK_TIME_LOGIC_0()      (CaptureBuf >= IR_us_LOGIC_0 - IR_DEVIATION && CaptureBuf <= IR_us_LOGIC_0 + IR_DEVIATION)
#define CHECK_TIME_LOGIC_1()      (CaptureBuf >= IR_us_LOGIC_1 - IR_DEVIATION && CaptureBuf <= IR_us_LOGIC_1 + IR_DEVIATION)
#define CHECK_TIME_START()        (CaptureBuf >= IR_us_START - IR_DEVIATION && CaptureBuf <= IR_us_START + IR_DEVIATION)
#define CHECK_TIME_REPEAT_START() (CaptureBuf >= IR_us_REPEAT_START - IR_DEVIATION && CaptureBuf <= IR_us_REPEAT_START + IR_DEVIATION)
#define CHECK_TIME_REPEAT_END()   (CaptureBuf >= IR_us_REPEAT_END - IR_DEVIATION && CaptureBuf <= IR_us_REPEAT_END + IR_DEVIATION)

void IR_Init()
{
    // 设置为上升沿捕获
    __HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_INPUTCHANNELPOLARITY_RISING);
    // 启用捕获
    HAL_TIM_IC_Start_IT(&IR_TIM, IR_TIM_CHANNE);
}

void IR_CaptureCallback()
{
    if (CaptureFlag) {
        // 重置计数器
        __HAL_TIM_SET_COUNTER(&IR_TIM, 0);
        // 设置为下降沿捕获
        __HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_ICPOLARITY_FALLING);
        // 清除状态标志位置
        CaptureFlag = 0;
    } else {
        // 获取当前的捕获值
        CaptureBuf = HAL_TIM_ReadCapturedValue(&IR_TIM, IR_TIM_CHANNE);
        // 设置为上升沿捕获
        __HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_INPUTCHANNELPOLARITY_RISING);
        // 设置状态标志位置
        CaptureFlag = 1;
        // 接收到数据开始位
        if (CHECK_TIME_START()) {
            IRFlag = 1;
            // 清空数据
            for (uint8_t i = 0; i < 4; ++i)
                IRData[i] = 0;
            // 退出,等待接收数据
            return;
        }
        // 接收到循环开始位
        else if (CHECK_TIME_REPEAT_START()) {
            // 重复码标记
            IRFlag = 40;
            // 退出,等待循环第二位
            return;
        }
        // 接收数据
        if (IRFlag && IRFlag <= 32) {
            // 判断为逻辑1
            if (CHECK_TIME_LOGIC_1()) {
                IRData[(IRFlag - 1) / 8] |= (1 << (7 - (IRFlag - 1) % 8));
            }
            // 判断不是逻辑0
            else if (!CHECK_TIME_LOGIC_0()) {
                // 直接退出接收
                IRFlag = 0;
                return;
            }
            ++IRFlag;
            // 接收32个，接收完成
            if (IRFlag == 33) {
                // 校验
                if (IR_CHECK_ADDRESS && IRData[0] != (uint8_t)~IRData[1] ||
                    IR_CHECK_COMMAND && IRData[2] != (uint8_t)~IRData[3])
                    return;
                IRReceiveFlag = 1;
                return;
            }
        }
        if (IRFlag == 40) {
            if (CHECK_TIME_REPEAT_END())
                IRReceiveFlag = IR_CHECK_REPEAT;
        }
    }
}

uint8_t IR_Scanf(uint32_t* data)
{
    if (IRReceiveFlag) {
        IRReceiveFlag = 0;
        *data         = 0;
        for (uint8_t i = 0; i < 4; i++)
            *data |= IRData[i] << 8 * (3 - i);
        return 1;
    }
    return 0;
}
