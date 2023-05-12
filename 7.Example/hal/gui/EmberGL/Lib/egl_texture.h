//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_TEXTURE_H
#define EGL_TEXTURE_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_pixel.h"
EGL_NAMESPACE_BEGIN

// new
enum {ptx_file_ver=0x1100};
class texture;
//----------------------------------------------------------------------------


//============================================================================
// e_tex_type
//============================================================================
enum e_tex_type
{
  textype_none    =0x00,
  textype_2d      =0x01,
  textype_3d      =0x02,
  textype_cube    =0x03,
  textype_array2d =0x04
};
//----------------------------------------------------------------------------


//============================================================================
// e_tex_layout
//============================================================================
enum e_tex_layout
{
  texlayout_linear =0x00,
};
//----------------------------------------------------------------------------


//============================================================================
// e_tex_filter
//============================================================================
enum e_tex_filter
{
  texfilter_point   =0x00,
  texfilter_linear  =0x01
};
//----------------------------------------------------------------------------


//============================================================================
// e_tex_address
//============================================================================
enum e_tex_address
{
  texaddr_wrap  =0x00,
  texaddr_clamp =0x01,
};
//----------------------------------------------------------------------------


//============================================================================
// texture
//============================================================================
class texture
{
public:
  // construction
  texture();
  explicit texture(const void *ptx_file_);
  void init(const void *ptx_file_);
  EGL_INLINE bool is_valid() const;
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE e_tex_type type() const;
  EGL_INLINE e_pixel_format format() const;
  EGL_INLINE e_tex_layout layout() const;
  EGL_INLINE uint8_t num_lods() const;
  EGL_INLINE uint16_t width() const;
  EGL_INLINE uint16_t height() const;
  EGL_INLINE uint16_t depth() const;
  EGL_INLINE const void *data() const;
  //--------------------------------------------------------------------------

private:
  texture(const texture&); // not implemented
  void operator=(const texture&); // not implemented
  //--------------------------------------------------------------------------

  //==========================================================================
  // ptx_header
  //==========================================================================
  struct ptx_header
  {
    char file_sig[4];
    uint16_t file_ver;
    uint16_t flags;
    uint32_t fsize;
    uint8_t type;
    uint8_t format;
    uint8_t layout;
    uint8_t num_lods;
    uint16_t width;
    uint16_t height;
    uint16_t depth;
    uint16_t reserved[5];
    uint8_t data[1];
  };
  //--------------------------------------------------------------------------

  const ptx_header *m_header;
};
//----------------------------------------------------------------------------


//============================================================================
// sampler2d
//============================================================================
template<e_pixel_format src_fmt, e_tex_filter filter=texfilter_linear, e_tex_address addr=texaddr_wrap>
class sampler2d
{
public:
  // construction
  sampler2d();
  //--------------------------------------------------------------------------

  // sampling
  template<e_pixel_format dst_fmt> void sample(pixel<dst_fmt>&, const texture&, const vec2f &uv_) const;
  void sample(color_rgbaf&, const texture&, const vec2f &uv_) const;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// texture
//============================================================================
bool texture::is_valid() const
{
  return m_header!=0;
}
//----------------------------------------------------------------------------

e_tex_type texture::type() const
{
  return e_tex_type(m_header->type);
}
//----

e_pixel_format texture::format() const
{
  return e_pixel_format(m_header->format);
}
//----

e_tex_layout texture::layout() const
{
  return e_tex_layout(m_header->layout);
}
//----

uint8_t texture::num_lods() const
{
  return m_header->num_lods;
}
//----

uint16_t texture::width() const
{
  return m_header->width;
}
//----

uint16_t texture::height() const
{
  return m_header->height;
}
//----

uint16_t texture::depth() const
{
  return m_header->depth;
}
//----

const void *texture::data() const
{
  return m_header->data;
}
//----------------------------------------------------------------------------


//============================================================================
// sampler2d
//============================================================================
template<typename T, unsigned num_comp> struct pixel_format_blend;
//----

template<typename T>
struct pixel_format_blend<T, 1>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    typedef typename pixel<src_fmt>::component_t src_comp_t;
    uint8_t wu1=uint8_t(fu_*128.0f), wu0=128-wu1;
    uint8_t wv1=uint8_t(fv_*128.0f), wv0=128-wv1;
    uint16_t w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    src_comp_t r=src_comp_t((w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::rsize));
    res_.set_rgba(r, 0, 0, 0);
  }
};
//----

