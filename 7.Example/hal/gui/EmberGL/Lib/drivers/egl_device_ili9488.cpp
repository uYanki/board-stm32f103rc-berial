//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifdef ARDUINO
#include "egl_device_ili9488.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------


//============================================================================
// locals
//============================================================================
static const uint8_t s_tft_init_commands[]=
{
   3, 0xf9, 0x00, 0x08,
   3, 0xc0, 0x19, 0x1a,       // VREG1OUT=POSITIVE, VREG2OUT=NEGATIVE
   3, 0xc1, 0x45, 0x00,       // VGH>=14V
   2, 0xc2, 0x33,
   3, 0xc5, 0x00, 0x28,       // VCM_REG[7:0]<=0X80
   3, 0xb1, 0x90, 0x11,       // OSC freq set (0xa0=62Hz, 0Xb0=70Hz)
   2, 0xb4, 0x02,             // 2 dot frame mode, freq<=70Hz
   4, 0xb6, 0x00, 0x42, 0x3b, // 0 GS SS SM ISC[3:0]
   2, 0xb7, 0x07,
  16, 0xe0, 0x1f, 0x25, 0x22,
      0x0b, 0x06, 0x0a, 0x4e,
      0xc6, 0x39, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
  16, 0xe1, 0x1f, 0x3f, 0x3f,
      0x0f, 0x1f, 0x0f, 0x46,
      0x49, 0x31, 0x05, 0x09,
      0x03, 0x1c, 0x1a, 0x00,
   3, 0xb6, 0x00, 0x22,
   2, 0x36, 0x08,
   2, 0x3a, 0x55,
   1, 0x11,
   0
};
//----------------------------------------------------------------------------


//============================================================================
// graphics_device_ili9488
//============================================================================
graphics_device_ili9488 *graphics_device_ili9488::s_active_dev=0;
//----------------------------------------------------------------------------

graphics_device_ili9488::graphics_device_ili9488()
{
  EGL_ASSERT(!s_active_dev);
  s_active_dev=this;
  m_tile_rt0=0;
}
//----

graphics_device_ili9488::graphics_device_ili9488(uint8_t pin_cs_, uint8_t pin_rs_, uint8_t pin_wr_, uint8_t pin_rd_, uint8_t pin_reset_)
{
  EGL_ASSERT(!s_active_dev);
  s_active_dev=this;
  m_tile_rt0=0;
  init(pin_cs_, pin_rs_, pin_wr_, pin_rd_, pin_reset_);
}
//----

graphics_device_ili9488::~graphics_device_ili9488()
{
  s_active_dev=0;
}
//----

