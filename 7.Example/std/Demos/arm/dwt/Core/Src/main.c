#include "board.h"

#include "dwt/dwt.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

    dwt_init();

    for (;;) {
        println("1");
        dwt_delay(2e6);
        println("2");
        dwt_delay(4e6);
    }
}
