//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_PIXEL_H
#define EGL_PIXEL_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_color.h"
EGL_NAMESPACE_BEGIN

// new
//----------------------------------------------------------------------------


//============================================================================
// e_pixel_format
//============================================================================
enum e_pixel_format
{
  pixfmt_none          =0x00,
  // <8bpc formats
  pixfmt_r3g3b2        =0x01,
  pixfmt_r5g6b5        =0x02,
  pixfmt_b5g6r5        =0x03,
  pixfmt_r5g5b5a1      =0x04,
  pixfmt_r4g4b4a4      =0x05,
  // 8bpc formats
  pixfmt_r8            =0x10,
  pixfmt_r8g8          =0x11,
  pixfmt_r8g8b8        =0x12,
  pixfmt_b8g8r8        =0x13,
  pixfmt_r8g8b8a8      =0x14,
  pixfmt_b8g8r8a8      =0x15,
  // 10-11bpc formats
  pixfmt_r11g11b10     =0x20,
  pixfmt_r11g11b10f    =0x21,
  pixfmt_r10g10b10a2   =0x22,
  pixfmt_r10g10b10a2f  =0x23,
  // 16bpc formats
  pixfmt_r16           =0x30,
  pixfmt_r16f          =0x31,
  pixfmt_r16g16        =0x32,
  pixfmt_r16g16f       =0x33,
  pixfmt_r16g16b16     =0x34,
  pixfmt_r16g16b16f    =0x35,
  pixfmt_r16g16b16a16  =0x36,
  pixfmt_r16g16b16a16f =0x37,
  // 32bpc formats
  pixfmt_r32           =0x40,
  pixfmt_r32f          =0x41,
  pixfmt_r32g32        =0x42,
  pixfmt_r32g32f       =0x43,
  pixfmt_r32g32b32     =0x44,
  pixfmt_r32g32b32f    =0x45,
  pixfmt_r32g32b32a32  =0x46,
  pixfmt_r32g32b32a32f =0x47,
};
//----

