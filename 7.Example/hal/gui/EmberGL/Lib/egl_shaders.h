//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_SHADERS_H
#define EGL_SHADERS_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_texture.h"
EGL_NAMESPACE_BEGIN

// new
struct ips_color_rgba32;
template<class Sampler, bool swap_uv=false> struct ips_texture;
struct vs_static;
template<e_pixel_format> struct ps_color;
template<class Sampler> struct ps_texture;
//----------------------------------------------------------------------------


//============================================================================
// ips_color_rgba32
//============================================================================
struct ips_color_rgba32
{
	// construction
  EGL_INLINE ips_color_rgba32();
  EGL_INLINE ips_color_rgba32(uint8_t r_, uint8_t g_, uint8_t b_);
  EGL_INLINE ips_color_rgba32(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_);
  EGL_INLINE void set(uint8_t r_, uint8_t g_, uint8_t b_);
  EGL_INLINE void set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_);
  //--------------------------------------------------------------------------

  // shader invocation
	template<e_pixel_format dst_fmt> EGL_INLINE void exec(pixel<dst_fmt> &res_, uint16_t, uint16_t) const;
  //--------------------------------------------------------------------------

  uint8_t m_r, m_g, m_b, m_a;
};
//----------------------------------------------------------------------------


//============================================================================
// ips_texture
//============================================================================
template<class Sampler, bool swap_uv>
struct ips_texture
{
  // construction
  EGL_INLINE ips_texture();
  EGL_INLINE ips_texture(const texture&, int16_t x_, int16_t y_, float xscale_, float yscale_, const color_rgbaf &tint=color_rgbaf::one());
  EGL_INLINE void set(const texture&, int16_t x_, int16_t y_, float xscale_, float yscale_, const color_rgbaf &tint=color_rgbaf::one());
  //--------------------------------------------------------------------------

  // shader invocation
  template<e_pixel_format dst_fmt> EGL_INLINE void exec(pixel<dst_fmt> &res_, uint16_t x_, uint16_t y_) const;
  //--------------------------------------------------------------------------

  const texture *tex;
  int16_t x, y;
  float scale_x, scale_y;
  color_rgbaf tint;
  Sampler sampler;
};
//----------------------------------------------------------------------------


//============================================================================
// vs_static
//============================================================================
struct vs_static
{
  //==========================================================================
  // vsout
  //==========================================================================
  struct vsout
  {
    vec3f opos;
    mat33f tbn;
    vec3f normal;
    vec2f uv;
  };
  //--------------------------------------------------------------------------

  template<class PSIn, class PSOState, class VIn>
  EGL_INLINE void exec(PSIn &psin_, const PSOState &pso_state_, const typename VIn::transform_state &tstate_, const VIn &vin_, uint8_t) const
  {
    vec3f opos=get_pos(vin_, tstate_);
    psin_.pos=vec4f(opos, 1.0f)*pso_state_.obj_to_proj;
    vsout vo;
    vo.tbn=get_tbn(vin_, tstate_);
    vo.normal=get_normal(vin_, tstate_);
    vo.uv=get_uv(vin_, tstate_);
    psin_.set_attribs(vo);
  }
};
//----------------------------------------------------------------------------


//============================================================================
// ps_color
//============================================================================
template<e_pixel_format pix_fmt>
struct ps_color
{
  //==========================================================================
  // psin
  //==========================================================================
  struct psin
  {
    vec4f pos;
    //------------------------------------------------------------------------

    template<class VSOut> EGL_INLINE void set_attribs(const VSOut&) {}
  };
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE ps_color();
  EGL_INLINE ps_color(const color_rgbaf&);
  EGL_INLINE ps_color(const pixel<pix_fmt>&);
  //--------------------------------------------------------------------------

  template<class PSC> EGL_INLINE void exec(rasterizer_pixel_out<PSC>&, const typename PSC::pso_state&, const psin &v0_, const psin &v1_, const psin &v2_, const vec3f &bc_, uint8_t prim_idx_) const;
  //--------------------------------------------------------------------------

  pixel<pix_fmt> color;
};
//----------------------------------------------------------------------------


//============================================================================
// ps_texture
//============================================================================
template<class Sampler>
struct ps_texture
{
  //==========================================================================
  // psin
  //==========================================================================
  struct psin
  {
    vec4f pos;
    vec2f uv;
    //------------------------------------------------------------------------

    template<class VSOut> EGL_INLINE void set_attribs(const VSOut &vo_)
    {
      uv=vo_.uv;
    }
  };
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE ps_texture();
  EGL_INLINE ps_texture(const texture&);
  //--------------------------------------------------------------------------

  template<class PSC> EGL_INLINE void exec(rasterizer_pixel_out<PSC>&, const typename PSC::pso_state&, const psin &v0_, const psin &v1_, const psin &v2_, const vec3f &bc_, uint8_t prim_idx_) const;
  //--------------------------------------------------------------------------

