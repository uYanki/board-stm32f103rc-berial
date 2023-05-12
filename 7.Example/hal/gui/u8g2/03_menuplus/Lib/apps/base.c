#include "base.h"

u8g2_t u8g2;

uint8_t* buf_ptr;
uint16_t buf_len;

void u8g2_init()
{
    u8g2Init(&u8g2);

    // u8g2_SetBusClock(&u8g2, 800000);
    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese1);
    // u8g2_SetContrast(&u8g2,10);

    buf_ptr = u8g2_GetBufferPtr(&u8g2);
    buf_len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);
}

void effect_disappear()
{
    uint16_t i;

    for (i = 0; i < buf_len; i += 2) 
        buf_ptr[i] &= 0x55;
    
    _update();

    for (i = 1; i < buf_len; i += 2) 
        buf_ptr[i] &= 0xAA;
    
    _update();

    for (i = 0; i < buf_len; i += 2) 
        buf_ptr[i] &= 0x00;
    
    _update();

    for (i = 1; i < buf_len; i += 2) 
        buf_ptr[i] &= 0x00;
    
    _update();
}
