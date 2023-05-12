//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_VERTEX_H
#define EGL_VERTEX_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_tform3.h"
EGL_NAMESPACE_BEGIN

// new
// default vertex formats
struct vertex_p;
struct vertex_pn;
struct vertex_pq;
struct vertex_pnu;
struct vertex_pqu;
struct vertex_p48;
struct vertex_p48n32;
struct vertex_p48tbn32;
struct vertex_p48n32uv32;
struct vertex_p48tbn32uv32;
//----------------------------------------------------------------------------


//============================================================================
// 
//============================================================================
template<class Vtx, class TState> EGL_INLINE vec3f get_pos(const Vtx&, const TState&)    {return vec3f(0.0f, 0.0f, 0.0f);}
template<class Vtx, class TState> EGL_INLINE vec3f get_normal(const Vtx&, const TState&) {return vec3f(0.0f, 0.0f, 0.0f);}
template<class Vtx, class TState> EGL_INLINE mat33f get_tbn(const Vtx&, const TState&)   {return mat33f(0.0f, 0.0f, 0.0f);}
template<class Vtx, class TState> EGL_INLINE vec2f get_uv(const Vtx&, const TState&)     {return vec2f(0.0f, 0.0f);}
//----------------------------------------------------------------------------


//============================================================================
// default vertex input formats
//============================================================================
// Default vertex formats matching the default formats defined in vfmt.xml.
// Note that you can add custom vertex formats for your projects and the below
// formats are provided just for convenience. By implementing the relevant
// accessor functions the custom vertex formats will be compatible with
// different vertex shaders.
//----------------------------------------------------------------------------

//============================================================================
// vertex_p
//============================================================================
struct vertex_p
{
  enum {id=1};
  struct transform_state {template<class PSC> EGL_INLINE transform_state(const PSC&) {}};
  //--------------------------------------------------------------------------

  vec3f pos;
};
EGL_STATIC_ASSERT(sizeof(vertex_p)==12);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_p &v_, const vertex_p::transform_state&)
{
  return v_.pos;
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_pn
//============================================================================
struct vertex_pn
{
  enum {id=2};
  struct transform_state {template<class PSC> EGL_INLINE transform_state(const PSC&) {}};
  //--------------------------------------------------------------------------

  vec3f pos;
  vec3f normal;
};
EGL_STATIC_ASSERT(sizeof(vertex_pn)==24);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_pn &v_, const vertex_pn::transform_state&)
{
  return v_.pos;
}
//----

EGL_INLINE vec3f get_normal(const vertex_pn &v_, const vertex_pn::transform_state&)
{
  return v_.normal;
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_pq
//============================================================================
struct vertex_pq
{
  enum {id=3};
  struct transform_state {template<class PSC> EGL_INLINE transform_state(const PSC&) {}};
  //--------------------------------------------------------------------------

  vec3f pos;
  vec3f qrot;
};
EGL_STATIC_ASSERT(sizeof(vertex_pq)==24);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_pq &v_, const vertex_pq::transform_state&)
{
  return v_.pos;
}
//----

EGL_INLINE vec3f get_normal(const vertex_pq &v_, const vertex_pq::transform_state&)
{
  float qx=v_.qrot.x>1.0f?v_.qrot.x-2.0f:v_.qrot.x;
  float qw=sqrt(1.0f-qx*qx+v_.qrot.y*v_.qrot.y+v_.qrot.z*v_.qrot.z);
  return vec3f(2.0f*(v_.qrot.y*qw+v_.qrot.z*qx),
                2.0f*(v_.qrot.z*v_.qrot.y-qx*qw),
                2.0f*(-qx*qx-v_.qrot.y*v_.qrot.y)+1.0f);
}
//----

EGL_INLINE mat33f get_tbn(const vertex_pq &v_, const vertex_pq::transform_state&)
{
  float qx=v_.qrot.x>1.0f?v_.qrot.x-2.0f:v_.qrot.x;
  float qw=sqrt(1.0f-qx*qx+v_.qrot.y*v_.qrot.y+v_.qrot.z*v_.qrot.z);
  mat33f rm;
  convert(rm, quatf(qx, v_.qrot.y, v_.qrot.z, qw));
  if(v_.qrot.x>1.0f)
    rm.y=-rm.y;
  return rm;
}
//--------------------------------------------------------------------------


//============================================================================
// vertex_pnu
//============================================================================
struct vertex_pnu
{
  enum {id=4};
  struct transform_state {template<class PSC> EGL_INLINE transform_state(const PSC&) {}};
  //--------------------------------------------------------------------------

