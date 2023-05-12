//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_COLOR_H
#define EGL_COLOR_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_math.h"
EGL_NAMESPACE_BEGIN

// new
template<typename> struct color_rgb;
template<typename> struct color_rgba;
template<typename> struct color_xyz;
template<typename> struct color_xyza;
template<typename> struct color_yiq;
template<typename> struct color_yiqa;
template<typename> struct color_hsv;
template<typename> struct color_hsva;
// RGB color types
typedef color_rgb<float32_t> color_rgbf;
typedef color_rgb<float64_t> color_rgbd;
typedef color_rgb<uint8_t> color_rgb32;
typedef color_rgb<int32_t> color_rgbi;
typedef color_rgb<uint32_t> color_rgbu;
typedef color_rgba<float32_t> color_rgbaf;
typedef color_rgba<float64_t> color_rgbad;
typedef color_rgba<uint8_t> color_rgba32;
typedef color_rgba<int32_t> color_rgbai;
typedef color_rgba<uint32_t> color_rgbau;
// XYZ color types
typedef color_xyz<float32_t> color_xyzf;
typedef color_xyz<float64_t> color_xyzd;
typedef color_xyz<uint8_t> color_xyz32;
typedef color_xyz<int32_t> color_xyzi;
typedef color_xyz<uint32_t> color_xyzu;
typedef color_xyza<float32_t> color_xyzaf;
typedef color_xyza<float64_t> color_xyzad;
typedef color_xyza<uint8_t> color_xyza32;
typedef color_xyza<int32_t> color_xyzai;
typedef color_xyza<uint32_t> color_xyzau;
// YIQ color types
typedef color_yiq<float32_t> color_yiqf;
typedef color_yiq<float64_t> color_yiqd;
typedef color_yiq<uint8_t> color_yiq32;
typedef color_yiq<int32_t> color_yiqi;
typedef color_yiq<uint32_t> color_yiqu;
typedef color_yiqa<float32_t> color_yiqaf;
typedef color_yiqa<float64_t> color_yiqad;
typedef color_yiqa<uint8_t> color_yiqa32;
typedef color_yiqa<int32_t> color_yiqai;
typedef color_yiqa<uint32_t> color_yiqau;
// HSV color types
typedef color_hsv<float32_t> color_hsvf;
typedef color_hsv<float64_t> color_hsvd;
typedef color_hsv<uint8_t> color_hsv32;
typedef color_hsv<int32_t> color_hsvi;
typedef color_hsv<uint32_t> color_hsvu;
typedef color_hsva<float32_t> color_hsvaf;
typedef color_hsva<float64_t> color_hsvad;
typedef color_hsva<uint8_t> color_hsva32;
typedef color_hsva<int32_t> color_hsvai;
typedef color_hsva<uint32_t> color_hsvau;
// RGB color spaces
enum e_rgb_color_space
{
  rgbcs_srgb,     // sRGB / Rec. 709 / BT.709
  rgbcs_rec2020,  // Rec. 2020 / BT.2020
  rgbcs_dcip3,    // DCI-P3
  rgbcs_dcip3t,   // DCI-P3 Theater
  rgbcs_adobe,    // Adobe RGB
  rgbcs_adobe_wg, // Adobe Wide Gamut RGB
  rgbcs_prophoto, // ProPhoto
  rgbcs_aces_ap0, // ACES AP0
  rgbcs_aces_ap1, // ACES AP1
};
// RGB <-> XYZ color space conversion
template<typename T> mat33<T> tform_rgb_to_xyz(const vec2<T> &prim_rxy_, const vec2<T> &prim_gxy_, const vec2<T> &prim_bxy_, const vec2<T> &wp_);
template<typename T> mat33<T> tform_xyz_to_rgb(const vec2<T> &prim_rxy_, const vec2<T> &prim_gxy_, const vec2<T> &prim_bxy_, const vec2<T> &wp_);
template<typename T> mat33<T> tform_rgb_to_xyz(e_rgb_color_space);
template<typename T> mat33<T> tform_xyz_to_rgb(e_rgb_color_space);
// RGB <-> YIQ color space conversion
template<typename T> EGL_INLINE mat33<T> tform_srgb_to_yiq();
template<typename T> EGL_INLINE mat33<T> tform_yiq_to_srgb();
template<typename T> EGL_INLINE color_yiq<T> srgb_to_yiq(const color_rgb<T>&);
template<typename T> EGL_INLINE color_rgb<T> yiq_to_srgb(const color_yiq<T>&);
template<typename T> EGL_INLINE color_yiqa<T> srgb_to_yiq(const color_rgba<T>&);
template<typename T> EGL_INLINE color_rgba<T> yiq_to_srgb(const color_yiqa<T>&);
// RGB <-> HSV color space conversion
template<typename T> color_rgb<T> hsv_to_srgb(const color_hsv<T>&);
template<typename T> color_hsv<T> srgb_to_hsv(const color_rgb<T>&);
template<typename T> color_rgba<T> hsv_to_srgb(const color_hsva<T>&);
template<typename T> color_hsva<T> srgb_to_hsv(const color_rgba<T>&);
// sRGB <-> linear conversion
template<typename T> EGL_INLINE T linear_to_srgb(T);
template<typename T> EGL_INLINE color_rgb<T> linear_to_srgb(const color_rgb<T>&);
template<typename T> EGL_INLINE color_rgba<T> linear_to_srgb(const color_rgba<T>&);
template<typename T> EGL_INLINE T srgb_to_linear(T);
template<typename T> EGL_INLINE color_rgb<T> srgb_to_linear(const color_rgb<T>&);
template<typename T> EGL_INLINE color_rgba<T> srgb_to_linear(const color_rgba<T>&);
// color operations
template<typename T> typename math<T>::scalar_t EGL_INLINE luminance_srgb(const color_rgb<T>&);
template<typename T> typename math<T>::scalar_t EGL_INLINE luminance_srgb(const color_rgba<T>&);
// CIE standard illuminant white points
enum e_cie_white_point
{
  // CIE 1931 2-degree standard observer illuminant white points
  ciewpnt_2deg_a,       // A   (2856K) - Incandescent / Tungsten
  ciewpnt_2deg_b,       // B   (4874K) - Direct sunlight at noon
  ciewpnt_2deg_c,       // C   (6774K) - Average / North sky Daylight
  ciewpnt_2deg_d50,     // D50 (5003K) - Horizon Light. ICC profile PCS
  ciewpnt_2deg_d55,     // D55 (5503K) - Mid-morning / Mid-afternoon Daylight
  ciewpnt_2deg_d60,     // D60 (6000K)
  ciewpnt_2deg_d65,     // D65 (6504K) - Noon Daylight: Television, sRGB color space
  ciewpnt_2deg_d65_p3t, // D65 DCI-P3 Theater
  ciewpnt_2deg_d75,     // D75 (7504K) - North sky Daylight
  ciewpnt_2deg_e,       // E   (5454K) - Equal energy
  ciewpnt_2deg_f1,      // F1  (6430K) - Daylight Fluorescent
  ciewpnt_2deg_f2,      // F2  (4230K) - Cool White Fluorescent
  ciewpnt_2deg_f3,      // F3  (3450K) - White Fluorescent
  ciewpnt_2deg_f4,      // F4  (2940K) - Warm White Fluorescent
  ciewpnt_2deg_f5,      // F5  (6350K) - Daylight Fluorescent
  ciewpnt_2deg_f6,      // F6  (4150K) - Lite White Fluorescent
  ciewpnt_2deg_f7,      // F7  (6500K) - D65 simulator, Daylight simulator
  ciewpnt_2deg_f8,      // F8  (5000K) - D50 simulator, Sylvania F40 Design 50t
  ciewpnt_2deg_f9,      // F9  (4150K) - Cool White Deluxe Fluorescent
  ciewpnt_2deg_f10,     // F10 (5000K) - Philips TL85, Ultralume 50
  ciewpnt_2deg_f11,     // F11 (4000K) - Philips TL84, Ultralume 40
  ciewpnt_2deg_f12,     // F12 (3000K) - Philips TL83, Ultralume 30
  // CIE 1964 10-degree standard observer illuminant white points
  ciewpnt_10deg_a,      // A   (2856K) - Incandescent / Tungsten
  ciewpnt_10deg_b,      // B   (4874K) - Direct sunlight at noon
  ciewpnt_10deg_c,      // C   (6774K) - Average / North sky Daylight
  ciewpnt_10deg_d50,    // D50 (5003K) - Horizon Light. ICC profile PCS
  ciewpnt_10deg_d55,    // D55 (5503K) - Mid-morning / Mid-afternoon Daylight
  ciewpnt_10deg_d65,    // D65 (6504K) - Noon Daylight: Television, sRGB color space
  ciewpnt_10deg_d75,    // D75 (7504K) - North sky Daylight
  ciewpnt_10deg_e,      // E   (5454K) - Equal energy
  ciewpnt_10deg_f1,     // F1  (6430K) - Daylight Fluorescent
  ciewpnt_10deg_f2,     // F2  (4230K) - Cool White Fluorescent
  ciewpnt_10deg_f3,     // F3  (3450K) - White Fluorescent
  ciewpnt_10deg_f4,     // F4  (2940K) - Warm White Fluorescent
  ciewpnt_10deg_f5,     // F5  (6350K) - Daylight Fluorescent
  ciewpnt_10deg_f6,     // F6  (4150K) - Lite White Fluorescent
  ciewpnt_10deg_f7,     // F7  (6500K) - D65 simulator, Daylight simulator
  ciewpnt_10deg_f8,     // F8  (5000K) - D50 simulator, Sylvania F40 Design 50t
  ciewpnt_10deg_f9,     // F9  (4150K) - Cool White Deluxe Fluorescent
  ciewpnt_10deg_f10,    // F10 (5000K) - Philips TL85, Ultralume 50
  ciewpnt_10deg_f11,    // F11 (4000K) - Philips TL84, Ultralume 40
  ciewpnt_10deg_f12,    // F12 (3000K) - Philips TL83, Ultralume 30
};
template<typename T> vec2<T> cie_white_point(e_cie_white_point);
//----------------------------------------------------------------------------


//============================================================================
// math constants
//============================================================================
template<typename T> struct math<color_xyz<T> >:math<T> {};
template<typename T> struct math<color_xyza<T> >:math<T> {};
template<typename T> struct math<color_rgb<T> >:math<T> {};
template<typename T> struct math<color_rgba<T> >:math<T> {};
template<typename T> struct math<color_yiq<T> >:math<T> {};
template<typename T> struct math<color_yiqa<T> >:math<T> {};
template<typename T> struct math<color_hsv<T> >:math<T> {};
template<typename T> struct math<color_hsva<T> >:math<T> {};
//----------------------------------------------------------------------------


