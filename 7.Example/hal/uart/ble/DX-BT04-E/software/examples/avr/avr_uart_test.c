

// ICC-AVR application builder : 2007-5-20 17:21:25
// Target : M16
// Crystal: 7.3728Mhz
// AVR mega16 串口测试
// AVR与虚拟仪器

#include <iom16v.h>
#include <macros.h>

#define F_CPU 7372800

const unsigned char buffer[] = {"www.szdx-smart.com"};

//******************************************************************************
// 串口初始化
void USART_Init(unsigned int baud)
{
    unsigned int tmp;

    tmp   = F_CPU / baud / 16 - 1;
    UBRRH = (unsigned char)(tmp >> 8);
    UBRRL = (unsigned char)tmp;
    UCSRB = (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1);
}

// ICC 生成的初始化
// UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9600 (0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
    UCSRB = 0x00;  // disable while setting baud rate
    UCSRA = 0x00;
    UCSRC = BIT(URSEL) | 0x06;
    UBRRL = 0x2F;  // set baud rate lo
    UBRRH = 0x00;  // set baud rate hi
    UCSRB = 0x18;
}

//******************************************************************************
// 下面两个函数直接从数据手册上拷贝过来的

// 数据发送【发送5 到8 位数据位的帧】
void USART_Transmit(unsigned char data)
{
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = data;
}

// 数据接收【以5 到8 个数据位的方式接收数 据帧】
unsigned char USART_Receive(void)
{
    while (!(UCSRA & (1 << RXC)))
        ;
    return UDR;
}

// 连续发送字符
void USART_Transmit_2(void)
{
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = 'H';
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = 'e';
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = 'l';
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = 'l';
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = 'o';
}

//******************************************************************************
void main(void)
{
    unsigned char n = 0, tmp = 0;

    USART_Init(9600);  // 波特率9600 初始化串口
    // uart0_init();

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
