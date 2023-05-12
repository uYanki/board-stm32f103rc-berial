#include "DFRobot_SSD1306.h"

DFRobot_SSD1306::DFRobot_SSD1306(void) : DFRobot_Display(128, 64) {}

int16_t DFRobot_SSD1306::begin(void)
{
    // config
    writeCmd(SSD1306_DISPLAYOFF);
    writeCmd(SSD1306_SETDISPLAYCLOCKDIV);
    writeCmd(0xF0);  // Increase speed of the display max ~96Hz
    writeCmd(SSD1306_SETMULTIPLEX);
    writeCmd(0x3F);
    writeCmd(SSD1306_SETDISPLAYOFFSET);
    writeCmd(0x00);
    writeCmd(SSD1306_SETSTARTLINE);
    writeCmd(SSD1306_CHARGEPUMP);
    writeCmd(0x14);
    writeCmd(SSD1306_MEMORYMODE);
    writeCmd(0x00);
    writeCmd(SSD1306_SEGREMAP);
    writeCmd(SSD1306_COMSCANINC);
    writeCmd(SSD1306_SETCOMPINS);
    writeCmd(0x12);
    writeCmd(SSD1306_SETCONTRAST);
    writeCmd(0xCF);
    writeCmd(SSD1306_SETPRECHARGE);
    writeCmd(0xF1);
    writeCmd(SSD1306_DISPLAYALLON_RESUME);
    writeCmd(SSD1306_NORMALDISPLAY);
    writeCmd(0x2e);  // stop scroll
    writeCmd(SSD1306_DISPLAYON);

    fillScreen(0);
    return BEGIN_WAR_NOTEST;
}

void DFRobot_SSD1306::fillScreen(uint16_t color)
{
    uint8_t i = 0, j = 0;
    uint8_t buf[16] = {0};
    if (color) {
        color = 0xff;
        for (i = 0; i < 16; i++) {
            buf[i] = 0xff;
        }
    }
    memset(SSD1306_FrameBuffer, color, 1024);
    writeCmd(SSD1306_COLUMNADDR);
    writeCmd(0x0);
    writeCmd(0x7f);

    writeCmd(SSD1306_PAGEADDR);
    writeCmd(0x0);
    writeCmd(0x7);

    for (i = 0; i < 128 * 64 / 8 / 16; i++) {
        writeDatBytes(buf, 16);
    }
}

void DFRobot_SSD1306::drawVLine(int16_t x, int16_t y, int16_t height_, uint16_t color)
{
    uint8_t  i             = 0;
    uint8_t  dataBuffer[8] = {0};
    uint8_t  lenth         = 0;
    uint8_t  bufferCount   = 0;
    uint16_t bufferAddr    = x + y / 8 * 128;
    //_DEBUG_PRINT("\n  drawFastVLine");

    if (limitVLine(x, y, height_) < 0) { return; }
    writeCmd(SSD1306_COLUMNADDR);
    writeCmd(x);
    writeCmd(x);
    writeCmd(SSD1306_PAGEADDR);
    writeCmd(y / 8);
    writeCmd((y + height_) / 8);

    lenth = 8 - y % 8;
    if (y % 8 + height_ > 8) {
        for (i = 0; i < lenth; i++) {
            writeBufferPixel(bufferAddr, y % 8 + i, color);
        }
        height_ -= lenth;
    } else {
        for (i = 0; i < height_; i++) {
            writeBufferPixel(bufferAddr, y % 8 + i, color);
        }
        height_ = 0;
    }
    dataBuffer[0] = SSD1306_FrameBuffer[bufferAddr];

    while (height_ > 0) {
        bufferAddr += 128;
        if (height_ > 7) {
            if (color) {
                SSD1306_FrameBuffer[bufferAddr] = 0xff;
            } else {
                SSD1306_FrameBuffer[bufferAddr] = 0x00;
            }
            height_ -= 8;
        } else {
            for (i = 0; i < height_; i++) {
                writeBufferPixel(bufferAddr, i, color);
            }
            height_ = 0;
        }
        bufferCount++;
        dataBuffer[bufferCount] = SSD1306_FrameBuffer[bufferAddr];
    }

    writeDatBytes(dataBuffer, bufferCount + 1);
}

void DFRobot_SSD1306::drawBuffer(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t* pBuf)
{
    uint8_t i = 0, j = 0;
    uint8_t var1 = 0;
    if (limitPixel(x, y) < 0) { return; }
    writeCmd(SSD1306_COLUMNADDR);
    writeCmd(x);
    writeCmd(x + w);
    writeCmd(SSD1306_PAGEADDR);
    writeCmd(y / 8);
    writeCmd((y + h) / 8);

    var1 = h / 8 + 1;
    for (i = 0; i < w; i++) {
        for (j = 0; j < var1; j++) {
            writeDatBytes(pBuf, var1);
        }
        pBuf += var1;
    }
}

void DFRobot_SSD1306::displayON(void)
{
    writeCmd(0xaf);
}

void DFRobot_SSD1306::displayOFF(void)
{
    writeCmd(0xae);
}

int16_t DFRobot_SSD1306::setRotaion(eROTATION eRotation)
{
    switch (eRotation) {
        case eROTATION_0:
            writeCmd(0xa0);
            writeCmd(0xc0);
            break;
        case eROTATION_180:
            writeCmd(0xa1);
            writeCmd(0xc8);
            break;
        default: return DISPLAY_ERR_NOTSUPPORT;
    }
    eRotation = eRotation;
    return DISPLAY_ERR_OK;
}

void DFRobot_SSD1306::writeBufferPixel(uint16_t addr, uint8_t bitAddr, uint16_t color)
{
    if (color) {
        SSD1306_FrameBuffer[addr] |= 0x01 << bitAddr;
    } else {
        SSD1306_FrameBuffer[addr] &= ~(0x01 << bitAddr);
    }
}
