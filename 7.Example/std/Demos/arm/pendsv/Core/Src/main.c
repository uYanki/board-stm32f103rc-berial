#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

static void PrintHello(void);

uint32_t TaskStack[100];  // for psp

/**
 * P13:
 * - 创建伪现场时，由软件手动传入的函数入口
 * - 上下文切换时，由硬件自动置入当前运行位置
 */

int main(void)
{
    uint32_t* p_stk;

    board_init();

    __set_PSP(__get_MSP());
    __set_CONTROL(0x02);  // bit1: 0:use msp, 1:use psp

    /* 创建伪现场 */
    p_stk      = &TaskStack[100];

    /* 硬件自动入栈部分 */
    *(--p_stk) = (uint32_t)0x01000000;  // xPSR
    *(--p_stk) = (uint32_t)PrintHello;  // P13(PC)：Entry Point
    *(--p_stk) = (uint32_t)0x00000000;  // R14(LR)
    *(--p_stk) = (uint32_t)0xAAAAAAAA;  // R12
    *(--p_stk) = (uint32_t)0xBBBBBBBB;  // R3
    *(--p_stk) = (uint32_t)0xCCCCCCCC;  // R2
    *(--p_stk) = (uint32_t)0xDDDDDDDD;  // R1
    *(--p_stk) = (uint32_t)0xEEEEEEEE;  // R0：argument

    /* 软件手动入栈部分 */
    *(--p_stk) = (uint32_t)0x11111111;  // R11
    *(--p_stk) = (uint32_t)0x22222222;  // R10
    *(--p_stk) = (uint32_t)0x33333333;  // R9
    *(--p_stk) = (uint32_t)0x44444444;  // R8
    *(--p_stk) = (uint32_t)0x55555555;  // R7
    *(--p_stk) = (uint32_t)0x66666666;  // R6
    *(--p_stk) = (uint32_t)0x77777777;  // R5
    *(--p_stk) = (uint32_t)0x88888888;  // R4

    /* 触发 PendSV 中断 */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    LOGD("Hello World");  // 不执行

    for (;;)
        ;
}

void PrintHello(void)
{
    LOGD("Hello PendSV");  // 执行

    for (;;)
        ;
}