template<>
struct pixel_format_blend<float32_t, 1>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    float wu1=fu_, wu0=1.0f-wu1;
    float wv1=fv_, wv0=1.0f-wv1;
    float w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    float32_t r=w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r;
    res_.set_rgba(r, 0, 0, 0);
  }
};
//----

template<typename T>
struct pixel_format_blend<T, 2>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    typedef typename pixel<src_fmt>::component_t src_comp_t;
    uint8_t wu1=uint8_t(fu_*128.0f), wu0=128-wu1;
    uint8_t wv1=uint8_t(fv_*128.0f), wv0=128-wv1;
    uint16_t w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    src_comp_t r=src_comp_t((w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::rsize));
    src_comp_t g=src_comp_t((w00*p00_.c.g+w01*p01_.c.g+w10*p10_.c.g+w11*p11_.c.g)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::gsize));
    res_.set_rgba(r, g, 0, 0);
  }
};
//----

template<>
struct pixel_format_blend<float32_t, 2>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    float wu1=fu_, wu0=1.0f-wu1;
    float wv1=fv_, wv0=1.0f-wv1;
    float w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    float32_t r=w00*p00_.c.r+w01*p01_.c.r+w10*p10_.r+w11*p11_.c.r;
    float32_t g=w00*p00_.c.g+w01*p01_.c.g+w10*p10_.g+w11*p11_.c.g;
    res_.set_rgba(r, g, 0, 0);
  }
};
//----

template<typename T>
struct pixel_format_blend<T, 3>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    typedef typename pixel<src_fmt>::component_t src_comp_t;
    uint8_t wu1=uint8_t(fu_*128.0f), wu0=128-wu1;
    uint8_t wv1=uint8_t(fv_*128.0f), wv0=128-wv1;
    uint16_t w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    src_comp_t r=src_comp_t((w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::rsize));
    src_comp_t g=src_comp_t((w00*p00_.c.g+w01*p01_.c.g+w10*p10_.c.g+w11*p11_.c.g)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::gsize));
    src_comp_t b=src_comp_t((w00*p00_.c.b+w01*p01_.c.b+w10*p10_.c.b+w11*p11_.c.b)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::bsize));
    res_.set_rgba(r, g, b, 0);
  }
};
//----

template<>
struct pixel_format_blend<float32_t, 3>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    float wu1=fu_, wu0=1.0f-wu1;
    float wv1=fv_, wv0=1.0f-wv1;
    float w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    float32_t r=w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r;
    float32_t g=w00*p00_.c.g+w01*p01_.c.g+w10*p10_.c.g+w11*p11_.c.g;
    float32_t b=w00*p00_.c.b+w01*p01_.c.b+w10*p10_.c.b+w11*p11_.c.b;
    res_.set_rgba(r, g, b, 0);
  }
};
//----

template<typename T>
struct pixel_format_blend<T, 4>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    typedef typename pixel<src_fmt>::component_t src_comp_t;
    uint8_t wu1=uint8_t(fu_*128.0f), wu0=128-wu1;
    uint8_t wv1=uint8_t(fv_*128.0f), wv0=128-wv1;
    uint16_t w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    src_comp_t r=src_comp_t((w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::rsize));
    src_comp_t g=src_comp_t((w00*p00_.c.g+w01*p01_.c.g+w10*p10_.c.g+w11*p11_.c.g)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::gsize));
    src_comp_t b=src_comp_t((w00*p00_.c.b+w01*p01_.c.b+w10*p10_.c.b+w11*p11_.c.b)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::bsize));
    src_comp_t a=src_comp_t((w00*p00_.c.a+w01*p01_.c.a+w10*p10_.c.a+w11*p11_.c.a)>>(7+7-sizeof(src_comp_t)*8+pixel<src_fmt>::asize));
    res_.set_rgba(r, g, b, a);
  }
};
//----

template<>
struct pixel_format_blend<float32_t, 4>
{
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  static EGL_INLINE void bilinear_blend(pixel<dst_fmt> &res_, const pixel<src_fmt> &p00_, const pixel<src_fmt> &p01_, const pixel<src_fmt> &p10_, const pixel<src_fmt> &p11_, ufloat1_t fu_, ufloat1_t fv_)
  {
    float wu1=fu_, wu0=1.0f-wu1;
    float wv1=fv_, wv0=1.0f-wv1;
    float w00=wv0*wu0, w01=wv0*wu1, w10=wv1*wu0, w11=wv1*wu1;
    float32_t r=w00*p00_.c.r+w01*p01_.c.r+w10*p10_.c.r+w11*p11_.c.r;
    float32_t g=w00*p00_.c.g+w01*p01_.c.g+w10*p10_.c.g+w11*p11_.c.g;
    float32_t b=w00*p00_.c.b+w01*p01_.c.b+w10*p10_.c.b+w11*p11_.c.b;
    float32_t a=w00*p00_.c.a+w01*p01_.c.a+w10*p10_.c.a+w11*p11_.c.a;
    res_.set_rgba(r, g, b, a);
  }
};
//----------------------------------------------------------------------------