//============================================================================
// color_rgb
//============================================================================
template<typename T>
struct color_rgb
{
  // construction
  EGL_INLINE color_rgb();
  EGL_INLINE color_rgb(T v_);
  EGL_INLINE color_rgb(T r_, T g_, T b_);
  EGL_INLINE color_rgb(const color_rgba<T>&);
  template<typename U> EGL_INLINE color_rgb(const color_rgb<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T v_);
  EGL_INLINE void set(T r_, T g_, T b_);
  template<typename U> EGL_INLINE void set(const color_rgb<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_rgb zero();
  static EGL_INLINE color_rgb one();
  //--------------------------------------------------------------------------

  T r, g, b;
};
//----------------------------------------------------------------------------


//============================================================================
// color_rgba
//============================================================================
template<typename T>
struct color_rgba
{
  // construction
  EGL_INLINE color_rgba();
  EGL_INLINE color_rgba(T v_);
  EGL_INLINE color_rgba(T r_, T g_, T b_);
  EGL_INLINE color_rgba(T r_, T g_, T b_, T a_);
  EGL_INLINE color_rgba(const color_rgb<T>&);
  EGL_INLINE color_rgba(const color_rgb<T>&, T a_);
  template<typename U> EGL_INLINE color_rgba(const color_rgba<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T v_);
  EGL_INLINE void set(T r_, T g_, T b_);
  EGL_INLINE void set(T r_, T g_, T b_, T a_);
  EGL_INLINE void set(const color_rgb<T>&, T a_);
  template<typename U> EGL_INLINE void set(const color_rgba<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_rgba zero();
  static EGL_INLINE color_rgba one();
  //--------------------------------------------------------------------------

  T r, g, b, a;
};
//----------------------------------------------------------------------------


//============================================================================
// color_xyz
//============================================================================
template<typename T>
struct color_xyz
{
  // construction
  EGL_INLINE color_xyz();
  EGL_INLINE color_xyz(T v_);
  EGL_INLINE color_xyz(T x_, T y_, T z_);
  EGL_INLINE color_xyz(const color_xyza<T>&);
  template<typename U> EGL_INLINE color_xyz(const color_xyz<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T v_);
  EGL_INLINE void set(T x_, T y_, T z_);
  template<typename U> EGL_INLINE void set(const color_xyz<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_xyz zero();
  static EGL_INLINE color_xyz one();
  //--------------------------------------------------------------------------

  T x, y, z;
};
//----------------------------------------------------------------------------


//============================================================================
// color_xyza
//============================================================================
template<typename T>
struct color_xyza
{
  // construction
  EGL_INLINE color_xyza();
  EGL_INLINE color_xyza(T v_);
  EGL_INLINE color_xyza(T x_, T y_, T z_);
  EGL_INLINE color_xyza(T x_, T y_, T z_, T a_);
  EGL_INLINE color_xyza(const color_xyz<T>&);
  EGL_INLINE color_xyza(const color_xyz<T>&, T a_);
  template<typename U> EGL_INLINE color_xyza(const color_xyza<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T v_);
  EGL_INLINE void set(T x_, T y_, T z_);
  EGL_INLINE void set(T x_, T y_, T z_, T a_);
  EGL_INLINE void set(const color_xyz<T>&, T a_);
  template<typename U> EGL_INLINE void set(const color_xyza<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_xyza zero();
  static EGL_INLINE color_xyza one();
  //--------------------------------------------------------------------------

  T x, y, z, a;
};
//----------------------------------------------------------------------------


//============================================================================
// color_yiq
//============================================================================
template<typename T>
struct color_yiq
{
  // construction
  EGL_INLINE color_yiq();
  EGL_INLINE color_yiq(T y_, T i_, T q_);
  EGL_INLINE color_yiq(const color_yiqa<T>&);
  template<typename U> EGL_INLINE color_yiq(const color_yiq<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T y_, T i_, T q_);
  template<typename U> EGL_INLINE void set(const color_yiq<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_yiq zero();
  static EGL_INLINE color_yiq one();
  //--------------------------------------------------------------------------

  T y, i, q;
};
//----------------------------------------------------------------------------


//============================================================================
// color_yiqa
//============================================================================
template<typename T>
struct color_yiqa
{
  // construction
  EGL_INLINE color_yiqa();
  EGL_INLINE color_yiqa(T y_, T i_, T q_);
  EGL_INLINE color_yiqa(T y_, T i_, T q_, T a_);
  EGL_INLINE color_yiqa(const color_yiq<T>&);
  EGL_INLINE color_yiqa(const color_yiq<T>&, T a_);
  template<typename U> EGL_INLINE color_yiqa(const color_yiqa<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T y_, T i_, T q_);
  EGL_INLINE void set(T y_, T i_, T q_, T a_);
  EGL_INLINE void set(const color_yiq<T>&, T a_);
  template<typename U> EGL_INLINE void set(const color_yiqa<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_yiqa zero();
  static EGL_INLINE color_yiqa one();
  //--------------------------------------------------------------------------

  T y, i, q, a;
};
//----------------------------------------------------------------------------


//============================================================================
// color_hsv
//============================================================================
template<typename T>
struct color_hsv
{
  // construction
  EGL_INLINE color_hsv();
  EGL_INLINE color_hsv(T h_, T s_, T v_);
  EGL_INLINE color_hsv(const color_hsva<T>&);
  template<typename U> EGL_INLINE color_hsv(const color_hsv<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T h_, T s_, T v_);
  template<typename U> EGL_INLINE void set(const color_hsv<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_hsv zero();
  static EGL_INLINE color_hsv one();
  //--------------------------------------------------------------------------

  T h, s, v;
};
//----------------------------------------------------------------------------


//============================================================================
// color_hsva
//============================================================================
template<typename T>
struct color_hsva
{
  // construction
  EGL_INLINE color_hsva();
  EGL_INLINE color_hsva(T h_, T s_, T v_);
  EGL_INLINE color_hsva(T h_, T s_, T v_, T a_);
  EGL_INLINE color_hsva(const color_hsv<T>&);
  EGL_INLINE color_hsva(const color_hsv<T>&, T a_);
  template<typename U> EGL_INLINE color_hsva(const color_hsva<U>&);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T h_, T s_, T v_);
  EGL_INLINE void set(T h_, T s_, T v_, T a_);
  EGL_INLINE void set(const color_hsv<T>&, T a_);
  template<typename U> EGL_INLINE void set(const color_hsva<U>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE color_hsva zero();
  static EGL_INLINE color_hsva one();
  //--------------------------------------------------------------------------

  T h, s, v, a;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// color_rgb
//============================================================================
template<typename T>
color_rgb<T> color_rgb<T>::zero()
{
  return color_rgb<T>(0);
}
//----

template<typename T>
color_rgb<T> color_rgb<T>::one()
{
  return color_rgb<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_rgb<T>::color_rgb()
{
}
//----

template<typename T>
color_rgb<T>::color_rgb(T v_)
  :r(v_)
  ,g(v_)
  ,b(v_)
{
}
//----

template<typename T>
color_rgb<T>::color_rgb(T r_, T g_, T b_)
  :r(r_)
  ,g(g_)
  ,b(b_)
{
}
//----

template<typename T>
color_rgb<T>::color_rgb(const color_rgba<T> &c_)
  :r(c_.r)
  ,g(c_.g)
  ,b(c_.b)
{
}
//----

template<typename T>
template<typename U>
color_rgb<T>::color_rgb(const color_rgb<U> &c_)
  :r(T(c_.r))
  ,g(T(c_.g))
  ,b(T(c_.b))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_rgb<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_rgb<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_rgb<T>::set(T v_)
{
  r=v_;
  g=v_;
  b=v_;
}
//----

template<typename T>
void color_rgb<T>::set(T r_, T g_, T b_)
{
  r=r_;
  g=g_;
  b=b_;
}
//----

template<typename T>
template<typename U>
void color_rgb<T>::set(const color_rgb<U> &c_)
{
  r=T(c_.r);
  g=T(c_.g);
  b=T(c_.b);
}
//----------------------------------------------------------------------------

// vec3 interface for color_rgb
template<typename T> EGL_INLINE bool is_zero(const color_rgb<T> &c_)                                             {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_black(const color_rgb<T> &c_)                                            {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_rgb<T> &c_)                                              {return is_sat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_rgb<T> &c_)                                             {return is_ssat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_rgb<T> &c0_, const color_rgb<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)==reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_rgb<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_rgb<T> &c_)           {return s_==reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_rgb<T> &c0_, const color_rgb<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)!=reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_rgb<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_rgb<T> &c_)           {return s_!=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE color_rgb<T> operator<(const color_rgb<T> &c0_, const color_rgb<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator<(const color_rgb<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator<(typename math<T>::scalar_t s_, const color_rgb<T> &c_)    {const vec3<T> &v=s_<reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator>(const color_rgb<T> &c0_, const color_rgb<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator>(const color_rgb<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator>(typename math<T>::scalar_t s_, const color_rgb<T> &c_)    {const vec3<T> &v=s_>reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator<=(const color_rgb<T> &c0_, const color_rgb<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator<=(const color_rgb<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<=s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator<=(typename math<T>::scalar_t s_, const color_rgb<T> &c_)   {const vec3<T> &v=s_<=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator>=(const color_rgb<T> &c0_, const color_rgb<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator>=(const color_rgb<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>=s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator>=(typename math<T>::scalar_t s_, const color_rgb<T> &c_)   {const vec3<T> &v=s_>=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sel_eq(const color_rgb<T> &t0_, const color_rgb<T> &t1_,
                                                     const color_rgb<T> &c0_, const color_rgb<T> &c1_)           {const vec3<T> &v=sel_eq(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sel_lt(const color_rgb<T> &t0_, const color_rgb<T> &t1_,
                                                     const color_rgb<T> &c0_, const color_rgb<T> &c1_)           {const vec3<T> &v=sel_lt(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sel_le(const color_rgb<T> &t0_, const color_rgb<T> &t1_,
                                                     const color_rgb<T> &c0_, const color_rgb<T> &c1_)           {const vec3<T> &v=sel_le(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sel_eqz(const color_rgb<T> &t_,
                                                      const color_rgb<T> &c0_, const color_rgb<T> &c1_)          {const vec3<T> &v=sel_eqz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sel_ltz(const color_rgb<T> &t_,
                                                      const color_rgb<T> &c0_, const color_rgb<T> &c1_)          {const vec3<T> &v=sel_ltz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sel_lez(const color_rgb<T> &t_,
                                                      const color_rgb<T> &c0_, const color_rgb<T> &c1_)          {const vec3<T> &v=sel_lez(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_rgb<T> &cr_, const color_rgb<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)+=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_rgb<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_rgb<T> &cr_, const color_rgb<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)-=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_rgb<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_rgb<T> &cr_, const color_rgb<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)*=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_rgb<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_rgb<T> &cr_, const mat33<T> &m_)                           {reinterpret_cast<vec3<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator*=(color_rgb<T> &cr_, const mat44<T> &m_)                           {reinterpret_cast<vec3<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator*=(color_rgb<T> &cr_, const quat<T> &q_)                            {reinterpret_cast<vec3<T>&>(cr_)*=q_;}
template<typename T> EGL_INLINE void operator/=(color_rgb<T> &cr_, const color_rgb<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)/=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_rgb<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_rgb<T> operator+(const color_rgb<T> &c0_, const color_rgb<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)+reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator+(const color_rgb<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)+s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator+(typename math<T>::scalar_t s_, const color_rgb<T> &c_)    {const vec3<T> &v=s_+reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator-(const color_rgb<T> &c0_, const color_rgb<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)-reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator-(const color_rgb<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)-s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator-(typename math<T>::scalar_t s_, const color_rgb<T> &c_)    {const vec3<T> &v=s_-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator-(const color_rgb<T> &c_)                                   {const vec3<T> &v=-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const color_rgb<T> &c0_, const color_rgb<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)*reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const color_rgb<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(typename math<T>::scalar_t s_, const color_rgb<T> &c_)    {const vec3<T> &v=s_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const color_rgb<T> &c_, const mat33<T> &m_)               {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*m_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const mat33<T> &m_, const color_rgb<T> &c_)               {const vec3<T> &v=m_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const color_rgb<T> &c_, const mat44<T> &m_)               {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*m_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const mat44<T> &m_, const color_rgb<T> &c_)               {const vec3<T> &v=m_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator*(const color_rgb<T> &c_, const quat<T> &q_)                {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*q_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator/(const color_rgb<T> &c0_, const color_rgb<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)/reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> operator/(const color_rgb<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)/s_; return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_rgb<T> &c_)                           {return min(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_rgb<T> min(const color_rgb<T> &c0_,
                                                  const color_rgb<T> &c1_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> min(const color_rgb<T> &c0_,
                                                  const color_rgb<T> &c1_,
                                                  const color_rgb<T> &c2_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> min(const color_rgb<T> &c0_,
                                                  const color_rgb<T> &c1_,
                                                  const color_rgb<T> &c2_,
                                                  const color_rgb<T> &c3_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> min(const color_rgb<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> min(typename math<T>::scalar_t s_, const color_rgb<T> &c_)          {const vec3<T> &v=min(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_rgb<T> &c_)                           {return max(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_rgb<T> max(const color_rgb<T> &c0_,
                                                  const color_rgb<T> &c1_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> max(const color_rgb<T> &c0_,
                                                  const color_rgb<T> &c1_,
                                                  const color_rgb<T> &c2_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> max(const color_rgb<T> &c0_,
                                                  const color_rgb<T> &c1_,
                                                  const color_rgb<T> &c2_,
                                                  const color_rgb<T> &c3_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> max(const color_rgb<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> max(typename math<T>::scalar_t s_, const color_rgb<T> &c_)          {const vec3<T> &v=max(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> mul(const color_rgb<T> &c0_, const color_rgb<T> &c1_)               {const vec3<T> &v=mul(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> madd(const color_rgb<T> &c0_,
                                                   const color_rgb<T> &c1_,
                                                   const color_rgb<T> &c2_)                                      {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> madd(const color_rgb<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, add_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> madd(const color_rgb<T> &c_,
                                                   const color_rgb<T> &mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(mul_), add_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> madd(const color_rgb<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_rgb<T> &add_)                                     {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, reinterpret_cast<const vec3<T>&>(add_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> rcp(const color_rgb<T> &c_)                                         {const vec3<T> &v=rcp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> rcp_z(const color_rgb<T> &c_)                                       {const vec3<T> &v=rcp_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> floor(const color_rgb<T> &c_)                                       {const vec3<T> &v=floor(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> ceil(const color_rgb<T> &c_)                                        {const vec3<T> &v=ceil(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> trunc(const color_rgb<T> &c_)                                       {const vec3<T> &v=trunc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> frc(const color_rgb<T> &c_)                                         {const vec3<T> &v=frc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> mod(const color_rgb<T> &c_, T div_)                                 {const vec3<T> &v=mod(reinterpret_cast<const vec3<T>&>(c_), div_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> cycle(const color_rgb<T> &c_, T cycle_)                             {const vec3<T> &v=cycle(reinterpret_cast<const vec3<T>&>(c_), cycle_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> cycle1(const color_rgb<T> &c_)                                      {const vec3<T> &v=cycle1(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sat(const color_rgb<T> &c_)                                         {const vec3<T> &v=sat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> ssat(const color_rgb<T> &c_)                                        {const vec3<T> &v=ssat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> clamp(const color_rgb<T> &c_,
                                                    const color_rgb<T> &min_,
                                                    const color_rgb<T> &max_)                                    {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(min_), reinterpret_cast<const vec3<T>&>(max_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> clamp(const color_rgb<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                             {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), min_, max_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> abs(const color_rgb<T> &c_)                                         {const vec3<T> &v=abs(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sgn(const color_rgb<T> &c_)                                         {const vec3<T> &v=sgn(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sgn_zp(const color_rgb<T> &c_)                                      {const vec3<T> &v=sgn_zp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sqr(const color_rgb<T> &c_)                                         {const vec3<T> &v=sqr(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> cubic(const color_rgb<T> &c_)                                       {const vec3<T> &v=cubic(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sqrt(const color_rgb<T> &c_)                                        {const vec3<T> &v=sqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> sqrt_z(const color_rgb<T> &c_)                                      {const vec3<T> &v=sqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> cbrt(const color_rgb<T> &c_)                                        {const vec3<T> &v=cbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> rsqrt(const color_rgb<T> &c_)                                       {const vec3<T> &v=rsqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> rsqrt_z(const color_rgb<T> &c_)                                     {const vec3<T> &v=rsqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> rcbrt(const color_rgb<T> &c_)                                       {const vec3<T> &v=rcbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> rcbrt_z(const color_rgb<T> &c_)                                     {const vec3<T> &v=rcbrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_rgb<T> &c_)                          {return norm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_rgb<T> &c_)                         {return rnorm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_rgb<T> &c_)                       {return rnorm_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_rgb<T> &c_)                         {return norm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_rgb<T> &c_)                        {return rnorm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_rgb<T> &c_)                      {return rnorm2_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_rgb<T> &c_)                       {return norm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_rgb<T> &c_)                      {return rnorm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_rgb<T> &c_)                    {return rnorm_l1_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_rgb<T> exp(const color_rgb<T> &c_)                                         {const vec3<T> &v=exp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> exp2(const color_rgb<T> &c_)                                        {const vec3<T> &v=exp2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> ln(const color_rgb<T> &c_)                                          {const vec3<T> &v=ln(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> log2(const color_rgb<T> &c_)                                        {const vec3<T> &v=log2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> log10(const color_rgb<T> &c_)                                       {const vec3<T> &v=log10(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> pow(const color_rgb<T> &c_, typename math<T>::scalar_t p_)          {const vec3<T> &v=pow(reinterpret_cast<const vec3<T>&>(c_), p_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> unit(const color_rgb<T> &c_)                                        {const vec3<T> &v=unit(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> unit_z(const color_rgb<T> &c_)                                      {const vec3<T> &v=unit_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> cross(const color_rgb<T> &c0_, const color_rgb<T> &c1_)             {const vec3<T> &v=cross(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_rgb<T> &c0_, const color_rgb<T> &c1_) {return dot(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_rgb<T> &c_)                          {return dot1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_rgb<T> &c_)                                                       {neg(reinterpret_cast<vec3<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_rgb<T> &c_, Rand &rnd_)                            {rand_u(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_rgb<T> &c_, Rand &rnd_)                        {rand_real1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_rgb<T> &c_, Rand &rnd_)                       {rand_ureal1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_rgb<T> smoothstep(const color_rgb<T> &c_)                                  {const vec3<T> &v=smoothstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> smootherstep(const color_rgb<T> &c_)                                {const vec3<T> &v=smootherstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> lerp(const color_rgb<T> &c0_,
                                                   const color_rgb<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                {const vec3<T> &v=lerp(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), t_); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> reflect(const color_rgb<T> &c_, const color_rgb<T> &cn_)            {const vec3<T> &v=reflect(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE color_rgb<T> reflect_u(const color_rgb<T> &c_, const color_rgb<T> &cn_)          {const vec3<T> &v=reflect_u(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_rgb<T> shuffle(const color_rgb<T> &c_)                  {const vec3<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_rgb<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_rgb32 &cr_, const color_rgb<T> &c_)                           {pack_u1(reinterpret_cast<vec3_32u&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_rgb32 &cr_, const color_rgb<T> &c_)                           {pack_s1(reinterpret_cast<vec3_32s&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_rgb<T> &cr_, const color_rgb32 &c_)                         {unpack_u1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_rgb<T> &cr_, const color_rgb32 &c_)                         {unpack_s1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_rgba
//============================================================================
template<typename T>
color_rgba<T> color_rgba<T>::zero()
{
  return color_rgba<T>(0);
}
//----

template<typename T>
color_rgba<T> color_rgba<T>::one()
{
  return color_rgba<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_rgba<T>::color_rgba()
{
}
//----

template<typename T>
color_rgba<T>::color_rgba(T v_)
  :r(v_)
  ,g(v_)
  ,b(v_)
  ,a(1)
{
}
//----

template<typename T>
color_rgba<T>::color_rgba(T r_, T g_, T b_)
  :r(r_)
  ,g(g_)
  ,b(b_)
  ,a(1)
{
}
//----

template<typename T>
color_rgba<T>::color_rgba(T r_, T g_, T b_, T a_)
  :r(r_)
  ,g(g_)
  ,b(b_)
  ,a(a_)
{
}
//----

template<typename T>
color_rgba<T>::color_rgba(const color_rgb<T> &c_)
  :r(c_.r)
  ,g(c_.g)
  ,b(c_.b)
  ,a(1)
{
}
//----

template<typename T>
color_rgba<T>::color_rgba(const color_rgb<T> &c_, T a_)
  :r(c_.r)
  ,g(c_.g)
  ,b(c_.b)
  ,a(a_)
{
}
//----

template<typename T>
template<typename U>
color_rgba<T>::color_rgba(const color_rgba<U> &c_)
  :r(T(c_.r))
  ,g(T(c_.g))
  ,b(T(c_.b))
  ,a(T(c_.a))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_rgba<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_rgba<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_rgba<T>::set(T v_)
{
  r=v_;
  g=v_;
  b=v_;
}
//----

template<typename T>
void color_rgba<T>::set(T r_, T g_, T b_)
{
  r=r_;
  g=g_;
  b=b_;
}
//----

template<typename T>
void color_rgba<T>::set(T r_, T g_, T b_, T a_)
{
  r=r_;
  g=g_;
  b=b_;
  a=a_;
}
//----

template<typename T>
void color_rgba<T>::set(const color_rgb<T> &c_, T a_)
{
  r=c_.r;
  g=c_.g;
  b=c_.b;
  a=a_;
}
//----

template<typename T>
template<typename U>
void color_rgba<T>::set(const color_rgba<U> &c_)
{
  r=T(c_.r);
  g=T(c_.g);
  b=T(c_.b);
  a=T(c_.a);
}
//----------------------------------------------------------------------------

// vec4 interface for color_rgba
template<typename T> EGL_INLINE bool is_zero(const color_rgba<T> &c_)                                              {return is_zero(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_black(const color_rgba<T> &c_)                                             {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_rgba<T> &c_)                                               {return is_sat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_rgba<T> &c_)                                              {return is_ssat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_rgba<T> &c0_, const color_rgba<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)==reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_rgba<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_rgba<T> &c_)            {return s_==reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_rgba<T> &c0_, const color_rgba<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)!=reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_rgba<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_rgba<T> &c_)            {return s_!=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE color_rgba<T> operator<(const color_rgba<T> &c0_, const color_rgba<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator<(const color_rgba<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator<(typename math<T>::scalar_t s_, const color_rgba<T> &c_)    {const vec4<T> &v=s_<reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator>(const color_rgba<T> &c0_, const color_rgba<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator>(const color_rgba<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator>(typename math<T>::scalar_t s_, const color_rgba<T> &c_)    {const vec4<T> &v=s_>reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator<=(const color_rgba<T> &c0_, const color_rgba<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator<=(const color_rgba<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<=s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator<=(typename math<T>::scalar_t s_, const color_rgba<T> &c_)   {const vec4<T> &v=s_<=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator>=(const color_rgba<T> &c0_, const color_rgba<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator>=(const color_rgba<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>=s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator>=(typename math<T>::scalar_t s_, const color_rgba<T> &c_)   {const vec4<T> &v=s_>=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sel_eq(const color_rgba<T> &t0_, const color_rgba<T> &t1_,
                                                     const color_rgba<T> &c0_, const color_rgba<T> &c1_)           {const vec4<T> &v=sel_eq(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sel_lt(const color_rgba<T> &t0_, const color_rgba<T> &t1_,
                                                     const color_rgba<T> &c0_, const color_rgba<T> &c1_)           {const vec4<T> &v=sel_lt(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sel_le(const color_rgba<T> &t0_, const color_rgba<T> &t1_,
                                                     const color_rgba<T> &c0_, const color_rgba<T> &c1_)           {const vec4<T> &v=sel_le(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sel_eqz(const color_rgba<T> &t_,
                                                      const color_rgba<T> &c0_, const color_rgba<T> &c1_)          {const vec4<T> &v=sel_eqz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sel_ltz(const color_rgba<T> &t_,
                                                      const color_rgba<T> &c0_, const color_rgba<T> &c1_)          {const vec4<T> &v=sel_ltz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sel_lez(const color_rgba<T> &t_,
                                                      const color_rgba<T> &c0_, const color_rgba<T> &c1_)          {const vec4<T> &v=sel_lez(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_rgba<T> &cr_, const color_rgba<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)+=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_rgba<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_rgba<T> &cr_, const color_rgba<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)-=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_rgba<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_rgba<T> &cr_, const color_rgba<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)*=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_rgba<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_rgba<T> &cr_, const mat44<T> &m_)                            {reinterpret_cast<vec4<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator/=(color_rgba<T> &cr_, const color_rgba<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)/=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_rgba<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_rgba<T> operator+(const color_rgba<T> &c0_, const color_rgba<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)+reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator+(const color_rgba<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)+s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator+(typename math<T>::scalar_t s_, const color_rgba<T> &c_)    {const vec4<T> &v=s_+reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator-(const color_rgba<T> &c0_, const color_rgba<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)-reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator-(const color_rgba<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)-s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator-(typename math<T>::scalar_t s_, const color_rgba<T> &c_)    {const vec4<T> &v=s_-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator-(const color_rgba<T> &c_)                                   {const vec4<T> &v=-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator*(const color_rgba<T> &c0_, const color_rgba<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)*reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator*(const color_rgba<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator*(typename math<T>::scalar_t s_, const color_rgba<T> &c_)    {const vec4<T> &v=s_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator*(const color_rgba<T> &c_, const mat44<T> &m_)               {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*m_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator*(const mat44<T> &m_, const color_rgba<T> &c_)               {const vec4<T> &v=m_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator/(const color_rgba<T> &c0_, const color_rgba<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)/reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> operator/(const color_rgba<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)/s_; return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_rgba<T> &c_)                            {return min(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_rgba<T> min(const color_rgba<T> &c0_,
                                                  const color_rgba<T> &c1_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> min(const color_rgba<T> &c0_,
                                                  const color_rgba<T> &c1_,
                                                  const color_rgba<T> &c2_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> min(const color_rgba<T> &c0_,
                                                  const color_rgba<T> &c1_,
                                                  const color_rgba<T> &c2_,
                                                  const color_rgba<T> &c3_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> min(const color_rgba<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> min(typename math<T>::scalar_t s_, const color_rgba<T> &c_)          {const vec4<T> &v=min(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_rgba<T> &c_)                            {return max(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_rgba<T> max(const color_rgba<T> &c0_,
                                                  const color_rgba<T> &c1_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> max(const color_rgba<T> &c0_,
                                                  const color_rgba<T> &c1_,
                                                  const color_rgba<T> &c2_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> max(const color_rgba<T> &c0_,
                                                  const color_rgba<T> &c1_,
                                                  const color_rgba<T> &c2_,
                                                  const color_rgba<T> &c3_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> max(const color_rgba<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> max(typename math<T>::scalar_t s_, const color_rgba<T> &c_)          {const vec4<T> &v=max(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> mul(const color_rgba<T> &c0_, const color_rgba<T> &c1_)              {const vec4<T> &v=mul(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> madd(const color_rgba<T> &c0_,
                                                   const color_rgba<T> &c1_,
                                                   const color_rgba<T> &c2_)                                       {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> madd(const color_rgba<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, add_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> madd(const color_rgba<T> &c_,
                                                   const color_rgba<T> &mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(mul_), add_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> madd(const color_rgba<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_rgba<T> &add_)                                      {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, reinterpret_cast<const vec4<T>&>(add_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> rcp(const color_rgba<T> &c_)                                         {const vec4<T> &v=rcp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> rcp_z(const color_rgba<T> &c_)                                       {const vec4<T> &v=rcp_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> floor(const color_rgba<T> &c_)                                       {const vec4<T> &v=floor(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> ceil(const color_rgba<T> &c_)                                        {const vec4<T> &v=ceil(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> trunc(const color_rgba<T> &c_)                                       {const vec4<T> &v=trunc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> frc(const color_rgba<T> &c_)                                         {const vec4<T> &v=frc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> mod(const color_rgba<T> &c_, T div_)                                 {const vec4<T> &v=mod(reinterpret_cast<const vec4<T>&>(c_), div_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> cycle(const color_rgba<T> &c_, T cycle_)                             {const vec4<T> &v=cycle(reinterpret_cast<const vec4<T>&>(c_), cycle_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> cycle1(const color_rgba<T> &c_)                                      {const vec4<T> &v=cycle1(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sat(const color_rgba<T> &c_)                                         {const vec4<T> &v=sat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> ssat(const color_rgba<T> &c_)                                        {const vec4<T> &v=ssat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> clamp(const color_rgba<T> &c_,
                                                    const color_rgba<T> &min_,
                                                    const color_rgba<T> &max_)                                     {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(min_), reinterpret_cast<const vec4<T>&>(max_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> clamp(const color_rgba<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                               {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), min_, max_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> abs(const color_rgba<T> &c_)                                         {const vec4<T> &v=abs(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sgn(const color_rgba<T> &c_)                                         {const vec4<T> &v=sgn(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sgn_zp(const color_rgba<T> &c_)                                      {const vec4<T> &v=sgn_zp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sqr(const color_rgba<T> &c_)                                         {const vec4<T> &v=sqr(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> cubic(const color_rgba<T> &c_)                                       {const vec4<T> &v=cubic(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sqrt(const color_rgba<T> &c_)                                        {const vec4<T> &v=sqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> sqrt_z(const color_rgba<T> &c_)                                      {const vec4<T> &v=sqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> cbrt(const color_rgba<T> &c_)                                        {const vec4<T> &v=cbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> rsqrt(const color_rgba<T> &c_)                                       {const vec4<T> &v=rsqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> rsqrt_z(const color_rgba<T> &c_)                                     {const vec4<T> &v=rsqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> rcbrt(const color_rgba<T> &c_)                                       {const vec4<T> &v=rcbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> rcbrt_z(const color_rgba<T> &c_)                                     {const vec4<T> &v=rcbrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_rgba<T> &c_)                           {return norm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_rgba<T> &c_)                          {return rnorm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_rgba<T> &c_)                        {return rnorm_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_rgba<T> &c_)                          {return norm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_rgba<T> &c_)                         {return rnorm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_rgba<T> &c_)                       {return rnorm2_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_rgba<T> &c_)                        {return norm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_rgba<T> &c_)                       {return rnorm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_rgba<T> &c_)                     {return rnorm_l1_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_rgba<T> exp(const color_rgba<T> &c_)                                         {const vec4<T> &v=exp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> exp2(const color_rgba<T> &c_)                                        {const vec4<T> &v=exp2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> ln(const color_rgba<T> &c_)                                          {const vec4<T> &v=ln(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> log2(const color_rgba<T> &c_)                                        {const vec4<T> &v=log2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> log10(const color_rgba<T> &c_)                                       {const vec4<T> &v=log10(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> pow(const color_rgba<T> &c_, typename math<T>::scalar_t p_)          {const vec4<T> &v=pow(reinterpret_cast<const vec4<T>&>(c_), p_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> unit(const color_rgba<T> &c_)                                        {const vec4<T> &v=unit(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> unit_z(const color_rgba<T> &c_)                                      {const vec4<T> &v=unit_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_rgba<T> &c0_, const color_rgba<T> &c1_) {return dot(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_rgba<T> &c_)                           {return dot1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_rgba<T> &c_)                                                        {neg(reinterpret_cast<vec4<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_rgba<T> &c_, Rand &rnd_)                             {rand_u(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_rgba<T> &c_, Rand &rnd_)                         {rand_real1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_rgba<T> &c_, Rand &rnd_)                        {rand_ureal1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_rgba<T> smoothstep(const color_rgba<T> &c_)                                  {const vec4<T> &v=smoothstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> smootherstep(const color_rgba<T> &c_)                                {const vec4<T> &v=smootherstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> lerp(const color_rgba<T> &c0_,
                                                   const color_rgba<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                  {const vec4<T> &v=lerp(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), t_); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> reflect(const color_rgba<T> &c_, const color_rgba<T> &cn_)           {const vec4<T> &v=reflect(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE color_rgba<T> reflect_u(const color_rgba<T> &c_, const color_rgba<T> &cn_)         {const vec4<T> &v=reflect_u(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_rgba<T> shuffle(const color_rgba<T> &c_)                  {const vec4<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_rgba<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_rgba32 &cr_, const color_rgba<T> &c_)                           {pack_u1(reinterpret_cast<vec4_32u&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_rgba32 &cr_, const color_rgba<T> &c_)                           {pack_s1(reinterpret_cast<vec4_32s&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_rgba<T> &cr_, const color_rgba32 &c_)                         {unpack_u1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_rgba<T> &cr_, const color_rgba32 &c_)                         {unpack_s1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_xyz
//============================================================================
template<typename T>
color_xyz<T> color_xyz<T>::zero()
{
  return color_xyz<T>(0);
}
//----

template<typename T>
color_xyz<T> color_xyz<T>::one()
{
  return color_xyz<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_xyz<T>::color_xyz()
{
}
//----

template<typename T>
color_xyz<T>::color_xyz(T v_)
  :x(v_)
  ,y(v_)
  ,z(v_)
{
}
//----

template<typename T>
color_xyz<T>::color_xyz(T x_, T y_, T z_)
  :x(x_)
  ,y(y_)
  ,z(z_)
{
}
//----

template<typename T>
color_xyz<T>::color_xyz(const color_xyza<T> &c_)
  :x(c_.x)
  ,y(c_.y)
  ,z(c_.z)
{
}
//----

template<typename T>
template<typename U>
color_xyz<T>::color_xyz(const color_xyz<U> &c_)
  :x(T(c_.x))
  ,y(T(c_.y))
  ,z(T(c_.z))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_xyz<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_xyz<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_xyz<T>::set(T v_)
{
  x=v_;
  y=v_;
  z=v_;
}
//----

template<typename T>
void color_xyz<T>::set(T x_, T y_, T z_)
{
  x=x_;
  y=y_;
  z=z_;
}
//----

template<typename T>
template<typename U>
void color_xyz<T>::set(const color_xyz<U> &c_)
{
  x=T(c_.x);
  y=T(c_.y);
  z=T(c_.z);
}
//----------------------------------------------------------------------------

// vec3 interface for color_xyz
template<typename T> EGL_INLINE bool is_zero(const color_xyz<T> &c_)                                             {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_black(const color_xyz<T> &c_)                                            {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_xyz<T> &c_)                                              {return is_sat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_xyz<T> &c_)                                             {return is_ssat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_xyz<T> &c0_, const color_xyz<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)==reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_xyz<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_xyz<T> &c_)           {return s_==reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_xyz<T> &c0_, const color_xyz<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)!=reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_xyz<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_xyz<T> &c_)           {return s_!=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE color_xyz<T> operator<(const color_xyz<T> &c0_, const color_xyz<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator<(const color_xyz<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator<(typename math<T>::scalar_t s_, const color_xyz<T> &c_)    {const vec3<T> &v=s_<reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator>(const color_xyz<T> &c0_, const color_xyz<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator>(const color_xyz<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator>(typename math<T>::scalar_t s_, const color_xyz<T> &c_)    {const vec3<T> &v=s_>reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator<=(const color_xyz<T> &c0_, const color_xyz<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator<=(const color_xyz<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<=s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator<=(typename math<T>::scalar_t s_, const color_xyz<T> &c_)   {const vec3<T> &v=s_<=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator>=(const color_xyz<T> &c0_, const color_xyz<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator>=(const color_xyz<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>=s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator>=(typename math<T>::scalar_t s_, const color_xyz<T> &c_)   {const vec3<T> &v=s_>=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sel_eq(const color_xyz<T> &t0_, const color_xyz<T> &t1_,
                                                     const color_xyz<T> &c0_, const color_xyz<T> &c1_)           {const vec3<T> &v=sel_eq(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sel_lt(const color_xyz<T> &t0_, const color_xyz<T> &t1_,
                                                     const color_xyz<T> &c0_, const color_xyz<T> &c1_)           {const vec3<T> &v=sel_lt(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sel_le(const color_xyz<T> &t0_, const color_xyz<T> &t1_,
                                                     const color_xyz<T> &c0_, const color_xyz<T> &c1_)           {const vec3<T> &v=sel_le(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sel_eqz(const color_xyz<T> &t_,
                                                      const color_xyz<T> &c0_, const color_xyz<T> &c1_)          {const vec3<T> &v=sel_eqz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sel_ltz(const color_xyz<T> &t_,
                                                      const color_xyz<T> &c0_, const color_xyz<T> &c1_)          {const vec3<T> &v=sel_ltz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sel_lez(const color_xyz<T> &t_,
                                                      const color_xyz<T> &c0_, const color_xyz<T> &c1_)          {const vec3<T> &v=sel_lez(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_xyz<T> &cr_, const color_xyz<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)+=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_xyz<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_xyz<T> &cr_, const color_xyz<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)-=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_xyz<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_xyz<T> &cr_, const color_xyz<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)*=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_xyz<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_xyz<T> &cr_, const mat33<T> &m_)                           {reinterpret_cast<vec3<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator*=(color_xyz<T> &cr_, const mat44<T> &m_)                           {reinterpret_cast<vec3<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator*=(color_xyz<T> &cr_, const quat<T> &q_)                            {reinterpret_cast<vec3<T>&>(cr_)*=q_;}
template<typename T> EGL_INLINE void operator/=(color_xyz<T> &cr_, const color_xyz<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)/=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_xyz<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_xyz<T> operator+(const color_xyz<T> &c0_, const color_xyz<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)+reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator+(const color_xyz<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)+s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator+(typename math<T>::scalar_t s_, const color_xyz<T> &c_)    {const vec3<T> &v=s_+reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator-(const color_xyz<T> &c0_, const color_xyz<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)-reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator-(const color_xyz<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)-s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator-(typename math<T>::scalar_t s_, const color_xyz<T> &c_)    {const vec3<T> &v=s_-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator-(const color_xyz<T> &c_)                                   {const vec3<T> &v=-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const color_xyz<T> &c0_, const color_xyz<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)*reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const color_xyz<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(typename math<T>::scalar_t s_, const color_xyz<T> &c_)    {const vec3<T> &v=s_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const color_xyz<T> &c_, const mat33<T> &m_)               {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*m_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const mat33<T> &m_, const color_xyz<T> &c_)               {const vec3<T> &v=m_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const color_xyz<T> &c_, const mat44<T> &m_)               {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*m_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const mat44<T> &m_, const color_xyz<T> &c_)               {const vec3<T> &v=m_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator*(const color_xyz<T> &c_, const quat<T> &q_)                {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*q_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator/(const color_xyz<T> &c0_, const color_xyz<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)/reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> operator/(const color_xyz<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)/s_; return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_xyz<T> &c_)                           {return min(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_xyz<T> min(const color_xyz<T> &c0_,
                                                  const color_xyz<T> &c1_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> min(const color_xyz<T> &c0_,
                                                  const color_xyz<T> &c1_,
                                                  const color_xyz<T> &c2_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> min(const color_xyz<T> &c0_,
                                                  const color_xyz<T> &c1_,
                                                  const color_xyz<T> &c2_,
                                                  const color_xyz<T> &c3_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> min(const color_xyz<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> min(typename math<T>::scalar_t s_, const color_xyz<T> &c_)          {const vec3<T> &v=min(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_xyz<T> &c_)                           {return max(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_xyz<T> max(const color_xyz<T> &c0_,
                                                  const color_xyz<T> &c1_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> max(const color_xyz<T> &c0_,
                                                  const color_xyz<T> &c1_,
                                                  const color_xyz<T> &c2_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> max(const color_xyz<T> &c0_,
                                                  const color_xyz<T> &c1_,
                                                  const color_xyz<T> &c2_,
                                                  const color_xyz<T> &c3_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> max(const color_xyz<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> max(typename math<T>::scalar_t s_, const color_xyz<T> &c_)          {const vec3<T> &v=max(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> mul(const color_xyz<T> &c0_, const color_xyz<T> &c1_)               {const vec3<T> &v=mul(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> madd(const color_xyz<T> &c0_,
                                                   const color_xyz<T> &c1_,
                                                   const color_xyz<T> &c2_)                                      {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> madd(const color_xyz<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, add_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> madd(const color_xyz<T> &c_,
                                                   const color_xyz<T> &mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(mul_), add_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> madd(const color_xyz<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_xyz<T> &add_)                                     {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, reinterpret_cast<const vec3<T>&>(add_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> rcp(const color_xyz<T> &c_)                                         {const vec3<T> &v=rcp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> rcp_z(const color_xyz<T> &c_)                                       {const vec3<T> &v=rcp_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> floor(const color_xyz<T> &c_)                                       {const vec3<T> &v=floor(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> ceil(const color_xyz<T> &c_)                                        {const vec3<T> &v=ceil(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> trunc(const color_xyz<T> &c_)                                       {const vec3<T> &v=trunc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> frc(const color_xyz<T> &c_)                                         {const vec3<T> &v=frc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> mod(const color_xyz<T> &c_, T div_)                                 {const vec3<T> &v=mod(reinterpret_cast<const vec3<T>&>(c_), div_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> cycle(const color_xyz<T> &c_, T cycle_)                             {const vec3<T> &v=cycle(reinterpret_cast<const vec3<T>&>(c_), cycle_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> cycle1(const color_xyz<T> &c_)                                      {const vec3<T> &v=cycle1(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sat(const color_xyz<T> &c_)                                         {const vec3<T> &v=sat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> ssat(const color_xyz<T> &c_)                                        {const vec3<T> &v=ssat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> clamp(const color_xyz<T> &c_,
                                                    const color_xyz<T> &min_,
                                                    const color_xyz<T> &max_)                                    {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(min_), reinterpret_cast<const vec3<T>&>(max_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> clamp(const color_xyz<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                             {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), min_, max_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> abs(const color_xyz<T> &c_)                                         {const vec3<T> &v=abs(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sgn(const color_xyz<T> &c_)                                         {const vec3<T> &v=sgn(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sgn_zp(const color_xyz<T> &c_)                                      {const vec3<T> &v=sgn_zp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sqr(const color_xyz<T> &c_)                                         {const vec3<T> &v=sqr(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> cubic(const color_xyz<T> &c_)                                       {const vec3<T> &v=cubic(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sqrt(const color_xyz<T> &c_)                                        {const vec3<T> &v=sqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> sqrt_z(const color_xyz<T> &c_)                                      {const vec3<T> &v=sqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> cbrt(const color_xyz<T> &c_)                                        {const vec3<T> &v=cbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> rsqrt(const color_xyz<T> &c_)                                       {const vec3<T> &v=rsqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> rsqrt_z(const color_xyz<T> &c_)                                     {const vec3<T> &v=rsqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> rcbrt(const color_xyz<T> &c_)                                       {const vec3<T> &v=rcbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> rcbrt_z(const color_xyz<T> &c_)                                     {const vec3<T> &v=rcbrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_xyz<T> &c_)                          {return norm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_xyz<T> &c_)                         {return rnorm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_xyz<T> &c_)                       {return rnorm_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_xyz<T> &c_)                         {return norm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_xyz<T> &c_)                        {return rnorm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_xyz<T> &c_)                      {return rnorm2_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_xyz<T> &c_)                       {return norm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_xyz<T> &c_)                      {return rnorm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_xyz<T> &c_)                    {return rnorm_l1_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_xyz<T> exp(const color_xyz<T> &c_)                                         {const vec3<T> &v=exp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> exp2(const color_xyz<T> &c_)                                        {const vec3<T> &v=exp2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> ln(const color_xyz<T> &c_)                                          {const vec3<T> &v=ln(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> log2(const color_xyz<T> &c_)                                        {const vec3<T> &v=log2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> log10(const color_xyz<T> &c_)                                       {const vec3<T> &v=log10(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> pow(const color_xyz<T> &c_, typename math<T>::scalar_t p_)          {const vec3<T> &v=pow(reinterpret_cast<const vec3<T>&>(c_), p_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> unit(const color_xyz<T> &c_)                                        {const vec3<T> &v=unit(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> unit_z(const color_xyz<T> &c_)                                      {const vec3<T> &v=unit_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> cross(const color_xyz<T> &c0_, const color_xyz<T> &c1_)             {const vec3<T> &v=cross(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_xyz<T> &c0_, const color_xyz<T> &c1_) {return dot(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_xyz<T> &c_)                          {return dot1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_xyz<T> &c_)                                                       {neg(reinterpret_cast<vec3<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_xyz<T> &c_, Rand &rnd_)                            {rand_u(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_xyz<T> &c_, Rand &rnd_)                        {rand_real1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_xyz<T> &c_, Rand &rnd_)                       {rand_ureal1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_xyz<T> smoothstep(const color_xyz<T> &c_)                                  {const vec3<T> &v=smoothstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> smootherstep(const color_xyz<T> &c_)                                {const vec3<T> &v=smootherstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> lerp(const color_xyz<T> &c0_,
                                                   const color_xyz<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                {const vec3<T> &v=lerp(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), t_); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> reflect(const color_xyz<T> &c_, const color_xyz<T> &cn_)            {const vec3<T> &v=reflect(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE color_xyz<T> reflect_u(const color_xyz<T> &c_, const color_xyz<T> &cn_)          {const vec3<T> &v=reflect_u(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_xyz<T> shuffle(const color_xyz<T> &c_)                  {const vec3<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_xyz<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_xyz32 &cr_, const color_xyz<T> &c_)                           {pack_u1(reinterpret_cast<vec3_32u&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_xyz32 &cr_, const color_xyz<T> &c_)                           {pack_s1(reinterpret_cast<vec3_32s&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_xyz<T> &cr_, const color_xyz32 &c_)                         {unpack_u1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_xyz<T> &cr_, const color_xyz32 &c_)                         {unpack_s1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_xyza
//============================================================================
template<typename T>
color_xyza<T> color_xyza<T>::zero()
{
  return color_xyza<T>(0);
}
//----

template<typename T>
color_xyza<T> color_xyza<T>::one()
{
  return color_xyza<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_xyza<T>::color_xyza()
{
}
//----

template<typename T>
color_xyza<T>::color_xyza(T v_)
  :x(v_)
  ,y(v_)
  ,z(v_)
  ,a(1)
{
}
//----

template<typename T>
color_xyza<T>::color_xyza(T x_, T y_, T z_)
  :x(x_)
  ,y(y_)
  ,z(z_)
  ,a(1)
{
}
//----

template<typename T>
color_xyza<T>::color_xyza(T x_, T y_, T z_, T a_)
  :x(x_)
  ,y(y_)
  ,z(z_)
  ,a(a_)
{
}
//----

template<typename T>
color_xyza<T>::color_xyza(const color_xyz<T> &c_)
  :x(c_.x)
  ,y(c_.y)
  ,z(c_.z)
  ,a(1)
{
}
//----

template<typename T>
color_xyza<T>::color_xyza(const color_xyz<T> &c_, T a_)
  :x(c_.x)
  ,y(c_.y)
  ,z(c_.z)
  ,a(a_)
{
}
//----

template<typename T>
template<typename U>
color_xyza<T>::color_xyza(const color_xyza<U> &c_)
  :x(T(c_.x))
  ,y(T(c_.y))
  ,z(T(c_.z))
  ,a(T(c_.a))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_xyza<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_xyza<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_xyza<T>::set(T v_)
{
  x=v_;
  y=v_;
  z=v_;
}
//----

template<typename T>
void color_xyza<T>::set(T x_, T y_, T z_)
{
  x=x_;
  y=y_;
  z=z_;
}
//----

template<typename T>
void color_xyza<T>::set(T x_, T y_, T z_, T a_)
{
  x=x_;
  y=y_;
  z=z_;
  a=a_;
}
//----

template<typename T>
void color_xyza<T>::set(const color_xyz<T> &c_, T a_)
{
  x=c_.x;
  y=c_.y;
  z=c_.z;
  a=a_;
}
//----

template<typename T>
template<typename U>
void color_xyza<T>::set(const color_xyza<U> &c_)
{
  x=T(c_.x);
  y=T(c_.y);
  z=T(c_.z);
  a=T(c_.a);
}
//----------------------------------------------------------------------------

// vec4 interface for color_xyza
template<typename T> EGL_INLINE bool is_zero(const color_xyza<T> &c_)                                              {return is_zero(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_black(const color_xyza<T> &c_)                                             {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_xyza<T> &c_)                                               {return is_sat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_xyza<T> &c_)                                              {return is_ssat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_xyza<T> &c0_, const color_xyza<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)==reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_xyza<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_xyza<T> &c_)            {return s_==reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_xyza<T> &c0_, const color_xyza<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)!=reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_xyza<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_xyza<T> &c_)            {return s_!=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE color_xyza<T> operator<(const color_xyza<T> &c0_, const color_xyza<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator<(const color_xyza<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator<(typename math<T>::scalar_t s_, const color_xyza<T> &c_)    {const vec4<T> &v=s_<reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator>(const color_xyza<T> &c0_, const color_xyza<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator>(const color_xyza<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator>(typename math<T>::scalar_t s_, const color_xyza<T> &c_)    {const vec4<T> &v=s_>reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator<=(const color_xyza<T> &c0_, const color_xyza<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator<=(const color_xyza<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<=s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator<=(typename math<T>::scalar_t s_, const color_xyza<T> &c_)   {const vec4<T> &v=s_<=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator>=(const color_xyza<T> &c0_, const color_xyza<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator>=(const color_xyza<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>=s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator>=(typename math<T>::scalar_t s_, const color_xyza<T> &c_)   {const vec4<T> &v=s_>=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sel_eq(const color_xyza<T> &t0_, const color_xyza<T> &t1_,
                                                     const color_xyza<T> &c0_, const color_xyza<T> &c1_)           {const vec4<T> &v=sel_eq(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sel_lt(const color_xyza<T> &t0_, const color_xyza<T> &t1_,
                                                     const color_xyza<T> &c0_, const color_xyza<T> &c1_)           {const vec4<T> &v=sel_lt(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sel_le(const color_xyza<T> &t0_, const color_xyza<T> &t1_,
                                                     const color_xyza<T> &c0_, const color_xyza<T> &c1_)           {const vec4<T> &v=sel_le(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sel_eqz(const color_xyza<T> &t_,
                                                      const color_xyza<T> &c0_, const color_xyza<T> &c1_)          {const vec4<T> &v=sel_eqz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sel_ltz(const color_xyza<T> &t_,
                                                      const color_xyza<T> &c0_, const color_xyza<T> &c1_)          {const vec4<T> &v=sel_ltz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sel_lez(const color_xyza<T> &t_,
                                                      const color_xyza<T> &c0_, const color_xyza<T> &c1_)          {const vec4<T> &v=sel_lez(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_xyza<T> &cr_, const color_xyza<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)+=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_xyza<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_xyza<T> &cr_, const color_xyza<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)-=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_xyza<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_xyza<T> &cr_, const color_xyza<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)*=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_xyza<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_xyza<T> &cr_, const mat44<T> &m_)                            {reinterpret_cast<vec4<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator/=(color_xyza<T> &cr_, const color_xyza<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)/=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_xyza<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_xyza<T> operator+(const color_xyza<T> &c0_, const color_xyza<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)+reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator+(const color_xyza<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)+s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator+(typename math<T>::scalar_t s_, const color_xyza<T> &c_)    {const vec4<T> &v=s_+reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator-(const color_xyza<T> &c0_, const color_xyza<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)-reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator-(const color_xyza<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)-s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator-(typename math<T>::scalar_t s_, const color_xyza<T> &c_)    {const vec4<T> &v=s_-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator-(const color_xyza<T> &c_)                                   {const vec4<T> &v=-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator*(const color_xyza<T> &c0_, const color_xyza<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)*reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator*(const color_xyza<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator*(typename math<T>::scalar_t s_, const color_xyza<T> &c_)    {const vec4<T> &v=s_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator*(const color_xyza<T> &c_, const mat44<T> &m_)               {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*m_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator*(const mat44<T> &m_, const color_xyza<T> &c_)               {const vec4<T> &v=m_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator/(const color_xyza<T> &c0_, const color_xyza<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)/reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> operator/(const color_xyza<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)/s_; return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_xyza<T> &c_)                            {return min(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_xyza<T> min(const color_xyza<T> &c0_,
                                                  const color_xyza<T> &c1_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> min(const color_xyza<T> &c0_,
                                                  const color_xyza<T> &c1_,
                                                  const color_xyza<T> &c2_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> min(const color_xyza<T> &c0_,
                                                  const color_xyza<T> &c1_,
                                                  const color_xyza<T> &c2_,
                                                  const color_xyza<T> &c3_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> min(const color_xyza<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> min(typename math<T>::scalar_t s_, const color_xyza<T> &c_)          {const vec4<T> &v=min(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_xyza<T> &c_)                            {return max(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_xyza<T> max(const color_xyza<T> &c0_,
                                                  const color_xyza<T> &c1_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> max(const color_xyza<T> &c0_,
                                                  const color_xyza<T> &c1_,
                                                  const color_xyza<T> &c2_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> max(const color_xyza<T> &c0_,
                                                  const color_xyza<T> &c1_,
                                                  const color_xyza<T> &c2_,
                                                  const color_xyza<T> &c3_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> max(const color_xyza<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> max(typename math<T>::scalar_t s_, const color_xyza<T> &c_)          {const vec4<T> &v=max(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> mul(const color_xyza<T> &c0_, const color_xyza<T> &c1_)              {const vec4<T> &v=mul(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> madd(const color_xyza<T> &c0_,
                                                   const color_xyza<T> &c1_,
                                                   const color_xyza<T> &c2_)                                       {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> madd(const color_xyza<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, add_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> madd(const color_xyza<T> &c_,
                                                   const color_xyza<T> &mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(mul_), add_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> madd(const color_xyza<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_xyza<T> &add_)                                      {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, reinterpret_cast<const vec4<T>&>(add_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> rcp(const color_xyza<T> &c_)                                         {const vec4<T> &v=rcp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> rcp_z(const color_xyza<T> &c_)                                       {const vec4<T> &v=rcp_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> floor(const color_xyza<T> &c_)                                       {const vec4<T> &v=floor(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> ceil(const color_xyza<T> &c_)                                        {const vec4<T> &v=ceil(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> trunc(const color_xyza<T> &c_)                                       {const vec4<T> &v=trunc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> frc(const color_xyza<T> &c_)                                         {const vec4<T> &v=frc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> mod(const color_xyza<T> &c_, T div_)                                 {const vec4<T> &v=mod(reinterpret_cast<const vec4<T>&>(c_), div_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> cycle(const color_xyza<T> &c_, T cycle_)                             {const vec4<T> &v=cycle(reinterpret_cast<const vec4<T>&>(c_), cycle_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> cycle1(const color_xyza<T> &c_)                                      {const vec4<T> &v=cycle1(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sat(const color_xyza<T> &c_)                                         {const vec4<T> &v=sat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> ssat(const color_xyza<T> &c_)                                        {const vec4<T> &v=ssat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> clamp(const color_xyza<T> &c_,
                                                    const color_xyza<T> &min_,
                                                    const color_xyza<T> &max_)                                     {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(min_), reinterpret_cast<const vec4<T>&>(max_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> clamp(const color_xyza<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                               {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), min_, max_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> abs(const color_xyza<T> &c_)                                         {const vec4<T> &v=abs(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sgn(const color_xyza<T> &c_)                                         {const vec4<T> &v=sgn(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sgn_zp(const color_xyza<T> &c_)                                      {const vec4<T> &v=sgn_zp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sqr(const color_xyza<T> &c_)                                         {const vec4<T> &v=sqr(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> cubic(const color_xyza<T> &c_)                                       {const vec4<T> &v=cubic(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sqrt(const color_xyza<T> &c_)                                        {const vec4<T> &v=sqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> sqrt_z(const color_xyza<T> &c_)                                      {const vec4<T> &v=sqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> cbrt(const color_xyza<T> &c_)                                        {const vec4<T> &v=cbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> rsqrt(const color_xyza<T> &c_)                                       {const vec4<T> &v=rsqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> rsqrt_z(const color_xyza<T> &c_)                                     {const vec4<T> &v=rsqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> rcbrt(const color_xyza<T> &c_)                                       {const vec4<T> &v=rcbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> rcbrt_z(const color_xyza<T> &c_)                                     {const vec4<T> &v=rcbrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_xyza<T> &c_)                           {return norm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_xyza<T> &c_)                          {return rnorm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_xyza<T> &c_)                        {return rnorm_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_xyza<T> &c_)                          {return norm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_xyza<T> &c_)                         {return rnorm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_xyza<T> &c_)                       {return rnorm2_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_xyza<T> &c_)                        {return norm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_xyza<T> &c_)                       {return rnorm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_xyza<T> &c_)                     {return rnorm_l1_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_xyza<T> exp(const color_xyza<T> &c_)                                         {const vec4<T> &v=exp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> exp2(const color_xyza<T> &c_)                                        {const vec4<T> &v=exp2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> ln(const color_xyza<T> &c_)                                          {const vec4<T> &v=ln(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> log2(const color_xyza<T> &c_)                                        {const vec4<T> &v=log2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> log10(const color_xyza<T> &c_)                                       {const vec4<T> &v=log10(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> pow(const color_xyza<T> &c_, typename math<T>::scalar_t p_)          {const vec4<T> &v=pow(reinterpret_cast<const vec4<T>&>(c_), p_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> unit(const color_xyza<T> &c_)                                        {const vec4<T> &v=unit(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> unit_z(const color_xyza<T> &c_)                                      {const vec4<T> &v=unit_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_xyza<T> &c0_, const color_xyza<T> &c1_) {return dot(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_xyza<T> &c_)                           {return dot1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_xyza<T> &c_)                                                        {neg(reinterpret_cast<vec4<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_xyza<T> &c_, Rand &rnd_)                             {rand_u(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_xyza<T> &c_, Rand &rnd_)                         {rand_real1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_xyza<T> &c_, Rand &rnd_)                        {rand_ureal1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_xyza<T> smoothstep(const color_xyza<T> &c_)                                  {const vec4<T> &v=smoothstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> smootherstep(const color_xyza<T> &c_)                                {const vec4<T> &v=smootherstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> lerp(const color_xyza<T> &c0_,
                                                   const color_xyza<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                  {const vec4<T> &v=lerp(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), t_); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> reflect(const color_xyza<T> &c_, const color_xyza<T> &cn_)           {const vec4<T> &v=reflect(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE color_xyza<T> reflect_u(const color_xyza<T> &c_, const color_xyza<T> &cn_)         {const vec4<T> &v=reflect_u(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_xyza<T> shuffle(const color_xyza<T> &c_)                  {const vec4<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_xyza<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_xyza32 &cr_, const color_xyza<T> &c_)                           {pack_u1(reinterpret_cast<vec4_32u&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_xyza32 &cr_, const color_xyza<T> &c_)                           {pack_s1(reinterpret_cast<vec4_32s&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_xyza<T> &cr_, const color_xyza32 &c_)                         {unpack_u1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_xyza<T> &cr_, const color_xyza32 &c_)                         {unpack_s1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_yiq
//============================================================================
template<typename T>
color_yiq<T> color_yiq<T>::zero()
{
  return color_yiq<T>(0);
}
//----

template<typename T>
color_yiq<T> color_yiq<T>::one()
{
  return color_yiq<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_yiq<T>::color_yiq()
{
}
//----

template<typename T>
color_yiq<T>::color_yiq(T y_, T i_, T q_)
  :y(y_)
  ,i(i_)
  ,q(q_)
{
}
//----

template<typename T>
color_yiq<T>::color_yiq(const color_yiqa<T> &c_)
  :y(c_.y)
  ,i(c_.i)
  ,q(c_.q)
{
}
//----

template<typename T>
template<typename U>
color_yiq<T>::color_yiq(const color_yiq<U> &c_)
  :y(T(c_.y))
  ,i(T(c_.i))
  ,q(T(c_.q))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_yiq<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_yiq<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_yiq<T>::set(T y_, T i_, T q_)
{
  y=y_;
  i=i_;
  q=q_;
}
//----

template<typename T>
template<typename U>
void color_yiq<T>::set(const color_yiq<U> &c_)
{
  y=T(c_.y);
  i=T(c_.i);
  q=T(c_.q);
}
//----------------------------------------------------------------------------

// vec3 interface for color_yiq
template<typename T> EGL_INLINE bool is_zero(const color_yiq<T> &c_)                                             {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_yiq<T> &c_)                                              {return is_sat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_yiq<T> &c_)                                             {return is_ssat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_yiq<T> &c0_, const color_yiq<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)==reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_yiq<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_yiq<T> &c_)           {return s_==reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_yiq<T> &c0_, const color_yiq<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)!=reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_yiq<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_yiq<T> &c_)           {return s_!=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE color_yiq<T> operator<(const color_yiq<T> &c0_, const color_yiq<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator<(const color_yiq<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator<(typename math<T>::scalar_t s_, const color_yiq<T> &c_)    {const vec3<T> &v=s_<reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator>(const color_yiq<T> &c0_, const color_yiq<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator>(const color_yiq<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator>(typename math<T>::scalar_t s_, const color_yiq<T> &c_)    {const vec3<T> &v=s_>reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator<=(const color_yiq<T> &c0_, const color_yiq<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator<=(const color_yiq<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<=s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator<=(typename math<T>::scalar_t s_, const color_yiq<T> &c_)   {const vec3<T> &v=s_<=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator>=(const color_yiq<T> &c0_, const color_yiq<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator>=(const color_yiq<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>=s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator>=(typename math<T>::scalar_t s_, const color_yiq<T> &c_)   {const vec3<T> &v=s_>=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sel_eq(const color_yiq<T> &t0_, const color_yiq<T> &t1_,
                                                     const color_yiq<T> &c0_, const color_yiq<T> &c1_)           {const vec3<T> &v=sel_eq(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sel_lt(const color_yiq<T> &t0_, const color_yiq<T> &t1_,
                                                     const color_yiq<T> &c0_, const color_yiq<T> &c1_)           {const vec3<T> &v=sel_lt(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sel_le(const color_yiq<T> &t0_, const color_yiq<T> &t1_,
                                                     const color_yiq<T> &c0_, const color_yiq<T> &c1_)           {const vec3<T> &v=sel_le(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sel_eqz(const color_yiq<T> &t_,
                                                      const color_yiq<T> &c0_, const color_yiq<T> &c1_)          {const vec3<T> &v=sel_eqz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sel_ltz(const color_yiq<T> &t_,
                                                      const color_yiq<T> &c0_, const color_yiq<T> &c1_)          {const vec3<T> &v=sel_ltz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sel_lez(const color_yiq<T> &t_,
                                                      const color_yiq<T> &c0_, const color_yiq<T> &c1_)          {const vec3<T> &v=sel_lez(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_yiq<T> &cr_, const color_yiq<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)+=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_yiq<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_yiq<T> &cr_, const color_yiq<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)-=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_yiq<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_yiq<T> &cr_, const color_yiq<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)*=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_yiq<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_yiq<T> &cr_, const mat44<T> &m_)                           {reinterpret_cast<vec3<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator/=(color_yiq<T> &cr_, const color_yiq<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)/=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_yiq<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_yiq<T> operator+(const color_yiq<T> &c0_, const color_yiq<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)+reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator+(const color_yiq<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)+s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator+(typename math<T>::scalar_t s_, const color_yiq<T> &c_)    {const vec3<T> &v=s_+reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator-(const color_yiq<T> &c0_, const color_yiq<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)-reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator-(const color_yiq<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)-s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator-(typename math<T>::scalar_t s_, const color_yiq<T> &c_)    {const vec3<T> &v=s_-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator-(const color_yiq<T> &c_)                                   {const vec3<T> &v=-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator*(const color_yiq<T> &c0_, const color_yiq<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)*reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator*(const color_yiq<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator*(typename math<T>::scalar_t s_, const color_yiq<T> &c_)    {const vec3<T> &v=s_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator*(const color_yiq<T> &c_, const mat44<T> &m_)               {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*m_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator*(const mat44<T> &m_, const color_yiq<T> &c_)               {const vec3<T> &v=m_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator/(const color_yiq<T> &c0_, const color_yiq<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)/reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> operator/(const color_yiq<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)/s_; return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_yiq<T> &c_)                           {return min(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_yiq<T> min(const color_yiq<T> &c0_,
                                                  const color_yiq<T> &c1_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> min(const color_yiq<T> &c0_,
                                                  const color_yiq<T> &c1_,
                                                  const color_yiq<T> &c2_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> min(const color_yiq<T> &c0_,
                                                  const color_yiq<T> &c1_,
                                                  const color_yiq<T> &c2_,
                                                  const color_yiq<T> &c3_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> min(const color_yiq<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> min(typename math<T>::scalar_t s_, const color_yiq<T> &c_)          {const vec3<T> &v=min(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_yiq<T> &c_)                           {return max(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_yiq<T> max(const color_yiq<T> &c0_,
                                                  const color_yiq<T> &c1_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> max(const color_yiq<T> &c0_,
                                                  const color_yiq<T> &c1_,
                                                  const color_yiq<T> &c2_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> max(const color_yiq<T> &c0_,
                                                  const color_yiq<T> &c1_,
                                                  const color_yiq<T> &c2_,
                                                  const color_yiq<T> &c3_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> max(const color_yiq<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> max(typename math<T>::scalar_t s_, const color_yiq<T> &c_)          {const vec3<T> &v=max(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> mul(const color_yiq<T> &c0_, const color_yiq<T> &c1_)               {const vec3<T> &v=mul(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> madd(const color_yiq<T> &c0_,
                                                   const color_yiq<T> &c1_,
                                                   const color_yiq<T> &c2_)                                      {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> madd(const color_yiq<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, add_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> madd(const color_yiq<T> &c_,
                                                   const color_yiq<T> &mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(mul_), add_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> madd(const color_yiq<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_yiq<T> &add_)                                     {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, reinterpret_cast<const vec3<T>&>(add_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> rcp(const color_yiq<T> &c_)                                         {const vec3<T> &v=rcp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> rcp_z(const color_yiq<T> &c_)                                       {const vec3<T> &v=rcp_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> floor(const color_yiq<T> &c_)                                       {const vec3<T> &v=floor(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> ceil(const color_yiq<T> &c_)                                        {const vec3<T> &v=ceil(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> trunc(const color_yiq<T> &c_)                                       {const vec3<T> &v=trunc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> frc(const color_yiq<T> &c_)                                         {const vec3<T> &v=frc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> mod(const color_yiq<T> &c_, T div_)                                 {const vec3<T> &v=mod(reinterpret_cast<const vec3<T>&>(c_), div_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> cycle(const color_yiq<T> &c_, T cycle_)                             {const vec3<T> &v=cycle(reinterpret_cast<const vec3<T>&>(c_), cycle_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> cycle1(const color_yiq<T> &c_)                                      {const vec3<T> &v=cycle1(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sat(const color_yiq<T> &c_)                                         {const vec3<T> &v=sat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> ssat(const color_yiq<T> &c_)                                        {const vec3<T> &v=ssat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> clamp(const color_yiq<T> &c_,
                                                    const color_yiq<T> &min_,
                                                    const color_yiq<T> &max_)                                    {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(min_), reinterpret_cast<const vec3<T>&>(max_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> clamp(const color_yiq<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                             {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), min_, max_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> abs(const color_yiq<T> &c_)                                         {const vec3<T> &v=abs(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sgn(const color_yiq<T> &c_)                                         {const vec3<T> &v=sgn(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sgn_zp(const color_yiq<T> &c_)                                      {const vec3<T> &v=sgn_zp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sqr(const color_yiq<T> &c_)                                         {const vec3<T> &v=sqr(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> cubic(const color_yiq<T> &c_)                                       {const vec3<T> &v=cubic(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sqrt(const color_yiq<T> &c_)                                        {const vec3<T> &v=sqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> sqrt_z(const color_yiq<T> &c_)                                      {const vec3<T> &v=sqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> cbrt(const color_yiq<T> &c_)                                        {const vec3<T> &v=cbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> rsqrt(const color_yiq<T> &c_)                                       {const vec3<T> &v=rsqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> rsqrt_z(const color_yiq<T> &c_)                                     {const vec3<T> &v=rsqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> rcbrt(const color_yiq<T> &c_)                                       {const vec3<T> &v=rcbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> rcbrt_z(const color_yiq<T> &c_)                                     {const vec3<T> &v=rcbrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_yiq<T> &c_)                          {return norm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_yiq<T> &c_)                         {return rnorm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_yiq<T> &c_)                       {return rnorm_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_yiq<T> &c_)                         {return norm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_yiq<T> &c_)                        {return rnorm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_yiq<T> &c_)                      {return rnorm2_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_yiq<T> &c_)                       {return norm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_yiq<T> &c_)                      {return rnorm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_yiq<T> &c_)                    {return rnorm_l1_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_yiq<T> exp(const color_yiq<T> &c_)                                         {const vec3<T> &v=exp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> exp2(const color_yiq<T> &c_)                                        {const vec3<T> &v=exp2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> ln(const color_yiq<T> &c_)                                          {const vec3<T> &v=ln(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> log2(const color_yiq<T> &c_)                                        {const vec3<T> &v=log2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> log10(const color_yiq<T> &c_)                                       {const vec3<T> &v=log10(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> pow(const color_yiq<T> &c_, typename math<T>::scalar_t p_)          {const vec3<T> &v=pow(reinterpret_cast<const vec3<T>&>(c_), p_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> unit(const color_yiq<T> &c_)                                        {const vec3<T> &v=unit(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> unit_z(const color_yiq<T> &c_)                                      {const vec3<T> &v=unit_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> cross(const color_yiq<T> &c0_, const color_yiq<T> &c1_)             {const vec3<T> &v=cross(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_yiq<T> &c0_, const color_yiq<T> &c1_) {return dot(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_yiq<T> &c_)                          {return dot1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_yiq<T> &c_)                                                       {neg(reinterpret_cast<vec3<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_yiq<T> &c_, Rand &rnd_)                            {rand_u(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_yiq<T> &c_, Rand &rnd_)                        {rand_real1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_yiq<T> &c_, Rand &rnd_)                       {rand_ureal1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_yiq<T> smoothstep(const color_yiq<T> &c_)                                  {const vec3<T> &v=smoothstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> smootherstep(const color_yiq<T> &c_)                                {const vec3<T> &v=smootherstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> lerp(const color_yiq<T> &c0_,
                                                   const color_yiq<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                {const vec3<T> &v=lerp(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), t_); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> reflect(const color_yiq<T> &c_, const color_yiq<T> &cn_)            {const vec3<T> &v=reflect(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE color_yiq<T> reflect_u(const color_yiq<T> &c_, const color_yiq<T> &cn_)          {const vec3<T> &v=reflect_u(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_yiq<T> shuffle(const color_yiq<T> &c_)                  {const vec3<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_yiq<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_yiq32 &cr_, const color_yiq<T> &c_)                           {pack_u1(reinterpret_cast<vec3_32u&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_yiq32 &cr_, const color_yiq<T> &c_)                           {pack_s1(reinterpret_cast<vec3_32s&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_yiq<T> &cr_, const color_yiq32 &c_)                         {unpack_u1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_yiq<T> &cr_, const color_yiq32 &c_)                         {unpack_s1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_yiqa
//============================================================================
template<typename T>
color_yiqa<T> color_yiqa<T>::zero()
{
  return color_yiqa<T>(0);
}
//----

template<typename T>
color_yiqa<T> color_yiqa<T>::one()
{
  return color_yiqa<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_yiqa<T>::color_yiqa()
{
}
//----

template<typename T>
color_yiqa<T>::color_yiqa(T y_, T i_, T q_)
  :y(y_)
  ,i(i_)
  ,q(q_)
  ,a(1)
{
}
//----

template<typename T>
color_yiqa<T>::color_yiqa(T y_, T i_, T q_, T a_)
  :y(y_)
  ,i(i_)
  ,q(q_)
  ,a(a_)
{
}
//----

template<typename T>
color_yiqa<T>::color_yiqa(const color_yiq<T> &c_)
  :y(c_.y)
  ,i(c_.i)
  ,q(c_.q)
  ,a(1)
{
}
//----

template<typename T>
color_yiqa<T>::color_yiqa(const color_yiq<T> &c_, T a_)
  :y(c_.y)
  ,i(c_.i)
  ,q(c_.q)
  ,a(a_)
{
}
//----

template<typename T>
template<typename U>
color_yiqa<T>::color_yiqa(const color_yiqa<U> &c_)
  :y(T(c_.y))
  ,i(T(c_.i))
  ,q(T(c_.q))
  ,a(T(c_.a))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_yiqa<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_yiqa<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_yiqa<T>::set(T y_, T i_, T q_)
{
  y=y_;
  i=i_;
  q=q_;
}
//----

template<typename T>
void color_yiqa<T>::set(T y_, T i_, T q_, T a_)
{
  y=y_;
  i=i_;
  q=q_;
  a=a_;
}
//----

template<typename T>
void color_yiqa<T>::set(const color_yiq<T> &c_, T a_)
{
  y=c_.y;
  i=c_.i;
  q=c_.q;
  a=a_;
}
//----

template<typename T>
template<typename U>
void color_yiqa<T>::set(const color_yiqa<U> &c_)
{
  y=T(c_.y);
  i=T(c_.i);
  q=T(c_.q);
  a=T(c_.a);
}
//----------------------------------------------------------------------------

// vec4 interface for color_yiqa
template<typename T> EGL_INLINE bool is_zero(const color_yiqa<T> &c_)                                              {return is_zero(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_yiqa<T> &c_)                                               {return is_sat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_yiqa<T> &c_)                                              {return is_ssat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)==reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)            {return s_==reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)!=reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)            {return s_!=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE color_yiqa<T> operator<(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator<(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator<(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)    {const vec4<T> &v=s_<reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator>(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator>(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator>(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)    {const vec4<T> &v=s_>reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator<=(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator<=(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<=s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator<=(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)   {const vec4<T> &v=s_<=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator>=(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator>=(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>=s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator>=(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)   {const vec4<T> &v=s_>=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sel_eq(const color_yiqa<T> &t0_, const color_yiqa<T> &t1_,
                                                     const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)           {const vec4<T> &v=sel_eq(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sel_lt(const color_yiqa<T> &t0_, const color_yiqa<T> &t1_,
                                                     const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)           {const vec4<T> &v=sel_lt(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sel_le(const color_yiqa<T> &t0_, const color_yiqa<T> &t1_,
                                                     const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)           {const vec4<T> &v=sel_le(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sel_eqz(const color_yiqa<T> &t_,
                                                      const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)          {const vec4<T> &v=sel_eqz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sel_ltz(const color_yiqa<T> &t_,
                                                      const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)          {const vec4<T> &v=sel_ltz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sel_lez(const color_yiqa<T> &t_,
                                                      const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)          {const vec4<T> &v=sel_lez(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_yiqa<T> &cr_, const color_yiqa<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)+=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_yiqa<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_yiqa<T> &cr_, const color_yiqa<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)-=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_yiqa<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_yiqa<T> &cr_, const color_yiqa<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)*=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_yiqa<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_yiqa<T> &cr_, const mat44<T> &m_)                            {reinterpret_cast<vec4<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator/=(color_yiqa<T> &cr_, const color_yiqa<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)/=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_yiqa<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_yiqa<T> operator+(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)+reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator+(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)+s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator+(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)    {const vec4<T> &v=s_+reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator-(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)-reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator-(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)-s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator-(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)    {const vec4<T> &v=s_-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator-(const color_yiqa<T> &c_)                                   {const vec4<T> &v=-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator*(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)*reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator*(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator*(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)    {const vec4<T> &v=s_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator*(const color_yiqa<T> &c_, const mat44<T> &m_)               {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*m_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator*(const mat44<T> &m_, const color_yiqa<T> &c_)               {const vec4<T> &v=m_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator/(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)/reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> operator/(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)/s_; return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_yiqa<T> &c_)                            {return min(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_yiqa<T> min(const color_yiqa<T> &c0_,
                                                  const color_yiqa<T> &c1_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> min(const color_yiqa<T> &c0_,
                                                  const color_yiqa<T> &c1_,
                                                  const color_yiqa<T> &c2_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> min(const color_yiqa<T> &c0_,
                                                  const color_yiqa<T> &c1_,
                                                  const color_yiqa<T> &c2_,
                                                  const color_yiqa<T> &c3_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> min(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> min(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)          {const vec4<T> &v=min(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_yiqa<T> &c_)                            {return max(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_yiqa<T> max(const color_yiqa<T> &c0_,
                                                  const color_yiqa<T> &c1_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> max(const color_yiqa<T> &c0_,
                                                  const color_yiqa<T> &c1_,
                                                  const color_yiqa<T> &c2_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> max(const color_yiqa<T> &c0_,
                                                  const color_yiqa<T> &c1_,
                                                  const color_yiqa<T> &c2_,
                                                  const color_yiqa<T> &c3_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> max(const color_yiqa<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> max(typename math<T>::scalar_t s_, const color_yiqa<T> &c_)          {const vec4<T> &v=max(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> mul(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_)              {const vec4<T> &v=mul(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> madd(const color_yiqa<T> &c0_,
                                                   const color_yiqa<T> &c1_,
                                                   const color_yiqa<T> &c2_)                                       {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> madd(const color_yiqa<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, add_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> madd(const color_yiqa<T> &c_,
                                                   const color_yiqa<T> &mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(mul_), add_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> madd(const color_yiqa<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_yiqa<T> &add_)                                      {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, reinterpret_cast<const vec4<T>&>(add_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> rcp(const color_yiqa<T> &c_)                                         {const vec4<T> &v=rcp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> rcp_z(const color_yiqa<T> &c_)                                       {const vec4<T> &v=rcp_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> floor(const color_yiqa<T> &c_)                                       {const vec4<T> &v=floor(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> ceil(const color_yiqa<T> &c_)                                        {const vec4<T> &v=ceil(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> trunc(const color_yiqa<T> &c_)                                       {const vec4<T> &v=trunc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> frc(const color_yiqa<T> &c_)                                         {const vec4<T> &v=frc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> mod(const color_yiqa<T> &c_, T div_)                                 {const vec4<T> &v=mod(reinterpret_cast<const vec4<T>&>(c_), div_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> cycle(const color_yiqa<T> &c_, T cycle_)                             {const vec4<T> &v=cycle(reinterpret_cast<const vec4<T>&>(c_), cycle_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> cycle1(const color_yiqa<T> &c_)                                      {const vec4<T> &v=cycle1(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sat(const color_yiqa<T> &c_)                                         {const vec4<T> &v=sat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> ssat(const color_yiqa<T> &c_)                                        {const vec4<T> &v=ssat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> clamp(const color_yiqa<T> &c_,
                                                    const color_yiqa<T> &min_,
                                                    const color_yiqa<T> &max_)                                     {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(min_), reinterpret_cast<const vec4<T>&>(max_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> clamp(const color_yiqa<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                               {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), min_, max_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> abs(const color_yiqa<T> &c_)                                         {const vec4<T> &v=abs(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sgn(const color_yiqa<T> &c_)                                         {const vec4<T> &v=sgn(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sgn_zp(const color_yiqa<T> &c_)                                      {const vec4<T> &v=sgn_zp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sqr(const color_yiqa<T> &c_)                                         {const vec4<T> &v=sqr(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> cubic(const color_yiqa<T> &c_)                                       {const vec4<T> &v=cubic(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sqrt(const color_yiqa<T> &c_)                                        {const vec4<T> &v=sqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> sqrt_z(const color_yiqa<T> &c_)                                      {const vec4<T> &v=sqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> cbrt(const color_yiqa<T> &c_)                                        {const vec4<T> &v=cbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> rsqrt(const color_yiqa<T> &c_)                                       {const vec4<T> &v=rsqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> rsqrt_z(const color_yiqa<T> &c_)                                     {const vec4<T> &v=rsqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> rcbrt(const color_yiqa<T> &c_)                                       {const vec4<T> &v=rcbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> rcbrt_z(const color_yiqa<T> &c_)                                     {const vec4<T> &v=rcbrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_yiqa<T> &c_)                           {return norm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_yiqa<T> &c_)                          {return rnorm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_yiqa<T> &c_)                        {return rnorm_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_yiqa<T> &c_)                          {return norm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_yiqa<T> &c_)                         {return rnorm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_yiqa<T> &c_)                       {return rnorm2_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_yiqa<T> &c_)                        {return norm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_yiqa<T> &c_)                       {return rnorm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_yiqa<T> &c_)                     {return rnorm_l1_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_yiqa<T> exp(const color_yiqa<T> &c_)                                         {const vec4<T> &v=exp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> exp2(const color_yiqa<T> &c_)                                        {const vec4<T> &v=exp2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> ln(const color_yiqa<T> &c_)                                          {const vec4<T> &v=ln(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> log2(const color_yiqa<T> &c_)                                        {const vec4<T> &v=log2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> log10(const color_yiqa<T> &c_)                                       {const vec4<T> &v=log10(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> pow(const color_yiqa<T> &c_, typename math<T>::scalar_t p_)          {const vec4<T> &v=pow(reinterpret_cast<const vec4<T>&>(c_), p_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> unit(const color_yiqa<T> &c_)                                        {const vec4<T> &v=unit(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> unit_z(const color_yiqa<T> &c_)                                      {const vec4<T> &v=unit_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_yiqa<T> &c0_, const color_yiqa<T> &c1_) {return dot(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_yiqa<T> &c_)                           {return dot1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_yiqa<T> &c_)                                                        {neg(reinterpret_cast<vec4<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_yiqa<T> &c_, Rand &rnd_)                             {rand_u(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_yiqa<T> &c_, Rand &rnd_)                         {rand_real1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_yiqa<T> &c_, Rand &rnd_)                        {rand_ureal1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_yiqa<T> smoothstep(const color_yiqa<T> &c_)                                  {const vec4<T> &v=smoothstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> smootherstep(const color_yiqa<T> &c_)                                {const vec4<T> &v=smootherstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> lerp(const color_yiqa<T> &c0_,
                                                   const color_yiqa<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                  {const vec4<T> &v=lerp(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), t_); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> reflect(const color_yiqa<T> &c_, const color_yiqa<T> &cn_)           {const vec4<T> &v=reflect(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE color_yiqa<T> reflect_u(const color_yiqa<T> &c_, const color_yiqa<T> &cn_)         {const vec4<T> &v=reflect_u(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_yiqa<T> shuffle(const color_yiqa<T> &c_)                  {const vec4<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_yiqa<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_yiqa32 &cr_, const color_yiqa<T> &c_)                           {pack_u1(reinterpret_cast<vec4_32u&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_yiqa32 &cr_, const color_yiqa<T> &c_)                           {pack_s1(reinterpret_cast<vec4_32s&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_yiqa<T> &cr_, const color_yiqa32 &c_)                         {unpack_u1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_yiqa<T> &cr_, const color_yiqa32 &c_)                         {unpack_s1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_hsv
//============================================================================
template<typename T>
color_hsv<T> color_hsv<T>::zero()
{
  return color_hsv<T>(0);
}
//----

template<typename T>
color_hsv<T> color_hsv<T>::one()
{
  return color_hsv<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_hsv<T>::color_hsv()
{
}
//----

template<typename T>
color_hsv<T>::color_hsv(T h_, T s_, T v_)
  :h(h_)
  ,s(s_)
  ,v(v_)
{
}
//----

template<typename T>
color_hsv<T>::color_hsv(const color_hsva<T> &c_)
  :h(c_.h)
  ,s(c_.s)
  ,v(c_.v)
{
}
//----

template<typename T>
template<typename U>
color_hsv<T>::color_hsv(const color_hsv<U> &c_)
  :h(T(c_.h))
  ,s(T(c_.s))
  ,v(T(c_.v))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_hsv<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_hsv<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_hsv<T>::set(T h_, T s_, T v_)
{
  h=h_;
  s=s_;
  v=v_;
}
//----

template<typename T>
template<typename U>
void color_hsv<T>::set(const color_hsv<U> &c_)
{
  h=T(c_.h);
  s=T(c_.s);
  v=T(c_.v);
}
//----------------------------------------------------------------------------

// vec3 interface for color_hsv
template<typename T> EGL_INLINE bool is_zero(const color_hsv<T> &c_)                                             {return is_zero(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_hsv<T> &c_)                                              {return is_sat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_hsv<T> &c_)                                             {return is_ssat(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_hsv<T> &c0_, const color_hsv<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)==reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_hsv<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_hsv<T> &c_)           {return s_==reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_hsv<T> &c0_, const color_hsv<T> &c1_)                {return reinterpret_cast<const vec3<T>&>(c0_)!=reinterpret_cast<const vec3<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_hsv<T> &c_, typename math<T>::scalar_t s_)           {return reinterpret_cast<const vec3<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_hsv<T> &c_)           {return s_!=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE color_hsv<T> operator<(const color_hsv<T> &c0_, const color_hsv<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator<(const color_hsv<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator<(typename math<T>::scalar_t s_, const color_hsv<T> &c_)    {const vec3<T> &v=s_<reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator>(const color_hsv<T> &c0_, const color_hsv<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator>(const color_hsv<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator>(typename math<T>::scalar_t s_, const color_hsv<T> &c_)    {const vec3<T> &v=s_>reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator<=(const color_hsv<T> &c0_, const color_hsv<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)<=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator<=(const color_hsv<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)<=s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator<=(typename math<T>::scalar_t s_, const color_hsv<T> &c_)   {const vec3<T> &v=s_<=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator>=(const color_hsv<T> &c0_, const color_hsv<T> &c1_)        {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)>=reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator>=(const color_hsv<T> &c_, typename math<T>::scalar_t s_)   {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)>=s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator>=(typename math<T>::scalar_t s_, const color_hsv<T> &c_)   {const vec3<T> &v=s_>=reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sel_eq(const color_hsv<T> &t0_, const color_hsv<T> &t1_,
                                                     const color_hsv<T> &c0_, const color_hsv<T> &c1_)           {const vec3<T> &v=sel_eq(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sel_lt(const color_hsv<T> &t0_, const color_hsv<T> &t1_,
                                                     const color_hsv<T> &c0_, const color_hsv<T> &c1_)           {const vec3<T> &v=sel_lt(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sel_le(const color_hsv<T> &t0_, const color_hsv<T> &t1_,
                                                     const color_hsv<T> &c0_, const color_hsv<T> &c1_)           {const vec3<T> &v=sel_le(reinterpret_cast<const vec3<T>&>(t0_), reinterpret_cast<const vec3<T>&>(t1_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sel_eqz(const color_hsv<T> &t_,
                                                      const color_hsv<T> &c0_, const color_hsv<T> &c1_)          {const vec3<T> &v=sel_eqz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sel_ltz(const color_hsv<T> &t_,
                                                      const color_hsv<T> &c0_, const color_hsv<T> &c1_)          {const vec3<T> &v=sel_ltz(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sel_lez(const color_hsv<T> &t_,
                                                      const color_hsv<T> &c0_, const color_hsv<T> &c1_)          {const vec3<T> &v=sel_lez(reinterpret_cast<const vec3<T>&>(t_), reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_hsv<T> &cr_, const color_hsv<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)+=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_hsv<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_hsv<T> &cr_, const color_hsv<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)-=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_hsv<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_hsv<T> &cr_, const color_hsv<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)*=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_hsv<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_hsv<T> &cr_, const mat44<T> &m_)                           {reinterpret_cast<vec3<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator/=(color_hsv<T> &cr_, const color_hsv<T> &c_)                       {reinterpret_cast<vec3<T>&>(cr_)/=reinterpret_cast<const vec3<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_hsv<T> &cr_, typename math<T>::scalar_t s_)                {reinterpret_cast<vec3<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_hsv<T> operator+(const color_hsv<T> &c0_, const color_hsv<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)+reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator+(const color_hsv<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)+s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator+(typename math<T>::scalar_t s_, const color_hsv<T> &c_)    {const vec3<T> &v=s_+reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator-(const color_hsv<T> &c0_, const color_hsv<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)-reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator-(const color_hsv<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)-s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator-(typename math<T>::scalar_t s_, const color_hsv<T> &c_)    {const vec3<T> &v=s_-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator-(const color_hsv<T> &c_)                                   {const vec3<T> &v=-reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator*(const color_hsv<T> &c0_, const color_hsv<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)*reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator*(const color_hsv<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator*(typename math<T>::scalar_t s_, const color_hsv<T> &c_)    {const vec3<T> &v=s_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator*(const color_hsv<T> &c_, const mat44<T> &m_)               {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)*m_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator*(const mat44<T> &m_, const color_hsv<T> &c_)               {const vec3<T> &v=m_*reinterpret_cast<const vec3<T>&>(c_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator/(const color_hsv<T> &c0_, const color_hsv<T> &c1_)         {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c0_)/reinterpret_cast<const vec3<T>&>(c1_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> operator/(const color_hsv<T> &c_, typename math<T>::scalar_t s_)    {const vec3<T> &v=reinterpret_cast<const vec3<T>&>(c_)/s_; return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_hsv<T> &c_)                           {return min(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_hsv<T> min(const color_hsv<T> &c0_,
                                                  const color_hsv<T> &c1_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> min(const color_hsv<T> &c0_,
                                                  const color_hsv<T> &c1_,
                                                  const color_hsv<T> &c2_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> min(const color_hsv<T> &c0_,
                                                  const color_hsv<T> &c1_,
                                                  const color_hsv<T> &c2_,
                                                  const color_hsv<T> &c3_)                                       {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> min(const color_hsv<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=min(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> min(typename math<T>::scalar_t s_, const color_hsv<T> &c_)          {const vec3<T> &v=min(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_hsv<T> &c_)                           {return max(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_hsv<T> max(const color_hsv<T> &c0_,
                                                  const color_hsv<T> &c1_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> max(const color_hsv<T> &c0_,
                                                  const color_hsv<T> &c1_,
                                                  const color_hsv<T> &c2_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> max(const color_hsv<T> &c0_,
                                                  const color_hsv<T> &c1_,
                                                  const color_hsv<T> &c2_,
                                                  const color_hsv<T> &c3_)                                       {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_), reinterpret_cast<const vec3<T>&>(c3_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> max(const color_hsv<T> &c_, typename math<T>::scalar_t s_)          {const vec3<T> &v=max(reinterpret_cast<const vec3<T>&>(c_), s_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> max(typename math<T>::scalar_t s_, const color_hsv<T> &c_)          {const vec3<T> &v=max(s_, reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> mul(const color_hsv<T> &c0_, const color_hsv<T> &c1_)               {const vec3<T> &v=mul(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> madd(const color_hsv<T> &c0_,
                                                   const color_hsv<T> &c1_,
                                                   const color_hsv<T> &c2_)                                      {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), reinterpret_cast<const vec3<T>&>(c2_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> madd(const color_hsv<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, add_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> madd(const color_hsv<T> &c_,
                                                   const color_hsv<T> &mul_,
                                                   typename math<T>::scalar_t add_)                              {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(mul_), add_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> madd(const color_hsv<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_hsv<T> &add_)                                     {const vec3<T> &v=madd(reinterpret_cast<const vec3<T>&>(c_), mul_, reinterpret_cast<const vec3<T>&>(add_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> rcp(const color_hsv<T> &c_)                                         {const vec3<T> &v=rcp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> rcp_z(const color_hsv<T> &c_)                                       {const vec3<T> &v=rcp_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> floor(const color_hsv<T> &c_)                                       {const vec3<T> &v=floor(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> ceil(const color_hsv<T> &c_)                                        {const vec3<T> &v=ceil(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> trunc(const color_hsv<T> &c_)                                       {const vec3<T> &v=trunc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> frc(const color_hsv<T> &c_)                                         {const vec3<T> &v=frc(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> mod(const color_hsv<T> &c_, T div_)                                 {const vec3<T> &v=mod(reinterpret_cast<const vec3<T>&>(c_), div_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> cycle(const color_hsv<T> &c_, T cycle_)                             {const vec3<T> &v=cycle(reinterpret_cast<const vec3<T>&>(c_), cycle_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> cycle1(const color_hsv<T> &c_)                                      {const vec3<T> &v=cycle1(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sat(const color_hsv<T> &c_)                                         {const vec3<T> &v=sat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> ssat(const color_hsv<T> &c_)                                        {const vec3<T> &v=ssat(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> clamp(const color_hsv<T> &c_,
                                                    const color_hsv<T> &min_,
                                                    const color_hsv<T> &max_)                                    {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(min_), reinterpret_cast<const vec3<T>&>(max_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> clamp(const color_hsv<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                             {const vec3<T> &v=clamp(reinterpret_cast<const vec3<T>&>(c_), min_, max_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> abs(const color_hsv<T> &c_)                                         {const vec3<T> &v=abs(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sqr(const color_hsv<T> &c_)                                         {const vec3<T> &v=sqr(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> cubic(const color_hsv<T> &c_)                                       {const vec3<T> &v=cubic(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sqrt(const color_hsv<T> &c_)                                        {const vec3<T> &v=sqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> sqrt_z(const color_hsv<T> &c_)                                      {const vec3<T> &v=sqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> cbrt(const color_hsv<T> &c_)                                        {const vec3<T> &v=cbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> rsqrt(const color_hsv<T> &c_)                                       {const vec3<T> &v=rsqrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> rsqrt_z(const color_hsv<T> &c_)                                     {const vec3<T> &v=rsqrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> rcbrt(const color_rgba<T> &c_)                                      {const vec3<T> &v=rcbrt(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> rcbrt_z(const color_rgba<T> &c_)                                    {const vec3<T> &v=rcbrt_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_hsv<T> &c_)                          {return norm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_hsv<T> &c_)                         {return rnorm(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_hsv<T> &c_)                       {return rnorm_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_hsv<T> &c_)                         {return norm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_hsv<T> &c_)                        {return rnorm2(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_hsv<T> &c_)                      {return rnorm2_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_hsv<T> &c_)                       {return norm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_hsv<T> &c_)                      {return rnorm_l1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_hsv<T> &c_)                    {return rnorm_l1_z(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE color_hsv<T> exp(const color_hsv<T> &c_)                                         {const vec3<T> &v=exp(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> exp2(const color_hsv<T> &c_)                                        {const vec3<T> &v=exp2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> ln(const color_hsv<T> &c_)                                          {const vec3<T> &v=ln(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> log2(const color_hsv<T> &c_)                                        {const vec3<T> &v=log2(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> log10(const color_hsv<T> &c_)                                       {const vec3<T> &v=log10(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> pow(const color_hsv<T> &c_, typename math<T>::scalar_t p_)          {const vec3<T> &v=pow(reinterpret_cast<const vec3<T>&>(c_), p_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> unit(const color_hsv<T> &c_)                                        {const vec3<T> &v=unit(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> unit_z(const color_hsv<T> &c_)                                      {const vec3<T> &v=unit_z(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> cross(const color_hsv<T> &c0_, const color_hsv<T> &c1_)             {const vec3<T> &v=cross(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_hsv<T> &c0_, const color_hsv<T> &c1_) {return dot(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_hsv<T> &c_)                          {return dot1(reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_hsv<T> &c_)                                                       {neg(reinterpret_cast<vec3<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_hsv<T> &c_, Rand &rnd_)                            {rand_u(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_hsv<T> &c_, Rand &rnd_)                        {rand_real1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_hsv<T> &c_, Rand &rnd_)                       {rand_ureal1(reinterpret_cast<vec3<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_hsv<T> smoothstep(const color_hsv<T> &c_)                                  {const vec3<T> &v=smoothstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> smootherstep(const color_hsv<T> &c_)                                {const vec3<T> &v=smootherstep(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> lerp(const color_hsv<T> &c0_,
                                                   const color_hsv<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                {const vec3<T> &v=lerp(reinterpret_cast<const vec3<T>&>(c0_), reinterpret_cast<const vec3<T>&>(c1_), t_); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> reflect(const color_hsv<T> &c_, const color_hsv<T> &cn_)            {const vec3<T> &v=reflect(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE color_hsv<T> reflect_u(const color_hsv<T> &c_, const color_hsv<T> &cn_)          {const vec3<T> &v=reflect_u(reinterpret_cast<const vec3<T>&>(c_), reinterpret_cast<const vec3<T>&>(cn_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_hsv<T> shuffle(const color_hsv<T> &c_)                  {const vec3<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec3<T>&>(c_)); return reinterpret_cast<const color_hsv<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_hsv32 &cr_, const color_hsv<T> &c_)                           {pack_u1(reinterpret_cast<vec3_32u&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_hsv32 &cr_, const color_hsv<T> &c_)                           {pack_s1(reinterpret_cast<vec3_32s&>(cr_), reinterpret_cast<const vec3<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_hsv<T> &cr_, const color_hsv32 &c_)                         {unpack_u1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_hsv<T> &cr_, const color_hsv32 &c_)                         {unpack_s1(reinterpret_cast<vec3<T>&>(cr_), reinterpret_cast<const vec3_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// color_hsva
//============================================================================
template<typename T>
color_hsva<T> color_hsva<T>::zero()
{
  return color_hsva<T>(0);
}
//----

template<typename T>
color_hsva<T> color_hsva<T>::one()
{
  return color_hsva<T>(1);
}
//----------------------------------------------------------------------------

template<typename T>
color_hsva<T>::color_hsva()
{
}
//----

template<typename T>
color_hsva<T>::color_hsva(T h_, T s_, T v_)
  :h(h_)
  ,s(s_)
  ,v(v_)
  ,a(1)
{
}
//----

template<typename T>
color_hsva<T>::color_hsva(T h_, T s_, T v_, T a_)
  :h(h_)
  ,s(s_)
  ,v(v_)
  ,a(a_)
{
}
//----

template<typename T>
color_hsva<T>::color_hsva(const color_hsv<T> &c_)
  :h(c_.h)
  ,s(c_.s)
  ,v(c_.v)
  ,a(1)
{
}
//----

template<typename T>
color_hsva<T>::color_hsva(const color_hsv<T> &c_, T a_)
  :h(c_.h)
  ,s(c_.s)
  ,v(c_.v)
  ,a(a_)
{
}
//----

template<typename T>
template<typename U>
color_hsva<T>::color_hsva(const color_hsva<U> &c_)
  :h(T(c_.h))
  ,s(T(c_.s))
  ,v(T(c_.v))
  ,a(T(c_.a))
{
}
//----------------------------------------------------------------------------

template<typename T>
const T &color_hsva<T>::operator[](unsigned idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &color_hsva<T>::operator[](unsigned idx_)
{
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void color_hsva<T>::set(T h_, T s_, T v_)
{
  h=h_;
  s=s_;
  v=v_;
}
//----

template<typename T>
void color_hsva<T>::set(T h_, T s_, T v_, T a_)
{
  h=h_;
  s=s_;
  v=v_;
  a=a_;
}
//----

template<typename T>
void color_hsva<T>::set(const color_hsv<T> &c_, T a_)
{
  h=c_.h;
  s=c_.s;
  v=c_.v;
  a=a_;
}
//----

template<typename T>
template<typename U>
void color_hsva<T>::set(const color_hsva<U> &c_)
{
  h=T(c_.h);
  s=T(c_.s);
  v=T(c_.v);
  a=T(c_.a);
}
//----------------------------------------------------------------------------

// vec4 interface for color_hsva
template<typename T> EGL_INLINE bool is_zero(const color_hsva<T> &c_)                                              {return is_zero(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_sat(const color_hsva<T> &c_)                                               {return is_sat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool is_ssat(const color_hsva<T> &c_)                                              {return is_ssat(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE bool operator==(const color_hsva<T> &c0_, const color_hsva<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)==reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator==(const color_hsva<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)==s_;}
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const color_hsva<T> &c_)            {return s_==reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE bool operator!=(const color_hsva<T> &c0_, const color_hsva<T> &c1_)                {return reinterpret_cast<const vec4<T>&>(c0_)!=reinterpret_cast<const vec4<T>&>(c1_);}
template<typename T> EGL_INLINE bool operator!=(const color_hsva<T> &c_, typename math<T>::scalar_t s_)            {return reinterpret_cast<const vec4<T>&>(c_)!=s_;}
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const color_hsva<T> &c_)            {return s_!=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE color_hsva<T> operator<(const color_hsva<T> &c0_, const color_hsva<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator<(const color_hsva<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator<(typename math<T>::scalar_t s_, const color_hsva<T> &c_)    {const vec4<T> &v=s_<reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator>(const color_hsva<T> &c0_, const color_hsva<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator>(const color_hsva<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator>(typename math<T>::scalar_t s_, const color_hsva<T> &c_)    {const vec4<T> &v=s_>reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator<=(const color_hsva<T> &c0_, const color_hsva<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)<=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator<=(const color_hsva<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)<=s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator<=(typename math<T>::scalar_t s_, const color_hsva<T> &c_)   {const vec4<T> &v=s_<=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator>=(const color_hsva<T> &c0_, const color_hsva<T> &c1_)       {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)>=reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator>=(const color_hsva<T> &c_, typename math<T>::scalar_t s_)   {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)>=s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator>=(typename math<T>::scalar_t s_, const color_hsva<T> &c_)   {const vec4<T> &v=s_>=reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sel_eq(const color_hsva<T> &t0_, const color_hsva<T> &t1_,
                                                     const color_hsva<T> &c0_, const color_hsva<T> &c1_)           {const vec4<T> &v=sel_eq(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sel_lt(const color_hsva<T> &t0_, const color_hsva<T> &t1_,
                                                     const color_hsva<T> &c0_, const color_hsva<T> &c1_)           {const vec4<T> &v=sel_lt(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sel_le(const color_hsva<T> &t0_, const color_hsva<T> &t1_,
                                                     const color_hsva<T> &c0_, const color_hsva<T> &c1_)           {const vec4<T> &v=sel_le(reinterpret_cast<const vec4<T>&>(t0_), reinterpret_cast<const vec4<T>&>(t1_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sel_eqz(const color_hsva<T> &t_,
                                                      const color_hsva<T> &c0_, const color_hsva<T> &c1_)          {const vec4<T> &v=sel_eqz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sel_ltz(const color_hsva<T> &t_,
                                                      const color_hsva<T> &c0_, const color_hsva<T> &c1_)          {const vec4<T> &v=sel_ltz(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sel_lez(const color_hsva<T> &t_,
                                                      const color_hsva<T> &c0_, const color_hsva<T> &c1_)          {const vec4<T> &v=sel_lez(reinterpret_cast<const vec4<T>&>(t_), reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE void operator+=(color_hsva<T> &cr_, const color_hsva<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)+=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator+=(color_hsva<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)+=s_;}
template<typename T> EGL_INLINE void operator-=(color_hsva<T> &cr_, const color_hsva<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)-=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator-=(color_hsva<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)-=s_;}
template<typename T> EGL_INLINE void operator*=(color_hsva<T> &cr_, const color_hsva<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)*=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator*=(color_hsva<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)*=s_;}
template<typename T> EGL_INLINE void operator*=(color_hsva<T> &cr_, const mat44<T> &m_)                            {reinterpret_cast<vec4<T>&>(cr_)*=m_;}
template<typename T> EGL_INLINE void operator/=(color_hsva<T> &cr_, const color_hsva<T> &c_)                       {reinterpret_cast<vec4<T>&>(cr_)/=reinterpret_cast<const vec4<T>&>(c_);}
template<typename T> EGL_INLINE void operator/=(color_hsva<T> &cr_, typename math<T>::scalar_t s_)                 {reinterpret_cast<vec4<T>&>(cr_)/=s_;}
template<typename T> EGL_INLINE color_hsva<T> operator+(const color_hsva<T> &c0_, const color_hsva<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)+reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator+(const color_hsva<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)+s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator+(typename math<T>::scalar_t s_, const color_hsva<T> &c_)    {const vec4<T> &v=s_+reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator-(const color_hsva<T> &c0_, const color_hsva<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)-reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator-(const color_hsva<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)-s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator-(typename math<T>::scalar_t s_, const color_hsva<T> &c_)    {const vec4<T> &v=s_-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator-(const color_hsva<T> &c_)                                   {const vec4<T> &v=-reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator*(const color_hsva<T> &c0_, const color_hsva<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)*reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator*(const color_hsva<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator*(typename math<T>::scalar_t s_, const color_hsva<T> &c_)    {const vec4<T> &v=s_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator*(const color_hsva<T> &c_, const mat44<T> &m_)               {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)*m_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator*(const mat44<T> &m_, const color_hsva<T> &c_)               {const vec4<T> &v=m_*reinterpret_cast<const vec4<T>&>(c_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator/(const color_hsva<T> &c0_, const color_hsva<T> &c1_)        {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c0_)/reinterpret_cast<const vec4<T>&>(c1_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> operator/(const color_hsva<T> &c_, typename math<T>::scalar_t s_)    {const vec4<T> &v=reinterpret_cast<const vec4<T>&>(c_)/s_; return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const color_hsva<T> &c_)                            {return min(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_hsva<T> min(const color_hsva<T> &c0_,
                                                  const color_hsva<T> &c1_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> min(const color_hsva<T> &c0_,
                                                  const color_hsva<T> &c1_,
                                                  const color_hsva<T> &c2_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> min(const color_hsva<T> &c0_,
                                                  const color_hsva<T> &c1_,
                                                  const color_hsva<T> &c2_,
                                                  const color_hsva<T> &c3_)                                        {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> min(const color_hsva<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=min(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> min(typename math<T>::scalar_t s_, const color_hsva<T> &c_)          {const vec4<T> &v=min(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const color_hsva<T> &c_)                            {return max(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_hsva<T> max(const color_hsva<T> &c0_,
                                                  const color_hsva<T> &c1_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> max(const color_hsva<T> &c0_,
                                                  const color_hsva<T> &c1_,
                                                  const color_hsva<T> &c2_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> max(const color_hsva<T> &c0_,
                                                  const color_hsva<T> &c1_,
                                                  const color_hsva<T> &c2_,
                                                  const color_hsva<T> &c3_)                                        {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_), reinterpret_cast<const vec4<T>&>(c3_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> max(const color_hsva<T> &c_, typename math<T>::scalar_t s_)          {const vec4<T> &v=max(reinterpret_cast<const vec4<T>&>(c_), s_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> max(typename math<T>::scalar_t s_, const color_hsva<T> &c_)          {const vec4<T> &v=max(s_, reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> mul(const color_hsva<T> &c0_, const color_hsva<T> &c1_)              {const vec4<T> &v=mul(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> madd(const color_hsva<T> &c0_,
                                                   const color_hsva<T> &c1_,
                                                   const color_hsva<T> &c2_)                                       {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), reinterpret_cast<const vec4<T>&>(c2_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> madd(const color_hsva<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, add_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> madd(const color_hsva<T> &c_,
                                                   const color_hsva<T> &mul_,
                                                   typename math<T>::scalar_t add_)                                {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(mul_), add_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> madd(const color_hsva<T> &c_,
                                                   typename math<T>::scalar_t mul_,
                                                   const color_hsva<T> &add_)                                      {const vec4<T> &v=madd(reinterpret_cast<const vec4<T>&>(c_), mul_, reinterpret_cast<const vec4<T>&>(add_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> rcp(const color_hsva<T> &c_)                                         {const vec4<T> &v=rcp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> rcp_z(const color_hsva<T> &c_)                                       {const vec4<T> &v=rcp_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> floor(const color_hsva<T> &c_)                                       {const vec4<T> &v=floor(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> ceil(const color_hsva<T> &c_)                                        {const vec4<T> &v=ceil(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> trunc(const color_hsva<T> &c_)                                       {const vec4<T> &v=trunc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> frc(const color_hsva<T> &c_)                                         {const vec4<T> &v=frc(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> mod(const color_hsva<T> &c_, T div_)                                 {const vec4<T> &v=mod(reinterpret_cast<const vec4<T>&>(c_), div_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> cycle(const color_hsva<T> &c_, T cycle_)                             {const vec4<T> &v=cycle(reinterpret_cast<const vec4<T>&>(c_), cycle_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> cycle1(const color_hsva<T> &c_)                                      {const vec4<T> &v=cycle1(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sat(const color_hsva<T> &c_)                                         {const vec4<T> &v=sat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> ssat(const color_hsva<T> &c_)                                        {const vec4<T> &v=ssat(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> clamp(const color_hsva<T> &c_,
                                                    const color_hsva<T> &min_,
                                                    const color_hsva<T> &max_)                                     {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(min_), reinterpret_cast<const vec4<T>&>(max_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> clamp(const color_hsva<T> &c_,
                                                    typename math<T>::scalar_t min_,
                                                    typename math<T>::scalar_t max_)                               {const vec4<T> &v=clamp(reinterpret_cast<const vec4<T>&>(c_), min_, max_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> abs(const color_hsva<T> &c_)                                         {const vec4<T> &v=abs(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sqr(const color_hsva<T> &c_)                                         {const vec4<T> &v=sqr(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> cubic(const color_hsva<T> &c_)                                       {const vec4<T> &v=cubic(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sqrt(const color_hsva<T> &c_)                                        {const vec4<T> &v=sqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> sqrt_z(const color_hsva<T> &c_)                                      {const vec4<T> &v=sqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> cbrt(const color_hsva<T> &c_)                                        {const vec4<T> &v=cbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> rsqrt(const color_hsva<T> &c_)                                       {const vec4<T> &v=rsqrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> rsqrt_z(const color_hsva<T> &c_)                                     {const vec4<T> &v=rsqrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> rcbrt(const color_rgba<T> &c_)                                       {const vec4<T> &v=rcbrt(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> rcbrt_z(const color_rgba<T> &c_)                                     {const vec4<T> &v=rcbrt_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const color_hsva<T> &c_)                           {return norm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const color_hsva<T> &c_)                          {return rnorm(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const color_hsva<T> &c_)                        {return rnorm_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const color_hsva<T> &c_)                          {return norm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const color_hsva<T> &c_)                         {return rnorm2(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const color_hsva<T> &c_)                       {return rnorm2_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const color_hsva<T> &c_)                        {return norm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const color_hsva<T> &c_)                       {return rnorm_l1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const color_hsva<T> &c_)                     {return rnorm_l1_z(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE color_hsva<T> exp(const color_hsva<T> &c_)                                         {const vec4<T> &v=exp(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> exp2(const color_hsva<T> &c_)                                        {const vec4<T> &v=exp2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> ln(const color_hsva<T> &c_)                                          {const vec4<T> &v=ln(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> log2(const color_hsva<T> &c_)                                        {const vec4<T> &v=log2(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> log10(const color_hsva<T> &c_)                                       {const vec4<T> &v=log10(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> pow(const color_hsva<T> &c_, typename math<T>::scalar_t p_)          {const vec4<T> &v=pow(reinterpret_cast<const vec4<T>&>(c_), p_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> unit(const color_hsva<T> &c_)                                        {const vec4<T> &v=unit(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> unit_z(const color_hsva<T> &c_)                                      {const vec4<T> &v=unit_z(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const color_hsva<T> &c0_, const color_hsva<T> &c1_) {return dot(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_));}
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const color_hsva<T> &c_)                           {return dot1(reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void neg(color_hsva<T> &c_)                                                        {neg(reinterpret_cast<vec4<T>&>(c_));}
template<typename T, class Rand> EGL_INLINE void rand_u(color_hsva<T> &c_, Rand &rnd_)                             {rand_u(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_real1(color_hsva<T> &c_, Rand &rnd_)                         {rand_real1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T, class Rand> EGL_INLINE void rand_ureal1(color_hsva<T> &c_, Rand &rnd_)                        {rand_ureal1(reinterpret_cast<vec4<T>&>(c_), rnd_);}
template<typename T> EGL_INLINE color_hsva<T> smoothstep(const color_hsva<T> &c_)                                  {const vec4<T> &v=smoothstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> smootherstep(const color_hsva<T> &c_)                                {const vec4<T> &v=smootherstep(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> lerp(const color_hsva<T> &c0_,
                                                   const color_hsva<T> &c1_,
                                                   typename math<T>::scalar_t t_)                                  {const vec4<T> &v=lerp(reinterpret_cast<const vec4<T>&>(c0_), reinterpret_cast<const vec4<T>&>(c1_), t_); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> reflect(const color_hsva<T> &c_, const color_hsva<T> &cn_)           {const vec4<T> &v=reflect(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE color_hsva<T> reflect_u(const color_hsva<T> &c_, const color_hsva<T> &cn_)         {const vec4<T> &v=reflect_u(reinterpret_cast<const vec4<T>&>(c_), reinterpret_cast<const vec4<T>&>(cn_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<unsigned shuffle_, typename T> EGL_INLINE color_hsva<T> shuffle(const color_hsva<T> &c_)                  {const vec4<T> &v=shuffle<shuffle_>(reinterpret_cast<const vec4<T>&>(c_)); return reinterpret_cast<const color_hsva<T>&>(v);}
template<typename T> EGL_INLINE void pack_u1(color_hsva32 &cr_, const color_hsva<T> &c_)                           {pack_u1(reinterpret_cast<vec4_32u&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void pack_s1(color_hsva32 &cr_, const color_hsva<T> &c_)                           {pack_s1(reinterpret_cast<vec4_32s&>(cr_), reinterpret_cast<const vec4<T>&>(c_));}
template<typename T> EGL_INLINE void unpack_u1(color_hsva<T> &cr_, const color_hsva32 &c_)                         {unpack_u1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32u&>(c_));}
template<typename T> EGL_INLINE void unpack_s1(color_hsva<T> &cr_, const color_hsva32 &c_)                         {unpack_s1(reinterpret_cast<vec4<T>&>(cr_), reinterpret_cast<const vec4_32s&>(c_));}
//----------------------------------------------------------------------------


//============================================================================
// RGB <-> XYZ color space conversion
//============================================================================
template<typename T>
mat33<T> tform_rgb_to_xyz(const vec2<T> &prim_rxy_, const vec2<T> &prim_gxy_, const vec2<T> &prim_bxy_, const vec2<T> &wp_)
{
  // generate RGB->XYZ transform matrix for given RGB primaries & white point
  typedef typename math<T>::scalar_t scalar_t;
  mat33<T> m(prim_rxy_.x/prim_rxy_.y, scalar_t(1.0), (scalar_t(1.0)-prim_rxy_.x-prim_rxy_.y)/prim_rxy_.y,
             prim_gxy_.x/prim_gxy_.y, scalar_t(1.0), (scalar_t(1.0)-prim_gxy_.x-prim_gxy_.y)/prim_gxy_.y,
             prim_bxy_.x/prim_bxy_.y, scalar_t(1.0), (scalar_t(1.0)-prim_bxy_.x-prim_bxy_.y)/prim_bxy_.y);
  vec3<T> wp_norm(wp_.x/wp_.y, scalar_t(1.0), (scalar_t(1.0)-wp_.x-wp_.y)/wp_.y);
  return mat33<T>(wp_norm*inv(m))*m;
}
//----

template<typename T>
mat33<T> tform_xyz_to_rgb(const vec2<T> &prim_rxy_, const vec2<T> &prim_gxy_, const vec2<T> &prim_bxy_, const vec2<T> &wp_)
{
  // generate XYZ->RGB transform matrix for given RGB primaries & white point
  typedef typename math<T>::scalar_t scalar_t;
  mat33<T> m(prim_rxy_.x/prim_rxy_.y, scalar_t(1.0), (scalar_t(1.0)-prim_rxy_.x-prim_rxy_.y)/prim_rxy_.y,
             prim_gxy_.x/prim_gxy_.y, scalar_t(1.0), (scalar_t(1.0)-prim_gxy_.x-prim_gxy_.y)/prim_gxy_.y,
             prim_bxy_.x/prim_bxy_.y, scalar_t(1.0), (scalar_t(1.0)-prim_bxy_.x-prim_bxy_.y)/prim_bxy_.y);
  vec3<T> wp_norm(wp_.x/wp_.y, scalar_t(1.0), (scalar_t(1.0)-wp_.x-wp_.y)/wp_.y);
  mat33<T> inv_m=inv(m);
  return inv_m*mat33<T>(rcp(wp_norm*inv_m));
}
//----

template<typename T>
mat33<T> tform_rgb_to_xyz(e_rgb_color_space rgb_cspace_)
{
  typedef typename math<T>::scalar_t scalar_t;
  switch(rgb_cspace_)
  {
    case rgbcs_srgb:     return tform_rgb_to_xyz(vec2<T>(scalar_t(0.64000), scalar_t(0.33000)), vec2<T>(scalar_t(0.30000), scalar_t(0.60000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_rec2020:  return tform_rgb_to_xyz(vec2<T>(scalar_t(0.70792), scalar_t(0.29203)), vec2<T>(scalar_t(0.17024), scalar_t(0.79652)), vec2<T>(scalar_t(0.13137), scalar_t( 0.04588)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_dcip3:    return tform_rgb_to_xyz(vec2<T>(scalar_t(0.68000), scalar_t(0.32000)), vec2<T>(scalar_t(0.26500), scalar_t(0.69000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_dcip3t:   return tform_rgb_to_xyz(vec2<T>(scalar_t(0.68000), scalar_t(0.32000)), vec2<T>(scalar_t(0.26500), scalar_t(0.69000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65_p3t));
    case rgbcs_adobe:    return tform_rgb_to_xyz(vec2<T>(scalar_t(0.64000), scalar_t(0.33000)), vec2<T>(scalar_t(0.21000), scalar_t(0.71000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_adobe_wg: return tform_rgb_to_xyz(vec2<T>(scalar_t(0.73469), scalar_t(0.26531)), vec2<T>(scalar_t(0.11416), scalar_t(0.82621)), vec2<T>(scalar_t(0.15664), scalar_t( 0.01770)), cie_white_point<T>(ciewpnt_2deg_d50));
    case rgbcs_prophoto: return tform_rgb_to_xyz(vec2<T>(scalar_t(0.73470), scalar_t(0.26530)), vec2<T>(scalar_t(0.15960), scalar_t(0.84040)), vec2<T>(scalar_t(0.03660), scalar_t( 0.00010)), cie_white_point<T>(ciewpnt_2deg_d50));
    case rgbcs_aces_ap0: return tform_rgb_to_xyz(vec2<T>(scalar_t(0.73470), scalar_t(0.26530)), vec2<T>(scalar_t(0.00000), scalar_t(1.00000)), vec2<T>(scalar_t(0.00010), scalar_t(-0.07700)), cie_white_point<T>(ciewpnt_2deg_d60));
    case rgbcs_aces_ap1: return tform_rgb_to_xyz(vec2<T>(scalar_t(0.71300), scalar_t(0.29300)), vec2<T>(scalar_t(0.16500), scalar_t(0.83000)), vec2<T>(scalar_t(0.12800), scalar_t( 0.04400)), cie_white_point<T>(ciewpnt_2deg_d60));
  }
  EGL_ERROR("Unknown color space.");
  return mat33<T>::zero();
}
//----

template<typename T>
mat33<T> tform_xyz_to_rgb(e_rgb_color_space rgb_cspace_)
{
  typedef typename math<T>::scalar_t scalar_t;
  switch(rgb_cspace_)
  {
    case rgbcs_srgb:     return tform_xyz_to_rgb(vec2<T>(scalar_t(0.64000), scalar_t(0.33000)), vec2<T>(scalar_t(0.30000), scalar_t(0.60000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_rec2020:  return tform_xyz_to_rgb(vec2<T>(scalar_t(0.70792), scalar_t(0.29203)), vec2<T>(scalar_t(0.17024), scalar_t(0.79652)), vec2<T>(scalar_t(0.13137), scalar_t( 0.04588)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_dcip3:    return tform_xyz_to_rgb(vec2<T>(scalar_t(0.68000), scalar_t(0.32000)), vec2<T>(scalar_t(0.26500), scalar_t(0.69000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_dcip3t:   return tform_xyz_to_rgb(vec2<T>(scalar_t(0.68000), scalar_t(0.32000)), vec2<T>(scalar_t(0.26500), scalar_t(0.69000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65_p3t));
    case rgbcs_adobe:    return tform_xyz_to_rgb(vec2<T>(scalar_t(0.64000), scalar_t(0.33000)), vec2<T>(scalar_t(0.21000), scalar_t(0.71000)), vec2<T>(scalar_t(0.15000), scalar_t( 0.06000)), cie_white_point<T>(ciewpnt_2deg_d65));
    case rgbcs_adobe_wg: return tform_xyz_to_rgb(vec2<T>(scalar_t(0.73469), scalar_t(0.26531)), vec2<T>(scalar_t(0.11416), scalar_t(0.82621)), vec2<T>(scalar_t(0.15664), scalar_t( 0.01770)), cie_white_point<T>(ciewpnt_2deg_d50));
    case rgbcs_prophoto: return tform_xyz_to_rgb(vec2<T>(scalar_t(0.73470), scalar_t(0.26530)), vec2<T>(scalar_t(0.15960), scalar_t(0.84040)), vec2<T>(scalar_t(0.03660), scalar_t( 0.00010)), cie_white_point<T>(ciewpnt_2deg_d50));
    case rgbcs_aces_ap0: return tform_xyz_to_rgb(vec2<T>(scalar_t(0.73470), scalar_t(0.26530)), vec2<T>(scalar_t(0.00000), scalar_t(1.00000)), vec2<T>(scalar_t(0.00010), scalar_t(-0.07700)), cie_white_point<T>(ciewpnt_2deg_d60));
    case rgbcs_aces_ap1: return tform_xyz_to_rgb(vec2<T>(scalar_t(0.71300), scalar_t(0.29300)), vec2<T>(scalar_t(0.16500), scalar_t(0.83000)), vec2<T>(scalar_t(0.12800), scalar_t( 0.04400)), cie_white_point<T>(ciewpnt_2deg_d60));
  }
  EGL_ERROR("Unknown color space.");
  return mat33<T>::zero();
}
//----------------------------------------------------------------------------


//============================================================================
// RGB <-> YIQ color space conversion
//============================================================================
template<typename T>
EGL_INLINE mat33<T> tform_srgb_to_yiq()
{
  typedef typename math<T>::scalar_t scalar_t;
  return mat33<T>(scalar_t(0.29906252315023879), scalar_t( 0.59619793162446932), scalar_t( 0.21158684041481168),
                  scalar_t(0.58673030847027274), scalar_t(-0.27501215228975523), scalar_t(-0.52313197234453446),
                  scalar_t(0.11420716837948833), scalar_t(-0.32118577933471409), scalar_t( 0.31154513192972272));
}
//----

template<typename T>
EGL_INLINE mat33<T> tform_yiq_to_srgb()
{
  typedef typename math<T>::scalar_t scalar_t;
  return mat33<T>(scalar_t(1.0),   scalar_t( 1.0),   scalar_t( 1.0),
                  scalar_t(0.956), scalar_t(-0.272), scalar_t(-1.106),
                  scalar_t(0.619), scalar_t(-0.647), scalar_t( 1.703));
}
//----

template<typename T>
EGL_INLINE color_yiq<T> srgb_to_yiq(const color_rgb<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_yiq<T>(scalar_t(0.29906252315023879)*c_.r+scalar_t(0.58673030847027274)*c_.g+scalar_t(0.11420716837948833)*c_.b,
                      scalar_t(0.59619793162446932)*c_.r-scalar_t(0.27501215228975523)*c_.g-scalar_t(0.32118577933471409)*c_.b,
                      scalar_t(0.21158684041481168)*c_.r-scalar_t(0.52313197234453446)*c_.g+scalar_t(0.31154513192972272)*c_.b);
}
//----

template<typename T>
EGL_INLINE color_rgb<T> yiq_to_srgb(const color_yiq<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_rgb<T>(c_.y+scalar_t(0.956)*c_.i+scalar_t(0.619)*c_.q,
                      c_.y-scalar_t(0.272)*c_.i-scalar_t(0.647)*c_.q,
                      c_.y-scalar_t(1.106)*c_.i+scalar_t(1.703)*c_.q);
}
//----

template<typename T>
EGL_INLINE color_yiqa<T> srgb_to_yiq(const color_rgba<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_yiqa<T>(scalar_t(0.29906252315023879)*c_.r+scalar_t(0.58673030847027274)*c_.g+scalar_t(0.11420716837948833)*c_.b,
                       scalar_t(0.59619793162446932)*c_.r-scalar_t(0.27501215228975523)*c_.g-scalar_t(0.32118577933471409)*c_.b,
                       scalar_t(0.21158684041481168)*c_.r-scalar_t(0.52313197234453446)*c_.g+scalar_t(0.31154513192972272)*c_.b,
                       c_.a);
}
//----

template<typename T>
EGL_INLINE color_rgba<T> yiq_to_srgb(const color_yiqa<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_rgba<T>(c_.y+scalar_t(0.956)*c_.i+scalar_t(0.619)*c_.q,
                       c_.y-scalar_t(0.272)*c_.i-scalar_t(0.647)*c_.q,
                       c_.y-scalar_t(1.106)*c_.i+scalar_t(1.703)*c_.q,
                       c_.a);
}
//----------------------------------------------------------------------------


//============================================================================
// RGB <-> HSV color space conversion
//============================================================================
template<typename T>
color_rgb<T> hsv_to_srgb(const color_hsv<T> &c_)
{
  // convert color from HSV->RGB
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT(c_.h>=0 && c_.h<=1);
  scalar_t f=c_.h*scalar_t(6.0);
  unsigned i=unsigned(f);
  f-=scalar_t(i);
  scalar_t p=c_.v*(1.0f-c_.s), q=c_.v*(1.0f-c_.s*f), t=p+c_.v*c_.s*f;
  const scalar_t clut[]={p, p, t, c_.v, c_.v, q, p, p, t, c_.v, c_.v};
  return color_rgb<T>(clut[i+4], clut[i+2], clut[i+0]);
}
//----

template<typename T>
color_hsv<T> srgb_to_hsv(const color_rgb<T> &c_)
{
  // convert color from HSV->RGB
  typedef typename math<T>::scalar_t scalar_t;
  if(c_.r>c_.g)
  {
    if(c_.r>c_.b)
    {
      if(c_.g<c_.b)
      {
        // max=r, min=g
        scalar_t d=c_.r-c_.g;
        return color_hsv<T>(scalar_t(1.0)-scalar_t(1.0/6.0)*(c_.b-c_.g)/d, d/c_.r);
      }
      // max=r, min=b
      scalar_t d=c_.r-c_.b;
      return color_hsv<T>(scalar_t(1.0/6.0)*(c_.g-c_.b)/d, d/c_.r, c_.r);
    }
    else
    {
      // max=b, min=g
      scalar_t d=c_.b-c_.g;
      return color_hsv<T>(scalar_t(1.0/6.0)*(scalar_t(4.0)+(c_.r-c_.g)/d), d/c_.b, c_.b);
    }
  }
  if(c_.g>c_.b)
  {
    // max=g, min=r/b
    scalar_t d=c_.g-(c_.r<c_.b?c_.r:c_.b);
    return color_hsv<T>(scalar_t(1.0/6.0)*(scalar_t(2.0)+(c_.b-c_.r)/d), d/c_.g, c_.g);
  }
  // max=b, min=r
  scalar_t d=c_.b-c_.r;
  return color_hsv<T>(d?scalar_t(1.0/6.0)*(scalar_t(4.0)-(c_.g-c_.r)/d):0, d/c_.b, c_.b);
}
//----

template<typename T>
color_rgba<T> hsv_to_srgb(const color_hsva<T> &c_)
{
  // convert color from HSV->RGB
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT(c_.h>=0 && c_.h<=1);
  scalar_t f=c_.h*scalar_t(6.0);
  unsigned i=unsigned(f);
  f-=scalar_t(i);
  scalar_t p=c_.v*(scalar_t(1.0)-c_.s), q=c_.v*(scalar_t(1.0)-c_.s*f), t=p+c_.v*c_.s*f;
  const scalar_t clut[]={p, p, t, c_.v, c_.v, q, p, p, t, c_.v, c_.v};
  return color_rgba<T>(clut[i+4], clut[i+2], clut[i+0], c_.a);
}
//----

template<typename T>
color_hsva<T> srgb_to_hsv(const color_rgba<T> &c_)
{
  // convert color from HSV->RGB
  typedef typename math<T>::scalar_t scalar_t;
  if(c_.r>c_.g)
  {
    if(c_.r>c_.b)
    {
      if(c_.g<c_.b)
      {
        // max=r, min=g
        scalar_t d=c_.r-c_.g;
        return color_hsva<T>(scalar_t(1.0)-scalar_t(1.0/6.0)*(c_.b-c_.g)/d, d/c_.r, c_.r, c_.a);
      }
      // max=r, min=b
      scalar_t d=c_.r-c_.b;
      return color_hsva<T>(scalar_t(1.0/6.0)*(c_.g-c_.b)/d, d/c_.r, c_.r, c_.a);
    }
    else
    {
      // max=b, min=g
      scalar_t d=c_.b-c_.g;
      return color_hsva<T>(scalar_t(1.0/6.0)*(scalar_t(4.0)+(c_.r-c_.g)/d), d/c_.b, c_.b, c_.a);
    }
  }
  if(c_.g>c_.b)
  {
    // max=g, min=r/b
    scalar_t d=c_.g-(c_.r<c_.b?c_.r:c_.b);
    return color_hsva<T>(scalar_t(1.0/6.0)*(scalar_t(2.0)+(c_.b-c_.r)/d), d/c_.g, c_.g, c_.a);
  }
  // max=b, min=r
  scalar_t d=c_.b-c_.r;
  return color_hsva<T>(d?scalar_t(1.0/6.0)*(scalar_t(4.0)-(c_.g-c_.r)/d):0, d/c_.b, c_.b, c_.a);
}
//----------------------------------------------------------------------------


//============================================================================
// sRGB <-> linear conversions
//============================================================================
template<typename T>
EGL_INLINE T linear_to_srgb(T v_)
{
  return v_<=T(0.0031308)?v_*T(12.92):T(1.055)*pow(v_, T(1.0/2.4))-T(0.055);
}
//----

template<typename T>
EGL_INLINE color_rgb<T> linear_to_srgb(const color_rgb<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_rgb<T>(c_.r<=scalar_t(0.0031308)?c_.r*scalar_t(12.92):scalar_t(1.055)*pow(c_.r, scalar_t(1.0/2.4))-scalar_t(0.055),
                      c_.g<=scalar_t(0.0031308)?c_.g*scalar_t(12.92):scalar_t(1.055)*pow(c_.g, scalar_t(1.0/2.4))-scalar_t(0.055),
                      c_.b<=scalar_t(0.0031308)?c_.b*scalar_t(12.92):scalar_t(1.055)*pow(c_.b, scalar_t(1.0/2.4))-scalar_t(0.055));
}
//----

template<typename T>
EGL_INLINE color_rgba<T> linear_to_srgb(const color_rgba<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_rgba<T>(c_.r<=scalar_t(0.0031308)?c_.r*scalar_t(12.92):scalar_t(1.055)*pow(c_.r, scalar_t(1.0/2.4))-scalar_t(0.055),
                       c_.g<=scalar_t(0.0031308)?c_.g*scalar_t(12.92):scalar_t(1.055)*pow(c_.g, scalar_t(1.0/2.4))-scalar_t(0.055),
                       c_.b<=scalar_t(0.0031308)?c_.b*scalar_t(12.92):scalar_t(1.055)*pow(c_.b, scalar_t(1.0/2.4))-scalar_t(0.055),
                       c_.a);
}
//----

template<typename T>
EGL_INLINE T srgb_to_linear(T v_)
{
  return v_<=T(0.04045)?v_/T(12.92):pow((v_+T(0.055))/T(1.055), T(2.4));
}
//----

template<typename T>
EGL_INLINE color_rgb<T> srgb_to_linear(const color_rgb<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_rgb<T>(c_.r<=scalar_t(0.04045)?c_.r/scalar_t(12.92):pow((c_.r+scalar_t(0.055))/scalar_t(1.055), scalar_t(2.4)),
                      c_.g<=scalar_t(0.04045)?c_.g/scalar_t(12.92):pow((c_.g+scalar_t(0.055))/scalar_t(1.055), scalar_t(2.4)),
                      c_.b<=scalar_t(0.04045)?c_.b/scalar_t(12.92):pow((c_.b+scalar_t(0.055))/scalar_t(1.055), scalar_t(2.4)));
}
//----

template<typename T>
EGL_INLINE color_rgba<T> srgb_to_linear(const color_rgba<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return color_rgba<T>(c_.r<=scalar_t(0.04045)?c_.r/scalar_t(12.92):pow((c_.r+scalar_t(0.055))/scalar_t(1.055), scalar_t(2.4)),
                       c_.g<=scalar_t(0.04045)?c_.g/scalar_t(12.92):pow((c_.g+scalar_t(0.055))/scalar_t(1.055), scalar_t(2.4)),
                       c_.b<=scalar_t(0.04045)?c_.b/scalar_t(12.92):pow((c_.b+scalar_t(0.055))/scalar_t(1.055), scalar_t(2.4)),
                       c_.a);
}
//----------------------------------------------------------------------------


//============================================================================
// luminance
//============================================================================
template<typename T>
typename math<T>::scalar_t EGL_INLINE luminance_srgb(const color_rgb<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return scalar_t(0.29893602129377533)*c_.r+scalar_t(0.58704307445112125)*c_.g+scalar_t(0.11402090425510331)*c_.b;
}
//----

template<typename T>
typename math<T>::scalar_t EGL_INLINE luminance_srgb(const color_rgba<T> &c_)
{
  typedef typename math<T>::scalar_t scalar_t;
  return scalar_t(0.29893602129377533)*c_.r+scalar_t(0.58704307445112125)*c_.g+scalar_t(0.11402090425510331)*c_.b;
}
//----------------------------------------------------------------------------


//============================================================================
// white_point
//============================================================================
template<typename T>
vec2<T> cie_white_point(e_cie_white_point wp_)
{
  typedef typename math<T>::scalar_t scalar_t;
  switch(wp_)
  {
    case ciewpnt_2deg_a:       return vec2<T>(scalar_t(0.44757), scalar_t(0.40745));
    case ciewpnt_2deg_b:       return vec2<T>(scalar_t(0.34842), scalar_t(0.35161));
    case ciewpnt_2deg_c:       return vec2<T>(scalar_t(0.31006), scalar_t(0.31616));
    case ciewpnt_2deg_d50:     return vec2<T>(scalar_t(0.34567), scalar_t(0.35850));
    case ciewpnt_2deg_d55:     return vec2<T>(scalar_t(0.33242), scalar_t(0.34743));
    case ciewpnt_2deg_d60:     return vec2<T>(scalar_t(0.32168), scalar_t(0.33767));
    case ciewpnt_2deg_d65:     return vec2<T>(scalar_t(0.31271), scalar_t(0.32902));
    case ciewpnt_2deg_d65_p3t: return vec2<T>(scalar_t(0.314),   scalar_t(0.351));
    case ciewpnt_2deg_d75:     return vec2<T>(scalar_t(0.29902), scalar_t(0.31485));
    case ciewpnt_2deg_e:       return vec2<T>(scalar_t(1.0/3.0), scalar_t(1.0/3.0));
    case ciewpnt_2deg_f1:      return vec2<T>(scalar_t(0.31310), scalar_t(0.33727));
    case ciewpnt_2deg_f2:      return vec2<T>(scalar_t(0.37208), scalar_t(0.37529));
    case ciewpnt_2deg_f3:      return vec2<T>(scalar_t(0.40910), scalar_t(0.39430));
    case ciewpnt_2deg_f4:      return vec2<T>(scalar_t(0.44018), scalar_t(0.40329));
    case ciewpnt_2deg_f5:      return vec2<T>(scalar_t(0.31379), scalar_t(0.34531));
    case ciewpnt_2deg_f6:      return vec2<T>(scalar_t(0.37790), scalar_t(0.38835));
    case ciewpnt_2deg_f7:      return vec2<T>(scalar_t(0.31292), scalar_t(0.32933));
    case ciewpnt_2deg_f8:      return vec2<T>(scalar_t(0.34588), scalar_t(0.35875));
    case ciewpnt_2deg_f9:      return vec2<T>(scalar_t(0.37417), scalar_t(0.37281));
    case ciewpnt_2deg_f10:     return vec2<T>(scalar_t(0.34609), scalar_t(0.35986));
    case ciewpnt_2deg_f11:     return vec2<T>(scalar_t(0.38052), scalar_t(0.37713));
    case ciewpnt_2deg_f12:     return vec2<T>(scalar_t(0.43695), scalar_t(0.40441));
    case ciewpnt_10deg_a:      return vec2<T>(scalar_t(0.45117), scalar_t(0.40594));
    case ciewpnt_10deg_b:      return vec2<T>(scalar_t(0.34980), scalar_t(0.35270));
    case ciewpnt_10deg_c:      return vec2<T>(scalar_t(0.31039), scalar_t(0.31905));
    case ciewpnt_10deg_d50:    return vec2<T>(scalar_t(0.34773), scalar_t(0.35952));
    case ciewpnt_10deg_d55:    return vec2<T>(scalar_t(0.33411), scalar_t(0.34877));
    case ciewpnt_10deg_d65:    return vec2<T>(scalar_t(0.31382), scalar_t(0.33100));
    case ciewpnt_10deg_d75:    return vec2<T>(scalar_t(0.29968), scalar_t(0.31740));
    case ciewpnt_10deg_e:      return vec2<T>(scalar_t(1.0/3.0), scalar_t(1.0/3.0));
    case ciewpnt_10deg_f1:     return vec2<T>(scalar_t(0.31811), scalar_t(0.33559));
    case ciewpnt_10deg_f2:     return vec2<T>(scalar_t(0.37925), scalar_t(0.36733));
    case ciewpnt_10deg_f3:     return vec2<T>(scalar_t(0.41761), scalar_t(0.38324));
    case ciewpnt_10deg_f4:     return vec2<T>(scalar_t(0.44920), scalar_t(0.39074));
    case ciewpnt_10deg_f5:     return vec2<T>(scalar_t(0.31975), scalar_t(0.34246));
    case ciewpnt_10deg_f6:     return vec2<T>(scalar_t(0.38660), scalar_t(0.37847));
    case ciewpnt_10deg_f7:     return vec2<T>(scalar_t(0.31569), scalar_t(0.32960));
    case ciewpnt_10deg_f8:     return vec2<T>(scalar_t(0.34902), scalar_t(0.35939));
    case ciewpnt_10deg_f9:     return vec2<T>(scalar_t(0.37829), scalar_t(0.37045));
    case ciewpnt_10deg_f10:    return vec2<T>(scalar_t(0.35090), scalar_t(0.35444));
    case ciewpnt_10deg_f11:    return vec2<T>(scalar_t(0.38541), scalar_t(0.37123));
    case ciewpnt_10deg_f12:    return vec2<T>(scalar_t(0.44256), scalar_t(0.39717));
  }
  EGL_ERROR("Unknown white point.");
  return vec2<T>(0, 0);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
