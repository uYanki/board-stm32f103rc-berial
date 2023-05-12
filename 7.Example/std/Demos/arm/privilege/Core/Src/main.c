#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

    // 指定应用程序的工作模式
    /* bit0: 0 特权模式, 1 非特权模式 */
    __set_CONTROL(0x01);

    LOGD("hello");

    // 非特权模式下在中断外操作 cortex 内核的寄存器会触发异常
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    // NVIC_SystemReset(); // 芯片软复位

    for (;;)
        ;
}
