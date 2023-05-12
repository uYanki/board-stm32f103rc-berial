#include "DFRobot_Display.h"

DFRobot_Display::DFRobot_Display(uint16_t width_, uint16_t height_)
{
    width           = width_ - 1;
    height          = height_ - 1;
    rawWidth        = width - 1;
    rawHeight       = height - 1;
    textSize        = 1;
    pfCharacterFont = DFRobot_Character_getCharacter;
    lineWidth       = 1;
}

size_t DFRobot_Display::write(uint8_t ch)
{
    _DEBUG_PRINT("\n  print char");
    drawText(&printfX, &printfY, (const char*)&ch);
    return 1;
}

size_t DFRobot_Display::write(const uint8_t* pCh, size_t size)
{
    _DEBUG_PRINT("\n  print string");
    drawText(&printfX, &printfY, (const char*)pCh);
    return strlen((const char*)pCh);
}

int16_t DFRobot_Display::drawText(int16_t* pX, int16_t* pY, const char* ch)
{
    uint8_t characterBuffer[32] = {0};
    uint8_t rslt                = 0;
    uint8_t i = 0, j = 0, k = 0;
    uint8_t var1      = 0;
    uint8_t textWidth = 0, textHeight = 0;
#ifdef _DEBUG
    _DEBUG_PRINT("\n  drawText :");
    _DEBUG_PRINT(ch);
    for (i = 0; i < strlen(ch); i++) {
        _DEBUG_PRINT(" ");
        _DEBUG_PRINTVAR(ch[i], HEX);
    }
#endif

    while (*ch) {
        // get character
        rslt = pfCharacterFont((uint8_t*)ch, characterBuffer, &textWidth, &textHeight);
        fillRect(*pX - cursorX, *pY - cursorY, textWidth * textSize, textHeight * textSize, textBackground);
#ifdef _DEBUG
        _DEBUG_PRINT("\n  get character result :");
        _DEBUG_PRINTVAR(rslt, HEX);
        _DEBUG_PRINT("\n  get character :");
        for (i = 0; i < 32; i++) {
            _DEBUG_PRINTVAR(characterBuffer[i], HEX);
            _DEBUG_PRINT(" ");
        }
#endif
        if (rslt < 0) {
            return rslt;
        } else {
            if (*ch > 0x06 && *ch < 0x0e) {
                if (*ch == '\n') {
                    *pX = 0;
                    *pY += textHeight * textSize;
                }
                ch += rslt;
                continue;
            }
            ch += rslt;
            // check range
            if (*pX > rawWidth - textWidth * textSize) {
                *pX = 0;
                *pY += textHeight * textSize;
            }
            if (*pY > rawHeight - textHeight * textSize) {
                return DISPLAY_WAR_OUTRANGE;
            }
            if (rslt > 1) {
                /*display, charater example:
                  data: 0xf0, 0x0f, 0x55, 0xaa ...
                  display pixel: ****0000 0000****
                                 0*0*0*0* *0*0*0*0
                */
                for (i = 0; i < 32; i++) {
                    var1 = characterBuffer[i];
                    for (j = 0; j < 8; j++) {
                        if (var1 & (0x01 << j)) {
                            for (k = 0; k < textSize; k++) {
                                drawVLine(*pX + (i % 2) * 8 * textSize + j * textSize + k - cursorX,
                                          *pY + (i / 2) * textSize - cursorY, textSize, textColor);
                            }
                        }
                    }
                }
            } else {
                /*display, charater example:
                  data: 0xf0, 0x0f, 0x55, 0xaa ...
                  display pixel: ****0000
                                 0000****
                                 0*0*0*0*
                                 *0*0*0*0
                */
                // 8 * 16 text size
                if (textHeight == 16) {
                    for (i = 0; i < 16; i++) {
                        var1 = characterBuffer[i];
                        for (j = 0; j < 8; j++) {
                            if (var1 & (0x01 << j)) {
                                for (k = 0; k < textSize; k++) {
                                    drawVLine(*pX + j * textSize + k - cursorX,
                                              *pY + i * textSize - cursorY, textSize, textColor);
                                }
                            }
                        }
                    }
                    // 6 * 8 text size
                } else {
                    for (i = 0; i < textWidth; i++) {
                        var1 = characterBuffer[i];
                        for (j = 0; j < 8; j++) {
                            if (var1 & (0x01 << j)) {
                                for (k = 0; k < textSize; k++) {
                                    drawVLine(*pX + i * textSize + k - cursorX, *pY + j * textSize - cursorY, textSize, textColor);
                                }
                            }
                        }
                    }
                }
            }
        }
        *pX += textWidth * textSize;
    }
}

