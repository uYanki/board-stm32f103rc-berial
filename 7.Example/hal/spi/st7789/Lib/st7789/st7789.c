
#include "st7789.h"

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_RDDPM      0x0A  // read display power mode
#define ST7789_RDD_MADCTL 0x0B  // read display MADCTL
#define ST7789_RDD_COLMOD 0x0C  // read display pixel format
#define ST7789_RDDIM      0x0D  // read display image mode
#define ST7789_RDDSM      0x0E  // read display signal mode
#define ST7789_RDDSR      0x0F  // read display self-diagnostic result (ST7789V)

#define ST7789_SLPIN  0x10  // enable sleep mode
#define ST7789_SLPOUT 0x11  // disable sleep mode
#define ST7789_PTLON  0x12
#define ST7789_NORON  0x13

#define ST7789_INVOFF  0x20  // non-invert color
#define ST7789_INVON   0x21  // invert color 反色
#define ST7789_GAMSET  0x26  // gamma set
#define ST7789_DISPOFF 0x28  // display off
#define ST7789_DISPON  0x29  // display on
#define ST7789_CASET   0x2A  // column address set
#define ST7789_RASET   0x2B  // row address set
#define ST7789_RAMWR   0x2C
#define ST7789_RGBSET  0x2D  // color setting for 4096, 64K and 262K colors
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_VSCRDEF 0x33  // vertical scrolling definition (ST7789V)
#define ST7789_TEOFF   0x34  // tearing effect line off
#define ST7789_TEON    0x35  // tearing effect line on
#define ST7789_MADCTL  0x36  // memory data access control
#define ST7789_IDMOFF  0x38  // idle mode off
#define ST7789_IDMON   0x39  // idle mode on
#define ST7789_RAMWRC  0x3C  // memory write continue (ST7789V)
#define ST7789_RAMRDC  0x3E  // memory read continue (ST7789V)
#define ST7789_COLMOD  0x3A

#define ST7789_RAMCTRL   0xB0  // RAM control
#define ST7789_RGBCTRL   0xB1  // RGB control
#define ST7789_PORCTRL   0xB2  // Porch control
#define ST7789_FRCTRL1   0xB3  // Frame rate control
#define ST7789_PARCTRL   0xB5  // Partial mode control
#define ST7789_GCTRL     0xB7  // Gate control
#define ST7789_GTADJ     0xB8  // Gate on timing adjustment
#define ST7789_DGMEN     0xBA  // Digital gamma enable
#define ST7789_VCOMS     0xBB  // VCOMS setting
#define ST7789_LCMCTRL   0xC0  // LCM control
#define ST7789_IDSET     0xC1  // ID setting
#define ST7789_VDVVRHEN  0xC2  // VDV and VRH command enable
#define ST7789_VRHS      0xC3  // VRH set
#define ST7789_VDVSET    0xC4  // VDV setting
#define ST7789_VCMOFSET  0xC5  // VCOMS offset set
#define ST7789_FRCTR2    0xC6  // FR Control 2
#define ST7789_CABCCTRL  0xC7  // CABC control
#define ST7789_REGSEL1   0xC8  // Register value section 1
#define ST7789_REGSEL2   0xCA  // Register value section 2
#define ST7789_PWMFRSEL  0xCC  // PWM frequency selection
#define ST7789_PWCTRL1   0xD0  // Power control 1
#define ST7789_VAPVANEN  0xD2  // Enable VAP/VAN signal output
#define ST7789_CMD2EN    0xDF  // Command 2 enable
#define ST7789_PVGAMCTRL 0xE0  // Positive voltage gamma control
#define ST7789_NVGAMCTRL 0xE1  // Negative voltage gamma control
#define ST7789_DGMLUTR   0xE2  // Digital gamma look-up table for red
#define ST7789_DGMLUTB   0xE3  // Digital gamma look-up table for blue
#define ST7789_GATECTRL  0xE4  // Gate control
#define ST7789_SPI2EN    0xE7  // SPI2 enable
#define ST7789_PWCTRL2   0xE8  // Power control 2
#define ST7789_EQCTRL    0xE9  // Equalize time control
#define ST7789_PROMCTRL  0xEC  // Program control
#define ST7789_PROMEN    0xFA  // Program mode enable
#define ST7789_NVMSET    0xFC  // NVM setting
#define ST7789_PROMACT   0xFE  // Program action

#define ST7789_RDID1 0xDA
#define ST7789_RDID2 0xDB
#define ST7789_RDID3 0xDC
#define ST7789_RDID4 0xDD

//

#define DELAY 0x80

static const uint8_t st7789_init_cmd[] = {
    // count of commands
    18,
    ST7789_SWRESET, DELAY, 50,                        // soft reset
    ST7789_COLMOD, 1, ST7789_COLOR_MODE,              // color mode
    ST7789_PORCTRL, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33,  // porch control
    ST7789_MADCTL, 1, ST7789_ROTATION,                // display rotation
    ST7789_GCTRL, 1, 0x35,                            // gate control
    ST7789_VCOMS, 1, 0x19,                            // default 0.75v for 0x20
    ST7789_LCMCTRL, 1, 0x2C,
    ST7789_VDVVRHEN, 1, 0x01,
    ST7789_VRHS, 1, 0x12,  // +-4.45v, defalut +-4.1v for 0x0B
    ST7789_VDVSET, 1, 0x20,
    ST7789_FRCTR2, 1, 0x0F,  // frame rate = 60hz
    ST7789_PWCTRL1, 2, 0xA4, 0XA1,
    ST7789_PVGAMCTRL, 14, 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23,
    ST7789_NVGAMCTRL, 14, 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23,
    ST7789_INVON, 0,           //	inverst color
    ST7789_SLPOUT, DELAY, 50,  //	out of sleep mode
    ST7789_NORON, 0,           //	normal display on
    ST7789_DISPON, DELAY, 50,  //	screen turned on
    // end
    0};

