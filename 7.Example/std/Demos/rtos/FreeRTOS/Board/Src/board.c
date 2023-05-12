#include "board.h"

static void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

#if USE_USER_LED
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  // 推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if USE_USER_KEY
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // 下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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

#if USE_IRQ_U1RX
    // USART1_NVIC
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

    // USART Init
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
#if USE_IRQ_U1RX
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
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

#if USE_IRQ_U2RX
    // USART2_NVIC
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

    USART_Init(USART2, &USART_InitStructure);
#if USE_IRQ_U2RX
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
    while ((USART1->SR & 0X40) == 0) {}
    USART1->DR = (uint8_t)ch;
#endif
#if USE_UART2
    while ((USART2->SR & 0X40) == 0) {}
    USART2->DR = (uint8_t)ch;
#endif
    return ch;
}

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
                    uart1_rxflg      = 1;
                    uart1_rxbuf[idx] = '\0';
                    idx              = 0;
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
                    uart2_rxflg      = 1;
                    uart2_rxbuf[idx] = '\0';
                    idx              = 0;
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
#if 0
    // irq freq = 72MHz / 72000 = 1000Hz
    while (SysTick_Config(72000))
        ;
#endif
    gpio_init();
    uart_init();
}
