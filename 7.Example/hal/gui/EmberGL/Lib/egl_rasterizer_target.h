//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_RASTERIZER_TARGET_H
#define EGL_RASTERIZER_TARGET_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_pixel.h"
EGL_NAMESPACE_BEGIN

// new
struct rasterizer_depth_target;
template<class PSC> class rasterizer_pixel_out;
//----------------------------------------------------------------------------


//============================================================================
// e_depth_format
//============================================================================
enum e_depth_format
{
  depthfmt_none,    // no depth (test always passes, writes do nothing)
  depthfmt_uint8,   // 8-bit depth in range [0, 255]
  depthfmt_uint16,  // 16-bit depth in range [0, 65535]
  depthfmt_float32  // float32 depth in range [0, 1]
};
//----

template<e_depth_format> struct depth_format_traits     {typedef void depth_format_t;      enum {type_size=0};};
template<> struct depth_format_traits<depthfmt_uint8>   {typedef uint8_t depth_format_t;   enum {type_size=1};};
template<> struct depth_format_traits<depthfmt_uint16>  {typedef uint16_t depth_format_t;  enum {type_size=2};};
template<> struct depth_format_traits<depthfmt_float32> {typedef float32_t depth_format_t; enum {type_size=4};};
EGL_INLINE uint8_t depth_format_pixel_size(e_depth_format);
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_depth_target
//============================================================================
struct rasterizer_depth_target
{
  void *data;
  uint16_t *hiz_data;
  e_depth_format format;
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_pixel_out
//============================================================================
template<class PSC>
class rasterizer_pixel_out
{
public:
  template<e_pixel_format pixfmt> EGL_INLINE void export_rt0(const pixel<pixfmt>&);
  EGL_INLINE void export_rt0(const color_rgbaf&);
  template<e_pixel_format pixfmt> EGL_INLINE void export_rt1(const pixel<pixfmt>&);
  EGL_INLINE void export_rt1(const color_rgbaf&);
  template<e_pixel_format pixfmt> EGL_INLINE void export_rt2(const pixel<pixfmt>&);
  EGL_INLINE void export_rt2(const color_rgbaf&);
  template<e_pixel_format pixfmt> EGL_INLINE void export_rt3(const pixel<pixfmt>&);
  EGL_INLINE void export_rt3(const color_rgbaf&);
  //--------------------------------------------------------------------------

private:
  EGL_STATIC_ASSERT_MSG(rasterizer_max_rts==4, pixel_output_supports_4_rts);
  typedef pixel<(e_pixel_format)PSC::rt0_fmt> rt0_fmt_t;
  typedef pixel<(e_pixel_format)PSC::rt1_fmt> rt1_fmt_t;
  typedef pixel<(e_pixel_format)PSC::rt2_fmt> rt2_fmt_t;
  typedef pixel<(e_pixel_format)PSC::rt3_fmt> rt3_fmt_t;
  friend class rasterizer;
  void *m_rts[rasterizer_max_rts];
  uint32_t m_px_offs;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// depth_format_pixel_size
//============================================================================
EGL_INLINE uint8_t depth_format_pixel_size(e_depth_format fmt_)
{
  switch(fmt_)
  {
    case depthfmt_none:    return 0;
    case depthfmt_uint8:   return 1;
    case depthfmt_uint16:  return 2;
    case depthfmt_float32: return 4;
  }
  EGL_ERROR_NOT_IMPL();
  return 0;
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_pixel_out
//============================================================================
template<class PSC>
template<e_pixel_format src_fmt>
void rasterizer_pixel_out<PSC>::export_rt0(const pixel<src_fmt> &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[0]);
  rt0_fmt_t &rt=((rt0_fmt_t*)m_rts[0])[m_px_offs];
  PSC::blend_rt0(rt, rt, pix_);
}
//----

template<class PSC>
void rasterizer_pixel_out<PSC>::export_rt0(const color_rgbaf &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[0]);
  rt0_fmt_t &rt=((rt0_fmt_t*)m_rts[0])[m_px_offs];
  PSC::blend_rt0(rt, rt, pixel<pixfmt_r32g32b32a32f>(pix_));
}
//----

template<class PSC>
template<e_pixel_format src_fmt>
void rasterizer_pixel_out<PSC>::export_rt1(const pixel<src_fmt> &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[1]);
  rt1_fmt_t &rt=((rt1_fmt_t*)m_rts[1])[m_px_offs];
  PSC::blend_rt1(rt, rt, pix_);
}
//----

template<class PSC>
void rasterizer_pixel_out<PSC>::export_rt1(const color_rgbaf &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[1]);
  rt1_fmt_t &rt=((rt1_fmt_t*)m_rts[1])[m_px_offs];
  PSC::blend_rt1(rt, rt, pixel<pixfmt_r32g32b32a32f>(pix_));
}
//----

template<class PSC>
template<e_pixel_format src_fmt>
void rasterizer_pixel_out<PSC>::export_rt2(const pixel<src_fmt> &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[2]);
  rt2_fmt_t &rt=((rt2_fmt_t*)m_rts[2])[m_px_offs];
  PSC::blend_rt2(rt, rt, pix_);
}
//----

template<class PSC>
void rasterizer_pixel_out<PSC>::export_rt2(const color_rgbaf &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[2]);
  rt2_fmt_t &rt=((rt2_fmt_t*)m_rts[2])[m_px_offs];
  PSC::blend_rt2(rt, rt, pixel<pixfmt_r32g32b32a32f>(pix_));
}
//----

template<class PSC>
template<e_pixel_format src_fmt>
void rasterizer_pixel_out<PSC>::export_rt3(const pixel<src_fmt> &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[3]);
  rt3_fmt_t &rt=((rt3_fmt_t*)m_rts[3])[m_px_offs];
  PSC::blend_rt3(rt, rt, pix_);
}
//----

template<class PSC>
void rasterizer_pixel_out<PSC>::export_rt3(const color_rgbaf &pix_)
{
  EGL_ASSERT_PEDANTIC(m_rts[3]);
  rt3_fmt_t &rt=((rt3_fmt_t*)m_rts[3])[m_px_offs];
  PSC::blend_rt3(rt, rt, pixel<pixfmt_r32g32b32a32f>(pix_));
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