template<e_pixel_format> struct pixel;
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// pixel
//============================================================================
#define EGL_PIXEL_RGBA_IMPL\
  EGL_INLINE pixel()                                                                    {}\
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), px_.get_a());}\
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, c_.g, c_.b, c_.a);}\
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), px_.get_a());}\
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {set_rgba(c_.r, c_.g, c_.b, c_.a);}\
  EGL_INLINE component_t get_r() const                                                  {return component_t(c.r<<(sizeof(component_t)*8-rsize));}\
  EGL_INLINE component_t get_g() const                                                  {return component_t(c.g<<(sizeof(component_t)*8-gsize));}\
  EGL_INLINE component_t get_b() const                                                  {return component_t(c.b<<(sizeof(component_t)*8-bsize));}\
  EGL_INLINE component_t get_a() const                                                  {return component_t(c.a<<(sizeof(component_t)*8-asize));}\
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r/float(uint32_t(rsize<32?(1<<rsize)-1:-1)), c.g/float(uint32_t(gsize<32?(1<<gsize)-1:-1)), c.b/float(uint32_t(bsize<32?(1<<bsize)-1:-1)), c.a/float(uint32_t(asize<32?(1<<asize)-1:-1)));}\
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)              {c.r=component_t(rsize>8?r_<<(rsize-8):r_>>(8-rsize)); c.g=component_t(gsize>8?g_<<(gsize-8):g_>>(8-gsize)); c.b=component_t(bsize>8?b_<<(bsize-8):b_>>(8-bsize)); c.a=component_t(asize>8?a_<<(asize-8):a_>>(8-asize));}\
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t a_)          {c.r=component_t(rsize>16?r_<<(rsize-16):r_>>(16-rsize)); c.g=component_t(gsize>16?g_<<(gsize-16):g_>>(16-gsize)); c.b=component_t(bsize>16?b_<<(bsize-16):b_>>(16-bsize)); c.a=component_t(asize>16?a_<<(asize-16):a_>>(16-asize));}\
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t a_)          {c.r=component_t(r_>>(32-rsize)); c.g=component_t(g_>>(32-gsize)); c.b=component_t(b_>>(32-bsize)); c.a=component_t(a_>>(32-asize));}\
  EGL_INLINE void set_rgba(float32_t r_, float32_t g_, float32_t b_, float32_t a_)      {c.r=component_t(r_*(rsize<32?uint32_t(1u<<rsize)-0.001f:42949672965.999f)); c.g=component_t(g_*(gsize<32?uint32_t(1u<<gsize)-0.001f:42949672965.999f)); c.b=component_t(b_*(bsize<32?uint32_t(1u<<bsize)-0.001f:42949672965.999f)); c.a=component_t(a_*(asize<32?uint32_t(1u<<bsize)-0.001f:42949672965.999f));}\
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)             {set_rgba(r_, g_, b_, a_);}\
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t a_)        {set_rgba(r_, g_, b_, a_);}\
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t a_)        {set_rgba(r_, g_, b_, a_);}\
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t g_, float32_t b_, float32_t a_)     {set_rgba(r_, g_, b_, a_);}
#define EGL_PIXEL_RGB_IMPL\
  EGL_INLINE pixel()                                                                    {}\
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), 0);}\
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, c_.g, c_.b, 0);}\
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), 0);}\
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {set_rgbaf(c_.r, c_.g, c_.b, 0.0f);}\
  EGL_INLINE component_t get_r() const                                                  {return component_t(c.r<<(sizeof(component_t)*8-rsize));}\
  EGL_INLINE component_t get_g() const                                                  {return component_t(c.g<<(sizeof(component_t)*8-gsize));}\
  EGL_INLINE component_t get_b() const                                                  {return component_t(c.b<<(sizeof(component_t)*8-bsize));}\
  EGL_INLINE component_t get_a() const                                                  {return 0;}\
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r/float(uint32_t(rsize<32?(1<<rsize)-1:-1)), c.g/float(uint32_t(gsize<32?(1<<gsize)-1:-1)), c.b/float(uint32_t(bsize<32?(1<<bsize)-1:-1)), 0.0f);}\
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t)                 {c.r=component_t(rsize>8?r_<<(rsize-8):r_>>(8-rsize)); c.g=component_t(gsize>8?g_<<(gsize-8):g_>>(8-gsize)); c.b=component_t(bsize>8?b_<<(bsize-8):b_>>(8-bsize));}\
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t)             {c.r=component_t(rsize>16?r_<<(rsize-16):r_>>(16-rsize)); c.g=component_t(gsize>16?g_<<(gsize-16):g_>>(16-gsize)); c.b=component_t(bsize>16?b_<<(bsize-16):b_>>(16-bsize));}\
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t)             {c.r=component_t(r_>>(32-rsize)); c.g=component_t(g_>>(32-gsize)); c.b=component_t(b_>>(32-bsize));}\
  EGL_INLINE void set_rgba(float32_t r_, float32_t g_, float32_t b_, float32_t)         {c.r=component_t(r_*(rsize<32?uint32_t(1u<<rsize)-0.001f:42949672965.999f)); c.g=component_t(g_*(gsize<32?uint32_t(1u<<gsize)-0.001f:42949672965.999f)); c.b=component_t(b_*(bsize<32?uint32_t(1u<<bsize)-0.001f:42949672965.999f));}\
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t)                {set_rgba(r_, g_, b_, 0);}\
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t)           {set_rgba(r_, g_, b_, 0);}\
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t)           {set_rgba(r_, g_, b_, 0);}\
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t g_, float32_t b_, float32_t)        {set_rgba(r_, g_, b_, 0);}
#define EGL_PIXEL_RG_IMPL\
  EGL_INLINE pixel()                                                                    {}\
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), px_.get_g(), 0, 0);}\
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, c_.g, 0, 0);}\
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), px_.get_g(), 0, 0);}\
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {set_rgbaf(c_.r, c_.g, 0.0f, 0.0f);}\
  EGL_INLINE component_t get_r() const                                                  {return component_t(c.r<<(sizeof(component_t)*8-rsize));}\
  EGL_INLINE component_t get_g() const                                                  {return component_t(c.g<<(sizeof(component_t)*8-gsize));}\
  EGL_INLINE component_t get_b() const                                                  {return 0;}\
  EGL_INLINE component_t get_a() const                                                  {return 0;}\
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r/float(uint32_t(rsize<32?(1<<rsize)-1:-1)), c.g/float(uint32_t(gsize<32?(1<<gsize)-1:-1)), 0.0f, 0.0f);}\
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t g_, uint8_t, uint8_t)                    {c.r=component_t(rsize>8?r_<<(rsize-8):r_>>(8-rsize)); c.g=component_t(gsize>8?g_<<(gsize-8):g_>>(8-gsize));}\
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t g_, uint16_t, uint16_t)                {c.r=component_t(rsize>16?r_<<(rsize-16):r_>>(16-rsize)); c.g=component_t(gsize>16?g_<<(gsize-16):g_>>(16-gsize));}\
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t g_, uint32_t, uint32_t)                {c.r=component_t(r_>>(32-rsize)); c.g=component_t(g_>>(32-gsize));}\
  EGL_INLINE void set_rgba(float32_t r_, float32_t g_, float32_t, float32_t)            {c.r=component_t(r_*(rsize<32?uint32_t(1u<<rsize)-0.001f:42949672965.999f)); c.g=component_t(g_*(gsize<32?uint32_t(1u<<gsize)-0.001f:42949672965.999f));}\
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t g_, uint8_t, uint8_t)                   {set_rgba(r_, g_, 0, 0);}\
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t g_, uint16_t, uint16_t)              {set_rgba(r_, g_, 0, 0);}\
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t g_, uint32_t, uint32_t)              {set_rgba(r_, g_, 0, 0);}\
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t g_, float32_t, float32_t)           {set_rgba(r_, g_, 0, 0);}
#define EGL_PIXEL_R_IMPL\
  EGL_INLINE pixel()                                                                    {}\
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), 0, 0, 0);}\
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, 0, 0, 0);}\
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), 0, 0, 0);}\
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {set_rgbaf(c_.r, 0.0f, 0.0f, 0.0f);}\
  EGL_INLINE component_t get_r() const                                                  {return component_t(c.r<<(sizeof(component_t)*8-rsize));}\
  EGL_INLINE component_t get_g() const                                                  {return 0;}\
  EGL_INLINE component_t get_b() const                                                  {return 0;}\
  EGL_INLINE component_t get_a() const                                                  {return 0;}\
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r/float(uint32_t(rsize<32?(1u<<rsize)-1:-1)), 0.0f, 0.0f, 0.0f);}\
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t, uint8_t, uint8_t)                       {c.r=component_t(rsize>8?r_<<(rsize-8):r_>>(8-rsize));}\
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t, uint16_t, uint16_t)                   {c.r=component_t(rsize>16?r_<<(rsize-16):r_>>(16-rsize));}\
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t, uint32_t, uint32_t)                   {c.r=component_t(r_>>(32-rsize));}\
  EGL_INLINE void set_rgba(float32_t r_, float32_t, float32_t, float32_t)               {c.r=component_t(r_*(rsize<32?uint32_t(1u<<rsize)-0.001f:42949672965.999f));}\
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t, uint8_t, uint8_t)                      {set_rgba(r_, 0, 0, 0);}\
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t, uint16_t, uint16_t)                 {set_rgba(r_, 0, 0, 0);}\
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t, uint32_t, uint32_t)                 {set_rgba(r_, 0, 0, 0);}\
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t, float32_t, float32_t)              {set_rgba(r_, 0, 0, 0);}
#define EGL_CHECK_PIXEL_SIZE(fmt__) EGL_STATIC_ASSERT(sizeof(pixel<fmt__>)*8==pixel<fmt__>::rsize+pixel<fmt__>::gsize+pixel<fmt__>::bsize+pixel<fmt__>::asize)
//----------------------------------------------------------------------------