void st7789_write_cmd(uint8_t cmd)
{
    st7789_start_cmd();
    st7789_transmit(&cmd, 1);
}

void st7789_write_data(uint8_t dat)
{
    st7789_start_data();
    st7789_transmit(&dat, 1);
}

void st7789_write_ndata(uint8_t* dat, uint16_t len)
{
    st7789_start_data();
    st7789_transmit(dat, len);
}

void st7789_reset(void)
{
    st7789_delay(25);
    st7789_rst_l();
    st7789_delay(25);
    st7789_rst_h();
    st7789_delay(50);
}

void st7789_init(void)
{
    st7789_reset();  // hard reest

#if ST7789_NSS_SIGNAL
    st7789_select();
#endif

    const uint8_t* addr = st7789_init_cmd;
    uint8_t        cmd_cnt, arg_cnt, ms;

    while (1) {
        // count of command
        if ((cmd_cnt = *addr++) == 0) break;
        while (cmd_cnt--) {
            // write comand
            // printf("cmd = 0x%x\r\n", *addr);
            st7789_write_cmd(*addr++);
            // counf of arguments
            if ((arg_cnt = *addr++) > 0) {
                // if high bit set, delay follows args
                if (arg_cnt & DELAY) {
                    ms = *addr++;
                    st7789_delay((ms == 255) ? 500 : ms);
                } else {
                    st7789_write_ndata((uint8_t*)addr, arg_cnt);
                    addr += arg_cnt;
                }
            }
        }
    }

#if ST7789_NSS_SIGNAL
    st7789_unselect();
#endif
}

void st7789_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t data[4] = {0x00};

    x1 += ST7789_XSTART;
    x2 += ST7789_XSTART;
    y1 += ST7789_YSTART;
    y2 += ST7789_YSTART;

    // column address set
    data[0] = x1 >> 8;
    data[1] = x1 & 0xFF;
    data[2] = x2 >> 8;
    data[3] = x2 & 0xFF;
    st7789_write_cmd(ST7789_CASET);
    st7789_write_ndata(data, 4);

    // row address set
    data[0] = y1 >> 8;
    data[1] = y1 & 0xFF;
    data[2] = y2 >> 8;
    data[3] = y2 & 0xFF;
    st7789_write_cmd(ST7789_RASET);
    st7789_write_ndata(data, 4);

    // write to RAM
    st7789_write_cmd(ST7789_RAMWR);
}

// invert color
void st7789_invert(uint8_t enable)
{
#if ST7789_NSS_SIGNAL
    st7789_select();
#endif
    st7789_write_cmd(enable ? ST7789_INVON : ST7789_INVOFF);
#if ST7789_NSS_SIGNAL
    st7789_unselect();
#endif
}

// //////////////////////////////////////////////////////////////

void st7789_clear(uint16_t color)
{
    st7789_fill_rect(0, 0, ST7789_WIDTH, ST7789_HEIGHT, color);
}

void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
#if 0
    if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT)) return;
#endif
#if ST7789_NSS_SIGNAL
    st7789_select();
#endif
    st7789_set_window(x, y, x + 1, y + 1);
    uint8_t data[] = {color >> 8, color & 0xFF};
    st7789_write_ndata(data, sizeof(data));
#if ST7789_NSS_SIGNAL
    st7789_unselect();
#endif
}

void st7789_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
#if ST7789_NSS_SIGNAL
    st7789_select();
#endif
    uint32_t i, b, j;
    st7789_set_window(x, y, x + font.width - 1, y + font.height - 1);
    for (i = 0; i < font.height; ++i) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; ++j) {
            if ((b << j) & 0x8000) {
                uint8_t data[] = {color >> 8, color & 0xFF};
                st7789_write_ndata(data, sizeof(data));
            } else {
                uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
                st7789_write_ndata(data, sizeof(data));
            }
        }
    }
#if ST7789_NSS_SIGNAL
    st7789_unselect();
#endif
}

void st7789_draw_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
{
    while (*str) {
        if (x + font.width >= ST7789_WIDTH) {
            x = 0;
            y += font.height;
            if (y + font.height >= ST7789_HEIGHT) break;
            if (*str == ' ') {
                // skip spaces in the beginning of the new line
                ++str;
                continue;
            }
        }

        st7789_draw_char(x, y, *str, font, color, bgcolor);
        x += font.width;
        ++str;
    }
}

void st7789_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
#if 1
    if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT)) return;
    if ((x + w) > ST7789_WIDTH) w = ST7789_WIDTH - x;
    if ((y + h) > ST7789_HEIGHT) h = ST7789_HEIGHT - y;
#endif

#if ST7789_NSS_SIGNAL
    st7789_select();
#endif

    st7789_set_window(x, y, x + w - 1, y + h - 1);

    uint32_t size    = w * h;
    uint8_t  data[2] = {color >> 8, color & 0xFF};
    st7789_start_data();
    while (size--) st7789_transmit(data, 2);

#if ST7789_NSS_SIGNAL
    st7789_unselect();
#endif
}

void st7789_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
#if ST7789_NSS_SIGNAL
    st7789_select();
#endif

#if 1
    if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT)) return;
    if ((x + w) > ST7789_WIDTH) return;
    if ((y + h) > ST7789_HEIGHT) return;
#endif

    st7789_set_window(x, y, x + w - 1, y + h - 1);
    st7789_write_ndata((uint8_t*)data, sizeof(uint16_t) * w * h);

#if ST7789_NSS_SIGNAL
    st7789_unselect();
#endif
}
