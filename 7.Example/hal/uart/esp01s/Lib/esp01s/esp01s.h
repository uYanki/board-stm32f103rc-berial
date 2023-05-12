#ifndef __ESP01S_H_
#define __ESP01S_H_

#include "usart.h"

#define ESP01S_UART huart2

#define ESP01S_RESET_LOW   // HAL_GPIO_WritePin()
#define ESP01S_RESET_HIGH  //

// pin->ch_pd
#define ESP01S_POWER_ON   //
#define ESP01S_POWER_OFF  //

/*
    【修改波特率】
    AT+CIOBAUD=?     ---- 查询命令参数
    +CIOBAUD:(9600-921600)

    OK

    AT+CIOBAUD=115200
    BAUD->115200

    【选择 WIFI 应用模式 】
    AT+CWMODE=1
            1   Station 模式
            2   AP 模式
            3   AP 兼 Station 模式

    【列出当前可用 AP】
    AT+CWLAP=<ssid>,< mac >,<ch>
    AT+CWLAP

    【AT+CWJAP加入 AP】
    AT+CWJAP=<ssid>,< pwd >
*/

// 无线局域网络（WLAN）

typedef enum {
    STA = 1,
    AP,
    STA_AP
} NetMode;

typedef enum {  // 协议
    TCP,
    UDP
} Protocol;

typedef enum {  // 加密方式
    OPEN = 0,
    WEP,
    WPA_PSK,
    WPA2_PSK,
    WPA_WPA2_PSK
} Encryption;

typedef struct {
    char       ssid[33];  // 热点名
    Encryption ecn;       // 加密方式
    int32_t    rssi;      // 信号强度
    uint8_t    mac[20];   // MAC地址
    uint8_t    ch;        // 信道
} AccessPoint;

void    ESP01S_Transmit(const char* cmd);
uint8_t ESP01S_Receive(const char* reply, uint32_t timeout);

#define ESP01S_TransmitReceive(cmd, reply, timeout) ESP01S_Transmit(cmd), ESP01S_Receive(reply, timeout)

void    ESP01S_SetBaud();
uint8_t ESP01S_SetMode(NetMode md);

void ESP01S_Reset(void);

uint8_t ESP01S_BuildAp(const char* ssid, const char* pwd, Encryption ecn);

// wifi
void    ESP01S_ScanAP(AccessPoint* ap_list, uint16_t size);
uint8_t ESP01S_JoinAP(const char* ssid, const char* pwd);
void    ESP01S_QuitAp();

uint8_t ESP01S_CreateTCPServer(uint16_t port);
uint8_t ESP01S_EnableMultipleId(uint8_t enable);

// server
void ESP01S_ConnectSever(Protocol mode, const char* ip, const char* port, const char* com, uint8_t id);
void ESP01S_CloseTCP(uint8_t id);
void ESP01S_CloseUCP(uint8_t id);

void ESP01S_GetIP();
void ESP01S_Send();
void ESP01S_Open();
void ESP01S_Close();

#endif