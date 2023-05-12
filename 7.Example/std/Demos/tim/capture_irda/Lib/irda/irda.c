#include "irda.h"

void irda_init(void)
{
    // PB0: TIM3_CH3

    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_SetBits(GPIOB, GPIO_Pin_0);
    }

    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_ICInitTypeDef       TIM_ICInitStructure;  // 输入捕获

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

        TIM_TimeBaseStructure.TIM_Period        = 10000;
        TIM_TimeBaseStructure.TIM_Prescaler     = (72 - 1);
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  // 10ms

        TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;  // IC3
        TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
        TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
        TIM_ICInitStructure.TIM_ICFilter    = 0x03;  // 输入滤波器, 8个定时器时钟周期滤波
        TIM_ICInit(TIM3, &TIM_ICInitStructure);
    }

    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC3, ENABLE);
    }

    TIM_Cmd(TIM3, ENABLE);
}

// bit[7]: 收到了引导码标志
// bit[6]: 得到了一个按键的所有信息
// bit[5]: 保留
// bit[4]: 标记上升沿是否已经被捕获
// bit[3:0]: 溢出计时器
uint8_t irda_state = 0;  // 接收状态

uint16_t irda_time = 0;  // 高电平时间
uint32_t irda_data = 0;  // 收到的数据
uint8_t  irda_cnt  = 0;  // 按键计次

void TIM3_IRQHandler(void)
{
    if (SET == TIM_GetITStatus(TIM3, TIM_IT_Update)) {
        if (irda_state & 0x80) {  // 上次有数据被接收到了
            irda_state &= ~0X10;  // 取消上升沿已经被捕获标记
            if ((irda_state & 0X0F) == 0X00)
                irda_state |= 1 << 6;  // 标记已完成按键信息采集
            if ((irda_state & 0X0F) < 14)
                ++irda_state;
            else {
                irda_state &= ~(1 << 7);  // 清空引导标识
                irda_state &= 0XF0;       // 清空计数器
            }
        }
    }

    if (SET == TIM_GetITStatus(TIM3, TIM_IT_CC3)) {
        // 上升沿捕获
        if (PBin(0)) {
            // 切换为下降沿捕获
            TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
            TIM_SetCounter(TIM3, 0);
            // 标记已捕获上升沿
            irda_state |= 0X10;
        }
        // 下降沿捕获
        else {
            irda_time = TIM_GetCapture3(TIM3);
            // 切换为上升沿捕获
            TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
            // 是否已捕获上升沿
            if (irda_state & 0X10) {
                // 接收到引导码
                if (irda_state & 0X80) {
                    // 接收0, 标准值 560us
                    if (irda_time > 300 && irda_time < 800) {
                        irda_data <<= 1;
                        irda_data |= 0;
                    }
                    // 接收1, 标准值 1680us
                    else if (irda_time > 1400 && irda_time < 1800) {
                        irda_data <<= 1;
                        irda_data |= 1;
                    }
                    // 按键次数增加, 标准值 2500us(2.5ms)
                    else if (irda_time > 2200 && irda_time < 2600) {
                        irda_cnt++;
                        irda_state &= 0XF0;  // 清空计时器
                    }
                }
                // 接收到引导码, 标准值 4500us(4.5ms)
                else if (irda_time > 4200 && irda_time < 4700) {
                    // 标记成功接收到了引导码
                    irda_state |= 1 << 7;
                    irda_cnt = 0;
                }
            }

            irda_state &= ~(1 << 4);
        }
    }

    TIM_ClearFlag(TIM3, TIM_IT_Update | TIM_IT_CC3);
}

// 返回 0 表示没有键被按下
uint8_t irda_scan(void)
{
    uint8_t t1, t2, ret = 0;

    // 完成按键信息采集
    if (irda_state & (1 << 6)) {
        t1 = irda_data >> 24;           // 地址码
        t2 = (irda_data >> 16) & 0xff;  // 地址反码

        // 检验遥控识别码(ID)及地址( 此处的u32转u8勿删 )
#if 0
        if (t1 == (uint8_t)~t2) {
#else
        if (t1 == (uint8_t)~t2 && t1 == REMOTE_ID) {
#endif
        t1 = irda_data >> 8;
        t2 = irda_data;
        if (t1 == (uint8_t)~t2) {
            // 键值正确
            ret = t1;
        }
    }

    // 按键数据错误/遥控已经没有按下了
    if ((ret == 0) || ((irda_state & 0X80) == 0)) {
        irda_state &= ~(1 << 6);  // 清除接收到有效按键标识
        irda_cnt = 0;             // 清除按键次数计数器
    }
}

return ret;
}
