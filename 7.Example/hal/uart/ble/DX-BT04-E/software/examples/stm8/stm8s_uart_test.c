

// 提供MCU寄存器的定义
#include "STM8S103F.h"

const unsigned char buffer[] = {"http://www.szdx-smart.com"};

//******************************************************************************
// 串口初始化
void USART_Init(unsigned int baud)
{
    CLK_CKDIVR = 0;  // 16MHz

    // 波特率16MHz时钟时为9600
    UART1_BRR2 = 0x02;
    UART1_BRR1 = 0x68;
    // 停止位
    UART1_CR3 &= 0xcf;  // 1 stop bit
    // 奇偶校验  数据位数
    UART1_CR1 &= 0xe9;  // none
    // 使能收发
    UART1_CR2 |= 0x0c;
}

//******************************************************************************
// 数据发送
void USART_Transmit(unsigned char data)
{
    while (!(UART1_SR & 0x80))
        ;
    UART1_DR = data;
}

// 数据接收
unsigned char USART_Receive(void)
{
    while (!(UART1_SR & 0x20))
        ;
    return UART1_DR;
}

// 连续发送字符
void USART_Transmit_2(void)
{
    while (!(UART1_SR & 0x80))
        ;
    UART1_DR = 'H';
    while (!(UART1_SR & 0x80))
        ;
    UART1_DR = 'e';
    while (!(UART1_SR & 0x80))
        ;
    UART1_DR = 'l';
    while (!(UART1_SR & 0x80))
        ;
    UART1_DR = 'l';
    while (!(UART1_SR & 0x80))
        ;
    UART1_DR = 'o';
}

//******************************************************************************
void main(void)
{
    unsigned char n = 0, tmp = 0;

    USART_Init();  // 波特率9600 初始化串口

    // 数据发送
    for (i = 0; n < 20; n++) {  // 发送数组里面的字符串
        USART_Transmit(buffer[n]);
    }
    USART_Transmit(0x0d);  // 发送一个回车
    USART_Transmit(0x0a);  // 发送一个换行

    // 数据接收
    while (1) {
        if (UCSRA & (1 << RXC)) {   // 如果接收缓存区有数据

            tmp = USART_Receive();  // 接收数据
            USART_Transmit(tmp);    // 发送数据

            if (tmp == 'a') {       // 回复接收到的数据，如果是a，再发一个OK回来
                USART_Transmit('O');
                USART_Transmit('K');
            }

            if (tmp == 'A') {  // 回复接收到的数据，如果是A，再发一个Hello回来
                USART_Transmit_2();
            }
        }
    }
}
