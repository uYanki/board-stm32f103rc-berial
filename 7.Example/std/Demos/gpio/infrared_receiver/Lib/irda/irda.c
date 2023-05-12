#include "irda.h"

uint32_t irda_data = 0;  // 帧数据(32bit)
uint8_t  irda_cnt  = 0;  // 按键次数
uint8_t  irda_flag = 0;  // 帧数据接收完成标志

void irda_init(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(IRDA_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

        GPIO_InitStructure.GPIO_Pin  = IRDA_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(IRDA_GPIO_PORT, &GPIO_InitStructure);
    }

    {
        EXTI_InitTypeDef EXTI_InitStructure;

        GPIO_EXTILineConfig(IRDA_GPIO_PORT_SOURCE, IRDA_GPIO_PIN_SOURCE);

        EXTI_InitStructure.EXTI_Line    = IRDA_EXTI_LINE;
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);
    }

    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        NVIC_InitStructure.NVIC_IRQChannel                   = IRDA_EXTI_IRQN;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 5;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}

uint8_t irda_handler(void)
{
    uint8_t bytes[4];

    bytes[0] = irda_data >> 24;           // 字节1, 值 = 遥控 ID
    bytes[1] = (irda_data >> 16) & 0xff;  // 字节2, 值 = 字节1反码
    bytes[2] = irda_data >> 8;            // 字节3, 值 = 键值
    bytes[3] = irda_data;                 // 字节4, 值 = 字节2反码

    irda_flag = 0;

#if 0
    if ((bytes[0] == (uint8_t)~bytes[1]) && (bytes[0] == IRDA_ID)) {
#else
    if (bytes[0] == (uint8_t)~bytes[1]) {
#endif
    if (bytes[2] == (uint8_t)~bytes[3])
        return bytes[2];
}

return 0;
}

void IRDA_EXTI_IRQHANDLER(void)
{
    uint8_t time;       // 高电平持续时间
    uint8_t start = 0;  // 引导码标志位, 数据帧的开始
    uint8_t bit   = 0;  // 数据暂存位

    if (SET == EXTI_GetITStatus(IRDA_EXTI_LINE)) {
        while (1) {
            if (SET == IrDa_DIN()) {
                // 获取高电平持续时间, 若 >=5ms， 则不是有用信号(干扰或连发码)
                time = 0;
                while (SET == IrDa_DIN()) {
                    ++time;
                    // 具体延时多少需使用 DWT 外设进行测量, 此处大约延时 20us
                    delay_us(16);
                    if (time == 250) {
                        goto exit;  // timeout
                    }
                }

                switch (time) {
                    // 数据0, 0.56ms: 0.2ms~1ms
                    case 10 ... 49: {
                        bit = 0;
                        break;
                    }
                    // 数据1, 1.68ms：1ms~2ms
                    case 50 ... 99: {
                        bit = 1;
                        break;
                    }
                    // 连发码, 2.1ms：2ms~4m
                    case 100 ... 199: {
                        irda_flag = 1;  // 帧完成
                        ++irda_cnt;     // 按键次数
                        break;
                    }
                    // 前导位, 4ms~4.5ms
                    case 200 ... 249: {
                        start = 1;
                        break;
                    }
                    default: break;
                }

                if (start) {
                    irda_data <<= 1;
                    irda_data |= bit;
                    irda_cnt = 0;
                }
            }
        }
    exit:
        EXTI_ClearITPendingBit(IRDA_EXTI_LINE);
    }
}
