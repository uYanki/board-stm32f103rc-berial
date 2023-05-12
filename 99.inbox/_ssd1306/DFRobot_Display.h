#ifndef DFROBOT_DISPLAY_H
#define DFROBOT_DISPLAY_H

#include "Arduino.h"
#include "DFRobot_Types.h"
#include "DFRobot_Character.h"
#include "Print.h"
#include "DFRobot_GT30L.h"

#define DISPLAY_BLACK       0x0000 /*   0,   0,   0 */
#define DISPLAY_NAVY        0x000F /*   0,   0, 128 */
#define DISPLAY_DARKGREEN   0x03E0 /*   0, 128,   0 */
#define DISPLAY_DARKCYAN    0x03EF /*   0, 128, 128 */
#define DISPLAY_MAROON      0x7800 /* 128,   0,   0 */
#define DISPLAY_PURPLE      0x780F /* 128,   0, 128 */
#define DISPLAY_OLIVE       0x7BE0 /* 128, 128,   0 */
#define DISPLAY_LIGHTGREY   0xC618 /* 192, 192, 192 */
#define DISPLAY_DARKGREY    0x7BEF /* 128, 128, 128 */
#define DISPLAY_BLUE        0x001F /*   0,   0, 255 */
#define DISPLAY_GREEN       0x07E0 /*   0, 255,   0 */
#define DISPLAY_CYAN        0x07FF /*   0, 255, 255 */
#define DISPLAY_RED         0xF800 /* 255,   0,   0 */
#define DISPLAY_MAGENTA     0xF81F /* 255,   0, 255 */
#define DISPLAY_YELLOW      0xFFE0 /* 255, 255,   0 */
#define DISPLAY_WHITE       0xFFFF /* 255, 255, 255 */
#define DISPLAY_ORANGE      0xFD20 /* 255, 165,   0 */
#define DISPLAY_GREENYELLOW 0xAFE5 /* 173, 255,  47 */
#define DISPLAY_PINK        0xF81F

#define swap_int16(a, b) \
    {                    \
        int16_t t = a;   \
        a         = b;   \
        b         = t;   \
    }

typedef enum {
    eDIRECTION_VERTICAL,
    eDIRECTION_HORIZONTAL
} eDirection_t;

class DFRobot_Display : public Print {
public:
    DFRobot_Display(uint16_t width, uint16_t height);

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
    virtual void fillScreen(uint16_t color)                      = 0;

    virtual void drawVLine(int16_t x, int16_t y, int16_t height, uint16_t color);
    virtual void drawHLine(int16_t x, int16_t y, int16_t width, uint16_t color);
    virtual void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    virtual void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    virtual void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    virtual void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    virtual void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

    virtual int16_t setRotaion(eROTATION ratotion);
    virtual void    drawBuffer_24(int16_t x, int16_t y, uint8_t* pBuf, uint16_t count);
    virtual void    drawBuffer_16(int16_t x, int16_t y, uint16_t* pBuf, uint16_t count);

    int16_t drawBmp(Stream* s, int16_t x, int16_t y);
    void    drawBmp(uint8_t* pData, int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t size, uint16_t color);
    void    drawBmp(uint8_t* pData, int16_t x, int16_t y, uint16_t w, uint16_t h);

    eROTATION getRotation(void);
    void      setTextColor(uint16_t color);
    void      setTextBackground(uint16_t color);
    int16_t   getTextBackground();
    int16_t   getTextColor(void);
    void      setTextSize(uint8_t size);
    uint8_t   getTextSize(void);
    void      setLineWidth(uint16_t w);
    uint16_t  getLineWidth();
    int16_t   getWidth(void);
    int16_t   getHeight(void);
    void      setCursor(int16_t x, int16_t y);
    void      getCursor(int16_t* pX, int16_t* pY);
    eSHAPE    getDisplayShape(void);
    int16_t   getDisplayRadius(void);
    void      setOrign(int16_t x, int16_t y);
    void      getOrign(int16_t* pX, int16_t* pY);
    void      supportChineseFont(void);

    int16_t width, height, rawWidth, rawHeight;

protected:
    int16_t   textColor;
    int16_t   textBackground;
    uint16_t  textSize;
    eROTATION eRotation;
    eSHAPE    eShape;
    int16_t   cursorX, cursorY;
    uint16_t  displayRadius;
    uint16_t  lineWidth;

    pfCharacterFont_t pfCharacterFont;

    void setDisplayShape(eSHAPE eShape);

    void    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t part, int16_t offset, uint16_t color);
    void    circleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t part, uint16_t color);
    int16_t drawText(int16_t* pX, int16_t* pY, const char* ch);
    int32_t readN(Stream* s, uint8_t* pBuf, int32_t length);
    size_t  write(uint8_t ch);
    size_t  write(const uint8_t* pCh, size_t size);
    int16_t printfX, printfY;

    int16_t limitVLine(int16_t& x, int16_t& y, int16_t& h)
    {
        int16_t h_ = h, x_ = x, y_ = y, y0_, y1_;
        if (h < 0) {
            h_ = -h;
            y_ = y - h_ + 1;
        }
        x_  = x + cursorX;
        y0_ = y_ + cursorY;
        y1_ = y0_ + h_ - 1;

        if ((x_ < 0) || (x_ > width) || (y0_ > height) || (y1_ < 0)) {
            _DEBUG_PRINT("\n  limitVLine faild");
            return -1;
        }
        if (y0_ < 0) y0_ = 0;
        if (y1_ > height) y1_ = height;
        x = x_;
        y = y0_;
        h = y1_ - y0_ + 1;
        return 0;
    }

    int16_t limitHLine(int16_t& x, int16_t& y, int16_t& w)
    {
        int16_t w_ = w, x_ = x, y_ = y, x0_, x1_;
        if (w < 0) {
            w_ = -w;
            x_ = x - w_ + 1;
        }
        y_  = y + cursorY;
        x0_ = x_ + cursorX;
        x1_ = x0_ + w_ - 1;
        if ((y_ < 0) || (y_ > height) || (x0_ > width) || (x1_ < 0)) {
            return -1;
        }
        if (x0_ < 0) x0_ = 0;
        if (x1_ > width) x1_ = width;
        x = x0_;
        y = y_;
        w = x1_ - x0_ + 1;
        return 0;
    }

    int16_t limitPixel(int16_t& x, int16_t& y)
    {
        x += cursorX;
        y += cursorY;
        if ((x < 0) || (y > height) || (x > width) || (y < 0)) {
            return -1;
        }
        return 0;
    }

    eDirection_t calcLineDirection(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
    {
        if (abs(abs(x0) - abs(x1)) > abs(abs(y0) - abs(y1)))
            return eDIRECTION_VERTICAL;
        return eDIRECTION_HORIZONTAL;
    }

    void drawPixelWidth(int16_t x, int16_t y, eDirection_t eDirection, uint16_t color)
    {
        if (lineWidth == 1) {
            drawPixel(x, y, color);
        } else if (lineWidth > 1) {
            if (eDirection == eDIRECTION_HORIZONTAL) {
                drawHLine(x - (lineWidth / 2), y, lineWidth, color);
            } else {
                drawVLine(x, y - (lineWidth / 2), lineWidth, color);
            }
        }
    }

    uint16_t color24To16(uint8_t r, uint8_t g, uint8_t b)
    {
        return (uint16_t)((((uint16_t)r >> 3) << 11) | (((uint16_t)g >> 2) << 5) | ((uint16_t)b >> 3));
    }
};

#endif
