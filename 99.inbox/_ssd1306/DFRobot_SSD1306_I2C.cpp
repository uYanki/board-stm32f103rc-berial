#include "DFRobot_SSD1306_I2C.h"


DFRobot_SSD1306_I2C::DFRobot_SSD1306_I2C(uint8_t I2CAddr_)
{
  I2CAddr = I2CAddr_;
}


inline void DFRobot_SSD1306_I2C::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  int16_t         bufferAddr = x + y / 8 * 128;
  if(limitPixel(x, y) < 0) {return;}
  writeCmd(SSD1306_COLUMNADDR);
  writeCmd(x);
  writeCmd(x);
  writeCmd(SSD1306_PAGEADDR);
  writeCmd(y / 8);
  writeCmd(y / 8);
  writeBufferPixel(bufferAddr, y % 8, color);
  Wire.begin();
  Wire.beginTransmission(I2CAddr);
  Wire.write(0x40);
  Wire.write(SSD1306_FrameBuffer[bufferAddr]);
  Wire.endTransmission();
}


inline void DFRobot_SSD1306_I2C::writeCmd(uint8_t cmd)
{
  SSD1306_I2CBEGIN();
  Wire.beginTransmission(I2CAddr);
  Wire.write(0x80);
  Wire.write(cmd);
  Wire.endTransmission();
}


inline void DFRobot_SSD1306_I2C::writeDatBytes(uint8_t* pDat, uint16_t count)
{
  SSD1306_I2CBEGIN();
  Wire.beginTransmission(I2CAddr);
  Wire.write(0x40);
  while(count --) {
    Wire.write(*pDat);
    pDat ++;
  }
  Wire.endTransmission();
}


