#ifndef DFROBOT_SSD1306_H
#define DFROBOT_SSD1306_H

#include "DFRobot_Display_Config.h"

#define SSD1306_JUMPTABLE_BYTES 4
#define SSD1306_JUMPTABLE_LSB   1
#define SSD1306_JUMPTABLE_SIZE  2
#define SSD1306_JUMPTABLE_WIDTH 3
#define SSD1306_JUMPTABLE_START 4
#define SSD1306_WIDTH_POS       0
#define SSD1306_HEIGHT_POS      1
#define SSD1306_FIRST_CHAR_POS  2
#define SSD1306_CHAR_NUM_POS    3

// Display commands
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_EXTERNALVCC         0x1
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_PAGEADDR            0x22
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETSEGMENTREMAP     0xA1
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SWITCHCAPVCC        0x2

class DFRobot_SSD1306 : public DFRobot_Display {
public:
    DFRobot_SSD1306(void);

    uint8_t SSD1306_FrameBuffer[1024];

    virtual void writeCmd(uint8_t cmd)                        = 0;
    virtual void writeDatBytes(uint8_t* pDat, uint16_t count) = 0;

    int16_t begin(void);
    void    displayON(void);
    void    displayOFF(void);
    void    fillScreen(uint16_t color);
    void    drawVLine(int16_t x, int16_t y, int16_t height, uint16_t color);
    void    drawBuffer(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t* pBuf);
    int16_t setRotaion(eROTATION eRotation);

protected:
    void writeBufferPixel(uint16_t addr, uint8_t bitAddr, uint16_t color);
};

#endif
