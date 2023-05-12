
#include "st7735.h"

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN  0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON  0x12
#define ST7735_NORON  0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR  0x30
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#if 0

#define ST7735_NOP                   0x00  // No Operation: NOP
#define ST7735_SW_RESET              0x01  // Software reset: SWRESET
#define ST7735_READ_ID               0x04  // Read Display ID: RDDID
#define ST7735_READ_STATUS           0x09  // Read Display Statu: RDDST
#define ST7735_READ_POWER_MODE       0x0A  // Read Display Power: RDDPM
#define ST7735_READ_MADCTL           0x0B  // Read Display: RDDMADCTL
#define ST7735_READ_PIXEL_FORMAT     0x0C  // Read Display Pixel: RDDCOLMOD
#define ST7735_READ_IMAGE_MODE       0x0D  // Read Display Image: RDDIM
#define ST7735_READ_SIGNAL_MODE      0x0E  // Read Display Signal: RDDSM
#define ST7735_SLEEP_IN              0x10  // Sleep in & booster off: SLPIN
#define ST7735_SLEEP_OUT             0x11  // Sleep out & booster on: SLPOUT
#define ST7735_PARTIAL_DISPLAY_ON    0x12  // Partial mode on: PTLON
#define ST7735_NORMAL_DISPLAY_OFF    0x13  // Partial off (Normal): NORON
#define ST7735_DISPLAY_INVERSION_OFF 0x20  // Display inversion off: INVOFF
#define ST7735_DISPLAY_INVERSION_ON  0x21  // Display inversion on: INVON
#define ST7735_GAMMA_SET             0x26  // Gamma curve select: GAMSET
#define ST7735_DISPLAY_OFF           0x28  // Display off: DISPOFF
#define ST7735_DISPLAY_ON            0x29  // Display on: DISPON
#define ST7735_CASET                 0x2A  // Column address set: CASET
#define ST7735_RASET                 0x2B  // Row address set: RASET
#define ST7735_WRITE_RAM             0x2C  // Memory write: RAMWR
#define ST7735_RGBSET                0x2D  // LUT for 4k,65k,262k color: RGBSET
#define ST7735_READ_RAM              0x2E  // Memory read: RAMRD
#define ST7735_PTLAR                 0x30  // Partial start/end address set: PTLAR
#define ST7735_TE_LINE_OFF           0x34  // Tearing effect line off: TEOFF
#define ST7735_TE_LINE_ON            0x35  // Tearing effect mode set & on: TEON
#define ST7735_MADCTL                0x36  // Memory data access control: MADCTL
#define ST7735_IDLE_MODE_OFF         0x38  // Idle mode off: IDMOFF
#define ST7735_IDLE_MODE_ON          0x39  // Idle mode on: IDMON
#define ST7735_COLOR_MODE            0x3A  // Interface pixel format: COLMOD
#define ST7735_FRAME_RATE_CTRL1      0xB1  // In normal mode (Full colors): FRMCTR1
#define ST7735_FRAME_RATE_CTRL2      0xB2  // In Idle mode (8-colors): FRMCTR2
#define ST7735_FRAME_RATE_CTRL3      0xB3  // In partial mode + Full colors: FRMCTR3
#define ST7735_FRAME_INVERSION_CTRL  0xB4  // Display inversion control: INVCTR
#define ST7735_DISPLAY_SETTING       0xB6  // Display function setting
#define ST7735_PWR_CTRL1             0xC0  // Power control setting: PWCTR1
#define ST7735_PWR_CTRL2             0xC1  // Power control setting: PWCTR2
#define ST7735_PWR_CTRL3             0xC2  // In normal mode (Full colors): PWCTR3
#define ST7735_PWR_CTRL4             0xC3  // In Idle mode (8-colors): PWCTR4
#define ST7735_PWR_CTRL5             0xC4  // In partial mode + Full colors: PWCTR5
#define ST7735_VCOMH_VCOML_CTRL1     0xC5  // VCOM control 1: VMCTR1
#define ST7735_VMOF_CTRL             0xC7  // Set VCOM offset control: VMOFCTR
#define ST7735_WRID2                 0xD1  // Set LCM version code: WRID2
#define ST7735_WRID3                 0xD2  // Customer Project code: WRID3
#define ST7735_NV_CTRL1              0xD9  // NVM control status: NVCTR1
#define ST7735_READ_ID1              0xDA  // Read ID1: RDID1
#define ST7735_READ_ID2              0xDB  // Read ID2: RDID2
#define ST7735_READ_ID3              0xDC  // Read ID3: RDID3
#define ST7735_NV_CTRL2              0xDE  // NVM Read Command: NVCTR2
#define ST7735_NV_CTRL3              0xDF  // NVM Write Command: NVCTR3
#define ST7735_PV_GAMMA_CTRL         0xE0  // Set Gamma adjustment (+ polarity): GAMCTRP1
#define ST7735_NV_GAMMA_CTRL         0xE1  // Set Gamma adjustment (- polarity): GAMCTRN1
#define ST7735_EXT_CTRL              0xF0  // Extension command control
#define ST7735_PWR_CTRL6             0xFC  // In partial mode + Idle mode: PWCTR6
#define ST7735_VCOM4_LEVEL           0xFF  // VCOM 4 level control

