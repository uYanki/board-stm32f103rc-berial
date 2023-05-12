#include "DFRobot_SSD1306_I2C.h"
#include "DFRobot_Character.h"
#include "DFRobot_GT30L.h"
#include <Wire.h>
#include <SPI.h>

DFRobot_SSD1306_I2C OLED(0x3c);

#ifdef __AVR__
uint8_t       pin_cs = 5;
#else
uint8_t       pin_cs = D5;
#endif

void setup(void)
{
  Serial.begin(115200);
  //GT30L : our character IC
  GT30L_setCSPin(pin_cs);
  OLED.begin();
  OLED.setTextColor(1);
  OLED.supportChineseFont();
}

void loop(void)
{
  uint8_t       i = 0;
  OLED.fillScreen(0);delay(500);
  OLED.fillScreen(1);delay(500);
  
  OLED.fillScreen(0);
  for(i = 0; i < 32; i ++) {
    OLED.drawVLine(i , 0, i, 1);
  }
  delay(500);
  OLED.fillScreen(0);OLED.drawCircle(128 / 2, 64 / 2, 30, 1);
  
  OLED.drawRect(0, 0, 50, 25, 1);delay(500);
  OLED.fillScreen(0);OLED.drawLine(0, 0, 127, 63, 1);
  OLED.drawHLine(0, 5, 127, 1);
  OLED.drawVLine(20, 0, 63, 1);delay(500);
  OLED.fillScreen(0);OLED.drawTriangle(128 / 2, 0, 0, 63, 126, 62, 1);delay(500);
  
  OLED.fillScreen(0);OLED.fillCircle(128 / 2, 64 / 2, 30, 1);delay(500);
  OLED.fillScreen(0);OLED.fillRect(0, 0, 50, 25, 1);delay(500);
  OLED.fillScreen(0);OLED.fillTriangle(128 / 2, 0, 0, 63, 127, 63, 1);delay(500);
}