void DFRobot_Display::drawVLine(int16_t x, int16_t y, int16_t height_, uint16_t color)
{
    if ((x + cursorX < 0) || (x + cursorX > width)) { return; }
    int8_t  direction = 1;
    int16_t var1      = y + height_;
    if (height_ < 0) {
        direction = -1;
    }
    for (; y != var1; y += direction) {
        drawPixel(x, y, color);
    }
}

void DFRobot_Display::drawHLine(int16_t x, int16_t y, int16_t width_, uint16_t color)
{
    if ((y + cursorY < 0) || (y + cursorY > height)) { return; }
    int8_t  direction = 1;
    int16_t var1      = x + width_;
    if (width_ < 0) {
        direction = -1;
    }
    for (; x != var1; x += direction) {
        drawPixel(x, y, color);
    }
}

void DFRobot_Display::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    int8_t dirX = (width > 0) ? -1 : 1;
    int8_t dirY = (height > 0) ? -1 : 1;
    drawHLine(x, y, width, color);
    drawHLine(x, y + height + dirY, width, color);
    drawVLine(x, y, height, color);
    drawVLine(x + width + dirX, y, height, color);
}

void DFRobot_Display::fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    int8_t  directionX = 1;
    int16_t var1       = x + width;
    if (width < 0) {
        directionX = -1;
    }
    for (; x != var1; x += directionX) {
        drawVLine(x, y, height, color);
    }
}

void DFRobot_Display::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t dx = abs(x1 - x0), dy = abs(y1 - y0);
    uint8_t steep = 0;

    eDirection_t eDirection = calcLineDirection(x0, y0, x1, y1);
    if (dx < dy) {
        steep = 1;
        swap_int16(x0, y0);
        swap_int16(x1, y1);
        swap_int16(dx, dy);
    }
    int8_t  dirX = (x1 - x0) > 0 ? 1 : -1;
    int8_t  dirY = (y1 - y0) > 0 ? 1 : -1;
    int16_t endX = x0, endY = y0;
    int32_t var1 = dy * 2;
    int32_t var2 = (dy - dx) * 2;
    int32_t var3 = dy * 2 - dx;

    if (steep) {
        while (endX != x1) {
            if (var3 < 0) {
                var3 += var1;
            } else {
                endY += dirY;
                var3 += var2;
            }
            // drawPixel(endY, endX, color);
            drawPixelWidth(endY, endX, eDirection, color);
            endX += dirX;
        }
    } else {
        while (endX != x1) {
            if (var3 < 0) {
                var3 += var1;
            } else {
                endY += dirY;
                var3 += var2;
            }
            // drawPixel(endX, endY, color);
            drawPixelWidth(endX, endY, eDirection, color);
            endX += dirX;
        }
    }
}

void DFRobot_Display::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    r            = abs(r);
    int16_t varX = 0, varY = r;
    int16_t endY = 0;
    int32_t var1 = 3 - 2 * r;

    while (varX <= varY) {
        drawPixel(x0 + varX, y0 + varY, color);
        drawPixel(x0 - varX, y0 + varY, color);
        drawPixel(x0 + varX, y0 - varY, color);
        drawPixel(x0 - varX, y0 - varY, color);
        drawPixel(x0 + varY, y0 + varX, color);
        drawPixel(x0 - varY, y0 + varX, color);
        drawPixel(x0 + varY, y0 - varX, color);
        drawPixel(x0 - varY, y0 - varX, color);
        if (var1 < 0) {
            var1 = var1 + 4 * varX + 6;
        } else {
            var1 = var1 + 4 * (varX - varY) + 10;
            varY--;
        }
        varX++;
    }
}

void DFRobot_Display::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    r = abs(r);

    int16_t varX = 0, varY = r;
    int16_t endY = 0;
    int32_t var1 = 3 - 2 * r;

    while (varX <= varY) {
        drawVLine(x0 + varX, y0 - varY, 2 * varY + 1, color);
        drawVLine(x0 + varY, y0 - varX, 2 * varX + 1, color);
        drawVLine(x0 - varX, y0 - varY, 2 * varY + 1, color);
        drawVLine(x0 - varY, y0 - varX, 2 * varX + 1, color);
        if (var1 < 0) {
            var1 = var1 + 4 * varX + 6;
        } else {
            var1 = var1 + 4 * (varX - varY) + 10;
            varY--;
        }
        varX++;
    }
}

