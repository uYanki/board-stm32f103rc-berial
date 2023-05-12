#include "board.h"

#include "task.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void vTask1(void* arg);
void vTask2(void* arg);

bool flag;

int main(void)
{
    board_init();
	
    xTaskCreate(vTask1, "Task1", 512, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", 512, NULL, 2, NULL);
    vTaskStartScheduler();
}

void vTask1(void* arg)
{
    for (;;) {
        flag = false;
    }
}

void vTask2(void* arg)
{
    TickType_t const xDelayTime    = pdMS_TO_TICKS(50UL),
                     xLastWakeTime = xTaskGetTickCount();

    uint16_t i                     = 0;

    for (;;) {
        flag = true;
        while (++i) {}
#if 1
        vTaskDelay(xDelayTime);
#else
        xTaskDelayUntil(&xLastWakeTime, xDelayTime);
#endif
    }
}


