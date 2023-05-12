#ifndef _SSD1306_CONFIG_H_
#define _SSD1306_CONFIG_H_

#define CONFIG_ENABLE_FONT 0

#if CONFIG_ENABLE_FONT == 1
#define CONFIG_ENABLE_FONT_CN_16X16   1
#define CONFIG_ENABLE_FONT_ASCII_6X8  1
#define CONFIG_ENABLE_FONT_ASCII_8X16 1
#endif

#define SSD1306_DEV (SSD1306_ADDR_LOW << 1)  // <--- 器件地址
#define SSD1306_I2C hi2c2                    // <--- 硬件I2C

#endif