void DFRobot_Display::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void DFRobot_Display::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    int16_t a, b, y, last;
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap_int16(y0, y1);
        swap_int16(x0, x1);
    }
    if (y1 > y2) {
        swap_int16(y2, y1);
        swap_int16(x2, x1);
    }
    if (y0 > y1) {
        swap_int16(y0, y1);
        swap_int16(x0, x1);
    }

    if (y0 == y2) {  // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        drawHLine(a, y0, b - a + 1, color);
        return;
    }

    int16_t dx01 = x1 - x0;
    int16_t dy01 = y1 - y0;
    int16_t dx02 = x2 - x0;
    int16_t dy02 = y2 - y0;
    int16_t dx12 = x2 - x1;
    int16_t dy12 = y2 - y1;
    int32_t sa   = 0;
    int32_t sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1;  // Include y1 scanline
    else
        last = y1 - 1;  // Skip it

    if (dy01 == 0 || dy02 == 0) { return; }
    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) swap_int16(a, b);
        drawHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) swap_int16(a, b);
        drawHLine(a, y, b - a + 1, color);
    }
}

void DFRobot_Display::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    r = abs(r);
    // smarter version
    if (w > 0) {
        drawHLine(x + r, y, w - 2 * r + 1, color);      // Top
        drawHLine(x + r, y + h, w - 2 * r + 1, color);  // Bottom
    } else {
        drawHLine(x - r, y, -(abs(w) - 2 * r + 1), color);      // Top
        drawHLine(x - r, y + h, -(abs(w) - 2 * r + 1), color);  // Bottom
    }
    if (h > 0) {
        drawVLine(x, y + r, h - 2 * r + 1, color);
        drawVLine(x + w, y + r, h - 2 * r + 1, color);
    } else {
        drawVLine(x, y - r, -(abs(h) - 2 * r + 1), color);
        drawVLine(x + w, y - r, -(abs(h) - 2 * r + 1), color);
    }
    // draw four corners
    if (w > 0) {
        if (h > 0) {
            circleHelper(x + r, y + r, r, 1, color);
            circleHelper(x + w - r, y + r, r, 2, color);
            circleHelper(x + r, y + h - r, r, 8, color);
            circleHelper(x + w - r, y + h - r, r, 4, color);
        } else {
            circleHelper(x + r, y - r, r, 8, color);
            circleHelper(x + w - r, y - r, r, 4, color);
            circleHelper(x + r, y + h + r, r, 1, color);
            circleHelper(x + w - r, y + h + r, r, 2, color);
        }
    } else {
        if (h > 0) {
            circleHelper(x - r, y + r, r, 2, color);
            circleHelper(x + w + r, y + r, r, 1, color);
            circleHelper(x + w + r, y + h - r, r, 8, color);
            circleHelper(x - r, y + h - r, r, 4, color);
        } else {
            circleHelper(x - r, y - r, r, 4, color);
            circleHelper(x + w + r, y - r, r, 8, color);
            circleHelper(x - r, y + h + r, r, 2, color);
            circleHelper(x + w + r, y + h + r, r, 1, color);
        }
    }
}

// fill a rounded rectangle!
void DFRobot_Display::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    r = abs(r);
    if (w > 0) {
        if (h > 0) {
            fillRect(x + r, y, w - 2 * r + 1, h + 1, color);
        } else {
            fillRect(x + r, y + h, w - 2 * r + 1, abs(h) + 1, color);
        }
    } else {
        if (h > 0) {
            fillRect(x + w + r, y, abs(w) - 2 * r + 1, h + 1, color);
        } else {
            fillRect(x + w + r, y + h, abs(w) - 2 * r + 1, abs(h) + 1, color);
        }
    }
    if (w > 0) {
        if (h > 0) {
            fillCircleHelper(x + r, y + r, r, 2, h - 2 * r, color);
            fillCircleHelper(x + w - r, y + r, r, 1, h - 2 * r, color);
        } else {
            fillCircleHelper(x + r, y + h + r, r, 2, abs(h) - 2 * r, color);
            fillCircleHelper(x + w - r, y + h + r, r, 1, abs(h) - 2 * r, color);
        }
    } else {
        if (h > 0) {
            fillCircleHelper(x - r, y + r, r, 1, h - 2 * r, color);
            fillCircleHelper(x + w + r, y + r, r, 2, h - 2 * r, color);
        } else {
            fillCircleHelper(x + w + r, y + h + r, r, 2, abs(h) - 2 * r, color);
            fillCircleHelper(x - r, y + h + r, r, 1, abs(h) - 2 * r, color);
        }
    }
}

