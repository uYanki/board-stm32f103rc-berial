#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "ds18b20/ds18b20.h"

int main(void)
{
    board_init();
    ds18b20_init();
    for (;;) {
        LOGD("%.5f", ds18B20_read_temperature(NULL));
        delay_ms(200);
    }
}
