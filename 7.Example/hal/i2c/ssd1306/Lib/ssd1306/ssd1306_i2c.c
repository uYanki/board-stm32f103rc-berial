
#include "ssd1306_i2c.h"

#include "font.h"

// 写命令
inline void ssd1306_write_cmd(uint8_t cmd) { ssd1306_write(0x00, &cmd, 1); }

// 写数据
inline void ssd1306_write_data(uint8_t data) { ssd1306_write(0x40, &data, 1); }

// 写数据
inline void ssd1306_write_ndata(uint8_t* data, uint16_t len) { ssd1306_write(0x40, data, len); }

// 初始化
void ssd1306_init(void)
{
    HAL_Delay(500);  // 这里的延时很重要

    ssd1306_write_cmd(0xAE);  // display off
    ssd1306_write_cmd(0x20);  // Set Memory Addressing Mode
    ssd1306_write_cmd(0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    ssd1306_write_cmd(0xb0);  // Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_write_cmd(0xc8);  // Set COM Output Scan Direction
    ssd1306_write_cmd(0x00);  // set low column address
    ssd1306_write_cmd(0x10);  // set high column address
    ssd1306_write_cmd(0x40);  // set start line address
    ssd1306_write_cmd(0x81);  // set contrast control register
    ssd1306_write_cmd(0xff);  // 亮度调节 0x00~0xff
    ssd1306_write_cmd(0xa1);  // set segment re-map 0 to 127
    ssd1306_write_cmd(0xa6);  // set normal display
    ssd1306_write_cmd(0xa8);  // set multiplex ratio(1 to 64)
    ssd1306_write_cmd(0x3F);
    ssd1306_write_cmd(0xa4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    ssd1306_write_cmd(0xd3);  // -set display offset
    ssd1306_write_cmd(0x00);  // -not offset
    ssd1306_write_cmd(0xd5);  // set display clock divide ratio/oscillator frequency
    ssd1306_write_cmd(0xf0);  // set divide ratio
    ssd1306_write_cmd(0xd9);  // set pre-charge period
    ssd1306_write_cmd(0x22);  //
    ssd1306_write_cmd(0xda);  // set com pins hardware configuration
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(0xdb);  // set vcomh
    ssd1306_write_cmd(0x20);  // 0x20,0.77xVcc
    ssd1306_write_cmd(0x8d);  // set DC-DC enable
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(0xaf);  // turn on SSD1306 panel
}

// 设置起始点坐标
void ssd1306_set_cursor(uint8_t x, uint8_t y)
{
    ssd1306_write_cmd(0xb0 + y);
    ssd1306_write_cmd(((x & 0xf0) >> 4) | 0x10);
    ssd1306_write_cmd(x & 0x0f);
}

// 全屏填充(全亮:0xFF,全灭:0x00)
void ssd1306_fill(uint8_t data)
{
    static const uint8_t zerobuff[128] = {0x00};

    uint8_t y;
    for (y = 0; y < 8; ++y) {
        ssd1306_set_cursor(0, y);
        ssd1306_write_ndata((uint8_t*)zerobuff, 128);
    }
}

// 清屏
void ssd1306_clear(void)
{
    ssd1306_fill(0x00);
}

// 唤醒
void ssd1306_display_on(void)
{
    ssd1306_write_cmd(0X8D);  // 设置电荷泵
    ssd1306_write_cmd(0X14);  // 开启电荷泵
    ssd1306_write_cmd(0XAF);  // SSD1306唤醒
}

// 休眠(功耗<10uA)
void ssd1306_display_off(void)
{
    ssd1306_write_cmd(0X8D);  // 设置电荷泵
    ssd1306_write_cmd(0X10);  // 关闭电荷泵
    ssd1306_write_cmd(0XAE);  // SSD1306休眠
}

#if CONFIG_ENABLE_FONT

// 显示 ascii 字符 ( size = 1: 6*8, 2: 8*16)
void ssd1306_show_str(uint8_t x, uint8_t y, uint8_t str[], uint8_t size)
{
    uint8_t c = 0, i = 0, j = 0;
    switch (size) {
#if CONFIG_ENABLE_FONT_ASCII_6X8
        case 1: {
            while (str[j] != '\0') {
                c = str[j] - 32;
                if (x > 126) {
                    x = 0;
                    ++y;
                }
                ssd1306_set_cursor(x, y);
                ssd1306_write_ndata(&F6x8[c], 6);
                x += 6;
                ++j;
            }
        } break;
#endif
#if CONFIG_ENABLE_FONT_ASCII_8X16
        case 2: {
            while (str[j] != '\0') {
                c = str[j] - 32;
                if (x > 120) {
                    x = 0;
                    ++y;
                }
                uint32_t idx = c << 4;  // idx = c * 16
                ssd1306_set_cursor(x, y);
                ssd1306_write_ndata(&F8X16[idx + 0], 8);
                ssd1306_set_cursor(x, y + 1);
                ssd1306_write_ndata(&F8X16[idx + 8], 8);
                x += 8;
                ++j;
            }
        } break;
#endif
        default: break;
    }
}

// 显示 ascii 字符 ( size = 1: 6*8, 2: 8*16) , 透明
void ssd1306_show_str_ex(uint8_t x, uint8_t y, uint8_t str[], uint8_t size)
{
    uint8_t c = 0, i = 0, j = 0;
    switch (size) {
#if CONFIG_ENABLE_FONT_ASCII_6X8
        case 1: {
            while (str[j] != '\0') {
                c = str[j] - 32;
                if (x > 126) {
                    x = 0;
                    ++y;
                }

                for (i = 0; i < 6; ++i, ++x) {
                    if (F6x8[c][i] == 0x00) continue;
                    ssd1306_set_cursor(x, y);
                    ssd1306_write_data(F6x8[c][i]);
                }

                ++j;
            }
        } break;
#endif
#if CONFIG_ENABLE_FONT_ASCII_8X16
        case 2: {
            while (str[j] != '\0') {
                c = str[j] - 32;
                if (x > 120) {
                    x = 0;
                    ++y;
                }
                uint32_t idx = c << 4;  // idx = c * 16

                uint8_t i;
                for (i = 0; i < 8; ++i, ++x, ++idx) {
                    if (F8X16[idx] == 0x00) continue;
                    ssd1306_set_cursor(x, y);
                    ssd1306_write_data(&F8X16[idx]);
                }
                for (i = 0; i < 8; ++i, ++x, ++idx) {
                    if (F8X16[idx] == 0x00) continue;
                    ssd1306_set_cursor(x, y + 1);
                    ssd1306_write_data(&F8X16[idx]);
                }

                ++j;
            }
        } break;
#endif
        default: break;
    }
}

#endif

#if CONFIG_ENABLE_FONT
#if CONFIG_ENABLE_FONT_CN_16X16

// 显示汉字（需先取模，16*16点阵，N 为 汉字在 font.h 中的索引）
void ssd1306_show_cn(uint8_t x, uint8_t y, uint8_t N)
{
    uint32_t idx = N << 5;  // *32
    ssd1306_set_cursor(x, y);
    ssd1306_write_ndata(&F16x16[idx + 0], 16);
    ssd1306_set_cursor(x, y + 1);
    ssd1306_write_ndata(&F16x16[idx + 16], 16);
}
#endif
#endif

// 显示图片 (x:0~127, y:0~7,x+w<ssd1306_width,y+h<ssd1306_height)
void ssd1306_show_img(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t img[])
{
    uint32_t j = 0;
    while (h--) {
        ssd1306_set_cursor(x, y++);
        ssd1306_write_ndata(&img[j], w);
        j += w;
    }
}

// 显示动画
void ssd1306_show_anim(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t frames, uint8_t fps, uint8_t imgs[])
{
    uint32_t       size = w * h;  //  frame size
    const uint8_t* p    = imgs;
    for (uint8_t i = 0; i < frames; ++i, p += size) {
        ssd1306_show_img(x, y, w, h, (uint8_t*)p);
        HAL_Delay(1000 / fps);
    }
}

// 填充图片
void ssd1306_fill_img(uint8_t img[SSD1306_BUFFER_SZIE])
{
    ssd1306_set_cursor(0, 0);
    ssd1306_write_ndata(img, SSD1306_BUFFER_SZIE);
}

// 填充动画
void ssd1306_fill_anim(uint8_t frames, uint8_t fps, uint8_t imgs[][SSD1306_BUFFER_SZIE])
{
    for (uint8_t i = 0; i < frames; ++i) {
        ssd1306_fill_img(imgs[i]);
        HAL_Delay(1000 / fps);
    }
}