void DFRobot_Display::circleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t part, uint16_t color)
{
    r            = abs(r);
    int16_t varX = 0, varY = r;
    int16_t endY = 0;
    int32_t var1 = 3 - 2 * r;

    while (varX <= varY) {
        if (part & 0x04) {
            drawPixel(x0 + varX, y0 + varY, color);
            drawPixel(x0 + varY, y0 + varX, color);
        }
        if (part & 0x02) {
            drawPixel(x0 + varX, y0 - varY, color);
            drawPixel(x0 + varY, y0 - varX, color);
        }
        if (part & 0x08) {
            drawPixel(x0 - varY, y0 + varX, color);
            drawPixel(x0 - varX, y0 + varY, color);
        }
        if (part & 0x01) {
            drawPixel(x0 - varX, y0 - varY, color);
            drawPixel(x0 - varY, y0 - varX, color);
        }

        if (var1 < 0) {
            var1 = var1 + 4 * varX + 6;
        } else {
            var1 = var1 + 4 * (varX - varY) + 10;
            varY--;
        }
        varX++;
    }
}

void DFRobot_Display::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t part, int16_t offset, uint16_t color)
{
    r            = abs(r);
    int16_t varX = 0, varY = r;
    int16_t endY = 0;
    int32_t var1 = 3 - 2 * r;

    while (varX <= varY) {
        if (part & 0x02) {
            drawVLine(x0 - varX, y0 - varY, 2 * varY + 1 + offset, color);
            drawVLine(x0 - varY, y0 - varX, 2 * varX + 1 + offset, color);
        }
        if (part & 0x01) {
            drawVLine(x0 + varX, y0 - varY, 2 * varY + 1 + offset, color);
            drawVLine(x0 + varY, y0 - varX, 2 * varX + 1 + offset, color);
        }
        if (var1 < 0) {
            var1 = var1 + 4 * varX + 6;
        } else {
            var1 = var1 + 4 * (varX - varY) + 10;
            varY--;
        }
        varX++;
    }
}

int32_t DFRobot_Display::readN(Stream* s, uint8_t* pBuf, int32_t length)
{
    int32_t l = length;
    if (pBuf == 0) {
        while ((length--) && (s->available())) {
            s->read();
        }
    } else {
        while ((length--) && (s->available())) {
            *pBuf = s->read();
            pBuf++;
        }
    }
    return l - length;
}

int16_t DFRobot_Display::drawBmp(Stream* s, int16_t x, int16_t y)
{
    uint16_t i = 0, j = 0, k = 0;
    int32_t  picWidth = 0, picHeight = 0;
    uint8_t  colorBuf[4] = {0};

    limitPixel(x, y);
    if (s == 0) {
        return DISPLAY_ERR;
    } else {
        readN(s, 0, 18);
        readN(s, colorBuf, 4);
        picWidth = colorBuf[0] | ((uint32_t)colorBuf[1] << 8) | ((uint32_t)colorBuf[2] << 16) | ((uint32_t)colorBuf[3] << 24);
        _DEBUG_PRINT("\n  pic width :");
        _DEBUG_PRINTVAR(picWidth, DEC);
        readN(s, colorBuf, 4);
        picHeight = colorBuf[0] | ((uint32_t)colorBuf[1] << 8) | ((uint32_t)colorBuf[2] << 16) | ((uint32_t)colorBuf[3] << 24);
        _DEBUG_PRINT("\n  pic height :");
        _DEBUG_PRINTVAR(picHeight, DEC);
        readN(s, 0, 4);
        readN(s, colorBuf, 4);
        if (colorBuf[0] > 0) {
            _DEBUG_PRINT("\n  not support");
            return DISPLAY_ERR_NOTSUPPORT;
        }
        readN(s, 0, 20);

#if ((defined __ets__) || (defined ESP_PLATFORM))
        k                 = picWidth * 3;
        uint8_t* colorDat = (uint8_t*)malloc(k);
        if (colorDat == NULL) { return DISPLAY_ERR_MEMOVER; }
        for (i = 0; i < picHeight; i++) {
#ifdef __ets__
            ESP.wdtFeed();
#endif
            readN(s, colorDat, k);
            drawBuffer_24(x, y + (picHeight - i), colorDat, k);
        }
        free(colorDat);
#else
        for (i = 0; i < picHeight; i++) {
            for (j = 0; j < picWidth; j++) {
                readN(s, colorBuf, 3);
                drawBuffer_24(x, y + (picHeight - i), colorBuf, 3);
            }
        }
#endif
    }
}

