#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "nr_micro_shell.h"

int main(void)
{
    char ch;

    board_init();

    shell_init();

    while (1) {
        if (SET == USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
            ch = USART_ReceiveData(USART2);
            shell(ch);  // rx char
        }
    }
}
