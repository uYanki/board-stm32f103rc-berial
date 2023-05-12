#ifndef EGL_DEVICE_ST7735_H
#define EGL_DEVICE_ST7735_H
//----------------------------------------------------------------------------

//============================================================================
// interface
//============================================================================
// external
#include "../egl_device.h"

#include "main.h"

// rotation

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

// lcd config

#define ST7735_IS_160X80 1
#define ST7735_XSTART    0  // offset
#define ST7735_YSTART    24
#define ST7735_WIDTH     160  // size
#define ST7735_HEIGHT    80
#define ST7735_ROTATION  (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)

// registers

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

EGL_NAMESPACE_BEGIN

// new
class graphics_device_st7735;
//----------------------------------------------------------------------------

//============================================================================
// graphics_device_st7735
//============================================================================

// stm32
typedef GPIO_TypeDef*      port_t;
typedef uint16_t           pin_t;
typedef SPI_HandleTypeDef* spi_t;

class graphics_device_st7735 : public graphics_device<graphics_device_st7735> {
public:
    // device properties
    enum { fb_width  = ST7735_WIDTH,
           fb_height = ST7735_HEIGHT };
    enum { fb_format = pixfmt_b5g6r5 };
    typedef pixel<e_pixel_format(fb_format)> fb_format_t;
    //--------------------------------------------------------------------------

    // construction
    graphics_device_st7735();
    ~graphics_device_st7735();

    void init(spi_t spi_, port_t port_cs_, port_t port_rs_, port_t port_dc_, port_t port_bl_, pin_t pin_cs_, pin_t pin_rs_, pin_t pin_dc_, pin_t pin_bl_);
    void init_rasterizer(const rasterizer_cfg&, const rasterizer_tiling_cfg&, const rasterizer_vertex_cache_cfg&);
    void init_dma(rasterizer_data_transfer*, uint8_t num_transfers_, fb_format_t* dma_buffer_, size_t dma_buffer_size_);
    //--------------------------------------------------------------------------

    // immediate rendering interface
    template <class IPShader>
    EGL_INLINE void fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader&);
    template <class IPShader>
    EGL_INLINE void fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader&);
    template <class IPShader>
    EGL_INLINE void fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader&);
    //--------------------------------------------------------------------------

private:
    graphics_device_st7735(const graphics_device_st7735&);  // not implemented
    void         operator=(const graphics_device_st7735&);  // not implemented
    virtual void submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16& reg_min_, const vec2u16& reg_end_, uint16_t thread_idx_);

    EGL_INLINE void write_cmd(uint8_t cmd_)
    {
        HAL_GPIO_WritePin(m_port_dc, m_pin_dc, GPIO_PIN_RESET);
        HAL_SPI_Transmit(m_spi, &cmd_, 1, HAL_MAX_DELAY);
    }

    EGL_INLINE void start_data() { HAL_GPIO_WritePin(m_port_dc, m_pin_dc, GPIO_PIN_SET); }
    EGL_INLINE void write_data(uint16_t data_)
    {
        uint8_t buff[2] = {uint8_t(data_ >> 8), uint8_t(data_ & 0xff)};
        HAL_SPI_Transmit(m_spi, buff, 2, HAL_MAX_DELAY);
        // uint8_t buff;
        // buff = data_ >> 8;
        // HAL_SPI_Transmit(m_spi, &buff, 1, HAL_MAX_DELAY);
        // buff = data_ & 0xff;
        // HAL_SPI_Transmit(m_spi, &buff, 1, HAL_MAX_DELAY);
    }
    EGL_INLINE void write_data(uint8_t* data_, uint8_t len_) { HAL_SPI_Transmit(m_spi, data_, len_, HAL_MAX_DELAY); }

    EGL_INLINE void set_data_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    EGL_INLINE void select() { HAL_GPIO_WritePin(m_port_cs, m_pin_cs, GPIO_PIN_RESET); }
    EGL_INLINE void unselect() { HAL_GPIO_WritePin(m_port_cs, m_pin_cs, GPIO_PIN_SET); }

    EGL_INLINE void backlight_on() { HAL_GPIO_WritePin(m_port_bl, m_pin_bl, GPIO_PIN_RESET); }
    EGL_INLINE void backlight_off() { HAL_GPIO_WritePin(m_port_bl, m_pin_bl, GPIO_PIN_SET); }

    EGL_INLINE void reset()
    {
        HAL_GPIO_WritePin(m_port_rs, m_pin_rs, GPIO_PIN_RESET);
        HAL_Delay(5);
        HAL_GPIO_WritePin(m_port_rs, m_pin_rs, GPIO_PIN_SET);
        HAL_Delay(5);
    }

    //--------------------------------------------------------------------------

    static graphics_device_st7735* s_active_dev;

    // spi
    spi_t m_spi;
    // ports
    port_t m_port_cs;
    port_t m_port_rs;
    port_t m_port_dc;
    port_t m_port_bl;
    // pins
    pin_t m_pin_cs;
    pin_t m_pin_rs;
    pin_t m_pin_dc;
    pin_t m_pin_bl;

    // tile data
    fb_format_t*           m_tile_rt0;
    rasterizer_tile_size_t m_tile_width;
    rasterizer_tile_size_t m_tile_height;
};
//----------------------------------------------------------------------------

//============================================================================
// graphics_device_st7735
//============================================================================
template <class IPShader>
void graphics_device_st7735::fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader& ips_)
{
    // draw horizontal line
    uint16_t x_end = x_ + width_;
    select();
    set_data_window(x_, y_, x_end - 1, y_);
    start_data();
    fb_format_t res;
    do {
        ips_.exec(res, x_, y_);
        write_data(res.v);
        ++x_;
    } while (x_ < x_end);
    unselect();
}
//----

template <class IPShader>
void graphics_device_st7735::fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader& ips_)
{
    // draw vertical line
    uint16_t y_end = y_ + height_;
    select();
    set_data_window(x_, y_, x_, y_end - 1);
    start_data();
    fb_format_t res;
    do {
        ips_.exec(res, x_, y_);
        write_data(res.v);
        ++y_;
    } while (y_ < y_end);
    unselect();
}
//----

template <class IPShader>
void graphics_device_st7735::fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader& ips_)
{
    // draw rectangle
    uint16_t x_end = x_ + width_, y_end = y_ + height_;
    select();
    set_data_window(x_, y_, x_end - 1, y_end - 1);
    start_data();
    fb_format_t res;
    do {
        uint16_t x = x_;
        do {
            ips_.exec(res, x, y_);
            write_data(res.v);
            ++x;
        } while (x < x_end);
        ++y_;
    } while (y_ < y_end);
    unselect();
}
//----------------------------------------------------------------------------

void graphics_device_st7735::set_data_window(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_)
{
    // setup data window

    x0_ += ST7735_XSTART;
    x1_ += ST7735_XSTART;
    y0_ += ST7735_YSTART;
    y1_ += ST7735_YSTART;

    // column address set
    write_cmd(ST7735_CASET);  // 0x2a
    start_data();
    write_data(x0_);
    write_data(x1_);
    // row address set
    write_cmd(ST7735_RASET);  // 0x2b
    start_data();
    write_data(y0_);
    write_data(y1_);
    // write to RAM
    write_cmd(ST7735_RAMWR);  // 0x2c
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