  vec3f pos;
  vec3f normal;
  vec2f uv;
};
EGL_STATIC_ASSERT(sizeof(vertex_pnu)==32);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_pnu &v_, const vertex_pnu::transform_state&)
{
  return v_.pos;
}
//----

EGL_INLINE vec3f get_normal(const vertex_pnu &v_, const vertex_pnu::transform_state&)
{
  return v_.normal;
}
//----

EGL_INLINE vec2f get_uv(const vertex_pnu &v_, const vertex_pnu::transform_state&)
{
  return v_.uv;
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_pqu
//============================================================================
struct vertex_pqu
{
  enum {id=5};
  struct transform_state {template<class PSC> EGL_INLINE transform_state(const PSC&) {}};
  //--------------------------------------------------------------------------

  vec3f pos;
  vec3f qrot;
  vec2f uv;
};
EGL_STATIC_ASSERT(sizeof(vertex_pqu)==32);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_pqu &v_, const vertex_pqu::transform_state&)
{
  return v_.pos;
}
//----

EGL_INLINE vec3f get_normal(const vertex_pqu &v_, const vertex_pqu::transform_state&)
{
  float qx=v_.qrot.x>1.0f?v_.qrot.x-2.0f:v_.qrot.x;
  float qw=sqrt(1.0f-qx*qx+v_.qrot.y*v_.qrot.y+v_.qrot.z*v_.qrot.z);
  return vec3f(2.0f*(v_.qrot.y*qw+v_.qrot.z*qx),
                2.0f*(v_.qrot.z*v_.qrot.y-qx*qw),
                2.0f*(-qx*qx-v_.qrot.y*v_.qrot.y)+1.0f);
}
//----

EGL_INLINE mat33f get_tbn(const vertex_pqu &v_, const vertex_pqu::transform_state&)
{
  float qx=v_.qrot.x>1.0f?v_.qrot.x-2.0f:v_.qrot.x;
  float qw=sqrt(1.0f-qx*qx+v_.qrot.y*v_.qrot.y+v_.qrot.z*v_.qrot.z);
  mat33f rm;
  convert(rm, quatf(qx, v_.qrot.y, v_.qrot.z, qw));
  if(v_.qrot.x>1.0f)
    rm.y=-rm.y;
  return rm;
}
//----

EGL_INLINE vec2f get_uv(const vertex_pqu &v_, const vertex_pqu::transform_state&)
{
  return v_.uv;
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_p48
//============================================================================
struct vertex_p48
{
  enum {id=6};
  struct transform_state
  {
    template<class PSC> EGL_INLINE transform_state(const PSC &pso_)
    {
      pos_offset=pso_.mesh_bvol_pos();
      pos_scale=pso_.mesh_bvol_rad()/32767.0f;
    }
    //------------------------------------------------------------------------

    vec3f pos_offset;
    float pos_scale;
  };
  //--------------------------------------------------------------------------

  vec3i16 pos;
};
EGL_STATIC_ASSERT(sizeof(vertex_p48)==6);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_p48 &v_, const vertex_p48::transform_state &tstate_)
{
  return madd(to_vec3<float>(v_.pos), tstate_.pos_scale, tstate_.pos_offset);
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_p48n32
//============================================================================
struct vertex_p48n32
{
  enum {id=7};
  struct transform_state
  {
    template<class PSC> EGL_INLINE transform_state(const PSC &pso_)
    {
      pos_offset=pso_.mesh_bvol_pos();
      pos_scale=pso_.mesh_bvol_rad()/32767.0f;
    }
    //------------------------------------------------------------------------

    vec3f pos_offset;
    float pos_scale;
  };
  //--------------------------------------------------------------------------

  vec3i16 pos;
  int8_t normal[3], dummy;
};
EGL_STATIC_ASSERT(sizeof(vertex_p48n32)==10);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_p48n32 &v_, const vertex_p48n32::transform_state &tstate_)
{
  return madd(to_vec3<float>(v_.pos), tstate_.pos_scale, tstate_.pos_offset);
}
//----