  const texture *tex;
  Sampler sampler;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// ips_color_rgba32
//============================================================================
ips_color_rgba32::ips_color_rgba32()
{
  m_r=m_g=m_b=m_a=0;
}
ips_color_rgba32::ips_color_rgba32(uint8_t r_, uint8_t g_, uint8_t b_)
{
  m_r=r_;
  m_g=g_;
  m_b=b_;
  m_a=255;
}
//----

ips_color_rgba32::ips_color_rgba32(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
{
  m_r=r_;
  m_g=g_;
  m_b=b_;
  m_a=a_;
}
//----

void ips_color_rgba32::set(uint8_t r_, uint8_t g_, uint8_t b_)
{
  m_r=r_;
  m_g=g_;
  m_b=b_;
  m_a=255;
}
//----

void ips_color_rgba32::set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
{
  m_r=r_;
  m_g=g_;
  m_b=b_;
  m_a=a_;
}
//----------------------------------------------------------------------------

template<e_pixel_format dst_fmt>
void ips_color_rgba32::exec(pixel<dst_fmt> &res_, uint16_t, uint16_t) const
{
  res_.set_rgba8(m_r, m_g, m_b, m_a);
}
//----------------------------------------------------------------------------


//============================================================================
// ips_texture
//============================================================================
template<class Sampler, bool swap_uv>
ips_texture<Sampler, swap_uv>::ips_texture()
  :tex(0)
  ,x(0)
  ,y(0)
  ,scale_x(0.0f)
  ,scale_y(0.0f)
  ,tint(color_rgbaf::one())
{
}
//----

template<class Sampler, bool swap_uv>
ips_texture<Sampler, swap_uv>::ips_texture(const texture &tex_, int16_t x_, int16_t y_, float scale_x_, float scale_y_, const color_rgbaf &tint_)
  :tex(&tex_)
  ,x(x_)
  ,y(y_)
  ,scale_x(scale_x_)
  ,scale_y(scale_y_)
  ,tint(tint_)
{
}
//----

template<class Sampler, bool swap_uv>
void ips_texture<Sampler, swap_uv>::set(const texture &tex_, int16_t x_, int16_t y_, float scale_x_, float scale_y_, const color_rgbaf &tint_)
{
  tex=&tex_;
  x=x_;
  y=y_;
  scale_x=scale_x_;
  scale_y=scale_y_;
  tint=tint_;
}
//----------------------------------------------------------------------------

template<class Sampler, bool swap_uv>
template<e_pixel_format dst_fmt>
void ips_texture<Sampler, swap_uv>::exec(pixel<dst_fmt> &res_, uint16_t x_, uint16_t y_) const
{
  float u=((x_-x)+0.5f)*scale_x;
  float v=((y_-y)+0.5f)*scale_y;
  color_rgbaf col;
  sampler.sample(col, *tex, swap_uv?vec2f(v, u):vec2f(u, v));
  res_=col*tint;
}
//----------------------------------------------------------------------------


//============================================================================
// ps_color
//============================================================================
template<e_pixel_format pix_fmt>
ps_color<pix_fmt>::ps_color()
  :color(color_rgbaf::zero())
{
}
//----

template<e_pixel_format pix_fmt>
ps_color<pix_fmt>::ps_color(const color_rgbaf &col_)
  :color(col_)
{
}
//----

template<e_pixel_format pix_fmt>
ps_color<pix_fmt>::ps_color(const pixel<pix_fmt> &col_)
  :color(col_)
{
}
//----------------------------------------------------------------------------

template<e_pixel_format pix_fmt>
template<class PSC>
void ps_color<pix_fmt>::exec(rasterizer_pixel_out<PSC> &psout_, const typename PSC::pso_state&, const psin&, const psin&, const psin&, const vec3f&, uint8_t) const
{
  psout_.export_rt0(color);
}
//----------------------------------------------------------------------------


//============================================================================
// ps_texture
//============================================================================
template<class Sampler>
ps_texture<Sampler>::ps_texture()
  :tex(0)
{
}
//----

template<class Sampler>
ps_texture<Sampler>::ps_texture(const texture &tex_)
  :tex(&tex_)
{
}
//----------------------------------------------------------------------------

template<class Sampler>
template<class PSC>
void ps_texture<Sampler>::exec(rasterizer_pixel_out<PSC> &psout_, const typename PSC::pso_state&, const psin &v0_, const psin &v1_, const psin &v2_, const vec3f &bc_, uint8_t) const
{
  EGL_ASSERT_PEDANTIC(tex);
  pixel<(e_pixel_format)PSC::rt0_fmt> smp;
  vec2f uv=v0_.uv*bc_.x+v1_.uv*bc_.y+v2_.uv*bc_.z;
  sampler.sample(smp, uv);
  psout_.export_rt0(smp);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
