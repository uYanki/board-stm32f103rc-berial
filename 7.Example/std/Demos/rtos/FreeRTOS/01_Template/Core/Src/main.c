#include "board.h"

#include "task.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void vTaskLED(void*);

static TaskHandle_t hTaskLED = NULL;

int main(void)
{
    board_init();

    xTaskCreate(vTaskLED, "LED", 512, "hello world!", 1, &hTaskLED);
    vTaskStartScheduler();
}

void vTaskLED(void* arg)
{
    for (;;) {
        println("%s", (char*)arg);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        GPIO_SetBits(LED_GPIO_Port, LED_GPIO_Pin);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        GPIO_ResetBits(LED_GPIO_Port, LED_GPIO_Pin);
    }

    /* delete current task */
    vTaskDelete(NULL);
}
