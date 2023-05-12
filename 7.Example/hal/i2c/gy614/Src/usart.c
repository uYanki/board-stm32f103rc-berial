#include "usart.h"
#include "string.h"
u8   usart_rx_data[30] = {0}, Receive_ok = 0;
void Usart_Int(uint32_t BaudRatePrescaler)
{
    GPIO_InitTypeDef  GPIO_usartx;
    USART_InitTypeDef Usart_X;
    /////////////////////////////////
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // USART1_TX   PA.9
    GPIO_usartx.GPIO_Pin   = GPIO_Pin_9;
    GPIO_usartx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_usartx.GPIO_Mode  = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_usartx);
    // USART1_RX	  PA.10
    GPIO_usartx.GPIO_Pin  = GPIO_Pin_10;
    GPIO_usartx.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
    GPIO_Init(GPIOA, &GPIO_usartx);

    Usart_X.USART_BaudRate            = BaudRatePrescaler;
    Usart_X.USART_WordLength          = USART_WordLength_8b;  // 8位数据格式
    Usart_X.USART_StopBits            = USART_StopBits_1;     // 1位停止位
    Usart_X.USART_Parity              = USART_Parity_No;
    Usart_X.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    Usart_X.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &Usart_X);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 开启接收中断
    USART_Cmd(USART1, ENABLE);
    /////////////////////////////////
}
// 发送一个字节数据
// input:byte,待发送的数据
void USART1_send_byte(uint8_t byte)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;  // 等待发送完成
    USART1->DR = byte;
}
// 发送多字节数据
void USART_Send_bytes(uint8_t* Buffer, uint8_t Length)
{
    uint8_t i = 0;
    while (i < Length) {
        USART1_send_byte(Buffer[i++]);
    }
}
// 发送多字节数据+校验和
void USART_Send(uint8_t* Buffer, uint8_t Length)
{
    uint8_t i = 0;
    while (i < Length) {
        if (i < (Length - 1))
            Buffer[Length - 1] += Buffer[i];  // 累加Length-1前的数据
        USART1_send_byte(Buffer[i++]);
    }
}
void send_3out(uint8_t* data, uint8_t length, uint8_t send)
{
    uint8_t TX_DATA[30] = {0}, i = 0, k = 0;
    TX_DATA[i++] = 0X5A;          // 帧头
    TX_DATA[i++] = 0X5A;          // 帧头
    TX_DATA[i++] = send;          // 功能字节
    TX_DATA[i++] = length;        // 数据个数
    for (k = 0; k < length; k++)  // 存入数据到缓存TX_DATA
    {
        TX_DATA[i++] = (uint8_t)data[k];
    }
    USART_Send(TX_DATA, length + 5);
}
// 发送一帧数据
void send_out(int16_t* data, uint8_t length, uint8_t send)
{
    uint8_t TX_DATA[30], i = 0, k = 0;
    memset(TX_DATA, 0, (2 * length + 5));  // 清零缓存TX_DATA
    TX_DATA[i++] = 0X5A;                   // 帧头
    TX_DATA[i++] = 0X5A;                   // 帧头
    TX_DATA[i++] = send;                   // 功能字节
    TX_DATA[i++] = 2 * length;             // 数据个数
    for (k = 0; k < length; k++)           // 存入数据到缓存TX_DATA
    {
        TX_DATA[i++] = (uint16_t)data[k] >> 8;
        TX_DATA[i++] = (uint16_t)data[k];
    }
    USART_Send(TX_DATA, 2 * length + 5);
}
void display(int16_t* num, u8 send, u8 count)
{
    u8 i = 0;
    USART1_send_byte(0X0d);
    USART1_send_byte(0X0a);
    USART1_send_byte(send);
    USART1_send_byte(':');
    for (i = 0; i < count; i++) {
        if (num[i] < 0) {
            num[i] = -num[i];
            USART1_send_byte('-');
        } else
            USART1_send_byte('+');

        USART1_send_byte(0x30 | (num[i] / 10000));
        USART1_send_byte(0x30 | (num[i] % 10000 / 1000));
        USART1_send_byte(0x30 | (num[i] % 1000 / 100));
        // USART_send_byte(0x2e);
        USART1_send_byte(0x30 | (num[i] % 100 / 10));
        USART1_send_byte(0x30 | (num[i] % 10));
        USART1_send_byte(',');
    }
}
void CHeck(uint8_t* re_data)
{
}