#endif

//

#define DELAY 0x80

static const uint8_t init_cmd[] = {

    // Init for 7735R, part 1 (red or green tab)
    15,                     // 15 commands in list:
    ST7735_SWRESET, DELAY,  //  1: Software reset, 0 args, w/delay
    150,                    //     150 ms delay
    ST7735_SLPOUT, DELAY,   //  2: Out of sleep mode, 0 args, w/delay
    255,                    //     500 ms delay
    ST7735_FRMCTR1, 3,      //  3: Frame rate ctrl - normal mode, 3 args:
    0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,      //  4: Frame rate control - idle mode, 3 args:
    0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,      //  5: Frame rate ctrl - partial mode, 6 args:
    0x01, 0x2C, 0x2D,       //     Dot inversion mode
    0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR, 1,       //  6: Display inversion ctrl, 1 arg, no delay:
    0x07,                   //     No inversion
    ST7735_PWCTR1, 3,       //  7: Power control, 3 args, no delay:
    0xA2,
    0x02,              //     -4.6V
    0x84,              //     AUTO mode
    ST7735_PWCTR2, 1,  //  8: Power control, 1 arg, no delay:
    0xC5,              //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3, 2,  //  9: Power control, 2 args, no delay:
    0x0A,              //     Opamp current small
    0x00,              //     Boost frequency
    ST7735_PWCTR4, 2,  // 10: Power control, 2 args, no delay:
    0x8A,              //     BCLK/2, Opamp current small & Medium low
    0x2A,
    ST7735_PWCTR5, 2,  // 11: Power control, 2 args, no delay:
    0x8A, 0xEE,
    ST7735_VMCTR1, 1,  // 12: Power control, 1 arg, no delay:
    0x0E,
    ST7735_INVOFF, 0,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL, 1,  // 14: Memory access control (directions), 1 arg:
    ST7735_ROTATION,   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD, 1,  // 15: set color mode, 1 arg, no delay:
    0x05,              //     16-bit color

#if (ST7735_SIZE == ST7735_128X128) || (ST7735_SIZE == ST7735_160X128) || (ST7735_SIZE == ST7735_128X160)

    // Init for 7735R, part 2 (1.44" display)
    2,                //  2 commands in list:
    ST7735_CASET, 4,  //  1: Column addr set, 4 args, no delay:
    0x00, 0x00,       //     XSTART = 0
    0x00, 0x7F,       //     XEND = 127
    ST7735_RASET, 4,  //  2: Row addr set, 4 args, no delay:
    0x00, 0x00,       //     XSTART = 0
    0x00, 0x7F,       //     XEND = 127

#elif (ST7735_SIZE == ST7735_160X80) || (ST7735_SIZE == ST7735_80X160)

    // Init for 7735S, part 2 (160x80 display)
    3,                //  3 commands in list:
    ST7735_CASET, 4,  //  1: Column addr set, 4 args, no delay:
    0x00, 0x00,       //     XSTART = 0
    0x00, 0x4F,       //     XEND = 79
    ST7735_RASET, 4,  //  2: Row addr set, 4 args, no delay:
    0x00, 0x00,       //     XSTART = 0
    0x00, 0x9F,       //     XEND = 159
    ST7735_INVON, 0,  //  3: Invert colors

#endif

    // Init for 7735R, part 3 (red or green tab)
    4,                                                                                                                    //  4 commands in list:
    ST7735_GMCTRP1, 16,                                                                                                   //  1: Gamma Adjustments (pos. polarity), 16 args, no delay:
    0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10, ST7735_GMCTRN1, 16,   //  2: Gamma Adjustments (neg. polarity), 16 args, no delay:
    0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10, ST7735_NORON, DELAY,  //  3: Normal display on, no args, w/delay
    10,                                                                                                                   //     10 ms delay
    ST7735_DISPON, DELAY,                                                                                                 //  4: Main screen turn on, no args w/delay
    100,                                                                                                                  //     100 ms delay

    // end
    0};

void st7735_write_cmd(uint8_t cmd)
{
    st7735_start_cmd();
    st7735_transmit(&cmd, 1);
}

void st7735_write_data(uint8_t dat)
{
    st7735_start_data();
    st7735_transmit(&dat, 1);
}

