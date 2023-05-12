#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "esp8266.h"
#include "timer.h"

#define ESP_TX_PIN    GPIO_Pin_2   //PA2
#define ESP_GPIOx     GPIOA
#define ESP_RX_PIN    GPIO_Pin_3   //PA3
#define ESP_USART     USART2
#define ESP_BUF_SIZE  128

BufferTypeDef ESP_RX_BUF;
u8 ESP_RX_BUF_BUFF[ESP_BUF_SIZE] = {0x00};

BufferClip ESP_RX_CLIP;
u8 ESP_RX_CLIP_DATA[ESP_BUF_SIZE] = {0x00};

u8 ESP_RX_STATE = 0;

void ESP8266_Init(void)
{
  ESP_RX_BUF.buf = ESP_RX_BUF_BUFF;
  ESP_RX_BUF.size = ESP_BUF_SIZE;
  ESP_RX_CLIP.data = ESP_RX_CLIP_DATA;
  ESP_RX_CLIP.size = ESP_BUF_SIZE;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  GPIO_PinAFConfig(ESP_GPIOx, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(ESP_GPIOx, GPIO_PinSource3, GPIO_AF_USART2);
  // GPIO for TX and RX
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = ESP_TX_PIN | ESP_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ESP_GPIOx, &GPIO_InitStructure);
  // NVIC
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  // USART
  USART_DeInit(ESP_USART);
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(ESP_USART, &USART_InitStructure);
  USART_ClearFlag(ESP_USART, USART_FLAG_CTS);
  USART_Cmd(ESP_USART, ENABLE);

  ESP8266_Reset();
  ESP8266_Set_Echo_Off();
  USART_ITConfig(ESP_USART, USART_IT_RXNE, ENABLE);
  printf("## ESP8266 Initialized ##\r\n");
}

void USART2_IRQHandler(void)
{
  unsigned char rev_byte;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  {
    rev_byte = USART_ReceiveData(USART2);
    Buffer_Push(&ESP_RX_BUF, rev_byte);
    // Reset the TIM3 counter and enable it
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}

void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
    printf("ESP_RX_STATE++\r\n");
    ESP_RX_STATE++;
  }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  TIM_Cmd(TIM3, DISABLE);
}

void ESP8266_Send_Data(u8 *data, u8 length)
{
  //printf("Data: ");
  for (u8 i = 0; i < length; i++) {
    //printf("%02X ", *(data + i));
    USART_SendData(ESP_USART, *(data + i));
    while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET) { // Wait till sent
      ;// Do nothing
    }
  }
  //printf("sent\r\n");
}

void ESP8266_Send_String(u8* data)
{
  u8* start = data;
  while (*data != '\0') {
    //printf("s:%02X\r\n", *data);
    USART_SendData(ESP_USART, *data);
    while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET) { // Wait till sent
      ;// Do nothing
    }
    data++;
  }
  printf("USART_Sent: %s\r\n", start);
}

u8 ESP8266_Send_Cmd(char *cmd, char *ack, u16 waittime)
{
  ESP8266_Send_String((u8 *)cmd);
  // Avoid being joinned by other commands
  Systick_Delay_ms(50);
  // Make sure waittime is set
  if (waittime < 10) waittime = 10;

  while (waittime--) {
    if(ESP_RX_STATE > 0) {
      printf("ESP_RX_STATE %d\r\n", ESP_RX_STATE);
      ESP_RX_STATE--;
      if (Buffer_Pop_All(&ESP_RX_BUF, &ESP_RX_CLIP) != NULL) {
        Buffer_Clip_Print(&ESP_RX_CLIP);
        if(strstr((char *)(ESP_RX_CLIP.data), ack) != NULL) {
          printf("return success\r\n\n");
          return ACK_SUCCESS;
        }
      }
    }
    Systick_Delay_ms(20);
  }

  printf("return defeat\r\n\n");
  return ACK_DEFEAT;
}

u8 ESP8266_Send_Cmd2(char *cmd, char *ack, char *ack2, u16 waittime)
{
  ESP8266_Send_String((u8 *)cmd);
  Systick_Delay_ms(50);
  // Make sure waittime is set
  if (waittime < 10) waittime = 10;

  while (waittime--) {
    if(ESP_RX_STATE > 0) {
      printf("ESP_RX_STATE %d\r\n", ESP_RX_STATE);
      ESP_RX_STATE--;
      if (Buffer_Pop_All(&ESP_RX_BUF, &ESP_RX_CLIP) != NULL) {
        Buffer_Clip_Print(&ESP_RX_CLIP);
        if(strstr((char *)(ESP_RX_CLIP.data), ack) != NULL) {
          printf("return success\r\n\n");
          return ACK_SUCCESS;
        }
        if (strlen(ack2) > 0) {
          if(strstr((char *)(ESP_RX_CLIP.data), ack2) != NULL) {
            printf("return success\r\n\n");
            return ACK_SUCCESS;
          }
        }
      }
    }
    Systick_Delay_ms(20);
  }
  printf("return defeat\r\n\n");
  return ACK_DEFEAT;
}

