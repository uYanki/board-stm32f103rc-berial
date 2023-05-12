#include "board.h"

//===================================================================//

void uart_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

#if USE_UART1

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

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
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

#endif

#if USE_UART2

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

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

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

#endif
}

//************************//

// redirect printf (without MicroLIB)
#if !defined(__MICROLIB)

// #pragma import(__use_no_semihosting)
__asm(".global __use_no_semihosting\n\t");

// 避免使用半主机模式
void _sys_exit(int x) { x = x; }

//__use_no_semihosting was requested, but _ttywrch was
void _ttywrch(int ch) { ch = ch; }

#if 0
struct __FILE {
    int handle;
};
#endif
FILE __stdout;

#endif

#if defined(__GNUC__) && !defined(__clang__)
int __io_putchar(int ch)
#else
int fputc(int ch, FILE* f)
#endif
{
    // TXE: TX Enable

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

//************************//

// redirect scanf (without MicroLIB)

#if USE_UART1
#define DBG_UART USART1
#elif USE_UART2
#define DBG_UART USART2
#endif

#ifdef DBG_UART

FILE __stdin;

int fgetc(FILE* f)
{
    while (RESET == USART_GetFlagStatus(DBG_UART, USART_FLAG_RXNE))
        ;

    return USART_ReceiveData(DBG_UART);
}

#endif

//===================================================================//

void board_init(void)
{
    uart_init();
}
