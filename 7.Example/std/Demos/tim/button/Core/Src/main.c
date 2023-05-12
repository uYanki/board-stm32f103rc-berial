#include "board.h"
#include "button/button.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

button_t button1;

void btncbk_down_press(void* btn) { LOGD("click"); }

void btncbk_double_press(void* btn) { LOGD("double press"); }

void btncbk_long_press(void* btn) { LOGD("long press"); }

void btncbk_continuos_press(void* btn) { LOGD("continuos press"); }

void btncbk_release_after_continuos_press(void* btn) { LOGD("release after continuos press"); }

uint8_t btn_read_level(void)
{
    return GPIO_ReadInputDataBit(KEY_GPIO_Port, KEY_GPIO_Pin);
}

int main(void)
{
    board_init();

    timer_init(2, 7200, 200);  // 20ms

    button_create("btn", &button1, btn_read_level, Bit_SET);

    button_attach(&button1, BUTTON_DOWM, btncbk_down_press);      // 单击
    button_attach(&button1, BUTTON_DOUBLE, btncbk_double_press);  // 双击
    button_attach(&button1, BUTTON_LONG, btncbk_long_press);      // 长按

    // 需开启宏 CONTINUOS_TRIGGER
    button_attach(&button1, BUTTON_CONTINUOS, btncbk_continuos_press);                     // 连按
    button_attach(&button1, BUTTON_CONTINUOS_FREE, btncbk_release_after_continuos_press);  // 连按后释放

    while (1) {
        if (timer_check(2)) {
            // LOGI("do button cycle");
            button_cycle_process();
        }
    }
}
