
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

    st7735_backlight_on();
}

void st7735_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t data[4] = {0x00};

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
