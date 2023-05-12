#include "shell.h"
#include "usart.h"

#define SHELL_UART &huart2

Shell shell;
char  shellBuffer[512];

/**
 * @brief 用户shell写
 *
 * @param data 数据
 * @param len 数据长度
 *
 * @return unsigned short 写入实际长度
 */
signed short userShellWrite(char* data, unsigned short len)
{
    return HAL_UART_Transmit(SHELL_UART, (uint8_t*)data, len, 0xFF) == HAL_OK ? len : 0;
}

/**
 * @brief 用户shell读
 *
 * @param data 数据
 * @param len 数据长度
 *
 * @return unsigned short 读取实际长度
 */
signed short userShellRead(char* data, unsigned short len)
{
    return HAL_UART_Receive(SHELL_UART, (uint8_t*)data, len, 0xFF) == HAL_OK ? len : 0;
}

/**
 * @brief 用户shell初始化
 *
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
    shell.read  = userShellRead;
    shellInit(&shell, shellBuffer, 512);
}
