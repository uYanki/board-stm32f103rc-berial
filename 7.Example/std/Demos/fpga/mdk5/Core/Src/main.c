#include "board.h"

#define TAG       "main"
#define DBG_LVL   SDK_DEBUG

// DOUT<->DIN

#define DOUT_Port GPIOB  // 32
#define DOUT_Pin  GPIO_Pin_4
#define DIN_Port  GPIOB  // 31
#define DIN_Pin   GPIO_Pin_3
#define SCLK_Port GPIOB  // 49
#define SCLK_Pin  GPIO_Pin_5
#define CS_Port   GPIOB  // 48
#define CS_Pin    GPIO_Pin_7

//

#define SCLK_L()  GPIO_ResetBits(SCLK_Port, SCLK_Pin),delay_us(10)
#define SCLK_H()  GPIO_SetBits(SCLK_Port, SCLK_Pin),delay_us(10)

#define DIN()     GPIO_ReadInputDataBit(DIN_Port, DIN_Pin)

#define DOUT_L()  GPIO_ResetBits(DOUT_Port, DOUT_Pin),delay_us(10)
#define DOUT_H()  GPIO_SetBits(DOUT_Port, DOUT_Pin),delay_us(10)

#define CS_L()    GPIO_ResetBits(CS_Port, CS_Pin)
#define CS_H()    GPIO_SetBits(CS_Port, CS_Pin)

void fpga_bus_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = SCLK_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCLK_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = DIN_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DIN_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = CS_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CS_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = DOUT_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DOUT_Port, &GPIO_InitStructure);

    CS_H();
}

void fpga_read_bytes(uint8_t addr, uint8_t len)
{
    uint8_t mask, data;

    CS_L();

    /* mode */

    SCLK_L();
    DOUT_L();
    SCLK_H();

    /* addr */

    mask = 0x80;
    do {
        SCLK_L();
        (addr & mask) ? DOUT_H() : DOUT_L();
        SCLK_H();
    } while (mask >>= 1);

    /* data */

    while (len--) {
        data = 0x00;
        mask = 0x80;
        do {
            SCLK_L();
            SCLK_H();
            if (DIN()) data |= mask;
        } while (mask >>= 1);
        println("0x%02x", data);
    }

    CS_H();
}

int main(void)
{
    board_init();

    fpga_bus_init();

    while (1) {
        fpga_read_bytes(0x00, 4);
        println("------------");
        delay_ms(1000);
    }

    return 0;
}