void ESP8266_Reset(void)
{
  ESP8266_Send_String((u8 *)"AT+RST\r\n");
  Systick_Delay_ms(5000);
}

void ESP8266_Set_Echo_Off(void)
{
  ESP8266_Send_String((u8 *)"ATE0\r\n");
  Systick_Delay_ms(500);
}

u8 ESP8266_Set_Stationmode(void)
{
  if(ESP8266_Send_Cmd("AT\r\n", "OK", 15) != ACK_SUCCESS) {
    ESP8266_Quit_Passthrough();
  }

  if(ESP8266_Send_Cmd("AT+CWMODE?\r\n", "+CWMODE:1", 50) != ACK_SUCCESS) { // 1:station
    printf("Not in station mode\r\n");
    if (ESP8266_Send_Cmd("AT+CWMODE=1\r\n", "OK", 50) != ACK_SUCCESS) {
      printf("Station mode switch failed\r\n");
    } else {
      printf("Station mode switch succeeded\r\n");
    }
    ESP8266_Reset();
  } else {
    printf("In station mode.\r\n");
  }
  
  if(ESP8266_Send_Cmd("AT+CWAUTOCONN?\r\n", "+CWAUTOCONN:1", 50) != ACK_SUCCESS) { // 0:no 1:auto
    printf("Auto connection is OFF\r\n");
    if (ESP8266_Send_Cmd("AT+CWAUTOCONN=1\r\n", "OK", 50) != ACK_SUCCESS) {
      printf("Auto connection turn ON failed\r\n");
    } else {
      printf("Auto connection turn ON succeeded\r\n");
    }
  } else {
    printf("Auto connection is ON\r\n");
  }

  if(ESP8266_Send_Cmd("AT+CWMODE?\r\n", "+CWMODE:1", 50) == ACK_SUCCESS) {
    return ACK_SUCCESS;
  } else {
    return ACK_DEFEAT;
  }
}

u8 ESP8266_Set_APmode(char *ap_ssid, char *ap_pwd, char chl, char ecn)
{
  char ap_information[150] = {0};

  if(ESP8266_Send_Cmd("AT\r\n", "OK", 15) != ACK_SUCCESS) {
    ESP8266_Quit_Passthrough();
  }
  if(ESP8266_Send_Cmd("AT+CWMODE?\r\n", "+CWMODE:2", 20) != ACK_SUCCESS) {
    // printf("Set ESP mode.\r\n");
    ESP8266_Send_Cmd("AT+CWMODE=2\r\n", "OK", 20); 
    ESP8266_Reset();
  } else {
    printf("Already in AP mode.\r\n");
  }

  sprintf(ap_information, "AT+CWSAP_DEF=\"%s\",\"%s\",%c,%c\r\n", ap_ssid, ap_pwd, chl, ecn);
  ESP8266_Send_Cmd(ap_information, "OK", 100);
  ESP8266_Send_Cmd("AT+CIPMUX=1\r\n", "OK", 20);
  ESP8266_Send_Cmd("AT+CIPSERVER=1,80\r\n", "OK", 20);
  if(ESP8266_Send_Cmd("AT+CWMODE?\r\n","+CWMODE:2",20) == ACK_SUCCESS) {
    return ACK_SUCCESS;
  } else {
    return ACK_DEFEAT;
  }
}

/*-------------------------------------------------
 * Connect to WiFi AP
 *-------------------------------------------------*/
u8 ESP8266_Connect_AP(const char *ssid, const char *passwd)
{
  if(ESP8266_Send_Cmd("AT+CWJAP?\r\n", "+CWJAP:", 50) == ACK_SUCCESS) {
    return ACK_SUCCESS;
  }
  printf("Connecting to AP [%s]\r\n", ssid);

  char wifi_information[150] = {0};
  sprintf(wifi_information, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, passwd);
  for(int retries = 0; retries < 5; retries ++ ) {
    if(ESP8266_Send_Cmd(wifi_information, "WIFI GOT IP", 600) == ACK_SUCCESS) {
      printf("AP conntected.\r\n");
      return ACK_SUCCESS;
    } else {
      printf("AP connection failed, retry count: %d\r\n", retries);
    }
  }
  return ACK_DEFEAT;
}