void st7735_write_ndata(uint8_t* dat, uint16_t len)
{
    st7735_start_data();
    st7735_transmit(dat, len);
}

void st7735_reset(void)
{
    st7735_rst_l();
    st7735_delay(10);
    st7735_rst_h();
    st7735_delay(10);
}

void st7735_init(void)
{
    st7735_select();

    st7735_reset();

    const uint8_t* addr = init_cmd;
    uint8_t        cmd_cnt, arg_cnt;
    uint16_t       ms;

    while (1) {
        // count of command
        cmd_cnt = *addr++;
        if (cmd_cnt == 0) break;
        while (cmd_cnt--) {
            // write comand
            st7735_write_cmd(*addr++);
            // counf of arguments
            arg_cnt = *addr++;
            // if high bit set, delay follows args
            if (arg_cnt & DELAY) {
                ms = *addr++;
                if (ms == 255)
                    ms = 500;
                st7735_delay(ms);
            } else {
                st7735_write_ndata((uint8_t*)addr, arg_cnt);
                addr += arg_cnt;
            }
        }
    }

    st7735_unselect();
}

void st7735_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t data[4]={0x00};

    x1 += ST7735_XSTART;
    x2 += ST7735_XSTART;
    y1 += ST7735_YSTART;
    y2 += ST7735_YSTART;

    // column address set
    data[0] = x1 >> 8;
    data[1] = x1 & 0xFF;
    data[2] = x2 >> 8;
    data[3] = x2 & 0xFF;
    st7735_write_cmd(ST7735_CASET);
    st7735_write_ndata(data, sizeof(data));

    // row address set
    data[0] = y1 >> 8;
    data[1] = y1 & 0xFF;
    data[2] = y2 >> 8;
    data[3] = y2 & 0xFF;
    st7735_write_cmd(ST7735_RASET);
    st7735_write_ndata(data, sizeof(data));

    // write to RAM
    st7735_write_cmd(ST7735_RAMWR);
}

void st7735_invert(uint8_t enable)  // invert color
{
    st7735_select();
    st7735_write_cmd(enable ? ST7735_INVON : ST7735_INVOFF);
    st7735_unselect();
}

//////////////////////////////////////////////////////////////

void st7735_clear(uint16_t color)
{
    st7735_fill_rect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
// if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
#if ST7735_NSS_SIGNAL
    st7735_select();
#endif
    st7735_set_window(x, y, x + 1, y + 1);
    uint8_t data[] = {color >> 8, color & 0xFF};
    st7735_write_ndata(data, sizeof(data));
#if ST7735_NSS_SIGNAL
    st7735_unselect();
#endif
}

void st7735_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
#if ST7735_NSS_SIGNAL
    st7735_select();
#endif
    uint32_t i, b, j;
    st7735_set_window(x, y, x + font.width - 1, y + font.height - 1);
    for (i = 0; i < font.height; ++i) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; ++j) {
            if ((b << j) & 0x8000) {
                uint8_t data[] = {color >> 8, color & 0xFF};
                st7735_write_ndata(data, sizeof(data));
            } else {
                uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
                st7735_write_ndata(data, sizeof(data));
            }
        }
    }
#if ST7735_NSS_SIGNAL
    st7735_unselect();
#endif
}

void st7735_draw_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
{
    while (*str) {
        if (x + font.width >= ST7735_WIDTH) {
            x = 0;
            y += font.height;
            if (y + font.height >= ST7735_HEIGHT) break;
            if (*str == ' ') {
                // skip spaces in the beginning of the new line
                ++str;
                continue;
            }
        }

        st7735_draw_char(x, y, *str, font, color, bgcolor);
        x += font.width;
        ++str;
    }
}

void st7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
#if ST7735_NSS_SIGNAL
    st7735_select();
#endif

#if 1
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if ((x + w) > ST7735_WIDTH) w = ST7735_WIDTH - x;
    if ((y + h) > ST7735_HEIGHT) h = ST7735_HEIGHT - y;
#endif

    st7735_set_window(x, y, x + w - 1, y + h - 1);
    uint32_t size   = w * h;
    uint8_t  data[] = {color >> 8, color & 0xFF};
    st7735_start_data();
    while (size--) st7735_transmit(data, 2);

#if ST7735_NSS_SIGNAL
    st7735_unselect();
#endif
}

void st7735_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h,const uint16_t* data)
{
#if ST7735_NSS_SIGNAL
    st7735_select();
#endif

#if 1
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if ((x + w) > ST7735_WIDTH) return;
    if ((y + h) > ST7735_HEIGHT) return;
#endif
	
		st7735_set_window(x, y, x + w - 1, y + h - 1);
	  st7735_write_ndata((uint8_t*)data,  sizeof(uint16_t) * w * h);
	
#if ST7735_NSS_SIGNAL
    st7735_unselect();
#endif
}
