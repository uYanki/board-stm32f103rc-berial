//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_FAST_MATH_H
#define EGL_FAST_MATH_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_core.h"
EGL_NAMESPACE_BEGIN

// new
EGL_INLINE float32_t fast_rsqrt(float32_t);
EGL_INLINE float64_t fast_rsqrt(float64_t);
EGL_INLINE float32_t fast_sqrt(float32_t);
EGL_INLINE float64_t fast_sqrt(float64_t);
EGL_INLINE float32_t fast_rcp(float32_t);
EGL_INLINE float64_t fast_rcp(float64_t);
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// fast_rsqrt
//============================================================================
EGL_INLINE float32_t fast_rsqrt(float32_t v_)
{
  // fast reciprocal square root: https://en.wikipedia.org/wiki/Fast_inverse_square_root
  union
  {
    float32_t f;
    uint32_t u;
  } v;
  v.f=v_;
  v.u=0x5f375a86-(v.u>>1); // slightly more precise than the original 0x5f3759df
  return v.f;
}
//----

EGL_INLINE float64_t fast_rsqrt(float64_t v_)
{
  // fast reciprocal square root: https://en.wikipedia.org/wiki/Fast_inverse_square_root
  union
  {
    float64_t f;
    uint64_t u;
  } v;
  v.f=v_;
  v.u=0x5fe6eb50c7b537a9-(v.u>>1);
  return v.f;
}
//----------------------------------------------------------------------------


//============================================================================
// fast_sqrt
//============================================================================
EGL_INLINE float32_t fast_sqrt(float32_t v_)
{
  // fast square root using fast reciprocal square root: https://en.wikipedia.org/wiki/Fast_inverse_square_root
  union
  {
    float32_t f;
    uint32_t u;
  } v;
  v.f=v_;
  v.u=0x5f375a86-(v.u>>1); // slightly more precise than the original 0x5f3759df
  return v_*v.f;
}
//----

EGL_INLINE float64_t fast_sqrt(float64_t v_)
{
  // fast square root using fast reciprocal square root: https://en.wikipedia.org/wiki/Fast_inverse_square_root
  union
  {
    float64_t f;
    uint64_t u;
  } v;
  v.f=v_;
  v.u=0x5fe6eb50c7b537a9-(v.u>>1);
  return v_*v.f;
}
//----------------------------------------------------------------------------


//============================================================================
// fast_rcp
//============================================================================
EGL_INLINE float32_t fast_rcp(float32_t v_)
{
  // fast reciprocal using fast reciprocal square root: https://en.wikipedia.org/wiki/Fast_inverse_square_root
  union
  {
    float32_t f;
    uint32_t u;
  } v;
  v.f=v_;
  v.u=0x5f375a85-(v.u>>1); // slightly more precise than the original 0x5f3759df
  return v.f*v.f;
}
//----

EGL_INLINE float64_t fast_rcp(float64_t v_)
{
  // fast reciprocal using fast reciprocal square root: https://en.wikipedia.org/wiki/Fast_inverse_square_root
  union
  {
    float64_t f;
    uint64_t u;
  } v;
  v.f=v_;
  v.u=0x5fe6eb50c7b537a9-(v.u>>1);
  return v.f*v.f;
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
