#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#if 0
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
#else
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;	
#endif
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

    for (;;) {
        delay_ms(50);
        println("%d", GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0));
    }
}
