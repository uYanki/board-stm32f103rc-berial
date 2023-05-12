#ifndef _WS2812_H
#define _WS2812_H

#include "main.h"

void ws2812_show(void);
void ws2812_set_all(uint8_t R, uint8_t G, uint8_t B);
void ws2812_clear(void);

uint32_t GetGRB(uint8_t R, uint8_t G, uint8_t B);
void     ws2812_set_pixel(uint16_t n, uint32_t GRB);

#endif
