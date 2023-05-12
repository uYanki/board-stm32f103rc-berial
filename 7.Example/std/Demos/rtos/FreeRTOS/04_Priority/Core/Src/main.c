#include "board.h"

#include "task.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void vTask1(void* arg);
void vTask2(void* arg);

TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;

int main(void)
{
    board_init();

    // prio: task1 < task2
    xTaskCreate(vTask1, "Task", 512, NULL, 1, &xTask1Handle);
    xTaskCreate(vTask2, "Task", 512, NULL, 2, &xTask2Handle);
    vTaskStartScheduler();
}

void vTask1(void* arg)
{
    // get current task priority
    UBaseType_t uxPriority = uxTaskPriorityGet(NULL);  // 3

    for (;;) {
        LOGD("task1");
        // prio: task1 < task2
        vTaskPrioritySet(NULL, uxPriority - 2);  // 1
    }
}

void vTask2(void* arg)
{
    // get current task priority
    UBaseType_t uxPriority = uxTaskPriorityGet(NULL);  // 2

    for (;;) {
        LOGD("task2");
        // prio: task1 > task2
        vTaskPrioritySet(xTask1Handle, uxPriority + 1);  // 3
    }
}
