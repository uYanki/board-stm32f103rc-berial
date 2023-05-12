#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "sdcard/sdio_test.h"

int main(void)
{
    board_init();

    sd_test();

    for (;;)
        ;
}
