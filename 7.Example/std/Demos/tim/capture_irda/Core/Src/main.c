#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "irda/irda.h"

int main(void)
{
    uint8_t key;

    board_init();
    irda_init();

    while (1) {
        key = irda_scan();
        if (key) LOGD("%d,0x%02X", irda_cnt, key);
    }
}