void graphics_device_ili9488::init(uint8_t pin_cs_, uint8_t pin_rs_, uint8_t pin_wr_, uint8_t pin_rd_, uint8_t pin_reset_)
{
  // setup control pins
  pinMode(pin_cs_, OUTPUT);
  pinMode(pin_rs_, OUTPUT);
  pinMode(pin_wr_, OUTPUT);
  pinMode(pin_rd_, OUTPUT);
  digitalWrite(pin_cs_, HIGH);
  digitalWrite(pin_rs_, HIGH);
  digitalWrite(pin_wr_, HIGH);
  digitalWrite(pin_rd_, HIGH);
  m_port_cs=portOutputRegister(digitalPinToPort(pin_cs_));
  m_port_rs=portOutputRegister(digitalPinToPort(pin_rs_));
  m_port_wr=portOutputRegister(digitalPinToPort(pin_wr_));
  m_port_rd=portOutputRegister(digitalPinToPort(pin_rd_));
  m_mask_cs=digitalPinToBitMask(pin_cs_);
  m_mask_rs=digitalPinToBitMask(pin_rs_);
  m_mask_wr=digitalPinToBitMask(pin_wr_);
  m_mask_rd=digitalPinToBitMask(pin_rd_);

#ifdef EGL_PLATFORM_TEENSY4X
  // setup data pins
  static const uint8_t s_data_pins[8]={19, 18, 14, 15, 22, 23, 20, 21};
  m_port_data=portOutputRegister(s_data_pins[0]);
  for(unsigned i=0; i<8; ++i)
  {
    pinMode(s_data_pins[i], OUTPUT);
    EGL_ASSERT(portOutputRegister(s_data_pins[i])==m_port_data); // ensure all data pins map to the same address for fast access
  }
#elif defined(EGL_PLATFORM_TEENSY35) || defined(EGL_PLATFORM_TEENSY36)
  static const uint8_t s_data_pins[8]={15, 22, 23, 13, 35, 36, 37, 38};
  for(unsigned i=0; i<8; ++i)
    pinMode(s_data_pins[i], OUTPUT);
#endif

  // reset display
  if(pin_reset_<0xff)
  {
    pinMode(pin_reset_, OUTPUT);
    digitalWrite(pin_reset_, HIGH);
    delay(50);
    digitalWrite(pin_reset_, LOW);
    delay(150);
    digitalWrite(pin_reset_, HIGH);
    delay(150);
  }

  // initialize TFT
  digitalWrite(pin_cs_, HIGH);
  digitalWrite(pin_wr_, HIGH);
  digitalWrite(pin_cs_, LOW);
  const uint8_t *addr=s_tft_init_commands;
  while(1)
  {
    uint8_t count=*addr++;
    if(!count--)
      break;
    write_cmd(*addr++);
    start_data();
    while(count--)
      write_data(*addr++);
  }
  delay(120);
  write_cmd(0x29);

  // clear the screen to black
  *m_port_cs&=~m_mask_cs;
  set_data_window(0, 0, fb_width-1, fb_height-1);
  start_data();
  for(uint32_t i=0; i<fb_width*fb_height; ++i)
  {
    write_data(0);
    write_data(0);
  }
  *m_port_cs|=m_mask_cs;
  EGL_LOG("ILI9488 graphics device initialized!\r\n");
}
//----

void graphics_device_ili9488::init_rasterizer(const rasterizer_cfg &rcfg_, const rasterizer_tiling_cfg &tcfg_, const rasterizer_vertex_cache_cfg &vccfg_)
{
  graphics_device<graphics_device_ili9488>::init(rcfg_, tcfg_, vccfg_);
  m_tile_rt0=(fb_format_t*)rcfg_.rts[0];
  m_tile_width=tcfg_.tile_width;
  m_tile_height=tcfg_.tile_height;
}
//----

void graphics_device_ili9488::init_dma(rasterizer_data_transfer*, uint8_t num_transfers_, fb_format_t *dma_buffer_, size_t dma_buffer_size_)
{
}
//----------------------------------------------------------------------------

void graphics_device_ili9488::submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16 &reg_min_, const vec2u16 &reg_end_, uint16_t thread_idx_)
{
  // access update pos, size and data
  uint16_t x=tx_*m_tile_width+reg_min_.x;
  uint16_t y=ty_*m_tile_height+reg_min_.y;
  uint16_t update_width=reg_end_.x-reg_min_.x;
  uint16_t update_height=reg_end_.y-reg_min_.y;
  fb_format_t *data=m_tile_rt0+reg_min_.x+reg_min_.y*m_tile_width;

  // transfer tile update region to the display
  if(m_tile_shader)
    m_tile_shader->transfer_region(render_targets(), depth_target(), size_t(data-m_tile_rt0), x, y, update_width, update_height, m_tile_width);
  else
  {
    *m_port_cs&=~m_mask_cs;
    set_data_window(x, y, x+update_width-1, y+update_height-1);
    start_data();
    fb_format_t *data_end=data+m_tile_width*update_height;
    do
    {
      fb_format_t *data_scan=data, *data_scan_end=data_scan+update_width;
      do
      {
        uint16_t v=data_scan->v;
        write_data(v>>8);
        write_data(v);
      } while(++data_scan<data_scan_end);
      data+=m_tile_width;
    } while(data<data_end);
    *m_port_cs|=m_mask_cs;
  }
}
//----------------------------------------------------------------------------
#endif // ARDUINO
