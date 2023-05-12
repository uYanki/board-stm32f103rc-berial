#ifndef DFROBOT_SSD1306_I2C_H
#define DFROBOT_SSD1306_I2C_H

#include "DFRobot_SSD1306.h"

#define SSD1306_I2CBEGIN() Wire.begin()

class DFRobot_SSD1306_I2C : public DFRobot_SSD1306 {
public:
    DFRobot_SSD1306_I2C(uint8_t I2CAddr);

    void drawPixel(int16_t x, int16_t y, uint16_t color);

private:
    uint8_t I2CAddr;

    void writeDatBytes(uint8_t* pDat, uint16_t count);
    void writeCmd(uint8_t cmd);
};

#endif