template<e_pixel_format src_fmt, e_tex_filter filter, e_tex_address addr>
sampler2d<src_fmt, filter, addr>::sampler2d()
{
}
//----------------------------------------------------------------------------

template<e_pixel_format src_fmt, e_tex_filter filter, e_tex_address addr>
template<e_pixel_format dst_fmt>
void sampler2d<src_fmt, filter, addr>::sample(pixel<dst_fmt> &px_, const texture &tex_, const vec2f &uv_) const
{
  EGL_ASSERT_PEDANTIC(tex_.format()==src_fmt);
  typedef pixel<dst_fmt> dst_fmt_t;
  typedef pixel<src_fmt> src_fmt_t;
  uint16_t twidth=tex_.width(), theight=tex_.height();
  float u=uv_.x, v=uv_.y;
  switch(filter)
  {
    // point sampling
    case texfilter_point:
    {
      uint16_t u0, v0;
      switch(addr)
      {
        case texaddr_wrap:
        {
          u0=uint16_t((u-::floor(u))*twidth);
          v0=uint16_t((v-::floor(v))*theight);
        } break;

        case texaddr_clamp:
        {
          u0=uint16_t(clamp(u*twidth, 0.0f, twidth-0.001f));
          v0=uint16_t(clamp(v*theight, 0.0f, theight-0.001f));
        } break;

        default: u0=v0=0; EGL_ERROR_NOT_IMPL();
      }
      px_=((const src_fmt_t*)tex_.data())[u0+v0*twidth];
    } break;

    // bilinear sampling
    case texfilter_linear:
    {
      uint16_t u0, u1, v0, v1;
      switch(addr)
      {
        case texaddr_wrap:
        {
          u=(u-::floor(u))*twidth-0.5f;
          if(u<0.0f)
            u+=twidth-0.001f; // small epsilon to avoid float precision issues with tiny negative floats
          EGL_ASSERT_PEDANTIC(u<twidth);
          v=(v-::floor(v))*theight-0.5f;
          if(v<0.0f)
            v+=theight-0.001f; // small epsilon to avoid float precision issues with tiny negative floats
          EGL_ASSERT_PEDANTIC(v<theight);
          u0=uint16_t(u);
          v0=uint16_t(v);
          u1=u0+1;
          u1=u1==twidth?0:u1;
          v1=v0+1;
          v1=v1==theight?0:v1;
        } break;

        case texaddr_clamp:
        {
          u=clamp(u*twidth-0.5f, 0.0f, twidth-0.001f);
          v=clamp(v*theight-0.5f, 0.0f, theight-0.001f);
          u0=uint16_t(u);
          v0=uint16_t(v);
          u1=u0+1;
          u1=u1==twidth?twidth-1:u1;
          v1=v0+1;
          v1=v1==theight?theight-1:v1;
        } break;

        default: u0=v0=u1=v1=0; EGL_ERROR_NOT_IMPL();
      }
      const src_fmt_t *data=(const src_fmt_t*)tex_.data();
      const src_fmt_t *dv0=data+v0*twidth, *dv1=data+v1*twidth;
      enum {num_comp= (src_fmt_t::rsize && dst_fmt_t::rsize?1:0)
                     +(src_fmt_t::gsize && dst_fmt_t::gsize?1:0)
                     +(src_fmt_t::bsize && dst_fmt_t::bsize?1:0)
                     +(src_fmt_t::asize && dst_fmt_t::asize?1:0)};
      pixel_format_blend<typename src_fmt_t::component_t, num_comp>::bilinear_blend(px_, dv0[u0], dv0[u1], dv1[u0], dv1[u1], frc(u), frc(v));
    } break;
  }
}
//----

template<e_pixel_format src_fmt, e_tex_filter filter, e_tex_address addr>
void sampler2d<src_fmt, filter, addr>::sample(color_rgbaf &c_, const texture &tex_, const vec2f &uv_) const
{
  pixel<pixfmt_r32g32b32a32f> px;
  sample(px, tex_, uv_);
  c_.set(px.c.r, px.c.g, px.c.b, px.c.a);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