void DFRobot_Display::drawBmp(uint8_t* pData, int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t size, uint16_t color)
{
    uint16_t i = 0, j = 0, k = 0, m = 0;
    uint8_t  var1 = 0;
    if ((w % 8) == 0)
        w = w / 8;
    else
        w = w / 8 + 1;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            var1 = pData[i * w + j];
            for (k = 0; k < 8; k++) {
                if (var1 & (0x80 >> k)) {
                    for (m = 0; m < size; m++) {
                        drawVLine(x + k * size + 8 * j * size + m, y + i * size, size, color);
                    }
                }
            }
        }
    }
}

void DFRobot_Display::drawBmp(uint8_t* pData, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    int16_t i = 0;
    for (i = 0; i < h; i++) {
        drawBuffer_24(x, y + i, pData + i * w * 3, w * 3);
    }
}

int16_t DFRobot_Display::setRotaion(eROTATION eRotation_)
{
    switch (eRotation) {
        case eROTATION_0:
            width  = rawWidth;
            height = rawHeight;
            break;
        case eROTATION_90:
            width  = rawHeight;
            height = rawWidth;
            break;
        case eROTATION_180:
            width  = rawWidth;
            height = rawHeight;
            break;
        case eROTATION_270:
            width  = rawHeight;
            height = rawWidth;
            break;
        default: return DISPLAY_ERR_NOTSUPPORT;
    }
    eRotation = eRotation_;
    return DISPLAY_ERR_OK;
}

eROTATION DFRobot_Display::getRotation(void)
{
    return eRotation;
}

void DFRobot_Display::setTextColor(uint16_t color)
{
    textColor = color;
}

int16_t DFRobot_Display::getTextColor(void)
{
    return textColor;
}

void DFRobot_Display::setTextBackground(uint16_t color)
{
    textBackground = color;
}

int16_t DFRobot_Display::getTextBackground()
{
    return textBackground;
}

void DFRobot_Display::setTextSize(uint8_t size)
{
    textSize = size;
}

uint8_t DFRobot_Display::getTextSize(void)
{
    return textSize;
}

void DFRobot_Display::setLineWidth(uint16_t w)
{
    lineWidth = w;
}

uint16_t DFRobot_Display::getLineWidth()
{
    return lineWidth;
}

int16_t DFRobot_Display::getWidth(void)
{
    return width;
}

int16_t DFRobot_Display::getHeight(void)
{
    return height;
}

void DFRobot_Display::setDisplayShape(eSHAPE eShape)
{
    switch (eShape) {
        case eSHAPE_CIRCLE:
            eShape        = eSHAPE_CIRCLE;
            displayRadius = width / 2;
            break;
        case eSHAPE_RECT: eShape = eSHAPE_RECT; break;
    }
}

eSHAPE DFRobot_Display::getDisplayShape(void)
{
    return eShape;
}

int16_t DFRobot_Display::getDisplayRadius(void)
{
    if (eShape == eSHAPE_CIRCLE) {
        return displayRadius;
    }
    return 0;
}

void DFRobot_Display::setCursor(int16_t x, int16_t y)
{
    if (x > width) {
        printfX = width;
    } else {
        printfX = x;
    }
    if (y > height) {
        printfY = height;
    } else {
        printfY = y;
    }
}

void DFRobot_Display::getCursor(int16_t* pX, int16_t* pY)
{
    *pX = cursorX;
    *pY = cursorY;
}

void DFRobot_Display::supportChineseFont(void)
{
    pfCharacterFont = GT30L_getCharacter;
}

void DFRobot_Display::setOrign(int16_t x, int16_t y)
{
    if (x > width) {
        cursorX = width;
    } else {
        cursorX = x;
    }
    if (y > height) {
        cursorY = height;
    } else {
        cursorY = y;
    }
}

void DFRobot_Display::getOrign(int16_t* pX, int16_t* pY)
{
    *pX = printfX;
    *pY = printfY;
}

void DFRobot_Display::drawBuffer_16(int16_t x, int16_t y, uint16_t* pBuf, uint16_t count)
{
}

void DFRobot_Display::drawBuffer_24(int16_t x, int16_t y, uint8_t* pBuf, uint16_t count)
{
    uint16_t i = 0;
    for (i = 0; i < (count / 3); i++)
        drawPixel(x + i, y, color24To16(pBuf[i * 3], pBuf[i * 3 + 1], pBuf[i * 3 + 2]));
}
