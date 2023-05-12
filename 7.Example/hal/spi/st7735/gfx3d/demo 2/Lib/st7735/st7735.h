/* vim: set ai et ts=4 sw=4: */
#ifndef __ST7735_H__
#define __ST7735_H__

#include "fonts.h"
#include "spi.h"

//

#define LCD_SPI_PORT hspi3

#define ST7735_NSS_SIGNAL 1

// w & h
#define ST7735_128X128 0  // 1.44
#define ST7735_160X80  1  // 0.96
#define ST7735_80X160  2
#define ST7735_160X128 3  // 1.8
#define ST7735_128X160 4

#define ST7735_SIZE ST7735_128X160

#if ST7735_SIZE == ST7735_128X128
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#elif ST7735_SIZE == ST7735_160X80
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#elif ST7735_SIZE == ST7735_80X160
#define ST7735_WIDTH  80
#define ST7735_HEIGHT 160
#elif ST7735_SIZE == ST7735_160X128
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#elif ST7735_SIZE == ST7735_128X160
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#endif

// offset
#define ST7735_XSTART 2
#define ST7735_YSTART 1

// rotation
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

// #define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV)  // rotate left
// #define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV)  // rotate right
#define ST7735_ROTATION (0)

/////////////////////////////////////////// Color

#define ST7735_BLACK             0x0000
#define ST7735_BLUE              0x001F
#define ST7735_RED               0xF800
#define ST7735_GREEN             0x07E0
#define ST7735_CYAN              0x07FF
#define ST7735_MAGENTA           0xF81F
#define ST7735_YELLOW            0xFFE0
#define ST7735_WHITE             0xFFFF
#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

///////////////////////////////////////////

#define st7735_select()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define st7735_unselect() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define st7735_start_cmd()  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define st7735_start_data() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)

#define st7735_rst_l() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define st7735_rst_h() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

#define st7735_delay(n) HAL_Delay(n)

#define st7735_transmit(buff, size) HAL_SPI_Transmit(&LCD_SPI_PORT, buff, size, 0xFF)
// #define st7735_receive(buff, size)                    HAL_SPI_Receive(&LCD_SPI_PORT, buff, size, 0xFF)
// #define st7735_transmit_receive(txbuff, rxbuff, size) HAL_SPI_TransmitReceive(&LCD_SPI_PORT, txbuff, rxbuff, size, 0xFF)

#define st7735_backlight_on()  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)
#define st7735_backlight_off() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)

///////////////////////////////////////////

void st7735_write_cmd(uint8_t cmd);
void st7735_write_data(uint8_t dat);
void st7735_write_ndata(uint8_t* dat, uint16_t len);

void st7735_init(void);
void st7735_reset(void);
void st7735_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7735_invert(uint8_t enable);

// call the following functions before calling st7735_select() and then call st7735_unselect() if not use ST7735_NSS_SIGNAL
void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void st7735_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void st7735_draw_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void st7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void st7735_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void st7735_clear(uint16_t color);

#endif