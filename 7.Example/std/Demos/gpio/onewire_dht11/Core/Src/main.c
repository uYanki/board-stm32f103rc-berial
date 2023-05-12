#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "dht11/dht11.h"

int main(void)
{
    dht11_data_t data;

    board_init();
    dht11_init();
    for (;;) {
        if (dht11_read_temp_and_humidity(&data)) {
           LOGD("humi = %d.%d, temp = %d.%d", data.humi_int, data.humi_deci, data.temp_int, data.temp_deci);
        } else {
           LOGD("fail");
        }
        delay_ms(1000);
    }
}
