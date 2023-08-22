// configuare template for 'board_conf.h'
#if 0

#ifndef __BOARD_CONF_H__
#define __BOARD_CONF_H__

#define USE_KEY       0
#define USE_IRQ_KEY   0
#define USE_LED       0

#define USE_PINCTRL   1

/* UART */

#define USE_UART1     0
#define USE_UART2     1

#define USE_IRQ_U1RX  0
#define USE_IRQ_U2RX  1

// baudrate
#define CONFIG_U1BAUD 115200
#define CONFIG_U2BAUD 115200

#if USE_UART2
#undef USE_IRQ_U1RX
#endif

// 1; Poll 2:IRQ_U1RX 4:IRQ_U2RX
#define USE_CMD_PARSER 4

#endif

#endif

#include "board.h"

static void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    (void)GPIO_InitStructure;
    (void)EXTI_InitStructure;
    (void)NVIC_InitStructure;

#if USE_LED
    // init GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  // 推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if USE_KEY
    // init GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // 下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#if USE_IRQ_KEY
    // init EXTI
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_Init(&EXTI_InitStructure);
    // init NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&NVIC_InitStructure);
#endif
#endif
}

#if USE_UART

void uart_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    (void)GPIO_InitStructure;
    (void)USART_InitStructure;
    (void)NVIC_InitStructure;

#if USE_UART1

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // USART1_TX
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART1_RX
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART Init
    USART_InitStructure.USART_BaudRate            = CONFIG_U1BAUD;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

#if USE_IRQ_U1RX
    // USART1_NVIC
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

    USART_Cmd(USART1, ENABLE);

#endif

#if USE_UART2

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // USART2_TX
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2_RX
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART Init
    USART_InitStructure.USART_BaudRate            = CONFIG_U2BAUD;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

#if USE_IRQ_U2RX
    // USART2_NVIC
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif

    USART_Cmd(USART2, ENABLE);

#endif
}

//************************//

#if !defined(__MICROLIB)

// redirect printf (without MicroLIB)

#pragma import(__use_no_semihosting)

struct __FILE {
    int handle;
};

FILE __stdout;

// 避免半主机模式

#if 1
void
#endif
_sys_exit(int x)
{
    x = x;
}

#endif

int fputc(int ch, FILE* f)
{
#if USE_UART1
    while ((USART1->SR & USART_SR_TC) == 0) {}
    USART1->DR = (uint8_t)ch;
#endif
#if USE_UART2
#if 0
    // TC: Transmission Complete
    while (RESET == USART_GetFlagStatus(USART2, USART_SR_TC)) {};
    USART_SendData(USART2, ch);
#else
    while ((USART2->SR & USART_SR_TC) == 0) {}
    USART2->DR = (uint8_t)ch;
#endif
#endif
    return ch;
}

#if 0
int fgetc(FILE* f)
{
    while (RESET == USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {}
    return (int)USART_ReceiveData(USART2);
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {}
    return (int)USART_ReceiveData(USART1);
}
#endif

#if USE_IRQ_U1RX

uint8_t uart1_rxflg              = 0;
uint8_t uart1_rxbuf[uart1_rxcap] = {0};

void USART1_IRQHandler(void)
{
    static uint8_t idx = 0;

    uint8_t buf;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        buf = USART_ReceiveData(USART1);

        if (uart1_rxflg == 0) {
            switch (buf) {
                case '\r': break;
                case '\n': {
                    // 换行符 -> 接收完成
                    uart1_rxbuf[idx] = '\0';
                    idx              = 0;
#if USE_CMD_PARSER & 2
                    uart1_rxflg = !cmd_parse((const char*)uart1_rxbuf);
#else
                    uart1_rxflg = 1;
#endif
                    break;
                }
                default:
                    uart1_rxbuf[idx++] = buf;
                    if (idx == uart1_rxcap) idx = 0;
                    break;
            }
        }
    }
}

#endif

#if USE_IRQ_U2RX

uint8_t uart2_rxflg              = 0;
uint8_t uart2_rxbuf[uart2_rxcap] = {0};

void USART2_IRQHandler(void)
{
    static uint8_t idx = 0;

    uint8_t buf;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        buf = USART_ReceiveData(USART2);

        if (uart2_rxflg == 0) {
            switch (buf) {
                case '\r': break;
                case '\n': {
                    // 换行符 -> 接收完成
                    uart2_rxbuf[idx] = '\0';
                    idx              = 0;
#if USE_CMD_PARSER & 4
                    uart2_rxflg = !cmd_parse((const char*)uart2_rxbuf);
#else
                    uart2_rxflg = 1;
#endif
                    break;
                }
                default:
                    uart2_rxbuf[idx++] = buf;
                    if (idx == uart2_rxcap) idx = 0;
                    break;
            }
        }
    }
}

#endif

#endif

void board_init(void)
{
    gpio_init();
#if USE_UART
    uart_init();
#if USE_CMD_PARSER
    cmd_init();
#endif
#endif
}