u8 ESP8266_Set_Link_Mux(u8 mode)
{
  char cmd[14] = {0};
  char ack[12] = {0};
  sprintf(cmd, "AT+CIPMUX=%d\r\n", mode);
  sprintf(ack, "+CIPMUX:%d", mode);
  if((ESP8266_Send_Cmd("AT+CIPMUX?\r\n", ack, 20)) == ACK_SUCCESS) {
    return ACK_SUCCESS;
  } else {
    printf("Set link mux.\r\n");
    return ESP8266_Send_Cmd(cmd, "OK", 20);
  }
}

u8 ESP8266_Connect_TCP(const char *addr, const char *port)
{
  char tcp_information[64]={0};

  sprintf(tcp_information, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", addr, port);
  //  printf("%s\n",tcp_information);
  for(u8 retries = 0; retries < 5; retries ++ ) {
    if(ESP8266_Send_Cmd(tcp_information, "CONNECT", 50) == ACK_SUCCESS) {
      return ACK_SUCCESS;
    }
  }
  return ACK_DEFEAT;
}


u8 ESP8266_Connect_UDP(const char *addr, const char *port)
{
  char cmd[64]={0};
  sprintf(cmd, "AT+CIPSTART=\"UDP\",\"%s\",%s\r\n", addr, port);
  //  printf("%s\n",tcp_information);
  for(int retries = 0; retries < 5; retries ++ ) {
    if(ESP8266_Send_Cmd2(cmd, "OK", "CONNECTED", 50) == ACK_SUCCESS) {
      return ACK_SUCCESS;
    }
  }
  return ACK_DEFEAT;
}


u8 ESP8266_Start_Passthrough(void)
{
  // Ensure IPMODE = 1
  if((ESP8266_Send_Cmd("AT+CIPMODE?\r\n", "+CIPMODE:1", 10)) != ACK_SUCCESS) {
    if (ESP8266_Send_Cmd("AT+CIPMODE=1\r\n", "OK", 10) != ACK_SUCCESS) {
      printf("Set CIPMODE=1 failed\r\n");
      return ACK_DEFEAT;
    }
  } else {
    printf("CIPMODE=1 already set\r\n");
  }
  return ESP8266_Send_Cmd("AT+CIPSEND\r\n", "OK", 15);
}


u8 ESP8266_Quit_Passthrough(void)
{
  // Return true on any result
  ESP8266_Send_Cmd("+++", "", 15);
  // Make a delay > 20ms, otherwise it will be joined by the AT command below and fail to quit.
  return ESP8266_Send_Cmd("AT\r\n","OK", 15);
}


u8 ESP8266_Passthrough_Request(Conn_Type type, const char *addr, char *port, void (*function)())
{
  if (ESP8266_Set_Link_Mux(0) != ACK_SUCCESS) {
    printf("Set MUX:0 failed\r\n");
    return ACK_DEFEAT;
  }

  if (type == UDP) {
    if(ESP8266_Connect_UDP(addr,port) != ACK_SUCCESS) {
      printf("UDP connection failed\r\n");
      return ACK_DEFEAT;
    }
  } else if (type == TCP) {
    if(ESP8266_Connect_TCP(addr,port) != ACK_SUCCESS) {
      printf("TCP connection failed\r\n");
      return ACK_DEFEAT;
    }
  } else {
    printf("Unknown connection type\r\n");
    return ACK_DEFEAT;
  }

  ESP8266_Start_Passthrough();
  function();
  ESP8266_Quit_Passthrough();

  ESP8266_Send_Cmd("AT+CIPCLOSE\r\v", "CLOSED", 20);  // Close the connection
  return ACK_SUCCESS;
}

/**
 * Passthrough inline methods
 */
void Passthrough_Echo_Test(char *request)
{
  ESP8266_Send_String((u8 *)request);
  u16 waittime = 100;
  while (waittime--) {
    if(ESP_RX_STATE > 0) {
      printf("ESP_RX_STATE %d\r\n", ESP_RX_STATE);
      ESP_RX_STATE--;
      if (Buffer_Pop_All(&ESP_RX_BUF, &ESP_RX_CLIP) != NULL) {
        Buffer_Clip_Print(&ESP_RX_CLIP);
      }
    }
    Systick_Delay_ms(10);
  }
  printf("No Echo\r\n");
}
