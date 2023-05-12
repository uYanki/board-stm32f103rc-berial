
#ifndef __TM1638_H__
#define __TM1638_H__

#include "gpio.h"
#include "delay/delay.h"

#define TM1638_ACTIVATE   0x8F  // Start up
#define TM1638_READ_BTN   0x42  // Buttons mode
#define TM1638_WRITE_LOC  0x44  // Write to a location 地址固定
#define TM1638_WRITE_INC  0x40  // Incremental write 地址增加
#define TM1638_SEG_ADR    0xC0  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
#define TM1638_LEDS_ADR   0xC1  // Leftmost LED address C1 C3 C5 C7 C9 CB CD CF
#define TM1638_BRIGHT_ADR 0x88  // Brightness address

void tm1638_init(void);
void tm1638_reset(void);

void tm1638_write_cmd(uint8_t cmd);
void tm1638_write_data(uint8_t addr, uint8_t data);

void tm1638_set_brightness(uint8_t brightness);

// 可同时按下多键(具体看原理图的按键是否接了二极管)
uint8_t tm1638_scan_keys(void);

void tm1638_set_led(uint8_t idx, uint8_t val);
void tm1638_set_leds(uint8_t mask);

void tm1638_set_seg(uint8_t idx, uint8_t val);
void tm1638_display_char(uint8_t idx, uint8_t ascii, uint8_t dot);
void tm1638_display_string(uint8_t idx, const char* str);

void tm1638_seg_table_test(void);

void tm1638_do_loading(uint8_t ms);

#endif
