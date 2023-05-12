
#include "st7735.h"

#define DELAY 0x80

// based on Adafruit ST7735 library for Arduino
static const uint8_t
    init_cmds1[] = {            // Init for 7735R, part 1 (red or green tab)
        15,                     // 15 commands in list:
        ST7735_SWRESET, DELAY,  //  1: Software reset, 0 args, w/delay
        150,                    //     150 ms delay
        ST7735_SLPOUT, DELAY,   //  2: Out of sleep mode, 0 args, w/delay
        255,                    //     500 ms delay
        ST7735_FRMCTR1, 3,      //  3: Frame rate ctrl - normal mode, 3 args:
        0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR2, 3,      //  4: Frame rate control - idle mode, 3 args:
        0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR3, 6,      //  5: Frame rate ctrl - partial mode, 6 args:
        0x01, 0x2C, 0x2D,       //     Dot inversion mode
        0x01, 0x2C, 0x2D,       //     Line inversion mode
        ST7735_INVCTR, 1,       //  6: Display inversion ctrl, 1 arg, no delay:
        0x07,                   //     No inversion
        ST7735_PWCTR1, 3,       //  7: Power control, 3 args, no delay:
        0xA2,
        0x02,              //     -4.6V
        0x84,              //     AUTO mode
        ST7735_PWCTR2, 1,  //  8: Power control, 1 arg, no delay:
        0xC5,              //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
        ST7735_PWCTR3, 2,  //  9: Power control, 2 args, no delay:
        0x0A,              //     Opamp current small
        0x00,              //     Boost frequency
        ST7735_PWCTR4, 2,  // 10: Power control, 2 args, no delay:
        0x8A,              //     BCLK/2, Opamp current small & Medium low
        0x2A,
        ST7735_PWCTR5, 2,  // 11: Power control, 2 args, no delay:
        0x8A, 0xEE,
        ST7735_VMCTR1, 1,  // 12: Power control, 1 arg, no delay:
        0x0E,
        ST7735_INVOFF, 0,  // 13: Don't invert display, no args, no delay
        ST7735_MADCTL, 1,  // 14: Memory access control (directions), 1 arg:
        ST7735_ROTATION,   //     row addr/col addr, bottom to top refresh
        ST7735_COLMOD, 1,  // 15: set color mode, 1 arg, no delay:
        0x05},             //     16-bit color

#if (defined(ST7735_IS_128X128) || defined(ST7735_IS_160X128))
    init_cmds2[] = {      // Init for 7735R, part 2 (1.44" display)
        2,                //  2 commands in list:
        ST7735_CASET, 4,  //  1: Column addr set, 4 args, no delay:
        0x00, 0x00,       //     XSTART = 0
        0x00, 0x7F,       //     XEND = 127
        ST7735_RASET, 4,  //  2: Row addr set, 4 args, no delay:
        0x00, 0x00,       //     XSTART = 0
        0x00, 0x7F},      //     XEND = 127
#endif                    // ST7735_IS_128X128

    init_cmds3[] = {                                                                                                          // Init for 7735R, part 3 (red or green tab)
        4,                                                                                                                    //  4 commands in list:
        ST7735_GMCTRP1, 16,                                                                                                   //  1: Magical unicorn dust, 16 args, no delay:
        0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10, ST7735_GMCTRN1, 16,   //  2: Sparkles and rainbows, 16 args, no delay:
        0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10, ST7735_NORON, DELAY,  //  3: Normal display on, no args, w/delay
        10,                                                                                                                   //     10 ms delay
        ST7735_DISPON, DELAY,                                                                                                 //  4: Main screen turn on, no args w/delay
        100};                                                                                                                 //     100 ms delay