template<> struct pixel<pixfmt_none> {};
//----------------------------------------------------------------------------

template<>
struct pixel<pixfmt_r3g3b2>
{
  enum {texfmt=pixfmt_r3g3b2};
  enum {rsize=3, gsize=3, bsize=2, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint8_t r:3, g:3, b:2;} c;
    uint8_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r3g3b2);
//----

template<>
struct pixel<pixfmt_r5g6b5>
{
  enum {texfmt=pixfmt_r5g6b5};
  enum {rsize=5, gsize=6, bsize=5, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint16_t r:5, g:6, b:5;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r5g6b5);
//----

template<>
struct pixel<pixfmt_b5g6r5>
{
  enum {texfmt=pixfmt_b5g6r5};
  enum {rsize=5, gsize=6, bsize=5, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint16_t b:5, g:6, r:5;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_b5g6r5);
//----

template<>
struct pixel<pixfmt_r5g5b5a1>
{
  enum {texfmt=pixfmt_r5g5b5a1};
  enum {rsize=5, gsize=5, bsize=5, asize=1};
  typedef uint8_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint16_t r:5, g:5, b:5, a:1;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r5g5b5a1);
//----

template<>
struct pixel<pixfmt_r4g4b4a4>
{
  enum {texfmt=pixfmt_r4g4b4a4};
  enum {rsize=4, gsize=4, bsize=4, asize=4};
  typedef uint8_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint16_t r:4, g:4, b:4, a:4;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r4g4b4a4);
//----

template<>
struct pixel<pixfmt_r8>
{
  enum {texfmt=pixfmt_r8};
  enum {rsize=8, gsize=0, bsize=0, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_R_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint8_t r;} c;
    uint8_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r8);
//----

template<>
struct pixel<pixfmt_r8g8>
{
  enum {texfmt=pixfmt_r8g8};
  enum {rsize=8, gsize=8, bsize=0, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_RG_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint8_t r, g;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r8g8);
//----

template<>
struct pixel<pixfmt_r8g8b8>
{
  enum {texfmt=pixfmt_r8g8b8};
  enum {rsize=8, gsize=8, bsize=8, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  struct {uint8_t r, g, b;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r8g8b8);
//----

template<>
struct pixel<pixfmt_b8g8r8>
{
  enum {texfmt=pixfmt_b8g8r8};
  enum {rsize=8, gsize=8, bsize=8, asize=0};
  typedef uint8_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  struct {uint8_t b, g, r;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_b8g8r8);
//----

template<>
struct pixel<pixfmt_r8g8b8a8>
{
  enum {texfmt=pixfmt_r8g8b8a8};
  enum {rsize=8, gsize=8, bsize=8, asize=8};
  typedef uint8_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint8_t r, g, b, a;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r8g8b8a8);
//----

template<>
struct pixel<pixfmt_b8g8r8a8>
{
  enum {texfmt=pixfmt_b8g8r8a8};
  enum {rsize=8, gsize=8, bsize=8, asize=8};
  typedef uint8_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint8_t b, g, r, a;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_b8g8r8a8);
//----

template<>
struct pixel<pixfmt_r11g11b10>
{
  enum {texfmt=pixfmt_r11g11b10};
  enum {rsize=11, gsize=11, bsize=10, asize=0};
  typedef uint16_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint32_t r:11, g:11, b:10;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r11g11b10);
//----

template<>
struct pixel<pixfmt_r11g11b10f>
{
  enum {texfmt=pixfmt_r11g11b10f};
  enum {rsize=11, gsize=11, bsize=10, asize=0};
  EGL_INLINE uint16_t get_r() const         {return uint16_t(c.r);}
  EGL_INLINE uint16_t get_g() const         {return uint16_t(c.g);}
  EGL_INLINE uint16_t get_b() const         {return uint16_t(c.b);}
  EGL_INLINE uint16_t get_a() const         {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const {EGL_ERROR_NOT_IMPL(); return color_rgbaf(0.0f, 0.0f, 0.0f, 0.0f);}
  //--------------------------------------------------------------------------

  union
  {
    struct {uint32_t r:11, g:11, b:10;} c;
    uint32_t v;
  };
};
//----

template<>
struct pixel<pixfmt_r10g10b10a2>
{
  enum {texfmt=pixfmt_r10g10b10a2};
  enum {rsize=10, gsize=10, bsize=10, asize=2};
  typedef uint16_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint32_t r:10, g:10, b:10, a:2;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r10g10b10a2);
//----

template<>
struct pixel<pixfmt_r10g10b10a2f>
{
  enum {texfmt=pixfmt_r10g10b10a2f};
  enum {rsize=10, gsize=10, bsize=10, asize=2};
  EGL_INLINE uint16_t get_r() const         {return uint16_t(c.r);}
  EGL_INLINE uint16_t get_g() const         {return uint16_t(c.g);}
  EGL_INLINE uint16_t get_b() const         {return uint16_t(c.b);}
  EGL_INLINE uint16_t get_a() const         {return uint16_t(c.a);}
  EGL_INLINE color_rgbaf get_colorf() const {EGL_ERROR_NOT_IMPL(); return color_rgbaf(0.0f, 0.0f, 0.0f, 0.0f);}
  //--------------------------------------------------------------------------

