
#include "board.h"

extern u16  all_data[16600];                               
extern u8   p_all_data[];                                  
extern void x_filter(void);                                // 输入X信号滤波1MS一次
extern void force_reset(u16 start_addr, u8 process_addr);  // 用于强制软件元使用
extern void force_set(u16 start_addr, u8 process_addr);    
u16         temp[5];
u16 *       p_data_given, *p_reset_coil, *p_over_coil, *p_enable_coil, *p_value;

void T_100MS(void)
{
    u16 timer_count;

    for (timer_count = 0; timer_count < 200; timer_count++) {
        p_data_given  = all_data + 0x1000 + timer_count;
        p_value       = all_data + 0x0800 + timer_count;
        p_enable_coil = all_data + 0x0280 + (timer_count / 0x10);
        if ((*p_enable_coil & (1 << timer_count % 0x10)) == (1 << timer_count % 0x10))  // 线圈壮态
        {
            if (*p_value < *p_data_given)  // 值壮态
                *p_value += 1;
        }
    }
}

void timer_enable(u16 timer_number)
{
    p_data_given = all_data + 0x1000 + timer_number;           // 计数器值地址
    p_value      = all_data + 0x0800 + timer_number;           //
    p_over_coil  = all_data + 0x0100 + (timer_number / 0x10);  // 溢出线圈
    if (*p_value < *p_data_given)
        *p_over_coil &= ~(1 << timer_number % 0x10);  // 值小于设定值时OFF
    else
        *p_over_coil |= (1 << timer_number % 0x10);  // 值到达设定值时ON
}

void timer_disble(u16 timer_number)
{
    p_data_given  = all_data + 0x1000 + timer_number;           // 计数器值地址
    p_value       = all_data + 0x0800 + timer_number;           //
    p_reset_coil  = all_data + 0x0380 + (timer_number / 0x10);  // 复位线圈
    p_over_coil   = all_data + 0x0100 + (timer_number / 0x10);  // 溢出线圈
    p_enable_coil = all_data + 0x0280 + (timer_number / 0x10);  // 使能线圈
    *p_reset_coil &= ~(1 << timer_number % 0x10);               // 复位线圈
    *p_over_coil &= ~(1 << timer_number % 0x10);                // 溢出线圈
    *p_enable_coil &= ~(1 << timer_number % 0x10);              // 使能线圈
    *p_data_given = 0;
    *p_value      = 0;
}

void T_10MS(void)
{
    u16 timer_count;
    for (timer_count = 200; timer_count < 246; timer_count++) {
        p_data_given  = all_data + 0x1000 + timer_count;
        p_value       = all_data + 0x0800 + timer_count;
        p_enable_coil = all_data + 0x0280 + (timer_count / 0x10);
        if ((*p_enable_coil & (1 << timer_count % 0x10)) == (1 << timer_count % 0x10))  // 线圈壮态
        {
            if (*p_value < *p_data_given)  // 值壮态
                *p_value += 1;
        }
    }
}

void T_1MS(void)
{
    u16 timer_count;
    for (timer_count = 246; timer_count < 250; timer_count++) {
        p_data_given  = all_data + 0x1000 + timer_count;
        p_value       = all_data + 0x0800 + timer_count;
        p_enable_coil = all_data + 0x0280 + (timer_count / 0x10);
        if ((*p_enable_coil & (1 << timer_count % 0x10)) == (1 << timer_count % 0x10))  // 线圈壮态
        {
            if (*p_value < *p_data_given)  // 值壮态
                *p_value += 1;
        }
    }
}

void T_H100MS(void)
{
    u16 timer_count;

    for (timer_count = 250; timer_count < 256; timer_count++) {
        p_data_given  = all_data + 0x1000 + timer_count;
        p_value       = all_data + 0x0800 + timer_count;
        p_enable_coil = all_data + 0x0280 + (timer_count / 0x10);
        if ((*p_enable_coil & (1 << timer_count % 0x10)) == (1 << timer_count % 0x10))  // 线圈壮态
        {
            if (*p_value < *p_data_given)  // 值壮态
                *p_value += 1;
        }
    }
}

void TIM5_IRQHandler(void)  // 1ms产生一次中断信号
{
    static u8  all_clock;
    static u16 minute, test;
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        all_clock++;
        all_data[0x070C]++;
        if (all_clock > 99)  // m8011 10MS m8012 100MS  m8013 1SEC m8014 1minute
            all_clock = 0, force_set(0X00E0, 12);
        T_1MS();
        if ((all_clock % 10) == 7)  // 10ms定时器设计每计五次刷新一次
            T_10MS(), force_reset(0X00E0, 11);
        if ((all_clock % 10) == 2)
            force_set(0X00E0, 11);
        if (all_clock == 50)  // 两种100MS定时器分开刷新
            T_100MS(), force_reset(0X00E0, 12);
        if (all_clock == 90)  // 每100ms秒钟分钟定时器
            T_H100MS(), minute++;
        if (minute % 10 == 5)  // 刷新秒钟 8013
            force_reset(0X00E0, 13);
        if (minute % 10 == 0)
            force_set(0X00E0, 13);
        if (minute == 300)  // 刷新分钟 8014
            force_reset(0X00E0, 14);
        if (minute == 0)
            force_set(0X00E0, 14);
        if (minute > 599)
            minute = 0;
        x_filter();  // 检查X输入壮态值
        test++;
    }
}

void TIM5_Init(void)  // 1ms timer
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseStructure.TIM_Period        = (100 - 1);
    TIM_TimeBaseStructure.TIM_Prescaler     = (720 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM5, ENABLE);
}