static void ST7735_Select()
{
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

void ST7735_Unselect()
{
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

static void ST7735_Reset()
{
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
    HAL_Delay(20);
}

static void ST7735_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&LCD_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ST7735_WriteData(uint8_t* buff, size_t buff_size)
{
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&LCD_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
}

static void ST7735_ExecuteCommandList(const uint8_t* addr)
{
    uint8_t  numCommands, numArgs;
    uint16_t ms;

    numCommands = *addr++;
    while (numCommands--) {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);

        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if (numArgs) {
            ST7735_WriteData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }

        if (ms) {
            ms = *addr++;
            if (ms == 255) ms = 500;
            HAL_Delay(ms);
        }
    }
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = {0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART};
    ST7735_WriteData(data, sizeof(data));

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + ST7735_YSTART;
    data[3] = y1 + ST7735_YSTART;
    ST7735_WriteData(data, sizeof(data));

    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

void ST7735_Init()
{
    ST7735_Select();
    ST7735_Reset();
    ST7735_ExecuteCommandList(init_cmds1);
    ST7735_ExecuteCommandList(init_cmds2);
    ST7735_ExecuteCommandList(init_cmds3);
    ST7735_Unselect();
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    ST7735_Select();

    ST7735_SetAddressWindow(x, y, x + 1, y + 1);
    uint8_t data[] = {color >> 8, color & 0xFF};
    ST7735_WriteData(data, sizeof(data));

    ST7735_Unselect();
}

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;
    ST7735_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);
    for (i = 0; i < font.height; ++i) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; ++j) {
            if ((b << j) & 0x8000) {
                uint8_t data[] = {color >> 8, color & 0xFF};
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
                ST7735_WriteData(data, sizeof(data));
            }
        }
    }
}

/*
Simpler (and probably slower) implementation:

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color) {
    uint32_t i, b, j;

    for(i = 0; i < font.height; ++i) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; ++j) {
            if((b << j) & 0x8000)  {
                ST7735_DrawPixel(x + j, y + i, color);
            }
        }
    }
}
*/

void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
{
    ST7735_Select();

    while (*str) {
        if (x + font.width >= ST7735_WIDTH) {
            x = 0;
            y += font.height;
            if (y + font.height >= ST7735_HEIGHT) break;
            if (*str == ' ') {
                // skip spaces in the beginning of the new line
                ++str;
                continue;
            }
        }

        ST7735_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        ++str;
    }

    ST7735_Unselect();
}

void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    // clipping
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if ((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if ((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    uint8_t data[] = {color >> 8, color & 0xFF};
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    for (y = h; y > 0; --y)
        for (x = w; x > 0; --x)
            HAL_SPI_Transmit(&LCD_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);

    ST7735_Unselect();
}

void ST7735_FillScreen(uint16_t color)
{
    ST7735_FillRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if ((x + w - 1) >= ST7735_WIDTH) return;
    if ((y + h - 1) >= ST7735_HEIGHT) return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ST7735_WriteData((uint8_t*)data, sizeof(uint16_t) * w * h);
    ST7735_Unselect();
}

void ST7735_InvertColors(bool invert)
{
    ST7735_Select();
    ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_Unselect();
}

///////////////////////////////////////////////////////

// Bresenham 画圆
void Gui_DrawCircle(uint16_t X, uint16_t Y, uint16_t R, uint16_t fc)
{
    uint16_t a, b;
    int      c;
    a = 0;
    b = R;
    c = 3 - 2 * R;
    while (a < b) {
        Gui_DrawPoint(X + a, Y + b, fc);  // 7
        Gui_DrawPoint(X - a, Y + b, fc);  // 6
        Gui_DrawPoint(X + a, Y - b, fc);  // 2
        Gui_DrawPoint(X - a, Y - b, fc);  // 3
        Gui_DrawPoint(X + b, Y + a, fc);  // 8
        Gui_DrawPoint(X - b, Y + a, fc);  // 5
        Gui_DrawPoint(X + b, Y - a, fc);  // 1
        Gui_DrawPoint(X - b, Y - a, fc);  // 4

        if (c < 0)
            c = c + 4 * a + 6;
        else {
            c = c + 4 * (a - b) + 10;
            b -= 1;
        }
        a += 1;
    }
    if (a == b) {
        Gui_DrawPoint(X + a, Y + b, fc);
        Gui_DrawPoint(X + a, Y + b, fc);
        Gui_DrawPoint(X + a, Y - b, fc);
        Gui_DrawPoint(X - a, Y - b, fc);
        Gui_DrawPoint(X + b, Y + a, fc);
        Gui_DrawPoint(X - b, Y + a, fc);
        Gui_DrawPoint(X + b, Y - a, fc);
        Gui_DrawPoint(X - b, Y - a, fc);
    }
}

// Bresenham 画线
void Gui_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t Color)
{
    int dx,   // difference in x's
        dy,   // difference in y's
        dx2,  // dx,dy * 2
        dy2,
        x_inc,  // amount in pixel space to move during drawing
        y_inc,  // amount in pixel space to move during drawing
        error,  // the discriminant i.e. error i.e. decision variable
        index;  // used for looping

    ST7735_SetAddressWindow(x0, y0, x0, y0);
    dx = x1 - x0;
    dy = y1 - y0;

    if (dx >= 0) {
        x_inc = 1;
    } else {
        x_inc = -1;
        dx    = -dx;
    }

    if (dy >= 0) {
        y_inc = 1;
    } else {
        y_inc = -1;
        dy    = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy) {
        // x距离大于y距离, 那么每个x轴上只有一个点, 每个y轴上有若干个点,  且线的点数等于x距离，以x轴递增画点

        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++)  // 要画的点数不会超过x距离
        {
            // 画点
            Gui_DrawPoint(x0, y0, Color);

            // test if error has overflowed
            if (error >= 0)  // 是否需要增加y坐标值
            {
                error -= dx2;
                // move to next line
                y0 += y_inc;
            }  // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc;
        }  // end for
    }      // end if |slope| <= 1
    else {
        // y轴大于x轴，则每个y轴上只有一个点，x轴若干个点, 以y轴为递增画点

        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++) {
            // set the pixel
            Gui_DrawPoint(x0, y0, Color);

            // test if error overflowed
            if (error >= 0) {
                error -= dy2;

                // move to next line
                x0 += x_inc;
            }  // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
        }  // end for
    }      // end else |slope| > 1
}