EGL_INLINE vec3f get_normal(const vertex_p48n32 &v_, const vertex_p48n32::transform_state&)
{
  vec3f n;
  unpack_s1(n, vec3_32s(v_.normal[0], v_.normal[1], v_.normal[2]));
  return n;
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_p48tbn32
//============================================================================
struct vertex_p48tbn32
{
  enum {id=8};
  struct transform_state
  {
    template<class PSC> EGL_INLINE transform_state(const PSC &pso_)
    {
      pos_offset=pso_.mesh_bvol_pos();
      pos_scale=pso_.mesh_bvol_rad()/32767.0f;
    }
    //------------------------------------------------------------------------

    vec3f pos_offset;
    float pos_scale;
  };
  //--------------------------------------------------------------------------

  vec3i16 pos;
  vec2u16 rot;
};
EGL_STATIC_ASSERT(sizeof(vertex_p48tbn32)==10);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_p48tbn32 &v_, const vertex_p48tbn32::transform_state &tstate_)
{
  return madd(to_vec3<float>(v_.pos), tstate_.pos_scale, tstate_.pos_offset);
}
//----

EGL_INLINE vec3f get_normal(const vertex_p48tbn32 &v_, const vertex_p48tbn32::transform_state&)
{
  uint32_t qr32=uint32_t(v_.rot.x)|(uint32_t(v_.rot.y<<16));
  return dequantize_mat33z_32<float>(qr32);
}
//----

EGL_INLINE mat33f get_tbn(const vertex_p48tbn32 &v_, const vertex_p48tbn32::transform_state&)
{
  uint32_t qr32=uint32_t(v_.rot.x)|(uint32_t(v_.rot.y)<<16);
  return dequantize_mat33_32<float>(qr32);
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_p48n32uv32
//============================================================================
struct vertex_p48n32uv32
{
  enum {id=9};
  struct transform_state
  {
    template<class PSC> EGL_INLINE transform_state(const PSC &pso_)
    {
      pos_offset=pso_.mesh_bvol_pos();
      pos_scale=pso_.mesh_bvol_rad()/32767.0f;
    }
    //------------------------------------------------------------------------

    vec3f pos_offset;
    float pos_scale;
  };
  //--------------------------------------------------------------------------

  vec3i16 pos;
  int8_t normal[3], dummy;
  vec2i16 uv;
};
EGL_STATIC_ASSERT(sizeof(vertex_p48n32uv32)==14);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_p48n32uv32 &v_, const vertex_p48n32uv32::transform_state &tstate_)
{
  return madd(to_vec3<float>(v_.pos), tstate_.pos_scale, tstate_.pos_offset);
}
//----

EGL_INLINE vec3f get_normal(const vertex_p48n32uv32 &v_, const vertex_p48n32uv32::transform_state&)
{
  vec3f n;
  unpack_s1(n, vec3_32s(v_.normal[0], v_.normal[1], v_.normal[2]));
  return n;
}
//----

EGL_INLINE vec2f get_uv(const vertex_p48n32uv32 &v_, const vertex_p48n32uv32::transform_state&)
{
  return to_vec2<float>(v_.uv)/2048.0f;
}
//----------------------------------------------------------------------------


//============================================================================
// vertex_p48tbn32uv32
//============================================================================
struct vertex_p48tbn32uv32
{
  enum {id=10};
  struct transform_state
  {
    template<class PSC> EGL_INLINE transform_state(const PSC &pso_)
    {
      pos_offset=pso_.mesh_bvol_pos();
      pos_scale=pso_.mesh_bvol_rad()/32767.0f;
    }
    //------------------------------------------------------------------------

    vec3f pos_offset;
    float pos_scale;
  };
  //--------------------------------------------------------------------------

  vec3i16 pos;
  vec2u16 rot;
  vec2i16 uv;
};
EGL_STATIC_ASSERT(sizeof(vertex_p48tbn32uv32)==14);
//----------------------------------------------------------------------------

// accessors
EGL_INLINE vec3f get_pos(const vertex_p48tbn32uv32 &v_, const vertex_p48tbn32uv32::transform_state &tstate_)
{
  return madd(to_vec3<float>(v_.pos), tstate_.pos_scale, tstate_.pos_offset);
}
//----

EGL_INLINE vec3f get_normal(const vertex_p48tbn32uv32 &v_, const vertex_p48tbn32uv32::transform_state&)
{
  uint32_t qr32=uint32_t(v_.rot.x)|(uint32_t(v_.rot.y)<<16);
  return dequantize_mat33z_32<float>(qr32);
}
//----

EGL_INLINE mat33f get_tbn(const vertex_p48tbn32uv32 &v_, const vertex_p48tbn32uv32::transform_state&)
{
  uint32_t qr32=uint32_t(v_.rot.x)|(uint32_t(v_.rot.y)<<16);
  return dequantize_mat33_32<float>(qr32);
}
//----

EGL_INLINE vec2f get_uv(const vertex_p48tbn32uv32 &v_, const vertex_p48tbn32uv32::transform_state&)
{
  return to_vec2<float>(v_.uv)/2048.0f;
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
