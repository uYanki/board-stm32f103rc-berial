#include "board.h"

#include "task.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

uint32_t ulIdleCycleCount = 0UL;

void vTask(void* arg);

int main(void)
{
    board_init();

    xTaskCreate(vTask, "Task", 512, NULL, 1, NULL);
    vTaskStartScheduler();
}

void vApplicationIdleHook(void)
{
    ++ulIdleCycleCount;
}

void vTask(void* arg)
{
    for (;;) {
        LOGD("%d", ulIdleCycleCount);
        vTaskDelay(250 / portTICK_RATE_MS);
    }
}
