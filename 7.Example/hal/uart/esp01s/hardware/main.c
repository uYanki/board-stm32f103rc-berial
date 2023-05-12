#include "include.h"
#include "config.h"


int main(void)
{
  Systick_Init();
  USART1_Init();
  PC13_Init();
  PC13_Off(); // Turn off PC13 LED

  TIM2_Init();
  TIM3_Init();

  RFID_Init();
  ESP8266_Init();

  ESP8266_Set_Stationmode();
  printf("## Switched to station mode ##\r\n");

  if(ESP8266_Connect_AP(WIFI_SSID, WIFI_PASSWORD) != ACK_SUCCESS) {
    printf("## Connect to WiFi failed. ##\r\n");
    while(1)
    {;}
  }
  PC13_On(); // Turn on LED after wifi is connected
  printf("## WiFi connected ##\r\n");

  if (ESP8266_Set_Link_Mux(0) != ACK_SUCCESS) {
    printf("## Set MUX failed. ##\r\n");
    while(1)
    {;}
  }
  printf("## MUX Set To 0 ##\r\n");

  u32 count = 0;
  char request[128];
  while(1) {
    if (ESP8266_Start_Passthrough() != ACK_SUCCESS) {
      printf("## Switch to passthrough failed ##\r\n");
      ESP8266_Send_Cmd("+++", "", 15);
      if(ESP8266_Connect_TCP(SERVER_IP, SERVER_PORT) != ACK_SUCCESS) {
        printf("Connect to %s failed. Retry after 5 seconds\r\n", SERVER_IP);
        Systick_Delay_ms(5000);
      } else {
        printf("Reconnected to %s.\r\n", SERVER_IP);
      }
    } else {
      PC13_On();
      sprintf(request, "No. %d:\r\n", count++);
      Passthrough_Echo_Test(request);
      RFID_Check_Version();
      RFID_Query_Config();
      RFID_Inventory_Single();
      PC13_Off();
      Systick_Delay_ms(3000);
      ESP8266_Quit_Passthrough();
    }
  }
}

