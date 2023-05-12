
#include "egl_device_st7735.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------

//============================================================================
// locals
//============================================================================

#define DELAY 0x80

// based on Adafruit ST7735 library for Arduino
static const uint8_t init_cmd[] =
    {                           // Init for 7735R, part 1 (red or green tab)
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

#if defined(ST7735_IS_128X128) || defined(ST7735_IS_160X128)
               // Init for 7735R, part 2 (1.44" display)
        2,                //  2 commands in list:
        ST7735_CASET, 4,  //  1: Column addr set, 4 args, no delay:
        0x00, 0x00,       //     XSTART = 0
        0x00, 0x7F,       //     XEND = 127
        ST7735_RASET, 4,  //  2: Row addr set, 4 args, no delay:
        0x00, 0x00,       //     XSTART = 0
        0x00, 0x7F,       //     XEND = 127
#endif                    // ST7735_IS_128X128

#ifdef ST7735_IS_160X80
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
        ST7735_GMCTRP1, 16,                                                                                                   //  1: Magical unicorn dust, 16 args, no delay:
        0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10, ST7735_GMCTRN1, 16,   //  2: Sparkles and rainbows, 16 args, no delay:
        0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10, ST7735_NORON, DELAY,  //  3: Normal display on, no args, w/delay
        10,                                                                                                                   //     10 ms delay
        ST7735_DISPON, DELAY,                                                                                                 //  4: Main screen turn on, no args w/delay
        100,                                                                                                                  //     100 ms delay

        // end
        0};

//----------------------------------------------------------------------------

//============================================================================
// graphics_device_st7735
//============================================================================
graphics_device_st7735* graphics_device_st7735::s_active_dev = 0;
//----------------------------------------------------------------------------

graphics_device_st7735::graphics_device_st7735()
{
    EGL_ASSERT(!s_active_dev);
    s_active_dev = this;
    m_tile_rt0   = 0;
}
//----

graphics_device_st7735::~graphics_device_st7735()
{
    s_active_dev = 0;
}
//----

void graphics_device_st7735::init(
    spi_t  spi_,
    port_t port_cs_,
    port_t port_rs_,
    port_t port_dc_,
    port_t port_bl_,
    pin_t  pin_cs_,
    pin_t  pin_rs_,
    pin_t  pin_dc_,
    pin_t  pin_bl_)
{
    HAL_Delay(1000);  // 等待稳定

    // setup control pins

    m_spi     = spi_;
    m_port_cs = port_cs_;
    m_port_rs = port_rs_;
    m_port_dc = port_dc_;
    m_port_bl = port_bl_;
    m_pin_cs  = pin_cs_;
    m_pin_rs  = pin_rs_;
    m_pin_dc  = pin_dc_;
    m_pin_bl  = pin_bl_;

    reset();

    select();

    const uint8_t* addr = init_cmd;
    uint8_t        cmd_cnt, arg_cnt;
    uint16_t       ms;

    while (1) {
        // count of command
        cmd_cnt = *addr++;

        if (cmd_cnt == 0) break;

        while (cmd_cnt--) {
            // write comand
            write_cmd(*addr++);
            // counf of arguments
            arg_cnt = *addr++;
            // if high bit set, delay follows args

            if (arg_cnt & DELAY) {
                ms = *addr++;
                if (ms == 255)
                    ms = 500;
                HAL_Delay(ms);
            } else {
                start_data();
                write_data((uint8_t*)addr, arg_cnt);
                addr += arg_cnt;
            }
        }
    }

    write_cmd(0 ? ST7735_INVON : ST7735_INVOFF);

    // clear the screen to black
    set_data_window(0, 0, fb_width - 1, fb_height - 1);
    start_data();
    for (uint32_t i = 0; i < fb_width * fb_height; ++i)
        write_data(0);

    EGL_LOG("ST7735 graphics device initialized!\r\n");

    unselect();

    HAL_Delay(1000);
    backlight_on();
}
//----

void graphics_device_st7735::init_rasterizer(const rasterizer_cfg& rcfg_, const rasterizer_tiling_cfg& tcfg_, const rasterizer_vertex_cache_cfg& vccfg_)
{
    graphics_device<graphics_device_st7735>::init(rcfg_, tcfg_, vccfg_);
    m_tile_rt0    = (fb_format_t*)rcfg_.rts[0];
    m_tile_width  = tcfg_.tile_width;
    m_tile_height = tcfg_.tile_height;
}
//----

void graphics_device_st7735::init_dma(rasterizer_data_transfer*, uint8_t num_transfers_, fb_format_t* dma_buffer_, size_t dma_buffer_size_)
{
}
//----------------------------------------------------------------------------

void graphics_device_st7735::submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16& reg_min_, const vec2u16& reg_end_, uint16_t thread_idx_)
{
    // access update pos, size and data
    uint16_t     x             = tx_ * m_tile_width + reg_min_.x;
    uint16_t     y             = ty_ * m_tile_height + reg_min_.y;
    uint16_t     update_width  = reg_end_.x - reg_min_.x;
    uint16_t     update_height = reg_end_.y - reg_min_.y;
    fb_format_t* data          = m_tile_rt0 + reg_min_.x + reg_min_.y * m_tile_width;

    // transfer tile update region to the display
    if (m_tile_shader)
        m_tile_shader->transfer_region(render_targets(), depth_target(), size_t(data - m_tile_rt0), x, y, update_width, update_height, m_tile_width);
    else {
        select();
        set_data_window(x, y, x + update_width - 1, y + update_height - 1);
        start_data();
        fb_format_t* data_end = data + m_tile_width * update_height;
        do {
            fb_format_t *data_scan = data, *data_scan_end = data_scan + update_width;
            do {
                write_data(data_scan->v);
            } while (++data_scan < data_scan_end);
            data += m_tile_width;
        } while (data < data_end);
        unselect();
    }
}