void Gui_DrawBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t mode)
{
    if (mode == 0) {
        Gui_DrawLine(x, y, x + w, y, 0xEF7D);
        Gui_DrawLine(x + w, y, x + w, y + h, 0x2965);
        Gui_DrawLine(x, y + h, x + w, y + h, 0x2965);
        Gui_DrawLine(x, y, x, y + h, 0xEF7D);
    }
    if (mode == 1) {
        Gui_DrawLine(x, y, x + w, y, 0x2965);
        Gui_DrawLine(x + w, y, x + w, y + h, 0xEF7D);
        Gui_DrawLine(x, y + h, x + w, y + h, 0xEF7D);
        Gui_DrawLine(x, y, x, y + h, 0x2965);
    }
    if (mode == 2) {
        Gui_DrawLine(x, y, x + w, y, 0xffff);
        Gui_DrawLine(x + w, y, x + w, y + h, 0xffff);
        Gui_DrawLine(x, y + h, x + w, y + h, 0xffff);
        Gui_DrawLine(x, y, x, y + h, 0xffff);
    }
}

// 凸起的按钮框
void DisplayButtonDown(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Gui_DrawLine(x1, y1, x2, y1, ST7735_GRAY2);              // H
    Gui_DrawLine(x1 + 1, y1 + 1, x2, y1 + 1, ST7735_GRAY1);  // H
    Gui_DrawLine(x1, y1, x1, y2, ST7735_GRAY2);              // V
    Gui_DrawLine(x1 + 1, y1 + 1, x1 + 1, y2, ST7735_GRAY1);  // V
    Gui_DrawLine(x1, y2, x2, y2, ST7735_WHITE);              // H
    Gui_DrawLine(x2, y1, x2, y2, ST7735_WHITE);              // V
}

// 凹下的按钮框
void DisplayButtonUp(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Gui_DrawLine(x1, y1, x2, y1, ST7735_WHITE);              // H
    Gui_DrawLine(x1, y1, x1, y2, ST7735_WHITE);              // V
    Gui_DrawLine(x1 + 1, y2 - 1, x2, y2 - 1, ST7735_GRAY1);  // H
    Gui_DrawLine(x1, y2, x2, y2, ST7735_GRAY2);              // H
    Gui_DrawLine(x2 - 1, y1 + 1, x2 - 1, y2, ST7735_GRAY1);  // V
    Gui_DrawLine(x2, y1, x2, y2, ST7735_GRAY2);              // V
}
