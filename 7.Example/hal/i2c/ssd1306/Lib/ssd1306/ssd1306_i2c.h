#ifndef _SSD1306_I2C_H_
#define _SSD1306_I2C_H_

#include "i2c.h"

#if 1
#include "ssd1306_conf.h"
#else
#include "ssd1306/ssd1306_conf.h"
#endif

// 通过调整0R电阻来改变地址
#define SSD1306_ADDR_LOW  0X3C
#define SSD1306_ADDR_HIGH 0X3D

#define SSD1306_WIDTH       (128)
#define SSD1306_HEIGHT      (64)
#define SSD1306_ROW_COUNT   (8)
#define SSD1306_COL_COUNT   (128)
#define SSD1306_BUFFER_SZIE (1024)  //(SSD1306_ROW_COUNT * SSD1306_COL_COUNT)

// font: PCtoLCD2002 ：共阴、列行式、逆向输出
// image: Image2Lcd  ：数据水平字节垂直，字节内像素数据反序

#define ssd1306_transmit(dat)        HAL_I2C_Master_Transmit(&SSD1306_I2C, SSD1306_DEV, (uint8_t*)dat, 1, 0xFF)
#define ssd1306_write(reg, dat, len) HAL_I2C_Mem_Write(&SSD1306_I2C, SSD1306_DEV, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)dat, len, 0xFF);

////////////////// functions

void ssd1306_write_cmd(uint8_t cmd);
void ssd1306_write_data(uint8_t data);
void ssd1306_write_ndata(uint8_t* data, uint16_t len);
void ssd1306_init(void);
void ssd1306_set_cursor(uint8_t x, uint8_t y);
void ssd1306_fill(uint8_t data);
void ssd1306_clear(void);
void ssd1306_display_on(void);
void ssd1306_display_off(void);
#if CONFIG_ENABLE_FONT
void ssd1306_show_str(uint8_t x, uint8_t y, uint8_t ch[], uint8_t size);
void ssd1306_show_str_ex(uint8_t x, uint8_t y, uint8_t str[], uint8_t size);
#if CONFIG_ENABLE_FONT_CN_16X16
void ssd1306_show_cn(uint8_t x, uint8_t y, uint8_t N);
#endif
#endif
void ssd1306_show_img(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t img[]);
void ssd1306_show_anim(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t frames, uint8_t fps, uint8_t imgs[]);
void ssd1306_fill_img(uint8_t img[SSD1306_BUFFER_SZIE]);
void ssd1306_fill_anim(uint8_t frames, uint8_t fps, uint8_t imgs[][SSD1306_BUFFER_SZIE]);

/////////////////////////

#endif
