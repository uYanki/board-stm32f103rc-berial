#include "esp01s.h"
#include <stdio.h>
#include <string.h>

#define rxbuf_cap 255  // 容量
int16_t rxbuf_len = 0;
char    rxbuf[rxbuf_cap + 1];  // 缓冲区

const char* server_protocol[] = {"TCP", "UDP"};

void ESP01S_Transmit(const char* cmd)
{
    HAL_UART_Transmit(&ESP01S_UART, (uint8_t*)cmd, strlen(cmd), 0xFF);
    HAL_UART_Transmit(&ESP01S_UART, "\r\n", 2, 0xFF);
}

uint8_t ESP01S_Receive(const char* reply, uint32_t timeout)
{
    rxbuf[rxbuf_len + 1] = '\0';
}

uint8_t ESP01S_Receives(const char* reply[], uint8_t size, uint32_t timeout) {}

void ESP01S_SetBaud();

uint8_t ESP01S_SetMode(NetMode md)
{
    static const char* modes[] = {"AT+CWMODE=1", "AT+CWMODE=2", "AT+CWMODE=3"};
    return ESP01S_TransmitReceive(modes[md], "OK", 1000);
}

void ESP01S_Reset(void)
{
#if 0
    ESP01S_Transmit("AT+RST");
#else
    ESP01S_RESET_LOW;
    HAL_Delay(50);
    ESP01S_RESET_HIGH;
#endif
}

uint8_t ESP01S_CreateTCPServer(uint16_t port)
{
    char buff[30];
    // 开启TCP服务器
    sprintf(buff, "AT+CIPSERVER=1,%d", port);
    if (ESP01S_TransmitReceive(buff, "OK", 2000)) return 0;
    // 关闭回显
    if (ESP01S_TransmitReceive("ATE0", "OK", 1000)) return 0;
    return 1;
}

uint8_t ESP01S_EnableMultipleId(uint8_t enable)
{
    return ESP01S_TransmitReceive(enable ? "AT+CIPMUX=1" : "AT+CIPMUX=0", "OK", 1000);
}

void ESP01S_ConnectServer(Protocol mode, const char* ip, const char* port, const char* com, uint8_t id)
{
    char tmp[3], buff[120];
    (id < 5) ? (sprintf(tmp, "%d,", id)) : (tmp[0] = '\0');
    sprintf(buff, "AT+CIPSTART=%s\"%s\",\"%s\",%s", tmp, server_protocol[mode], ip, com);
    //  "OK", "ALREAY CONNECT"
}

void ESP01S_CloseTCP(uint8_t id)
{
    char buff[32];
    sprintf(buff, "AT+CIPCLOSE=%d", id);
    ESP01S_SendCommand(buff);
}
void ESP01S_CloseUCP();

uint8_t ESP01S_BuildAP(const char* ssid, const char* pwd, Encryption ecn)
{
    char               buff[120];
    static const char* encryption[] = {"OPEN", "WEP", "WPA_PSK", "WPA2_PSK", "WPA_WPA2_PSK"};
    sprintf(buff, "AT+CWSAP=\"%s\",\"%s\",1,%s", ssid, pwd, encryption[ecn]);
    return ESP01S_TransmitReceive(buff, "OK", 1000);
}
void ESP01S_ScanAP(AccessPoint* ap_list, uint16_t size /* 缓冲区大小 */)
{
}

uint8_t ESP01S_JoinAP(const char* ssid, const char* pwd)
{
    char buff[120];
    sprintf(buff, "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
    return ESP01S_TransmitReceive(buff, "OK", 7000);
}

void ESP01S_QuitAp()
{
    ESP01S_Transmit("AT+CWQAP");
}