#ifndef __OLED_U8G2_H_
#define __OLED_U8G2_H_

#include "i2c.h"
#include "u8g2/u8g2.h"

#define CONFIG_SCREEN_HEIGHT 64   // height of screen
#define CONFIG_SCREEN_WIDTH  128  // width of screen

#define SSD1306_ADDRESS 0x78

uint8_t u8x8_byte_hw_i2c(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr);
uint8_t u8x8_gpio_and_delay(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr);
void    u8g2Init(u8g2_t* u8g2);

#endif