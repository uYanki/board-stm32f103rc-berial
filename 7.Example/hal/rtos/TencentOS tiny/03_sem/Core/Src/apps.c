// https://www.bookstack.cn/read/TencentOS-tiny/spilt.2.spilt.2.bc888230ee452d75.md

#include "main.h"
#include "tos_k.h"

// 任务栈大小
#define STK_SIZE_TASK_PRIO4 512
#define STK_SIZE_TASK_PRIO5 1024

// 任务栈空间
k_stack_t stack_task_prio4[STK_SIZE_TASK_PRIO4];
k_stack_t stack_task_prio5[STK_SIZE_TASK_PRIO5];

// 任务体
k_task_t task_prio4;
k_task_t task_prio5;

// 任务体入口
extern void entry_task_prio4(void* arg);
extern void entry_task_prio5(void* arg);

// 任务定义

void entry_task_prio4(void* arg)
{
    while (K_TRUE) {
        tos_kprintln("task_prio4 body");
        tos_task_delay(1000);  // delay 1s
    }
}

void entry_task_prio5(void* arg)
{
    tos_kprintln("[arg] %s\n", (char*)arg);

    uint32_t i = 0;
    while (K_TRUE) {
        if (i == 2) {
            // 挂起任务 task_prio4，即停止运行
            tos_kprintln("i = %d", i);
            tos_task_suspend(&task_prio4);
        } else if (i == 4) {
            // 恢复任务 task_prio4，即继续运行
            tos_kprintln("i = %d", i);
            tos_task_resume(&task_prio4);
        } else if (i == 6) {
            // 删除任务 task_prio4
            tos_kprintln("i = %d", i);
            tos_task_destroy(&task_prio4);
        }
        tos_kprintln("task_prio5 body");
        tos_task_delay(1000);
        ++i;
    }
}

void application_entry(void* arg)
{
    // task create
    tos_task_create(&task_prio4, "task_prio4", entry_task_prio4, 0, 4, stack_task_prio4, STK_SIZE_TASK_PRIO4, 0);
    tos_task_create(&task_prio5, "task_prio5", entry_task_prio5, (void*)"hello world", 5, stack_task_prio5, STK_SIZE_TASK_PRIO5, 0);
}
