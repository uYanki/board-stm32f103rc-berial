#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    __IO uint32_t a1 = 0x11111111;
    __IO uint32_t b1 = 0x22222222;
    __IO uint32_t c1 = 0x33333333;
    __IO uint32_t d1 = 0x44444444;
    __IO uint32_t e1 = 0x55555555;
    __IO uint32_t f1 = 0x66666666;
    __IO uint32_t g1 = 0x77777777;
    __IO uint32_t h1 = 0x88888888;

    board_init();

    __set_PSP(__get_MSP());
    __set_CONTROL(0x02);  // bit1: 0: use psp, 1: use msp

    {
        __IO uint32_t a2 = 0x11111111;
        __IO uint32_t b2 = 0x22222222;
        __IO uint32_t c2 = 0x33333333;
        __IO uint32_t d2 = 0x44444444;
        __IO uint32_t e2 = 0x55555555;
        __IO uint32_t f2 = 0x66666666;
        __IO uint32_t g2 = 0x77777777;
        __IO uint32_t h2 = 0x88888888;
    }
		
		delay_us(1);

    for (;;)
        ;
}
