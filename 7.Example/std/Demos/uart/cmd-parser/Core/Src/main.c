#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "cmd_parser/cmd.h"

void test1_cmd(void) { println("do test1"); }

void test2_cmd(void) { println("do test2"); }

void test3_cmd(void) { println("do test3"); }

REGISTER_CMD(test1, test1_cmd, test1 demo);
REGISTER_CMD(test2, test2_cmd, test2 demo);
REGISTER_CMD(test3, test3_cmd, test3 demo);

int main(void)
{
    board_init();

#if USE_CMD_PARSER == 4
    for (;;) {
        // 处理不支持的指令
        if (uart2_rxflg) {
            uart2_rxflg = 0;
            println("unsupport command");
        }
    }
#else
    cmd_init();
    while (1) {
        if (uart2_rxflg) {
            uart2_rxflg = 0;
            cmd_parse((const char*)uart2_rxbuf);
        }
    }
#endif
}
