#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "irda/irda.h"

int main(void)
{
    uint8_t key;

    board_init();
    irda_init();

    for (;;) {
        if (irda_flag == 1) {
            key = irda_handler();
            if (key) LOGD("0x%02X,%d", key, irda_cnt);
        }
    }
}
