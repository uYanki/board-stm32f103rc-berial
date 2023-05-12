#ifndef __ST7789_H__
#define __ST7789_H__

#include "spi.h"

#include "fonts.h"
#include "images.h"

/* port */

#define LCD_SPI_PORT      hspi3

#define ST7789_NSS_SIGNAL 1

#define st7789_delay(n)   HAL_Delay(n)  // delay_ms

#if 1  // chip select
#define st7789_select()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define st7789_unselect() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#else
#define st7789_select()   ;
#define st7789_unselect() ;
#endif

#define st7789_start_cmd()  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define st7789_start_data() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)

#define st7789_rst_l()      HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define st7789_rst_h()      HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

#if 1  // backlight control

#define st7789_backlight_on()  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)
#define st7789_backlight_off() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)

#endif

#define st7789_transmit(buff, size) HAL_SPI_Transmit(&LCD_SPI_PORT, buff, size, 0xFF)
// #define st7789_receive(buff, size)                    HAL_SPI_Receive(&LCD_SPI_PORT, buff, size, 0xFF)
// #define st7789_transmit_receive(txbuff, rxbuff, size) HAL_SPI_TransmitReceive(&LCD_SPI_PORT, txbuff, rxbuff, size, 0xFF)

/* resolution */

#define ST7789_135X240              0
#define ST7789_240X240              1
#define ST7789_170X320              2

#define LCD_SIZE                    ST7789_170X320

/* rotation */

// #define LCD_ROTATION 0
#define LCD_ROTATION                1
// #define LCD_ROTATION 2
// #define LCD_ROTATION 3

///////////////////

/* resolution & offset */

#if LCD_SIZE == ST7789_135X240

#if LCD_ROTATION == 0
#define ST7789_WIDTH  135
#define ST7789_HEIGHT 240
#define ST7789_XSTART 53
#define ST7789_YSTART 40
#elif LCD_ROTATION == 1
#define ST7789_WIDTH  240
#define ST7789_HEIGHT 135
#define ST7789_XSTART 40
#define ST7789_YSTART 52
#elif LCD_ROTATION == 2
#define ST7789_WIDTH  135
#define ST7789_HEIGHT 240
#define ST7789_XSTART 52
#define ST7789_YSTART 40
#elif LCD_ROTATION == 3
#define ST7789_WIDTH  240
#define ST7789_HEIGHT 135
#define ST7789_XSTART 40
#define ST7789_YSTART 53
#endif

#elif LCD_SIZE == ST7789_170X320

#if LCD_ROTATION == 0
#define ST7789_WIDTH  170
#define ST7789_HEIGHT 320
#define ST7789_XSTART 35
#define ST7789_YSTART 0
#elif LCD_ROTATION == 1
#define ST7789_WIDTH  320
#define ST7789_HEIGHT 170
#define ST7789_XSTART 0
#define ST7789_YSTART 35
#elif LCD_ROTATION == 2
#define ST7789_WIDTH  170
#define ST7789_HEIGHT 320
#define ST7789_XSTART 35
#define ST7789_YSTART 0
#elif LCD_ROTATION == 3
#define ST7789_WIDTH  320
#define ST7789_HEIGHT 170
#define ST7789_XSTART 0
#define ST7789_YSTART 35
#endif

#elif LCD_SIZE == ST7789_240X240

#define ST7789_WIDTH  240
#define ST7789_HEIGHT 240

#if LCD_ROTATION == 0
#define ST7789_XSTART 0
#define ST7789_YSTART 80
#elif LCD_ROTATION == 1
#define ST7789_XSTART 80
#define ST7789_YSTART 0
#elif LCD_ROTATION == 2
#define ST7789_XSTART 0
#define ST7789_YSTART 0
#elif LCD_ROTATION == 3
#define ST7789_XSTART 0
#define ST7789_YSTART 0
#endif

#endif

/* rotation */

/**
 * Memory Data Access Control Register (0x36H)
 * MAP:     D7  D6  D5  D4  D3  D2  D1  D0
 * param:   MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00

#if LCD_ROTATION == 0
#define ST7789_ROTATION ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB
#elif LCD_ROTATION == 1
#define ST7789_ROTATION ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB
#elif LCD_ROTATION == 2
#define ST7789_ROTATION ST7789_MADCTL_RGB
#elif LCD_ROTATION == 3
#define ST7789_ROTATION ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB
#else
#warning "LCD_ROTATION is not be defined or range not in [0,3]"
#endif

///////////////////

// #define ST7789_COLOR_65K   0x50
// #define ST7789_COLOR_262K  0x60
// #define ST7789_COLOR_12BIT 0x03
// #define ST7789_COLOR_16BIT 0x05
// #define ST7789_COLOR_18BIT 0x06
// #define ST7789_COLOR_16M   0x07

// #define ST7789_COLOR_MODE_16BIT (ST7789_COLOR_MODE_65K | ST7789_COLOR_MODE_16BIT)   // 0x55
// #define ST7789_COLOR_MODE_18BIT (ST7789_COLOR_MODE_262K | ST7789_COLOR_MODE_18BIT)  // 0x66

#define ST7789_COLOR_MODE_16BIT 0x55  //  RGB565 (16bit)
#define ST7789_COLOR_MODE_18BIT 0x66  //  RGB666 (18bit)

#define ST7789_COLOR_MODE       ST7789_COLOR_MODE_16BIT

/* Color (RGB565 format) */

#if ST7789_COLOR_MODE == ST7789_COLOR_MODE_16BIT

#define COLOR_RGB565_WHITE      0xFFFF
#define COLOR_RGB565_BLACK      0x0000
#define COLOR_RGB565_BLUE       0x001F
#define COLOR_RGB565_RED        0xF800
#define COLOR_RGB565_MAGENTA    0xF81F
#define COLOR_RGB565_GREEN      0x07E0
#define COLOR_RGB565_CYAN       0x7FFF
#define COLOR_RGB565_YELLOW     0xFFE0
#define COLOR_RGB565_BROWN      0XBC40
#define COLOR_RGB565_BRRED      0XFC07
#define COLOR_RGB565_DARKBLUE   0X01CF
#define COLOR_RGB565_LIGHTBLUE  0X7D7C
#define COLOR_RGB565_GRAYBLUE   0X5458
#define COLOR_RGB565_LIGHTGREEN 0X841F
#define COLOR_RGB565_LGRAY      0XC618
#define COLOR_RGB565_LGRAYBLUE  0XA651
#define COLOR_RGB565_LBBLUE     0X2B12

#define COLOR_RGB565(r, g, b)   (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#endif

///////////////////////////////////////////

void st7789_write_cmd(uint8_t cmd);
void st7789_write_data(uint8_t dat);
void st7789_write_ndata(uint8_t* dat, uint16_t len);

void st7789_init(void);
void st7789_reset(void);
void st7789_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7789_invert(uint8_t enable);

void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void st7789_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void st7789_draw_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void st7789_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void st7789_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void st7789_clear(uint16_t color);

#endif
