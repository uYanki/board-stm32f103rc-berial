#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "28byj-48/28byj-48.h"

uint32_t angle = 90;

void get_angle(void) { println("%d", angle); }

void inc_angle(void) { LOGD("%d", angle += 30); }

void dec_angle(void) { LOGD("%d", angle = ((angle > 30) ? 0 : (angle - 30))); }

void rst_angle(void) { LOGD("%d", angle = 0); }

void do_cc(void) { steeper_rotate(angle, true); }

void do_ccw(void) { steeper_rotate(angle, false); }

REGISTER_CMD(get, get_angle, get current angle);
REGISTER_CMD(inc, inc_angle, increase angle);
REGISTER_CMD(dec, dec_angle, decrease angle);
REGISTER_CMD(rst, rst_angle, reset angle);
REGISTER_CMD(cc, do_cc, do clockwise);
REGISTER_CMD(ccw, do_ccw, do counterclockwise);

// 使用 _list 查看指令表

int main(void)
{
    board_init();

    stepper_init();

    for (;;) {
        // 处理不支持的指令
        if (uart2_rxflg) {
            uart2_rxflg = 0;
            println("unsupport")
        }
    }
}
