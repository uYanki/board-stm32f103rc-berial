#include "board.h"

#define OI_TXD        PCout(13)
#define OI_RXD        PBin(9)

#define BuadRate_9600 100

uint8_t len = 0;        // 接收计数
uint8_t USART_buf[11];  // 接收缓冲区

enum {
    COM_START_BIT,
    COM_D0_BIT,
    COM_D1_BIT,
    COM_D2_BIT,
    COM_D3_BIT,
    COM_D4_BIT,
    COM_D5_BIT,
    COM_D6_BIT,
    COM_D7_BIT,
    COM_STOP_BIT,
};

uint8_t recvStat = COM_STOP_BIT;
uint8_t recvData = 0;

void IO_TXD(uint8_t Data)
{
    uint8_t i = 0;
    OI_TXD    = 0;
    delay_us(BuadRate_9600);
    for (i = 0; i < 8; i++) {
        if (Data & 0x01)
            OI_TXD = 1;
        else
            OI_TXD = 0;

        delay_us(BuadRate_9600);
        Data = Data >> 1;
    }
    OI_TXD = 1;
    delay_us(BuadRate_9600);
}

void USART_Send(u8* buf, uint8_t len)
{
    uint8_t t;
    for (t = 0; t < len; t++) {
        IO_TXD(buf[t]);
    }
}

void IOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStruct;

    // TXD
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    // RXD
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
    EXTI_InitStruct.EXTI_Line    = EXTI_Line9;
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发中断
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM4_Int_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // TIM
    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    // NVIC
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

int main(void)
{
    // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    IOConfig();
    TIM4_Int_Init(107, 71);  // 1MHz

    while (1) {
        if (len > 10) {
            len = 0;
            USART_Send(USART_buf, 11);
        }
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetFlagStatus(EXTI_Line9) != RESET) {
        if (OI_RXD == 0) {
            if (recvStat == COM_STOP_BIT) {
                recvStat = COM_START_BIT;
                TIM_Cmd(TIM4, ENABLE);
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != RESET) {
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
        recvStat++;
        if (recvStat == COM_STOP_BIT) {
            TIM_Cmd(TIM4, DISABLE);
            USART_buf[len++] = recvData;
            return;
        }
        if (OI_RXD) {
            recvData |= (1 << (recvStat - 1));
        } else {
            recvData &= ~(1 << (recvStat - 1));
        }
    }
}
