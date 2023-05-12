//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_DEVICE_ILI9488_H
#define EGL_DEVICE_ILI9488_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "../egl_device.h"
EGL_NAMESPACE_BEGIN

// new
class graphics_device_ili9488;
//----------------------------------------------------------------------------


//============================================================================
// graphics_device_ili9488
//============================================================================
class graphics_device_ili9488: public graphics_device<graphics_device_ili9488>
{
public:
  // device properties
  enum {fb_width=320, fb_height=480};
  enum {fb_format=pixfmt_b5g6r5};
  typedef pixel<e_pixel_format(fb_format)> fb_format_t;
  //--------------------------------------------------------------------------
  
  // construction
  graphics_device_ili9488();
  graphics_device_ili9488(uint8_t pin_cs_, uint8_t pin_rs_, uint8_t pin_wr_, uint8_t pin_rd_, uint8_t pin_reset_=0xff);
  ~graphics_device_ili9488();
  void init(uint8_t pin_cs_, uint8_t pin_rs_, uint8_t pin_wr_, uint8_t pin_rd_, uint8_t pin_reset_=0xff);
  void init_rasterizer(const rasterizer_cfg&, const rasterizer_tiling_cfg&, const rasterizer_vertex_cache_cfg&);
  void init_dma(rasterizer_data_transfer*, uint8_t num_transfers_, fb_format_t *dma_buffer_, size_t dma_buffer_size_);
  //--------------------------------------------------------------------------

  // immediate rendering interface
  template<class IPShader> EGL_INLINE void fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader&);
  template<class IPShader> EGL_INLINE void fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader&);
  template<class IPShader> EGL_INLINE void fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader&);
  //--------------------------------------------------------------------------

private:
  graphics_device_ili9488(const graphics_device_ili9488&); // not implemented
  void operator=(const graphics_device_ili9488&); // not implemented
  virtual void submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16 &reg_min_, const vec2u16 &reg_end_, uint16_t thread_idx_);
  EGL_INLINE void set_data_pins(uint8_t);
  EGL_INLINE void write_cmd(uint8_t);
  EGL_INLINE void start_data();
  EGL_INLINE void write_data(uint8_t);
  EGL_INLINE void wait_twrl();
  EGL_INLINE void wait_twrh();
  EGL_INLINE void set_data_window(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_);
  //--------------------------------------------------------------------------

  static graphics_device_ili9488 *s_active_dev;
#if defined(EGL_PLATFORM_TEENSY4X)
  typedef volatile uint32_t *port_reg_t;
#else
  typedef volatile uint8_t *port_reg_t;
#endif
  // pins
  port_reg_t m_port_cs;
  port_reg_t m_port_rs;
  port_reg_t m_port_wr;
  port_reg_t m_port_rd;
#ifdef EGL_PLATFORM_TEENSY4X
  port_reg_t m_port_data;
#endif
  uint32_t m_mask_cs;
  uint32_t m_mask_rs;
  uint32_t m_mask_wr;
  uint32_t m_mask_rd;
  // tile data
  fb_format_t *m_tile_rt0;
  rasterizer_tile_size_t m_tile_width;
  rasterizer_tile_size_t m_tile_height;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// graphics_device_ili9488
//============================================================================
template<class IPShader>
void graphics_device_ili9488::fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader &ips_)
{
  // draw horizontal line
	uint16_t x_end=x_+width_;
  *m_port_cs&=~m_mask_cs;
	set_data_window(x_, y_, x_end-1, y_);
  start_data();
  fb_format_t res;
  do
  {
    ips_.exec(res, x_, y_);
    write_data(res.v>>8);
    write_data(res.v);
    ++x_;
  } while(x_<x_end);
  *m_port_cs|=m_mask_cs;
}
//----

template<class IPShader>
void graphics_device_ili9488::fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader &ips_)
{
  // draw vertical line
	uint16_t y_end=y_+height_;
  *m_port_cs&=~m_mask_cs;
	set_data_window(x_, y_, x_, y_end-1);
  start_data();
  fb_format_t res;
  do
  {
    ips_.exec(res, x_, y_);
    write_data(res.v>>8);
    write_data(res.v);
    ++y_;
  } while(y_<y_end);
  *m_port_cs|=m_mask_cs;
}
//----

template<class IPShader>
void graphics_device_ili9488::fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader &ips_)
{
  // draw rectangle
  uint16_t x_end=x_+width_, y_end=y_+height_;
  *m_port_cs&=~m_mask_cs;
  set_data_window(x_, y_, x_end-1, y_end-1);
  start_data();
  fb_format_t res;
  do
  {
    uint16_t x=x_;
    do
    {
      ips_.exec(res, x, y_);
      write_data(res.v>>8);
      write_data(res.v);
      ++x;
    } while(x<x_end);
    ++y_;
  } while(y_<y_end);
  *m_port_cs|=m_mask_cs;
}
//----------------------------------------------------------------------------

void graphics_device_ili9488::set_data_pins(uint8_t v_)
{
#ifdef EGL_PLATFORM_TEENSY4X
  // Teensy 4.x implementation for setting the data to pins [19, 18, 14, 15, 22, 23, 20, 21]
  *m_port_data=(*m_port_data&~0x0f0f0000)|(uint32_t(v_&0xf)<<16)|(uint32_t(v_&0xf0)<<20);
#elif defined(EGL_PLATFORM_TEENSY35) || defined(EGL_PLATFORM_TEENSY36)
  // Teensy 3.5 & 3.6 implementation for setting the data to pins [15, 22, 23, 13, 35, 36, 37, 38]
  uint32_t reg=(v_&0x07)|((v_<<2)&0x20)|((v_<<4)&0xf00);
  GPIOC_PSOR=reg;
  GPIOC_PCOR=(~reg)&0xf27;
#endif
}
//----

void graphics_device_ili9488::write_cmd(uint8_t cmd_)
{
  *m_port_rs&=~m_mask_rs;
  set_data_pins(cmd_);
  *m_port_wr&=~m_mask_wr;
  wait_twrl();
  *m_port_wr|=m_mask_wr;
  wait_twrh();
}
//----

void graphics_device_ili9488::start_data()
{
  *m_port_rs|=m_mask_rs;
}
//----

void graphics_device_ili9488::write_data(uint8_t data_)
{
  set_data_pins(data_);
  *m_port_wr&=~m_mask_wr;
  wait_twrl();
  *m_port_wr|=m_mask_wr;
  wait_twrh();
}
//----

void graphics_device_ili9488::wait_twrl()
{
  // wait write control pulse low duration (min 20ns)
//  asm("nop; nop; nop; nop; nop; nop");
//  asm("nop; nop; nop; nop; nop; nop");
}
//----

void graphics_device_ili9488::wait_twrh()
{
  // wait write control pulse high duration (min 30ns)
//  asm("nop; nop; nop; nop; nop; nop");
//  asm("nop; nop; nop; nop; nop; nop");
//  asm("nop; nop; nop; nop; nop; nop");
}
//----

void graphics_device_ili9488::set_data_window(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_)
{
  // setup data window
  write_cmd(0x2a);
  start_data();
  write_data(x0_>>8);
  write_data(x0_);
  write_data(x1_>>8);
  write_data(x1_);
  write_cmd(0x2b);
  start_data();
  write_data(y0_>>8);
  write_data(y0_);
  write_data(y1_>>8);
  write_data(y1_);
  write_cmd(0x2c);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
