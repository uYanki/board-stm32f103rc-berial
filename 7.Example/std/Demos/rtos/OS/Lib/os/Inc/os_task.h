#ifndef __OS_TASK_H__
#define __OS_TASK_H__

#define os_inline __inline

#include "os_core.h"

// 任务控制块 task control block
typedef struct {
    // 任务堆栈 task stack pointer
    uint32_t* StkPtr;
    uint32_t  n_dly;
} OS_TaskTCB;

static OS_TaskTCB TCB_Task[OS_MAX_TASK];

// 任务就绪表, task ready table
uint32_t OSRdyTbl;

// 当前任务优先级, priority of current task
uint8_t OSPrioCur;

// 已就绪任务的最高优先级, highest priority of ready task
uint32_t OSPrioHighRdy;

typedef void (*os_task)(void* arg);

/**
 * @brief create a task
 *
 * @param [in] task task callback
 * @param [in] stk  task stack
 * @param [in] prio task priority ( highest:0, lowest:OS_MAX_TASK-1 )
 *
 */
void osTaskCreate(os_task task, const char* name, uint32_t* stk, void* arg, uint8_t prio)
{
    uint32_t* p_stk;

    p_stk                 = (uint32_t*)((uint32_t)(task)&0xFFFFFFF8u);

    // Cortex-M3 的栈是向下生长

    *(--p_stk)            = (uint32_t)0x01000000uL;
    *(--p_stk)            = (uint32_t)task;  // entry point

    *(--p_stk)            = (uint32_t)0x12121212uL;  // R12
    *(--p_stk)            = (uint32_t)0x03030303uL;  // R3
    *(--p_stk)            = (uint32_t)0x02020202uL;  // R2
    *(--p_stk)            = (uint32_t)0x01010101uL;  // R1
    *(--p_stk)            = (uint32_t)0x00000000uL;  // R0

    *(--p_stk)            = (uint32_t)0x11111111uL;  // R11
    *(--p_stk)            = (uint32_t)0x10101010uL;  // R10
    *(--p_stk)            = (uint32_t)0x09090909uL;  // R9
    *(--p_stk)            = (uint32_t)0x08080808uL;  // R8
    *(--p_stk)            = (uint32_t)0x07070707uL;  // R7
    *(--p_stk)            = (uint32_t)0x06060606uL;  // R6
    *(--p_stk)            = (uint32_t)0x05050505uL;  // R5
    *(--p_stk)            = (uint32_t)0x04040404uL;  // R4

    TCB_Task[prio].StkPtr = p_stk;
    TCB_Task[prio].n_dly  = 0;

    osSetPrioRdy(prio);
}

os_inline void osSetPrioRdy(uint8_t prio)
{
    OSRdyTbl |= 0x01 << prio;
}

os_inline void osDelPrioRdy(uint8_t prio)
{
    OSRdyTbl &= ~(0x01 << prio);
}

os_inline void osGetHighRdy(void)
{
    uint8_t prio = 0;

    while (prio < OS_MAX_TASK) {
        if (OSRdyTbl & (0x01 << prio))
            break;
        ++prio;
    }

    OSPrioHighRdy = prio;
}

void osIDLETask(void)
{
    while (1) {
        // __ASM volatile("WFE");
    }

    // WFE: wait for event
    // WFI: wait for interrupt
}

// https://blog.csdn.net/guet_kite/article/details/76034771 [3]
#define OS_STK_SIZE   10
#define IDLE_STK_SIZE 10

uint32_t  CPU_Stk[OS_STK_SIZE];
uint32_t* CPU_StkBase;

uint32_t IDLE_STK[IDLE_STK_SIZE];

// p_TCBHighRdy

OS_TaskTCB* pCurTaskTCB;
OS_TaskTCB* pHighRdyTCB;

void osStart(void)
{
    CPU_StkBase = CPU_Stk + OS_STK_SIZE - 1;
    osTaskCreate(osIDLETask, &IDLE_STK[IDLE_STK_SIZE - 1], OS_MAX_TASK - 1);  // 空闲任务占用最低优先级
    osGetHighRdy();
    OSPrioCur   = OSPrioHighRdy;
    pHighRdyTCB = &TCB_Task[OSPrioHighRdy];
    osStartHighRdy();

    NVIC_SetPriority(UART4_IRQn, 2);
}

void osStartHighRdy() {}

// load data to register

/**
osStartHighRdy;
LDR  R0,=NVIC_SYSPRI14
LDR  R1,=NVIC_PENDSV_PRI
STRB R1,[R0]

MOVS R0,#0; mov 
MSR  PSP,R0

LDR R0,=CPU_StkBase
LDR R1,[R0];
MSR MSP,R1;

LDR R0,=NVIC_INT_CTRL
LDR R1,=NVIC_PENDSVSET
STR R1,[R0];触发PendSV异常

CPSIE I;开中断
*/

/*

- 异常
SVC: Supervisor Call
PendVC: Pendable Service Call
- 通过向 NVIC Interrupt Control State Register 中的 PENDSVSET 置1来产生

FreeRTOS 中断

SVC_Handler: 启动首个

PendSV_Handler: 任务切换

SysTick_Handler:

*/

#endif