  union
  {
    struct {uint32_t r:10, g:10, b:10, a:2;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r10g10b10a2f);
//----

template<>
struct pixel<pixfmt_r16>
{
  enum {texfmt=pixfmt_r16};
  enum {rsize=16, gsize=0, bsize=0, asize=0};
  typedef uint16_t component_t;
  EGL_PIXEL_R_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint16_t r;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16);
//----

template<>
struct pixel<pixfmt_r16f>
{
  enum {texfmt=pixfmt_r16f};
  enum {rsize=16, gsize=0, bsize=0, asize=0};
  EGL_INLINE float16_t get_r() const        {return c.r;}
  EGL_INLINE float16_t get_g() const        {return 0;}
  EGL_INLINE float16_t get_b() const        {return 0;}
  EGL_INLINE float16_t get_a() const        {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const {EGL_ERROR_NOT_IMPL(); return color_rgbaf(0.0f, 0.0f, 0.0f, 0.0f);}
  //--------------------------------------------------------------------------

  union
  {
    struct {float16_t r;} c;
    uint16_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16f);
//----

template<>
struct pixel<pixfmt_r16g16>
{
  enum {texfmt=pixfmt_r16g16};
  enum {rsize=16, gsize=16, bsize=0, asize=0};
  typedef uint16_t component_t;
  EGL_PIXEL_RG_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint16_t r, g;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16g16);
//----

template<>
struct pixel<pixfmt_r16g16f>
{
  enum {texfmt=pixfmt_r16g16f};
  enum {rsize=16, gsize=16, bsize=0, asize=0};
  EGL_INLINE float16_t get_r() const        {return c.r;}
  EGL_INLINE float16_t get_g() const        {return c.g;}
  EGL_INLINE float16_t get_b() const        {return 0;}
  EGL_INLINE float16_t get_a() const        {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const {EGL_ERROR_NOT_IMPL(); return color_rgbaf(0.0f, 0.0f, 0.0f, 0.0f);}
  //--------------------------------------------------------------------------

  union
  {
    struct {float16_t r, g;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16g16f);
//----

template<>
struct pixel<pixfmt_r16g16b16>
{
  enum {texfmt=pixfmt_r16g16b16};
  enum {rsize=16, gsize=16, bsize=16, asize=0};
  typedef uint16_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  struct {uint16_t r, g, b;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16g16b16);
//----

template<>
struct pixel<pixfmt_r16g16b16f>
{
  enum {texfmt=pixfmt_r16g16b16f};
  enum {rsize=16, gsize=16, bsize=16, asize=0};
  EGL_INLINE float16_t get_r() const        {return c.r;}
  EGL_INLINE float16_t get_g() const        {return c.g;}
  EGL_INLINE float16_t get_b() const        {return c.b;}
  EGL_INLINE float16_t get_a() const        {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const {EGL_ERROR_NOT_IMPL(); return color_rgbaf(0.0f, 0.0f, 0.0f, 0.0f);}
  //--------------------------------------------------------------------------

  struct {float16_t r, g, b;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16g16b16f);
//----

template<>
struct pixel<pixfmt_r16g16b16a16>
{
  enum {texfmt=pixfmt_r16g16b16a16};
  enum {rsize=16, gsize=16, bsize=16, asize=16};
  typedef uint16_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  struct {uint16_t r, g, b, a;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16g16b16a16);
//----

template<>
struct pixel<pixfmt_r16g16b16a16f>
{
  enum {texfmt=pixfmt_r16g16b16a16f};
  enum {rsize=16, gsize=16, bsize=16, asize=16};
  EGL_INLINE float16_t get_r() const        {return c.r;}
  EGL_INLINE float16_t get_g() const        {return c.g;}
  EGL_INLINE float16_t get_b() const        {return c.b;}
  EGL_INLINE float16_t get_a() const        {return c.a;}
  EGL_INLINE color_rgbaf get_colorf() const {EGL_ERROR_NOT_IMPL(); return color_rgbaf(0.0f, 0.0f, 0.0f, 0.0f);}
  //--------------------------------------------------------------------------

  struct {float16_t r, g, b, a;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r16g16b16a16f);
//----

template<>
struct pixel<pixfmt_r32>
{
  enum {texfmt=pixfmt_r32};
  enum {rsize=32, gsize=0, bsize=0, asize=0};
  typedef uint32_t component_t;
  EGL_PIXEL_R_IMPL
  //--------------------------------------------------------------------------

  union
  {
    struct {uint32_t r;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32);
//----

template<>
struct pixel<pixfmt_r32f>
{
  enum {texfmt=pixfmt_r32f};
  enum {rsize=32, gsize=0, bsize=0, asize=0};
  typedef float32_t component_t;
  EGL_INLINE pixel()                                                                    {}
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), 0, 0, 0);}
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, 0, 0, 0);}
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), 0, 0, 0);}
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {c.r=c_.r;}
  EGL_INLINE float32_t get_r() const                                                    {return c.r;}
  EGL_INLINE float32_t get_g() const                                                    {return 0;}
  EGL_INLINE float32_t get_b() const                                                    {return 0;}
  EGL_INLINE float32_t get_a() const                                                    {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r, 0.0f, 0.0f, 0.0f);}
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t, uint8_t, uint8_t)                       {c.r=r_/255.0f;}
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t, uint16_t, uint16_t)                   {c.r=r_/65535.0f;}
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t, uint32_t, uint32_t)                   {c.r=r_/42949672965.0f;}
  EGL_INLINE void set_rgba(float32_t r_, float32_t, float32_t, float32_t)               {c.r=r_;}
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t, uint8_t, uint8_t)                      {c.r=r_/255.0f;}
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t, uint16_t, uint16_t)                 {c.r=r_/65535.0f;}
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t, uint32_t, uint32_t)                 {c.r=r_/42949672965.0f;}
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t, float32_t, float32_t)              {c.r=r_;}
  //--------------------------------------------------------------------------

