#include "lcd1602.h"

uint8_t backlight = LCD_BL_HIGH;
uint8_t display_control, display_mode;

static uint8_t row_offsets[] = {LCD_LINE1_START, LCD_LINE2_START, LCD_LINE3_START, LCD_LINE4_START};

void LCD_Init(void)
{
    LCD_WriteCmd(0x28);                    // 0b00111000 显示模式设置
    LCD_WriteCmd(0x0C);                    // 0b00001100 开启显示开关但不闪烁
    LCD_WriteCmd(display_mode = 0x06);     // 0b00000110 显示光标移动位置
    LCD_WriteCmd(display_control = 0x01);  // 0b00000001 清除屏幕
    return;
#if DRIVEMODE == DRIVEMODE_I2C_PCF8574
    //  Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x7 characters)
    LCD_WriteCmd(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x7DOTS);
#endif
    // LCD_WriteCmd(LCD_RETURNHOME);
    // Display on/off control --> D=0,C=0, B=0  ---> display off
    // LCD_WriteCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
    // clear display
    LCD_WriteCmd(LCD_CLEARDISPLAY);
    // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    LCD_WriteCmd(display_mode = LCD_ENTRYMODESET | LCD_ENTRYLEFT);
    // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
    LCD_WriteCmd(display_control = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
}

void LCD_WriteByte(uint8_t byte, uint8_t mode /* rs = 0x00:cmd, 0x01:data */)
{
#if DRIVEMODE == DRIVEMODE_4BIT

#elif DRIVEMODE == DRIVEMODE_8BIT

#elif DRIVEMODE == DRIVEMODE_I2C_PCF8574
#define LCD_RS mode
#define LCD_BL backlight
    uint8_t high      = (byte & 0xf0);
    uint8_t low       = ((byte << 4) & 0xf0);
    uint8_t buffer[4] = {
        high | LCD_RS | LCD_BL | LCD_EN_HIGH,
        high | LCD_RS | LCD_BL | LCD_EN_LOW,
        low | LCD_RS | LCD_BL | LCD_EN_HIGH,
        low | LCD_RS | LCD_BL | LCD_EN_LOW};
    HAL_Delay(5);
    HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_DEV, buffer, 4, 0xFF);
#endif
}

// Turn the (optional) backlight off/on
void LCD_BacklightOn(void)
{
#if DRIVEMODE == DRIVEMODE_4BIT

#elif DRIVEMODE == DRIVEMODE_8BIT

#elif DRIVEMODE == DRIVEMODE_I2C_PCF8574
    backlight = LCD_BL_HIGH;
    HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_DEV, &backlight, 1, 0xFF);
#endif
}
void LCD_BacklightOff(void)
{
#if DRIVEMODE == DRIVEMODE_4BIT

#elif DRIVEMODE == DRIVEMODE_8BIT

#elif DRIVEMODE == DRIVEMODE_I2C_PCF8574
    backlight = LCD_BL_LOW;
    HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_DEV, &backlight, 1, 0xFF);
#endif
}

void LCD_ShowChar(const char ch) { LCD_WriteData(ch); }

void LCD_ShowString(const char* str)
{
    while (*str) LCD_WriteData(*str++);
}

void LCD_ClearLine(uint8_t line)
{
    // 仅适用于写入方向为左的情况 ( display_mode & LCD_ENTRYLEFT != 0 )
    LCD_SetCursor(0x00, line);
    LCD_ShowString("                ");  // 16
}

void LCD_SetCursor(uint8_t col, uint8_t row)
{
    if (row >= LCD_DISPLAY_LINES) row = LCD_DISPLAY_LINES - 1;
    LCD_WriteCmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// 显示字符串（带自动换行）
void LCD_ShowStringWrap(uint8_t col, uint8_t row, const char* str)
{
    // set cursor
    row += col / LCD_DISPLAY_LENGTH;
    if (row >= LCD_DISPLAY_LINES) return;
    col %= LCD_DISPLAY_LENGTH;
    LCD_WriteCmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
    // show string
    for (uint8_t i = col; *str; ++i) {
        if (i == LCD_DISPLAY_LENGTH) {
            if (++row == LCD_DISPLAY_LINES) return;
            LCD_WriteCmd(LCD_SETDDRAMADDR | row_offsets[row]);  // wrap
            i = 0;
        }
        LCD_WriteData(*str++);
    }
}

// Turn the display on/off (quickly)
void LCD_DisplayOn(void) { LCD_WriteCmd(display_control |= LCD_DISPLAYON); }
void LCD_DisplayOff(void) { LCD_WriteCmd(display_control &= ~LCD_DISPLAYON); }
// Turns the underline cursor on/off
void LCD_CursorOn(void) { LCD_WriteCmd(display_control |= LCD_CURSORON); }
void LCD_CursorOff(void) { LCD_WriteCmd(display_control &= ~LCD_CURSORON); }
// Turn on and off the blinking cursor
void LCD_BlinkOn(void) { LCD_WriteCmd(display_control |= LCD_BLINKON); }
void LCD_BlinkOff(void) { LCD_WriteCmd(display_control &= ~LCD_BLINKON); }
// This is for text that flows Left to Right
void LCD_LeftToRight(void) { LCD_WriteCmd(display_mode |= LCD_ENTRYLEFT); }
// This is for text that flows Right to Left
void LCD_RightToLeft(void) { LCD_WriteCmd(display_mode &= ~LCD_ENTRYLEFT); }
// This will 'right justify' text from the cursor 每写1个字符，屏幕左移1次，但光标位置不变, 以实现内容右对齐光标
void LCD_AutoScrollOn(void) { LCD_WriteCmd(display_mode |= LCD_ENTRYSHIFTINCREMENT); }
// This will 'left justify' text from the cursor
void LCD_AutoScrollOff(void) { LCD_WriteCmd(display_mode &= ~LCD_ENTRYSHIFTINCREMENT); }

// Allows us to fill the first 8 CGRAM locations with custom characters 显示自定义字符
void LCD_CreateChar(uint8_t location /*0~7*/, uint8_t charmap[])
{
    // uint8_t cgram[8] = {0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78};
    location &= 0x07;  // only have 8 locations
    LCD_WriteCmd(LCD_SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; ++i) LCD_WriteData(charmap[i]);
}

// These commands scroll the display without changing the RAM  (once)
// LCD_CURSORSHIFT: 光标移动（可选），LCD_DISPLAYMOVE：显示屏内容移动（可选），LCD_MOVELEFT / LCD_MOVERIGHT：移动的方向
void LCD_ScrollLeft(void) { LCD_WriteCmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT); }
void LCD_ScrollRight(void) { LCD_WriteCmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT); }

// set cursor position to zero 光标归位
void LCD_ReturnHome(void) { LCD_WriteCmd(LCD_RETURNHOME); }
// clear display, set cursor position to zero
void LCD_Clear(void) { LCD_WriteCmd(LCD_CLEARDISPLAY); }