  union
  {
    struct {float32_t r;} c;
    uint32_t v;
  };
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32f);
//----

template<>
struct pixel<pixfmt_r32g32>
{
  enum {texfmt=pixfmt_r32g32};
  enum {rsize=32, gsize=32, bsize=0, asize=0};
  typedef uint32_t component_t;
  EGL_PIXEL_RG_IMPL
  //--------------------------------------------------------------------------

  struct {uint32_t r, g;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32f);
//----

template<>
struct pixel<pixfmt_r32g32f>
{
  enum {texfmt=pixfmt_r32g32f};
  enum {rsize=32, gsize=32, bsize=0, asize=0};
  typedef float32_t component_t;
  EGL_INLINE pixel()                                                                    {}
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), px_.get_g(), 0, 0);}
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, c_.g, 0, 0);}
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), px_.get_g(), 0, 0);}
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {c.r=c_.r; c.g=c_.g;}
  EGL_INLINE float32_t get_r() const                                                    {return c.r;}
  EGL_INLINE float32_t get_g() const                                                    {return c.g;}
  EGL_INLINE float32_t get_b() const                                                    {return 0;}
  EGL_INLINE float32_t get_a() const                                                    {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r, c.g, 0.0f, 0.0f);}
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t g_, uint8_t, uint8_t)                    {c.r=r_/255.0f; c.g=g_/255.0f;}
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t g_, uint16_t, uint16_t)                {c.r=r_/65535.0f; c.g=g_/65535.0f;}
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t g_, uint32_t, uint32_t)                {c.r=r_/42949672965.0f; c.g=g_/42949672965.0f;}
  EGL_INLINE void set_rgba(float32_t r_, float32_t g_, float32_t, float32_t)            {c.r=r_; c.g=g_;}
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t g_, uint8_t, uint8_t)                   {c.r=r_/255.0f; c.g=g_/255.0f;}
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t g_, uint16_t, uint16_t)              {c.r=r_/65535.0f; c.g=g_/65535.0f;}
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t g_, uint32_t, uint32_t)              {c.r=r_/42949672965.0f; c.g=g_/42949672965.0f;}
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t g_, float32_t, float32_t)           {c.r=r_; c.g=g_;}
  //--------------------------------------------------------------------------

  struct {float32_t r, g;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32g32f);
//----

template<>
struct pixel<pixfmt_r32g32b32>
{
  enum {texfmt=pixfmt_r32g32b32};
  enum {rsize=32, gsize=32, bsize=32, asize=0};
  typedef uint32_t component_t;
  EGL_PIXEL_RGB_IMPL
  //--------------------------------------------------------------------------

  struct {uint32_t r, g, b;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32g32b32);
//----

template<>
struct pixel<pixfmt_r32g32b32f>
{
  enum {texfmt=pixfmt_r32g32b32f};
  enum {rsize=32, gsize=32, bsize=32, asize=0};
  typedef float32_t component_t;
  EGL_INLINE pixel()                                                                    {}
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), 0);}
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, c_.g, c_.b, 0);}
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), 0);}
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {c.r=c_.r; c.g=c_.g; c.b=c_.b;}
  EGL_INLINE float32_t get_r() const                                                    {return c.r;}
  EGL_INLINE float32_t get_g() const                                                    {return c.g;}
  EGL_INLINE float32_t get_b() const                                                    {return c.b;}
  EGL_INLINE float32_t get_a() const                                                    {return 0;}
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r, c.g, c.b, 0.0f);}
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t)                 {c.r=r_/255.0f; c.g=g_/255.0f; c.b=b_/255.0f;}
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t)             {c.r=r_/65535.0f; c.g=g_/65535.0f; c.b=b_/65535.0f;}
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t)             {c.r=r_/42949672965.0f; c.g=g_/42949672965.0f; c.b=b_/42949672965.0f;}
  EGL_INLINE void set_rgba(float32_t r_, float32_t g_, float32_t b_, float32_t)         {c.r=r_; c.g=g_; c.b=b_;}
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t)                {c.r=r_/255.0f; c.g=g_/255.0f; c.b=b_/255.0f;}
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t)           {c.r=r_/65535.0f; c.g=g_/65535.0f; c.b=b_/65535.0f;}
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t)           {c.r=r_/42949672965.0f; c.g=g_/42949672965.0f; c.b=b_/42949672965.0f;}
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t g_, float32_t b_, float32_t)        {c.r=r_; c.g=g_; c.b=b_;}
  //--------------------------------------------------------------------------

  struct {float32_t r, g, b;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32g32b32f);
//----

template<>
struct pixel<pixfmt_r32g32b32a32>
{
  enum {texfmt=pixfmt_r32g32b32a32};
  enum {rsize=32, gsize=32, bsize=32, asize=32};
  typedef uint32_t component_t;
  EGL_PIXEL_RGBA_IMPL
  //--------------------------------------------------------------------------

  struct {uint32_t r, g, b, a;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32g32b32a32);
//----

template<>
struct pixel<pixfmt_r32g32b32a32f>
{
  enum {texfmt=pixfmt_r32g32b32a32f};
  enum {rsize=32, gsize=32, bsize=32, asize=32};
  typedef float32_t component_t;
  EGL_INLINE pixel()                                                                    {}
  template<e_pixel_format src_fmt> EGL_INLINE pixel(const pixel<src_fmt> &px_)          {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), px_.get_a());}
  EGL_INLINE pixel(const color_rgbaf &c_)                                               {set_rgba(c_.r, c_.g, c_.b, c_.a);}
  template<e_pixel_format src_fmt> EGL_INLINE void operator=(const pixel<src_fmt> &px_) {set_rgba(px_.get_r(), px_.get_g(), px_.get_b(), px_.get_a());}
  EGL_INLINE void operator=(const color_rgbaf &c_)                                      {c.r=c_.r; c.g=c_.g; c.b=c_.b; c.a=c_.a;}
  EGL_INLINE float32_t get_r() const                                                    {return c.r;}
  EGL_INLINE float32_t get_g() const                                                    {return c.g;}
  EGL_INLINE float32_t get_b() const                                                    {return c.b;}
  EGL_INLINE float32_t get_a() const                                                    {return c.a;}
  EGL_INLINE color_rgbaf get_colorf() const                                             {return color_rgbaf(c.r, c.g, c.b, c.a);}
  EGL_INLINE void set_rgba(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)              {c.r=r_/255.0f; c.g=g_/255.0f; c.b=b_/255.0f; c.a=a_/255.0f;}
  EGL_INLINE void set_rgba(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t a_)          {c.r=r_/65535.0f; c.g=g_/65535.0f; c.b=b_/65535.0f; c.a=a_/65535.0f;}
  EGL_INLINE void set_rgba(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t a_)          {c.r=r_/42949672965.0f; c.g=g_/42949672965.0f; c.b=b_/42949672965.0f; c.a=a_/42949672965.0f;}
  EGL_INLINE void set_rgba(float32_t r_, float32_t g_, float32_t b_, float32_t a_)      {c.r=r_; c.g=g_; c.b=b_; c.a=a_;}
  EGL_INLINE void set_rgba8(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)             {c.r=r_/255.0f; c.g=g_/255.0f; c.b=b_/255.0f; c.a=a_/255.0f;}
  EGL_INLINE void set_rgba16(uint16_t r_, uint16_t g_, uint16_t b_, uint16_t a_)        {c.r=r_/65535.0f; c.g=g_/65535.0f; c.b=b_/65535.0f; c.a=a_/65535.0f;}
  EGL_INLINE void set_rgba32(uint32_t r_, uint32_t g_, uint32_t b_, uint32_t a_)        {c.r=r_/42949672965.0f; c.g=g_/42949672965.0f; c.b=b_/42949672965.0f; c.a=a_/42949672965.0f;}
  EGL_INLINE void set_rgbaf(float32_t r_, float32_t g_, float32_t b_, float32_t a_)     {c.r=r_; c.g=g_; c.b=b_; c.a=a_;}
  //--------------------------------------------------------------------------

  struct {float32_t r, g, b, a;} c;
};
EGL_CHECK_PIXEL_SIZE(pixfmt_r32g32b32a32f);
//----------------------------------------------------------------------------

#undef EGL_PIXEL_RGBA_IMPL
#undef EGL_PIXEL_RGB_IMPL
#undef EGL_PIXEL_RG_IMPL
#undef EGL_PIXEL_R_IMPL
#undef EGL_CHECK_PIXEL_SIZE
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
