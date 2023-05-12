//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_MATH_H
#define EGL_MATH_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_core.h"
#ifdef ARDUINO
#include <math.h>
#define MATH_NAMESPACE
#else
#include <cmath>
#define MATH_NAMESPACE std
#endif
EGL_NAMESPACE_BEGIN

// new
#ifdef abs
#undef abs
#endif
template<typename> struct math;     // math constants
template<typename> struct vec2;     // 2d vector
template<typename> struct vec3;     // 3d vector
template<typename> struct vec4;     // 4d vector
template<typename> struct mat22;    // 2x2 matrix
template<typename> struct mat33;    // 3x3 matrix
template<typename> struct mat44;    // 4x4 matrix
template<typename> struct quat;     // quaternion
template<typename> struct complex;  // complex number
struct vec32s_t;
struct vec32u_t;
typedef math<float> mathf;
typedef math<double> mathd;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;
typedef vec2<unsigned> vec2u;
typedef vec2<int16_t> vec2i16;
typedef vec2<uint16_t> vec2u16;
typedef vec2<int32_t> vec2i32;
typedef vec2<uint32_t> vec2u32;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<int> vec3i;
typedef vec3<unsigned> vec3u;
typedef vec3<int16_t> vec3i16;
typedef vec3<uint16_t> vec3u16;
typedef vec3<int32_t> vec3i32;
typedef vec3<uint32_t> vec3u32;
typedef vec3<vec32s_t> vec3_32s;
typedef vec3<vec32u_t> vec3_32u;
typedef vec4<float> vec4f;
typedef vec4<double> vec4d;
typedef vec4<int> vec4i;
typedef vec4<unsigned> vec4u;
typedef vec4<int16_t> vec4i16;
typedef vec4<uint16_t> vec4u16;
typedef vec4<int32_t> vec4i32;
typedef vec4<uint32_t> vec4u32;
typedef vec4<vec32s_t> vec4_32s;
typedef vec4<vec32u_t> vec4_32u;
typedef mat22<float> mat22f;
typedef mat22<double> mat22d;
typedef mat22<int> mat22i;
typedef mat22<unsigned> mat22u;
typedef mat22<int16_t> mat22i16;
typedef mat22<uint16_t> mat22u16;
typedef mat22<int32_t> mat22i32;
typedef mat22<uint32_t> mat22u32;
typedef mat33<float> mat33f;
typedef mat33<double> mat33d;
typedef mat33<int> mat33i;
typedef mat33<unsigned> mat33u;
typedef mat33<int16_t> mat33i16;
typedef mat33<uint16_t> mat33u16;
typedef mat33<int32_t> mat33i32;
typedef mat33<uint32_t> mat33u32;
typedef mat44<float> mat44f;
typedef mat44<double> mat44d;
typedef mat44<int> mat44i;
typedef mat44<unsigned> mat44u;
typedef mat44<int16_t> mat44i16;
typedef mat44<uint16_t> mat44u16;
typedef mat44<int32_t> mat44i32;
typedef mat44<uint32_t> mat44u32;
typedef quat<float> quatf;
typedef quat<double> quatd;
typedef quat<int> quati;
typedef quat<unsigned> quatu;
typedef quat<int16_t> quati16;
typedef quat<uint16_t> quatu16;
typedef quat<int32_t> quati32;
typedef quat<uint32_t> quatu32;
typedef quat<vec32s_t> quat_32s;
typedef quat<vec32u_t> quat_32u;
typedef complex<float> complexf;
typedef complex<double> complexd;
typedef complex<int> complexi;
typedef complex<unsigned> complexu;
typedef complex<int16_t> complexi16;
typedef complex<uint16_t> complexu16;
typedef complex<int32_t> complexi32;
typedef complex<uint32_t> complexu32;
// traits
template<typename> struct is_type_vec {enum {res=false};};
template<typename> struct is_type_mat {enum {res=false};};
template<typename> struct is_type_quat {enum {res=false};};
template<typename> struct is_type_complex {enum {res=false};};
// macro to prefer egl free math functions in function scope, i.e. use EGL_NAMESPACE_PREFIX acos() instead of ::acos() when writing: float a=acos(b);
#define EGL_FUNC_PREFER_MATH_LIB\
  EGL_USING_NAMESPACE\
  using EGL_NAMESPACE_PREFIX mul; using EGL_NAMESPACE_PREFIX madd; using EGL_NAMESPACE_PREFIX rcp; using EGL_NAMESPACE_PREFIX rcp_z; using EGL_NAMESPACE_PREFIX floor; using EGL_NAMESPACE_PREFIX ceil; using EGL_NAMESPACE_PREFIX trunc; using EGL_NAMESPACE_PREFIX frc; using EGL_NAMESPACE_PREFIX mod;\
  using EGL_NAMESPACE_PREFIX cycle; using EGL_NAMESPACE_PREFIX cycle1; using EGL_NAMESPACE_PREFIX sat; using EGL_NAMESPACE_PREFIX ssat; using EGL_NAMESPACE_PREFIX clamp; using EGL_NAMESPACE_PREFIX abs; using EGL_NAMESPACE_PREFIX sgn; using EGL_NAMESPACE_PREFIX sgn_zp; using EGL_NAMESPACE_PREFIX sqr; using EGL_NAMESPACE_PREFIX cubic;\
  using EGL_NAMESPACE_PREFIX sqrt; using EGL_NAMESPACE_PREFIX sqrt_z; using EGL_NAMESPACE_PREFIX cbrt; using EGL_NAMESPACE_PREFIX rsqrt; using EGL_NAMESPACE_PREFIX rsqrt_z; using EGL_NAMESPACE_PREFIX rcbrt; using EGL_NAMESPACE_PREFIX rcbrt_z; using EGL_NAMESPACE_PREFIX norm;\
  using EGL_NAMESPACE_PREFIX rnorm; using EGL_NAMESPACE_PREFIX rnorm_z; using EGL_NAMESPACE_PREFIX rnorm_z; using EGL_NAMESPACE_PREFIX norm2; using EGL_NAMESPACE_PREFIX rnorm2; using EGL_NAMESPACE_PREFIX rnorm2_z; using EGL_NAMESPACE_PREFIX norm_l1; using EGL_NAMESPACE_PREFIX rnorm_l1;\
  using EGL_NAMESPACE_PREFIX rnorm_l1_z; using EGL_NAMESPACE_PREFIX exp; using EGL_NAMESPACE_PREFIX exp2; using EGL_NAMESPACE_PREFIX ln; using EGL_NAMESPACE_PREFIX log2; using EGL_NAMESPACE_PREFIX log10; using EGL_NAMESPACE_PREFIX pow; using EGL_NAMESPACE_PREFIX smoothstep; using EGL_NAMESPACE_PREFIX smootherstep; using EGL_NAMESPACE_PREFIX lerp; using EGL_NAMESPACE_PREFIX dot;\
  using EGL_NAMESPACE_PREFIX sin; using EGL_NAMESPACE_PREFIX cos; using EGL_NAMESPACE_PREFIX sincos; using EGL_NAMESPACE_PREFIX tan; using EGL_NAMESPACE_PREFIX csc; using EGL_NAMESPACE_PREFIX sec; using EGL_NAMESPACE_PREFIX cot; using EGL_NAMESPACE_PREFIX asin; using EGL_NAMESPACE_PREFIX acos;\
  using EGL_NAMESPACE_PREFIX atan; using EGL_NAMESPACE_PREFIX atan2; using EGL_NAMESPACE_PREFIX acsc; using EGL_NAMESPACE_PREFIX asec; using EGL_NAMESPACE_PREFIX acot; using EGL_NAMESPACE_PREFIX sinh; using EGL_NAMESPACE_PREFIX cosh; using EGL_NAMESPACE_PREFIX tanh; using EGL_NAMESPACE_PREFIX csch;\
  using EGL_NAMESPACE_PREFIX sech; using EGL_NAMESPACE_PREFIX coth; using EGL_NAMESPACE_PREFIX asinh; using EGL_NAMESPACE_PREFIX acosh; using EGL_NAMESPACE_PREFIX atanh; using EGL_NAMESPACE_PREFIX acsch; using EGL_NAMESPACE_PREFIX asech; using EGL_NAMESPACE_PREFIX acoth;
// scalar functions
template<typename T> EGL_INLINE T mul(T a_, T b_);             // a_*b_
template<typename T> EGL_INLINE T madd(T a_, T b_, T c_);      // a_*b_+c_
template<typename T> EGL_INLINE T rcp(T);                      // reciprocal: 1/x
template<typename T> EGL_INLINE T rcp_z(T);                    // reciprocal: 1/x, if x=0 returns 0
template<typename T> EGL_INLINE T floor(T v_);                 // greatest integer less than or equal to the value
template<typename T> EGL_INLINE T ceil(T v_);                  // smallest integer greater than or equal to the value
template<typename T> EGL_INLINE T trunc(T v_);                 // truncate decimal part from the value
template<typename T> EGL_INLINE T frc(T v_);                   // fractional part (for negative values returns 1-fractional part)
template<typename T> EGL_INLINE T mod(T v_, T div_);           // remainder of v_/div_
template<typename T> EGL_INLINE T cycle(T v_, T cycle_);       // same as mod() except for negative result returns: cycle_+mod(v_, cycle_)
template<typename T> EGL_INLINE T cycle1(T v_);                // same as frc() but for negative results returns: 1+frc(v_)
template<typename T> EGL_INLINE T sat(T);                      // saturate scalar to range [0, 1]
template<typename T> EGL_INLINE T ssat(T);                     // saturate scalar to range [-1, 1]
template<typename T> EGL_INLINE T clamp(T v_, T min_, T max_); // clamp scalar to range [min, max]
template<typename T> EGL_INLINE T abs(T);                      // absolute: |x|
template<typename T> EGL_INLINE T sgn(T);                      // signum (neg=-1, pos=+1, zero=0)
template<typename T> EGL_INLINE T sgn_zp(T);                   // signum (neg=-1, pos&zero=+1)
template<typename T> EGL_INLINE T sqr(T);                      // square: x*x
template<typename T> EGL_INLINE T cubic(T);                    // cubic: x*x*x
template<typename T> EGL_INLINE T sqrt(T);                     // square root
template<typename T> EGL_INLINE T sqrt_z(T);                   // square root (set to 0 if the value is less than zero)
template<typename T> EGL_INLINE T cbrt(T);                     // cubic root
template<typename T> EGL_INLINE T rsqrt(T);                    // reciprocal square root: 1/sqrt(x)
template<typename T> EGL_INLINE T rsqrt_z(T);                  // reciprocal square root: 1/sqrt(x). set to 0 if the value is less than or equal to zero
template<typename T> EGL_INLINE T rcbrt(T);                    // reciprocal cubic root: 1/cbrt(x)
template<typename T> EGL_INLINE T rcbrt_z(T);                  // reciprocal cubic root: 1/cbrt(x). set to 0 if the value is zero
template<typename T> EGL_INLINE T norm(T);                     // absolute: |x|
template<typename T> EGL_INLINE T rnorm(T);                    // absolute reciprocal: 1/|x|
template<typename T> EGL_INLINE T rnorm_z(T);                  // absolute reciprocal: 1/|x| (if x=0, returns 0)
template<typename T> EGL_INLINE T norm2(T);                    // square: x*x
template<typename T> EGL_INLINE T rnorm2(T);                   // square reciprocal: 1/(x*x)
template<typename T> EGL_INLINE T rnorm2_z(T);                 // square reciprocal: 1/(x*x) (if x=0, returns 0)
template<typename T> EGL_INLINE T norm_l1(T);                  // absolute: |x|
template<typename T> EGL_INLINE T rnorm_l1(T);                 // absolute reciprocal: 1/|x|
template<typename T> EGL_INLINE T rnorm_l1_z(T);               // absolute reciprocal: 1/|x| (if x=0, returns 0)
template<typename T> EGL_INLINE T exp(T);                      // natural exponent: e^x
template<typename T> EGL_INLINE T exp2(T);                     // 2^x
template<typename T> EGL_INLINE T ln(T);                       // natural logarithm
template<typename T> EGL_INLINE T log2(T);                     // base-2 logarithm
template<typename T> EGL_INLINE T log10(T);                    // base-10 logarithm
template<typename T> EGL_INLINE T pow(T, T);                   // power
template<typename T> EGL_INLINE T smoothstep(T t_);            // smoothstep function: t=[0, 1]
template<typename T> EGL_INLINE T smootherstep(T t_);          // smootherstep function: t=[0, 1]
template<typename T> EGL_INLINE T lerp(T a_, T b_, T t_);      // linear interpolation of scalars: f(t=0)=a_, f(t=1)=b_
template<typename T> EGL_INLINE T dot(T x_, T y_);             // x_*y_
EGL_INLINE float32_t u16f_to_fp32(uint16_t u16f_);             // convert unsigned 16-bit float to 32-bit float
EGL_INLINE float32_t s16f_to_fp32(uint16_t s16f_);             // convert signed 16-bit float to 32-bit float
EGL_INLINE uint16_t fp32_to_u16f(float32_t v_);                // convert 32-bit float to unsigned 16-bit float
EGL_INLINE uint16_t fp32_to_s16f(float32_t v_);                // convert 32-bit float to signed 16-bit float
// trigonometric scalar functions
template<typename T> EGL_INLINE T sin(T);                         // sine
template<typename T> EGL_INLINE T cos(T);                         // cosine
template<typename T> EGL_INLINE void sincos(T &sin_, T &cos_, T); // sine & cosine
template<typename T> EGL_INLINE T tan(T);                         // tangent
template<typename T> EGL_INLINE T csc(T);                         // cosecant: 1/sin(x)
template<typename T> EGL_INLINE T sec(T);                         // secant: 1/cos(x)
template<typename T> EGL_INLINE T cot(T);                         // cotangent: 1/tan(x)
template<typename T> EGL_INLINE T asin(T);                        // arcus sine
template<typename T> EGL_INLINE T acos(T);                        // arcus cosine
template<typename T> EGL_INLINE T atan(T);                        // arcus tangent
template<typename T> EGL_INLINE T atan2(T y_, T x_);              // arcus tangent
template<typename T> EGL_INLINE T acsc(T);                        // arcus cosecant
template<typename T> EGL_INLINE T asec(T);                        // arcus secant
template<typename T> EGL_INLINE T acot(T);                        // arcus cotangent
template<typename T> EGL_INLINE T sinh(T);                        // hyperbolic sine
template<typename T> EGL_INLINE T cosh(T);                        // hyperbolic cosine
template<typename T> EGL_INLINE T tanh(T);                        // hyperbolic tangent
template<typename T> EGL_INLINE T csch(T);                        // hyperbolic cosecant
template<typename T> EGL_INLINE T sech(T);                        // hyperbolic secant
template<typename T> EGL_INLINE T coth(T);                        // hyperbolic cotangent
template<typename T> EGL_INLINE T asinh(T);                       // arcus hyperbolic sine
template<typename T> EGL_INLINE T acosh(T);                       // arcus hyperbolic cosine
template<typename T> EGL_INLINE T atanh(T);                       // arcus hyperbolic tangent
template<typename T> EGL_INLINE T acsch(T);                       // arcus hyperbolic cosecant
template<typename T> EGL_INLINE T asech(T);                       // arcus hyperbolic secant
template<typename T> EGL_INLINE T acoth(T);                       // arcus hyperbolic cotangent
// 2d vector ops
template<typename T> EGL_INLINE bool is_zero(const vec2<T>&);                                     // test for zero-vector, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const vec2<T>&);                                      // test for saturated vector, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const vec2<T>&);                                     // test for signed saturated vector, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const vec2<T>&, const vec2<T>&);                  // test for equality of vectors, i.e. all components of the vectors are equal (exact)
template<typename T> EGL_INLINE bool operator==(const vec2<T>&, typename math<T>::scalar_t);      // test for equality of vector and scalar, i.e. all components of the vector equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const vec2<T>&);      // test for equality of vector and scalar, i.e. all components of the vector equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const vec2<T>&, const vec2<T>&);                  // test for inequality of vectors, i.e. any of the components of the vectors are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const vec2<T>&, typename math<T>::scalar_t);      // test for inequality of vector and scalar, i.e. any of the components of the vector is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const vec2<T>&);      // test for inequality of vector and scalar, i.e. any of the components of the vector is unequal to the scalar (exact)
template<typename T> EGL_INLINE vec2<T> operator<(const vec2<T>&, const vec2<T>&);                // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator<(const vec2<T>&, typename math<T>::scalar_t);    // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator<(typename math<T>::scalar_t, const vec2<T>&);    // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator>(const vec2<T>&, const vec2<T>&);                // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator>(const vec2<T>&, typename math<T>::scalar_t);    // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator>(typename math<T>::scalar_t, const vec2<T>&);    // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator<=(const vec2<T>&, const vec2<T>&);               // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator<=(const vec2<T>&, typename math<T>::scalar_t);   // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator<=(typename math<T>::scalar_t, const vec2<T>&);   // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator>=(const vec2<T>&, const vec2<T>&);               // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator>=(const vec2<T>&, typename math<T>::scalar_t);   // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> operator>=(typename math<T>::scalar_t, const vec2<T>&);   // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec2<T> sel_eq(const vec2<T> &t0_, const vec2<T> &t1_,            // component-wise equal vector select (t0_==t1_?v0_:v1_)
                                               const vec2<T> &v0_, const vec2<T> &v1_);
template<typename T> EGL_INLINE vec2<T> sel_lt(const vec2<T> &t0_, const vec2<T> &t1_,            // component-wise less-than vector select (t0_<t1_?v0_:v1_)
                                               const vec2<T> &v0_, const vec2<T> &v1_);
template<typename T> EGL_INLINE vec2<T> sel_le(const vec2<T> &t0_, const vec2<T> &t1_,            // component-wise less-or-equal vector select (t0_<=t1_?v0_:v1_)
                                               const vec2<T> &v0_, const vec2<T> &v1_);
template<typename T> EGL_INLINE vec2<T> sel_eqz(const vec2<T> &t_,                                // component-wise equal-to-zero vector select (t_==0?v0_:v1_)
                                                const vec2<T> &v0_, const vec2<T> &v1_);
template<typename T> EGL_INLINE vec2<T> sel_ltz(const vec2<T> &t_,                                // component-wise less-than-zero vector select (t_<0?v0_:v1_)
                                                const vec2<T> &v0_, const vec2<T> &v1_);
template<typename T> EGL_INLINE vec2<T> sel_lez(const vec2<T> &t_,                                // component-wise less-or-equal-to-zero vector select (t_<=0?v0_:v1_)
                                                const vec2<T> &v0_, const vec2<T> &v1_);
template<typename T> EGL_INLINE void operator+=(vec2<T>&, const vec2<T>&);                        // add vector to vector
template<typename T> EGL_INLINE void operator+=(vec2<T>&, typename math<T>::scalar_t);            // add scalar to each component of the vector
template<typename T> EGL_INLINE void operator-=(vec2<T>&, const vec2<T>&);                        // subtract vector from vector
template<typename T> EGL_INLINE void operator-=(vec2<T>&, typename math<T>::scalar_t);            // subtract scalar from each component of the vector
template<typename T> EGL_INLINE void operator*=(vec2<T>&, const vec2<T>&);                        // component-wise vector by vector multiplication
template<typename T> EGL_INLINE void operator*=(vec2<T>&, typename math<T>::scalar_t);            // vector by scalar multiplication
template<typename T> EGL_INLINE void operator*=(vec2<T>&, const mat22<T>&);                       // vector by matrix multiplication
template<typename T> EGL_INLINE void operator/=(vec2<T>&, const vec2<T>&);                        // component-wise vector by vector division
template<typename T> EGL_INLINE void operator/=(vec2<T>&, typename math<T>::scalar_t);            // vector by scalar division
template<typename T> EGL_INLINE vec2<T> operator+(const vec2<T>&, const vec2<T>&);                // add vector to vector
template<typename T> EGL_INLINE vec2<T> operator+(const vec2<T>&, typename math<T>::scalar_t);    // add scalar to each component of the vector
template<typename T> EGL_INLINE vec2<T> operator+(typename math<T>::scalar_t, const vec2<T>&);    // add each component of the vector to a scalar vector
template<typename T> EGL_INLINE vec2<T> operator-(const vec2<T>&, const vec2<T>&);                // subtract vector from vector
template<typename T> EGL_INLINE vec2<T> operator-(const vec2<T>&, typename math<T>::scalar_t);    // subtract scalar from each component of the vector
template<typename T> EGL_INLINE vec2<T> operator-(typename math<T>::scalar_t, const vec2<T>&);    // subtract each component of the vector from a scalar vector
template<typename T> EGL_INLINE vec2<T> operator-(const vec2<T>&);                                // negate the vector
template<typename T> EGL_INLINE vec2<T> operator*(const vec2<T>&, const vec2<T>&);                // component-wise vector by vector multiplication
template<typename T> EGL_INLINE vec2<T> operator*(const vec2<T>&, typename math<T>::scalar_t);    // vector by scalar multiplication
template<typename T> EGL_INLINE vec2<T> operator*(typename math<T>::scalar_t, const vec2<T>&);    // vector by scalar multiplication
template<typename T> EGL_INLINE vec2<T> operator*(const vec2<T>&, const mat22<T>&);               // vector by matrix multiplication
template<typename T> EGL_INLINE vec2<T> operator*(const mat22<T>&, const vec2<T>&);               // matrix by transposed vector multiplication
template<typename T> EGL_INLINE vec2<T> operator/(const vec2<T>&, const vec2<T>&);                // component-wise vector by vector division
template<typename T> EGL_INLINE vec2<T> operator/(const vec2<T>&, typename math<T>::scalar_t);    // vector by scalar division
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const vec2<T>&);                   // minimum component value
template<typename T> EGL_INLINE vec2<T> min(const vec2<T>&, const vec2<T>&);                      // component-wise minimum of the vectors
template<typename T> EGL_INLINE vec2<T> min(const vec2<T>&, const vec2<T>&, const vec2<T>&);
template<typename T> EGL_INLINE vec2<T> min(const vec2<T>&, const vec2<T>&, const vec2<T>&, const vec2<T>&);
template<typename T> EGL_INLINE vec2<T> min(const vec2<T>&, typename math<T>::scalar_t);          // component-wise minimum of the vector and scalar
template<typename T> EGL_INLINE vec2<T> min(typename math<T>::scalar_t, const vec2<T>&);          // component-wise minimum of the vector and scalar
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const vec2<T>&);                   // maximum component value
template<typename T> EGL_INLINE vec2<T> max(const vec2<T>&, const vec2<T>&);                      // component-wise maximum of the vectors
template<typename T> EGL_INLINE vec2<T> max(const vec2<T>&, const vec2<T>&, const vec2<T>&);
template<typename T> EGL_INLINE vec2<T> max(const vec2<T>&, const vec2<T>&, const vec2<T>&, const vec2<T>&);
template<typename T> EGL_INLINE vec2<T> max(const vec2<T>&, typename math<T>::scalar_t);          // component-wise maximum of the vector and scalar
template<typename T> EGL_INLINE vec2<T> max(typename math<T>::scalar_t, const vec2<T>&);          // component-wise maximum of the vector and scalar
template<typename T> EGL_INLINE vec2<T> mul(const vec2<T> &v0_, const vec2<T> &v1_);              // component-wise multiply: v0_*v1_
template<typename T> EGL_INLINE vec2<T> madd(const vec2<T> &v0_,                                  // component-wise multiply-add: v0_*v1+v2_
                                             const vec2<T> &v1_,
                                             const vec2<T> &v2_);
template<typename T> EGL_INLINE vec2<T> madd(const vec2<T> &v_,                                   // vector-scalar-scalar multiply-add: v_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE vec2<T> madd(const vec2<T> &v_,                                   // vector-vector-scalar multiply-add: v_*mul_+add_
                                             const vec2<T> &mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE vec2<T> madd(const vec2<T> &v_,                                   // vector-scalar-vector multiply-add: v_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             const vec2<T> &add_);
template<typename T> EGL_INLINE vec2<T> rcp(const vec2<T>&);                                      // component-wise vector reciprocal
template<typename T> EGL_INLINE vec2<T> rcp_z(const vec2<T>&);                                    // component-wise vector reciprocal (set 0 for 0 components)
template<typename T> EGL_INLINE vec2<T> floor(const vec2<T>&);                                    // component-wise greatest integer less than or equal to the value
template<typename T> EGL_INLINE vec2<T> ceil(const vec2<T>&);                                     // component-wise smallest integer greater than or equal to the value
template<typename T> EGL_INLINE vec2<T> trunc(const vec2<T>&);                                    // component-wise truncate decimal part from the value
template<typename T> EGL_INLINE vec2<T> frc(const vec2<T>&);                                      // component-wise fractional part (for negative values returns 1-fractional part)
template<typename T> EGL_INLINE vec2<T> mod(const vec2<T>&, typename math<T>::scalar_t div_);     // component-wise modulo
template<typename T> EGL_INLINE vec2<T> cycle(const vec2<T>&, typename math<T>::scalar_t cycle_); // component-wise cycle
template<typename T> EGL_INLINE vec2<T> cycle1(const vec2<T>&);                                   // component-wise cycle1
template<typename T> EGL_INLINE vec2<T> sat(const vec2<T>&);                                      // component-wise vector saturate
template<typename T> EGL_INLINE vec2<T> ssat(const vec2<T>&);                                     // component-wise vector signed saturate
template<typename T> EGL_INLINE vec2<T> clamp(const vec2<T>&,                                     // component-wise clamp of the vector
                                              const vec2<T> &min_,
                                              const vec2<T> &max_);
template<typename T> EGL_INLINE vec2<T> clamp(const vec2<T>&,                                     // component-wise clamp of the vector with scalars
                                              typename math<T>::scalar_t min_,
                                              typename math<T>::scalar_t max_);
template<typename T> EGL_INLINE vec2<T> abs(const vec2<T>&);                                      // component-wise vector absolute
template<typename T> EGL_INLINE vec2<T> sgn(const vec2<T>&);                                      // component-wise signum (neg=-1, pos=+1, zero=0)
template<typename T> EGL_INLINE vec2<T> sgn_zp(const vec2<T>&);                                   // component-wise signum (neg=-1, pos&zero=+1)
template<typename T> EGL_INLINE vec2<T> sqr(const vec2<T>&);                                      // component-wise square: x*x
template<typename T> EGL_INLINE vec2<T> cubic(const vec2<T>&);                                    // component-wise cubic: x*x*x
template<typename T> EGL_INLINE vec2<T> sqrt(const vec2<T>&);                                     // component-wise square root
template<typename T> EGL_INLINE vec2<T> sqrt_z(const vec2<T>&);                                   // component-wise square root (set 0 for components less than zero)
template<typename T> EGL_INLINE vec2<T> cbrt(const vec2<T>&);                                     // component-wise cubic root
template<typename T> EGL_INLINE vec2<T> rsqrt(const vec2<T>&);                                    // component-wise vector reciprocal square root
template<typename T> EGL_INLINE vec2<T> rsqrt_z(const vec2<T>&);                                  // component-wise vector reciprocal square root (set 0 for components less than or equal to zero)
template<typename T> EGL_INLINE vec2<T> rcbrt(const vec2<T>&);                                    // component-wise reciprocal cubic root
template<typename T> EGL_INLINE vec2<T> rcbrt_z(const vec2<T>&);                                  // component-wise reciprocal cubic root (set 0 for components that are 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const vec2<T>&);                  // vector norm (length): |v|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const vec2<T>&);                 // reciprocal of the vector norm: 1/|v|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const vec2<T>&);               // reciprocal of the vector norm: 1/|v| (if |v|=0, return 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const vec2<T>&);                 // squared vector norm: |v|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const vec2<T>&);                // reciprocal of the squared vector norm: 1/|v|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const vec2<T>&);              // reciprocal of the squared vector norm: 1/|v|^2 (if |v|=0, return 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const vec2<T>&);               // vector L1-norm (Manhattan norm)
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const vec2<T>&);              // reciprocal of the vector L1-norm
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const vec2<T>&);            // reciprocal of the vector L1-norm (if |v|=0, return 0)
template<typename T> EGL_INLINE vec2<T> exp(const vec2<T>&);                                      // component-wise natural exponent: e^x
template<typename T> EGL_INLINE vec2<T> exp2(const vec2<T>&);                                     // component-wise 2^x
template<typename T> EGL_INLINE vec2<T> ln(const vec2<T>&);                                       // component-wise natural logarithm
template<typename T> EGL_INLINE vec2<T> log2(const vec2<T>&);                                     // component-wise base-2 logarithm
template<typename T> EGL_INLINE vec2<T> log10(const vec2<T>&);                                    // component-wise base-10 logarithm
template<typename T> EGL_INLINE vec2<T> pow(const vec2<T>&, typename math<T>::scalar_t);          // component-wise power
template<typename T> EGL_INLINE vec2<T> unit(const vec2<T>&);                                     // unit vector of the vector: v/|v|
template<typename T> EGL_INLINE vec2<T> unit_z(const vec2<T>&);                                   // unit vector of the vector (if |v|=0, return v=[0, 0])
template<typename T> EGL_INLINE typename math<T>::scalar_t cross(const vec2<T>&, const vec2<T>&); // 2d vector cross-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const vec2<T>&, const vec2<T>&);   // vector dot-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const vec2<T>&);                  // vector dot-product with vector [1, 1] (component sum)
template<typename T> EGL_INLINE void neg(vec2<T>&);                                               // negate the vector
template<typename T, class Rng> EGL_INLINE void rand_u(vec2<T>&, Rng&);                           // random unit vector
template<typename T, class Rng> EGL_INLINE void rand_real1(vec2<T>&, Rng&);                       // random vector where each component is in range [-1, 1]
template<typename T, class Rng> EGL_INLINE void rand_ureal1(vec2<T>&, Rng&);                      // random vector where each component is in range [0, 1]
template<typename T> EGL_INLINE vec2<T> smoothstep(const vec2<T>&);                               // component-wise smoothstep function
template<typename T> EGL_INLINE vec2<T> smootherstep(const vec2<T>&);                             // component-wise smootherstep function
template<typename T> EGL_INLINE vec2<T> lerp(const vec2<T> &v0_,                                  // linear interpolation of vectors: f(t=0)=v0, f(t=1)=v1
                                             const vec2<T> &v1_,
                                             typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE vec2<T> reflect(const vec2<T> &v_, const vec2<T> &n_);            // reflect vector about normal
template<typename T> EGL_INLINE vec2<T> reflect_u(const vec2<T> &v_, const vec2<T> &n_);          // reflect vector about unit normal
template<typename T> EGL_INLINE typename math<T>::scalar_t perspective_project(const vec2<T>&);   // perspective project vector (divide by last component)
template<typename T, typename U> EGL_INLINE vec2<T> to_vec2(const vec2<U>&);                      // convert between vec2 types
template<unsigned shuffle_, typename T> EGL_INLINE vec2<T> shuffle(const vec2<T>&);               // shuffle vector components (e.g. shuffle<11>(v_) == vec2f(v_.y, v_.y))
// 3d vector ops
template<typename T> EGL_INLINE bool is_zero(const vec3<T>&);                                     // test for zero-vector, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const vec3<T>&);                                      // test for saturated vector, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const vec3<T>&);                                     // test for signed saturated vector, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const vec3<T>&, const vec3<T>&);                  // test for equality of vectors, i.e. all components of the vectors are equal (exact)
template<typename T> EGL_INLINE bool operator==(const vec3<T>&, typename math<T>::scalar_t);      // test for equality of vector and scalar, i.e. all components of the vector equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const vec3<T>&);      // test for equality of vector and scalar, i.e. all components of the vector equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const vec3<T>&, const vec3<T>&);                  // test for inequality of vectors, i.e. any of the components of the vectors are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const vec3<T>&, typename math<T>::scalar_t);      // test for inequality of vector and scalar, i.e. any of the components of the vector is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const vec3<T>&);      // test for inequality of vector and scalar, i.e. any of the components of the vector is unequal to the scalar (exact)
template<typename T> EGL_INLINE vec3<T> operator<(const vec3<T>&, const vec3<T>&);                // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator<(const vec3<T>&, typename math<T>::scalar_t);    // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator<(typename math<T>::scalar_t, const vec3<T>&);    // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator>(const vec3<T>&, const vec3<T>&);                // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator>(const vec3<T>&, typename math<T>::scalar_t);    // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator>(typename math<T>::scalar_t, const vec3<T>&);    // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator<=(const vec3<T>&, const vec3<T>&);               // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator<=(const vec3<T>&, typename math<T>::scalar_t);   // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator<=(typename math<T>::scalar_t, const vec3<T>&);   // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator>=(const vec3<T>&, const vec3<T>&);               // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator>=(const vec3<T>&, typename math<T>::scalar_t);   // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> operator>=(typename math<T>::scalar_t, const vec3<T>&);   // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec3<T> sel_eq(const vec3<T> &t0_, const vec3<T> &t1_,            // component-wise equal vector select (t0_==t1_?v0_:v1_)
                                               const vec3<T> &v0_, const vec3<T> &v1_);
template<typename T> EGL_INLINE vec3<T> sel_lt(const vec3<T> &t0_, const vec3<T> &t1_,            // component-wise less-than vector select (t0_<t1_?v0_:v1_)
                                               const vec3<T> &v0_, const vec3<T> &v1_);
template<typename T> EGL_INLINE vec3<T> sel_le(const vec3<T> &t0_, const vec3<T> &t1_,            // component-wise less-or-equal vector select (t0_<=t1_?v0_:v1_)
                                               const vec3<T> &v0_, const vec3<T> &v1_);
template<typename T> EGL_INLINE vec3<T> sel_eqz(const vec3<T> &t_,                                // component-wise equal-to-zero vector select (t_==0?v0_:v1_)
                                                const vec3<T> &v0_, const vec3<T> &v1_);
template<typename T> EGL_INLINE vec3<T> sel_ltz(const vec3<T> &t_,                                // component-wise less-than-zero vector select (t_<0?v0_:v1_)
                                                const vec3<T> &v0_, const vec3<T> &v1_);
template<typename T> EGL_INLINE vec3<T> sel_lez(const vec3<T> &t_,                                // component-wise less-or-equal-to-zero vector select (t_<=0?v0_:v1_)
                                                const vec3<T> &v0_, const vec3<T> &v1_);
template<typename T> EGL_INLINE void operator+=(vec3<T>&, const vec3<T>&);                        // add vector to vector
template<typename T> EGL_INLINE void operator+=(vec3<T>&, typename math<T>::scalar_t);            // add scalar to each component of the vector
template<typename T> EGL_INLINE void operator-=(vec3<T>&, const vec3<T>&);                        // subtract vector from vector
template<typename T> EGL_INLINE void operator-=(vec3<T>&, typename math<T>::scalar_t);            // subtract scalar from each component of the vector
template<typename T> EGL_INLINE void operator*=(vec3<T>&, const vec3<T>&);                        // component-wise vector by vector multiplication
template<typename T> EGL_INLINE void operator*=(vec3<T>&, typename math<T>::scalar_t);            // vector by scalar multiplication
template<typename T> EGL_INLINE void operator*=(vec3<T>&, const mat33<T>&);                       // vector by matrix multiplication
template<typename T> EGL_INLINE void operator*=(vec3<T>&, const mat44<T>&);                       // vector by matrix multiplication (assume vector w=1)
template<typename T> EGL_INLINE void operator*=(vec3<T>&, const quat<T>&);                        // vector by quaternion multiplication
template<typename T> EGL_INLINE void operator/=(vec3<T>&, const vec3<T>&);                        // component-wise vector by vector division
template<typename T> EGL_INLINE void operator/=(vec3<T>&, typename math<T>::scalar_t);            // vector by scalar division
template<typename T> EGL_INLINE vec3<T> operator+(const vec3<T>&, const vec3<T>&);                // add vector to vector
template<typename T> EGL_INLINE vec3<T> operator+(const vec3<T>&, typename math<T>::scalar_t);    // add scalar to each component of the vector
template<typename T> EGL_INLINE vec3<T> operator+(typename math<T>::scalar_t, const vec3<T>&);    // add each component of the vector to a scalar vector
template<typename T> EGL_INLINE vec3<T> operator-(const vec3<T>&, const vec3<T>&);                // subtract vector from vector
template<typename T> EGL_INLINE vec3<T> operator-(const vec3<T>&, typename math<T>::scalar_t);    // subtract scalar from each component of the vector
template<typename T> EGL_INLINE vec3<T> operator-(typename math<T>::scalar_t, const vec3<T>&);    // subtract each component of the vector from a scalar vector
template<typename T> EGL_INLINE vec3<T> operator-(const vec3<T>&);                                // negate the vector
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, const vec3<T>&);                // component-wise vector by vector multiplication
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, typename math<T>::scalar_t);    // vector by scalar multiplication
template<typename T> EGL_INLINE vec3<T> operator*(typename math<T>::scalar_t, const vec3<T>&);    // vector by scalar multiplication
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, const mat33<T>&);               // vector by matrix multiplication
template<typename T> EGL_INLINE vec3<T> operator*(const mat33<T>&, const vec3<T>&);               // matrix by transposed vector multiplication
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, const mat44<T>&);               // vector by matrix multiplication (assume vector w=1)
template<typename T> EGL_INLINE vec3<T> operator*(const mat44<T>&, const vec3<T>&);               // matrix by transposed vector multiplication (assume vector w=1)
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, const quat<T>&);                // vector by quaternion multiplication
template<typename T> EGL_INLINE vec3<T> operator/(const vec3<T>&, const vec3<T>&);                // component-wise vector by vector division
template<typename T> EGL_INLINE vec3<T> operator/(const vec3<T>&, typename math<T>::scalar_t);    // vector by scalar division
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const vec3<T>&);                   // minimum component value
template<typename T> EGL_INLINE vec3<T> min(const vec3<T>&, const vec3<T>&);                      // component-wise minimum of the vectors
template<typename T> EGL_INLINE vec3<T> min(const vec3<T>&, const vec3<T>&, const vec3<T>&);
template<typename T> EGL_INLINE vec3<T> min(const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
template<typename T> EGL_INLINE vec3<T> min(const vec3<T>&, typename math<T>::scalar_t);          // component-wise minimum of the vector and scalar
template<typename T> EGL_INLINE vec3<T> min(typename math<T>::scalar_t, const vec3<T>&);          // component-wise minimum of the vector and scalar
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const vec3<T>&);                   // maximum component value
template<typename T> EGL_INLINE vec3<T> max(const vec3<T>&, const vec3<T>&);                      // component-wise maximum of the vectors
template<typename T> EGL_INLINE vec3<T> max(const vec3<T>&, const vec3<T>&, const vec3<T>&);
template<typename T> EGL_INLINE vec3<T> max(const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
template<typename T> EGL_INLINE vec3<T> max(const vec3<T>&, typename math<T>::scalar_t);          // component-wise maximum of the vector and scalar
template<typename T> EGL_INLINE vec3<T> max(typename math<T>::scalar_t, const vec3<T>&);          // component-wise maximum of the vector and scalar
template<typename T> EGL_INLINE vec3<T> mul(const vec3<T> &v0_, const vec3<T> &v1_);              // component-wise vector multiply: v0_*v1_
template<typename T> EGL_INLINE vec3<T> madd(const vec3<T> &v0_,                                  // component-wise multiply-add: v0_*v1+v2_
                                             const vec3<T> &v1_,
                                             const vec3<T> &v2_);
template<typename T> EGL_INLINE vec3<T> madd(const vec3<T> &v_,                                   // vector-scalar-scalar multiply-add: v_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE vec3<T> madd(const vec3<T> &v_,                                   // vector-vector-scalar multiply-add: v_*mul_+add_
                                             const vec3<T> &mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE vec3<T> madd(const vec3<T> &v_,                                   // vector-scalar-vector multiply-add: v_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             const vec3<T> &add_);
template<typename T> EGL_INLINE vec3<T> rcp(const vec3<T>&);                                      // component-wise vector reciprocal
template<typename T> EGL_INLINE vec3<T> rcp_z(const vec3<T>&);                                    // component-wise vector reciprocal (set 0 for 0 components)
template<typename T> EGL_INLINE vec3<T> floor(const vec3<T>&);                                    // component-wise greatest integer less than or equal to the value
template<typename T> EGL_INLINE vec3<T> ceil(const vec3<T>&);                                     // component-wise smallest integer greater than or equal to the value
template<typename T> EGL_INLINE vec3<T> trunc(const vec3<T>&);                                    // component-wise truncate decimal part from the value
template<typename T> EGL_INLINE vec3<T> frc(const vec3<T>&);                                      // component-wise fractional part (for negative values returns 1-fractional part)
template<typename T> EGL_INLINE vec3<T> mod(const vec3<T>&, typename math<T>::scalar_t div_);     // component-wise modulo
template<typename T> EGL_INLINE vec3<T> cycle(const vec3<T>&, typename math<T>::scalar_t cycle_); // component-wise cycle
template<typename T> EGL_INLINE vec3<T> cycle1(const vec3<T>&);                                   // component-wise cycle1
template<typename T> EGL_INLINE vec3<T> sat(const vec3<T>&);                                      // component-wise vector saturate
template<typename T> EGL_INLINE vec3<T> ssat(const vec3<T>&);                                     // component-wise vector signed saturate
template<typename T> EGL_INLINE vec3<T> clamp(const vec3<T>&,                                     // component-wise clamp of the vector
                                              const vec3<T> &min_,
                                              const vec3<T> &max_);
template<typename T> EGL_INLINE vec3<T> clamp(const vec3<T>&,                                     // component-wise clamp of the vector with scalars
                                              typename math<T>::scalar_t min_,
                                              typename math<T>::scalar_t max_);
template<typename T> EGL_INLINE vec3<T> abs(const vec3<T>&);                                      // component-wise vector absolute
template<typename T> EGL_INLINE vec3<T> sgn(const vec3<T>&);                                      // component-wise signum (neg=-1, pos=+1, zero=0)
template<typename T> EGL_INLINE vec3<T> sgn_zp(const vec3<T>&);                                   // component-wise signum (neg=-1, pos&zero=+1)
template<typename T> EGL_INLINE vec3<T> sqr(const vec3<T>&);                                      // component-wise square: x*x
template<typename T> EGL_INLINE vec3<T> cubic(const vec3<T>&);                                    // component-wise cubic: x*x*x
template<typename T> EGL_INLINE vec3<T> sqrt(const vec3<T>&);                                     // component-wise vector square root
template<typename T> EGL_INLINE vec3<T> sqrt_z(const vec3<T>&);                                   // component-wise vector square root (set 0 for components less than zero)
template<typename T> EGL_INLINE vec2<T> cbrt(const vec2<T>&);                                     // component-wise cubic root
template<typename T> EGL_INLINE vec3<T> rsqrt(const vec3<T>&);                                    // component-wise vector reciprocal square root
template<typename T> EGL_INLINE vec3<T> rsqrt_z(const vec3<T>&);                                  // component-wise vector reciprocal square root (set 0 for components less than or equal to zero)
template<typename T> EGL_INLINE vec3<T> rcbrt(const vec3<T>&);                                    // component-wise reciprocal cubic root
template<typename T> EGL_INLINE vec3<T> rcbrt_z(const vec3<T>&);                                  // component-wise reciprocal cubic root (set 0 for components that are 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const vec3<T>&);                  // vector norm (length): |v|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const vec3<T>&);                 // reciprocal of the vector norm: 1/|v|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const vec3<T>&);               // reciprocal of the vector norm: 1/|v| (return 0, if |v|=0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const vec3<T>&);                 // squared vector norm: |v|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const vec3<T>&);                // reciprocal of the squared vector norm: 1/|v|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const vec3<T>&);              // reciprocal of the squared vector norm: 1/|v|^2 (return 0, if |v|=0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const vec3<T>&);               // vector L1-norm (Manhattan norm)
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const vec3<T>&);              // reciprocal of the vector L1-norm
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const vec3<T>&);            // reciprocal of the vector L1-norm (if |v|=0, return 0)
template<typename T> EGL_INLINE vec3<T> exp(const vec3<T>&);                                      // component-wise natural exponent
template<typename T> EGL_INLINE vec3<T> exp2(const vec3<T>&);                                     // component-wise 2^x
template<typename T> EGL_INLINE vec3<T> ln(const vec3<T>&);                                       // component-wise natural logarithm
template<typename T> EGL_INLINE vec3<T> log2(const vec3<T>&);                                     // component-wise base-2 logarithm
template<typename T> EGL_INLINE vec3<T> log10(const vec3<T>&);                                    // component-wise base-10 logarithm
template<typename T> EGL_INLINE vec3<T> pow(const vec3<T>&, typename math<T>::scalar_t);          // component-wise power
template<typename T> EGL_INLINE vec3<T> unit(const vec3<T>&);                                     // unit vector of the vector: v/|v|
template<typename T> EGL_INLINE vec3<T> unit_z(const vec3<T>&);                                   // unit vector of the vector (if |v|=0, return v=[0, 0, 0])
template<typename T> EGL_INLINE vec3<T> cross(const vec3<T>&, const vec3<T>&);                    // vector cross-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const vec3<T>&, const vec3<T>&);   // vector dot-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const vec3<T>&);                  // vector dot-product with vector [1, 1, 1] (component sum)
template<typename T> EGL_INLINE void neg(vec3<T>&);                                               // negate the vector
template<typename T, class Rng> EGL_INLINE void rand_u(vec3<T>&, Rng&);                           // random unit vector
template<typename T, class Rng> EGL_INLINE void rand_real1(vec3<T>&, Rng&);                       // random vector where each component is in range [-1, 1]
template<typename T, class Rng> EGL_INLINE void rand_ureal1(vec3<T>&, Rng&);                      // random vector where each component is in range [0, 1]
template<typename T> EGL_INLINE vec3<T> smoothstep(const vec3<T>&);                               // component-wise smoothstep function
template<typename T> EGL_INLINE vec3<T> smootherstep(const vec3<T>&);                             // component-wise smootherstep function
template<typename T> EGL_INLINE vec3<T> lerp(const vec3<T> &v0_,                                  // linear interpolation of vectors: f(t=0)=v0, f(t=1)=v1
                                             const vec3<T> &v1_,
                                             typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE vec3<T> reflect(const vec3<T> &v_, const vec3<T> &n_);            // reflect vector about normal
template<typename T> EGL_INLINE vec3<T> reflect_u(const vec3<T> &v_, const vec3<T> &n_);          // reflect vector about unit normal
template<typename T> EGL_INLINE vec2<T> perspective_project(const vec3<T>&);                      // perspective project vector (divide by last component)
template<typename T, typename U> EGL_INLINE vec3<T> to_vec3(const vec3<U>&);                      // convert between vec3 types
template<unsigned shuffle_, typename T> EGL_INLINE vec3<T> shuffle(const vec3<T>&);               // shuffle vector components (e.g. shuffle<211>(v_) == vec3f(v_.z, v_.y, v_.y))
template<typename T> EGL_INLINE void pack_s1(vec3_32s&, const vec3<T>&);                          // pack 3d vector (components in range [-1, 1]) to 32-bit 3d vector
template<typename T> EGL_INLINE void pack_u1(vec3_32u&, const vec3<T>&);                          // pack 3d vector (components in range [0, 1]) to 32-bit 3d vector
template<typename T> EGL_INLINE void unpack_s1(vec3<T>&, const vec3_32s&);                        // unpack 32-bit 3d vector to 3d vector (components in range [-1, 1])
template<typename T> EGL_INLINE void unpack_u1(vec3<T>&, const vec3_32u&);                        // unpack 32-bit 3d vector to 3d vector (components in range [0, 1])
// 4d vector ops
template<typename T> EGL_INLINE bool is_zero(const vec4<T>&);                                     // test for zero-vector, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const vec4<T>&);                                      // test for saturated vector, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const vec4<T>&);                                     // test for signed saturated vector, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const vec4<T>&, const vec4<T>&);                  // test for equality of vectors, i.e. all components of the vectors are equal (exact)
template<typename T> EGL_INLINE bool operator==(const vec4<T>&, typename math<T>::scalar_t);      // test for equality of vector and scalar, i.e. all components of the vector equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const vec4<T>&);      // test for equality of vector and scalar, i.e. all components of the vector equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const vec4<T>&, const vec4<T>&);                  // test for inequality of vectors, i.e. any of the components of the vectors are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const vec4<T>&, typename math<T>::scalar_t);      // test for inequality of vector and scalar, i.e. any of the components of the vector is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const vec4<T>&);      // test for inequality of vector and scalar, i.e. any of the components of the vector is unequal to the scalar (exact)
template<typename T> EGL_INLINE vec4<T> operator<(const vec4<T>&, const vec4<T>&);                // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator<(const vec4<T>&, typename math<T>::scalar_t);    // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator<(typename math<T>::scalar_t, const vec4<T>&);    // component-wise vector less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator>(const vec4<T>&, const vec4<T>&);                // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator>(const vec4<T>&, typename math<T>::scalar_t);    // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator>(typename math<T>::scalar_t, const vec4<T>&);    // component-wise vector greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator<=(const vec4<T>&, const vec4<T>&);               // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator<=(const vec4<T>&, typename math<T>::scalar_t);   // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator<=(typename math<T>::scalar_t, const vec4<T>&);   // component-wise vector less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator>=(const vec4<T>&, const vec4<T>&);               // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator>=(const vec4<T>&, typename math<T>::scalar_t);   // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> operator>=(typename math<T>::scalar_t, const vec4<T>&);   // component-wise vector greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE vec4<T> sel_eq(const vec4<T> &t0_, const vec4<T> &t1_,            // component-wise equal vector select (t0_==t1_?v0_:v1_)
                                               const vec4<T> &v0_, const vec4<T> &v1_);
template<typename T> EGL_INLINE vec4<T> sel_lt(const vec4<T> &t0_, const vec4<T> &t1_,            // component-wise less-than vector select (t0_<t1_?v0_:v1_)
                                               const vec4<T> &v0_, const vec4<T> &v1_);
template<typename T> EGL_INLINE vec4<T> sel_le(const vec4<T> &t0_, const vec4<T> &t1_,            // component-wise less-or-equal vector select (t0_<=t1_?v0_:v1_)
                                               const vec4<T> &v0_, const vec4<T> &v1_);
template<typename T> EGL_INLINE vec4<T> sel_eqz(const vec4<T> &t_,                                // component-wise equal-to-zero vector select (t_==0?v0_:v1_)
                                                const vec4<T> &v0_, const vec4<T> &v1_);
template<typename T> EGL_INLINE vec4<T> sel_ltz(const vec4<T> &t_,                                // component-wise less-than-zero vector select (t_<0?v0_:v1_)
                                                const vec4<T> &v0_, const vec4<T> &v1_);
template<typename T> EGL_INLINE vec4<T> sel_lez(const vec4<T> &t_,                                // component-wise less-or-equal-to-zero vector select (t_<=0?v0_:v1_)
                                                const vec4<T> &v0_, const vec4<T> &v1_);
template<typename T> EGL_INLINE void operator+=(vec4<T>&, const vec4<T>&);                        // add vector to vector
template<typename T> EGL_INLINE void operator+=(vec4<T>&, typename math<T>::scalar_t);            // add scalar to each component of the vector
template<typename T> EGL_INLINE void operator-=(vec4<T>&, const vec4<T>&);                        // subtract vector from vector
template<typename T> EGL_INLINE void operator-=(vec4<T>&, typename math<T>::scalar_t);            // subtract scalar from each component of the vector
template<typename T> EGL_INLINE void operator*=(vec4<T>&, const vec4<T>&);                        // component-wise vector by vector multiplication
template<typename T> EGL_INLINE void operator*=(vec4<T>&, typename math<T>::scalar_t);            // vector by scalar multiplication
template<typename T> EGL_INLINE void operator*=(vec4<T>&, const mat44<T>&);                       // vector by matrix multiplication
template<typename T> EGL_INLINE void operator/=(vec4<T>&, const vec4<T>&);                        // component-wise vector by vector division
template<typename T> EGL_INLINE void operator/=(vec4<T>&, typename math<T>::scalar_t);            // vector by scalar division
template<typename T> EGL_INLINE vec4<T> operator+(const vec4<T>&, const vec4<T>&);                // add vector to vector
template<typename T> EGL_INLINE vec4<T> operator+(const vec4<T>&, typename math<T>::scalar_t);    // add scalar to each component of the vector
template<typename T> EGL_INLINE vec4<T> operator+(typename math<T>::scalar_t, const vec4<T>&);    // add each component of the vector to a scalar vector
template<typename T> EGL_INLINE vec4<T> operator-(const vec4<T>&, const vec4<T>&);                // subtract vector from vector
template<typename T> EGL_INLINE vec4<T> operator-(const vec4<T>&, typename math<T>::scalar_t);    // subtract scalar from each component of the vector
template<typename T> EGL_INLINE vec4<T> operator-(typename math<T>::scalar_t, const vec4<T>&);    // subtract each component of the vector from a scalar vector
template<typename T> EGL_INLINE vec4<T> operator-(const vec4<T>&);                                // negate the vector
template<typename T> EGL_INLINE vec4<T> operator*(const vec4<T>&, const vec4<T>&);                // component-wise vector by vector multiplication
template<typename T> EGL_INLINE vec4<T> operator*(const vec4<T>&, typename math<T>::scalar_t);    // vector by scalar multiplication
template<typename T> EGL_INLINE vec4<T> operator*(typename math<T>::scalar_t, const vec4<T>&);    // vector by scalar multiplication
template<typename T> EGL_INLINE vec4<T> operator*(const vec4<T>&, const mat44<T>&);               // vector by matrix multiplication
template<typename T> EGL_INLINE vec4<T> operator*(const mat44<T>&, const vec4<T>&);               // matrix by transposed vector multiplication
template<typename T> EGL_INLINE vec4<T> operator/(const vec4<T>&, const vec4<T>&);                // component-wise vector by vector division
template<typename T> EGL_INLINE vec4<T> operator/(const vec4<T>&, typename math<T>::scalar_t);    // vector by scalar division
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const vec4<T>&);                   // minimum component value
template<typename T> EGL_INLINE vec4<T> min(const vec4<T>&, const vec4<T>&);                      // component-wise minimum of the vectors
template<typename T> EGL_INLINE vec4<T> min(const vec4<T>&, const vec4<T>&, const vec4<T>&);
template<typename T> EGL_INLINE vec4<T> min(const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&);
template<typename T> EGL_INLINE vec4<T> min(const vec4<T>&, typename math<T>::scalar_t);          // component-wise minimum of the vector and scalar
template<typename T> EGL_INLINE vec4<T> min(typename math<T>::scalar_t, const vec4<T>&);          // component-wise minimum of the vector and scalar
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const vec4<T>&);                   // maximum component value
template<typename T> EGL_INLINE vec4<T> max(const vec4<T>&, const vec4<T>&);                      // component-wise maximum of the vectors
template<typename T> EGL_INLINE vec4<T> max(const vec4<T>&, const vec4<T>&, const vec4<T>&);
template<typename T> EGL_INLINE vec4<T> max(const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&);
template<typename T> EGL_INLINE vec4<T> max(const vec4<T>&, typename math<T>::scalar_t);          // component-wise maximum of the vector and scalar
template<typename T> EGL_INLINE vec4<T> max(typename math<T>::scalar_t, const vec4<T>&);          // component-wise maximum of the vector and scalar
template<typename T> EGL_INLINE vec4<T> mul(const vec4<T> &v0_, const vec4<T> &v1_);              // component-wise multiply: v0_*v1_
template<typename T> EGL_INLINE vec4<T> madd(const vec4<T> &v0_,                                  // component-wise multiply-add: v0_*v1+v2_
                                             const vec4<T> &v1_,
                                             const vec4<T> &v2_);
template<typename T> EGL_INLINE vec4<T> madd(const vec4<T> &v_,                                   // vector-scalar-scalar multiply-add: v_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE vec4<T> madd(const vec4<T> &v_,                                   // vector-vector-scalar multiply-add: v_*mul_+add_
                                             const vec4<T> &mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE vec4<T> madd(const vec4<T> &v_,                                   // vector-scalar-vector multiply-add: v_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             const vec4<T> &add_);
template<typename T> EGL_INLINE vec4<T> rcp(const vec4<T>&);                                      // component-wise vector reciprocal
template<typename T> EGL_INLINE vec4<T> rcp_z(const vec4<T>&);                                    // component-wise vector reciprocal (set 0 for 0 components)
template<typename T> EGL_INLINE vec4<T> floor(const vec4<T>&);                                    // component-wise greatest integer less than or equal to the value
template<typename T> EGL_INLINE vec4<T> ceil(const vec4<T>&);                                     // component-wise smallest integer greater than or equal to the value
template<typename T> EGL_INLINE vec4<T> trunc(const vec4<T>&);                                    // component-wise truncate decimal part from the value
template<typename T> EGL_INLINE vec4<T> frc(const vec4<T>&);                                      // component-wise fractional part (for negative values returns 1-fractional part)
template<typename T> EGL_INLINE vec4<T> mod(const vec4<T>&, typename math<T>::scalar_t div_);     // component-wise modulo
template<typename T> EGL_INLINE vec4<T> cycle(const vec4<T>&, typename math<T>::scalar_t cycle_); // component-wise cycle
template<typename T> EGL_INLINE vec4<T> cycle1(const vec4<T>&);                                   // component-wise cycle1
template<typename T> EGL_INLINE vec4<T> sat(const vec4<T>&);                                      // component-wise vector saturate
template<typename T> EGL_INLINE vec4<T> ssat(const vec4<T>&);                                     // component-wise vector signed saturate
template<typename T> EGL_INLINE vec4<T> clamp(const vec4<T>&,                                     // component-wise clamp of the vector
                                              const vec4<T> &min_,
                                              const vec4<T> &max_);
template<typename T> EGL_INLINE vec4<T> clamp(const vec4<T>&,                                     // component-wise clamp of the vector with scalars
                                              typename math<T>::scalar_t min_,
                                              typename math<T>::scalar_t max_);
template<typename T> EGL_INLINE vec4<T> abs(const vec4<T>&);                                      // component-wise vector absolute
template<typename T> EGL_INLINE vec4<T> sgn(const vec4<T>&);                                      // component-wise signum (neg=-1, pos=+1, zero=0)
template<typename T> EGL_INLINE vec4<T> sgn_zp(const vec4<T>&);                                   // component-wise signum (neg=-1, pos&zero=+1)
template<typename T> EGL_INLINE vec4<T> sqr(const vec4<T>&);                                      // component-wise square: x*x
template<typename T> EGL_INLINE vec4<T> cubic(const vec4<T>&);                                    // component-wise cubic: x*x*x
template<typename T> EGL_INLINE vec4<T> sqrt(const vec4<T>&);                                     // component-wise vector square root
template<typename T> EGL_INLINE vec4<T> sqrt_z(const vec4<T>&);                                   // component-wise vector square root (set 0 for components less than zero)
template<typename T> EGL_INLINE vec4<T> cbrt(const vec4<T>&);                                     // component-wise cubic root
template<typename T> EGL_INLINE vec4<T> rsqrt(const vec4<T>&);                                    // component-wise vector reciprocal square root
template<typename T> EGL_INLINE vec4<T> rsqrt_z(const vec4<T>&);                                  // component-wise vector reciprocal square root (set 0 for components less than or equal to zero)
template<typename T> EGL_INLINE vec4<T> rcbrt(const vec4<T>&);                                    // component-wise reciprocal cubic root
template<typename T> EGL_INLINE vec4<T> rcbrt_z(const vec4<T>&);                                  // component-wise reciprocal cubic root (set 0 for components that are 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const vec4<T>&);                  // vector norm (length): |v|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const vec4<T>&);                 // reciprocal of the vector norm: 1/|v|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const vec4<T>&);               // reciprocal of the vector norm: 1/|v| (return 0, if |v|=0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const vec4<T>&);                 // squared vector norm: |v|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const vec4<T>&);                // reciprocal of the squared vector norm: 1/|v|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const vec4<T>&);              // reciprocal of the squared vector norm: 1/|v|^2 (return 0, if |v|=0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm_l1(const vec4<T>&);               // vector L1-norm (Manhattan norm)
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1(const vec4<T>&);              // reciprocal of the vector L1-norm
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const vec4<T>&);            // reciprocal of the vector L1-norm (if |v|=0, return 0)
template<typename T> EGL_INLINE vec4<T> exp(const vec4<T>&);                                      // component-wise natural exponent
template<typename T> EGL_INLINE vec4<T> exp2(const vec4<T>&);                                     // component-wise 2^x
template<typename T> EGL_INLINE vec4<T> ln(const vec4<T>&);                                       // component-wise natural logarithm
template<typename T> EGL_INLINE vec4<T> log2(const vec4<T>&);                                     // component-wise base-2 logarithm
template<typename T> EGL_INLINE vec4<T> log10(const vec4<T>&);                                    // component-wise base-10 logarithm
template<typename T> EGL_INLINE vec4<T> pow(const vec4<T>&, typename math<T>::scalar_t);          // component-wise power
template<typename T> EGL_INLINE vec4<T> unit(const vec4<T>&);                                     // unit vector of the vector: v/|v|
template<typename T> EGL_INLINE vec4<T> unit_z(const vec4<T>&);                                   // unit vector of the vector (if |v|=0, return v=[0, 0, 0, 0])
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const vec4<T>&, const vec4<T>&);   // vector dot-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const vec4<T>&);                  // vector dot-product with vector [1, 1, 1, 1] (component sum)
template<typename T> EGL_INLINE void neg(vec4<T>&);                                               // negate the vector
template<typename T, class Rng> EGL_INLINE void rand_u(vec4<T>&, Rng&);                           // random unit vector
template<typename T, class Rng> EGL_INLINE void rand_real1(vec4<T>&, Rng&);                       // random vector where each component is in range [-1, 1]
template<typename T, class Rng> EGL_INLINE void rand_ureal1(vec4<T>&, Rng&);                      // random vector where each component is in range [0, 1]
template<typename T> EGL_INLINE vec4<T> smoothstep(const vec4<T>&);                               // component-wise smoothstep function
template<typename T> EGL_INLINE vec4<T> smootherstep(const vec4<T>&);                             // component-wise smootherstep function
template<typename T> EGL_INLINE vec4<T> lerp(const vec4<T> &v0_,                                  // linear interpolation of vectors: f(t=0)=v0, f(t=1)=v1
                                             const vec4<T> &v1_,
                                             typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE vec4<T> reflect(const vec4<T> &v_, const vec4<T> &n_);            // reflect vector about normal
template<typename T> EGL_INLINE vec4<T> reflect_u(const vec4<T> &v_, const vec4<T> &n_);          // reflect vector about unit normal
template<typename T> EGL_INLINE vec3<T> perspective_project(const vec4<T>&);                      // perspective project vector (divide by last component)
template<typename T, typename U> EGL_INLINE vec4<T> to_vec4(const vec4<U>&);                      // convert between vec4 types
template<unsigned shuffle_, typename T> EGL_INLINE vec4<T> shuffle(const vec4<T>&);               // shuffle vector components (e.g. shuffle<2311>(v_) == vec4f(v_.z, v_.w, v_.y, v_.y))
template<typename T> EGL_INLINE void pack_s1(vec4_32u&, const vec4<T>&);                          // pack 4d vector (components in range [-1, 1]) to 32-bit 4d vector
template<typename T> EGL_INLINE void pack_u1(vec4_32u&, const vec4<T>&);                          // pack 4d vector (components in range [0, 1]) to 32-bit 4d vector
template<typename T> EGL_INLINE void unpack_s1(vec4<T>&, const vec4_32s&);                        // unpack 32-bit 4d vector to 4d vector (components in range [-1, 1])
template<typename T> EGL_INLINE void unpack_u1(vec4<T>&, const vec4_32u&);                        // unpack 32-bit 4d vector to 4d vector (components in range [0, 1])
// 2x2 matrix ops
template<typename T> EGL_INLINE bool is_zero(const mat22<T>&);                                    // test for zero-matrix, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const mat22<T>&);                                     // test for saturated matrix, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const mat22<T>&);                                    // test for signed saturated matrix, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const mat22<T>&, const mat22<T>&);                // test for equality of matrices, i.e. all components of the matrices are equal (exact)
template<typename T> EGL_INLINE bool operator==(const mat22<T>&, typename math<T>::scalar_t);     // test for equality of matrix and scalar, i.e. all components of the matrix equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const mat22<T>&);     // test for equality of matrix and scalar, i.e. all components of the matrix equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const mat22<T>&, const mat22<T>&);                // test for inequality of matrices, i.e. any of the components of the vectors are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const mat22<T>&, typename math<T>::scalar_t);     // test for inequality of matrix and scalar, i.e. any of the components of the matrix is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const mat22<T>&);     // test for inequality of matrix and scalar, i.e. any of the components of the matrix is unequal to the scalar (exact)
template<typename T> EGL_INLINE void operator+=(mat22<T>&, const mat22<T>&);                      // add matrix to matrix
template<typename T> EGL_INLINE void operator+=(mat22<T>&, typename math<T>::scalar_t);           // add scalar to matrix
template<typename T> EGL_INLINE void operator-=(mat22<T>&, const mat22<T>&);                      // subtract matrix from matrix
template<typename T> EGL_INLINE void operator-=(mat22<T>&, typename math<T>::scalar_t);           // subtract scalar from matrix
template<typename T> EGL_INLINE void operator*=(mat22<T>&, const mat22<T>&);                      // multiply matrix by matrix
template<typename T> EGL_INLINE void operator*=(mat22<T>&, typename math<T>::scalar_t);           // multiply matrix by scalar
template<typename T> EGL_INLINE void operator/=(mat22<T>&, typename math<T>::scalar_t);           // divide matrix by scalar
template<typename T> EGL_INLINE mat22<T> operator+(const mat22<T>&, const mat22<T>&);             // add matrix to matrix
template<typename T> EGL_INLINE mat22<T> operator+(const mat22<T>&, typename math<T>::scalar_t);  // add scalar to matrix
template<typename T> EGL_INLINE mat22<T> operator+(typename math<T>::scalar_t, const mat22<T>&);  // add matrix to scalar
template<typename T> EGL_INLINE mat22<T> operator-(const mat22<T>&, const mat22<T>&);             // subtract matrix from matrix
template<typename T> EGL_INLINE mat22<T> operator-(const mat22<T>&, typename math<T>::scalar_t);  // subtract scalar from matrix
template<typename T> EGL_INLINE mat22<T> operator-(typename math<T>::scalar_t, const mat22<T>&);  // subtract matrix from scalar
template<typename T> EGL_INLINE mat22<T> operator-(const mat22<T>&);                              // negate the matrix
template<typename T> EGL_INLINE mat22<T> operator*(const mat22<T>&, const mat22<T>&);             // multiply matrix by matrix
template<typename T> EGL_INLINE mat22<T> operator*(const mat22<T>&, typename math<T>::scalar_t);  // multiply matrix by scalar
template<typename T> EGL_INLINE mat22<T> operator*(typename math<T>::scalar_t, const mat22<T>&);  // multiply matrix by scalar
template<typename T> EGL_INLINE mat22<T> operator/(const mat22<T>&, typename math<T>::scalar_t);  // divide matrix by scalar
template<typename T> EGL_INLINE mat22<T> mul(const mat22<T> &m0_, const mat22<T> &m1_);           // component-wise multiply: m0_*m1_
template<typename T> EGL_INLINE mat22<T> madd(const mat22<T> &m0_,                                // component-wise multiply-add: m0_*m1+m2_
                                              const mat22<T> &m1_,
                                              const mat22<T> &m2_);
template<typename T> EGL_INLINE mat22<T> madd(const mat22<T> &m_,                                 // matrix-scalar-scalar multiply-add: m_*mul_+add_
                                              typename math<T>::scalar_t mul_,
                                              typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE mat22<T> madd(const mat22<T> &m_,                                 // matrix-matrix-scalar multiply-add: m_*mul_+add_
                                              const mat22<T> &mul_,
                                              typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE mat22<T> madd(const mat22<T> &m_,                                 // matrix-scalar-matrix multiply-add: m_*mul_+add_
                                              typename math<T>::scalar_t mul_,
                                              const mat22<T> &add_);
template<typename T> EGL_INLINE typename math<T>::scalar_t det(const mat22<T>&);                  // determinant of the matrix
template<typename T> EGL_INLINE mat22<T> inv(const mat22<T>&, typename math<T>::scalar_t *det_=0);// inverse of the matrix
template<typename T> EGL_INLINE typename math<T>::scalar_t tr(const mat22<T>&);                   // trace of the matrix
template<typename T> EGL_INLINE void identity(mat22<T>&);                                         // set matrix to identity
template<typename T> EGL_INLINE void transpose(mat22<T>&);                                        // transpose of the matrix
template<typename T> EGL_INLINE void transpose(mat22<T> &res_, const mat22<T>&);                  // transpose of the matrix
template<typename T> EGL_INLINE void neg(mat22<T>&);                                              // negate the matrix
template<typename T> EGL_INLINE mat22<T> lerp(const mat22<T> &m0_,                                // linear interpolation of matrices: f(t=0)=m0, f(t=1)=m1
                                              const mat22<T> &m1_,
                                              typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE void lerp(mat22<T> &res_,                                         // linear interpolation of matrices: f(t=0)=m0, f(t=1)=m1
                                          const mat22<T> &m0_,
                                          const mat22<T> &m1_,
                                          typename math<T>::scalar_t t_);
template<typename T, typename U> EGL_INLINE mat22<T> to_mat22(const mat22<U>&);                   // convert between mat22 types
template<typename T> EGL_INLINE void normalize(mat22<T>&);                                        // normalize matrix vectors
// 3x3 matrix ops
template<typename T> EGL_INLINE bool is_zero(const mat33<T>&);                                    // test for zero-matrix, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_orthogonal(const mat33<T>&);                              // test for orthogonal matrix
template<typename T> EGL_INLINE bool is_sat(const mat33<T>&);                                     // test for saturated matrix, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const mat33<T>&);                                    // test for signed saturated matrix, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const mat33<T>&, const mat33<T>&);                // test for equality of matrices, i.e. all components of the matrices are equal (exact)
template<typename T> EGL_INLINE bool operator==(const mat33<T>&, typename math<T>::scalar_t);     // test for equality of matrix and scalar, i.e. all components of the matrix equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const mat33<T>&);     // test for equality of matrix and scalar, i.e. all components of the matrix equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const mat33<T>&, const mat33<T>&);                // test for inequality of matrices, i.e. any of the components of the vectors are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const mat33<T>&, typename math<T>::scalar_t);     // test for inequality of matrix and scalar, i.e. any of the components of the matrix is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const mat33<T>&);     // test for inequality of matrix and scalar, i.e. any of the components of the matrix is unequal to the scalar (exact)
template<typename T> EGL_INLINE void operator+=(mat33<T>&, const mat33<T>&);                      // add matrix to matrix
template<typename T> EGL_INLINE void operator+=(mat33<T>&, typename math<T>::scalar_t);           // add scalar to matrix
template<typename T> EGL_INLINE void operator-=(mat33<T>&, const mat33<T>&);                      // subtract matrix from matrix
template<typename T> EGL_INLINE void operator-=(mat33<T>&, typename math<T>::scalar_t);           // subtract scalar from matrix
template<typename T> EGL_INLINE void operator*=(mat33<T>&, const mat33<T>&);                      // multiply matrix by matrix
template<typename T> EGL_INLINE void operator*=(mat33<T>&, const quat<T>&);                       // multiply matrix by quaternion
template<typename T> EGL_INLINE void operator*=(mat33<T>&, typename math<T>::scalar_t);           // multiply matrix by scalar
template<typename T> EGL_INLINE void operator/=(mat33<T>&, typename math<T>::scalar_t);           // divide matrix by scalar
template<typename T> EGL_INLINE mat33<T> operator+(const mat33<T>&, const mat33<T>&);             // add matrix to matrix
template<typename T> EGL_INLINE mat33<T> operator+(const mat33<T>&, typename math<T>::scalar_t);  // add scalar to matrix
template<typename T> EGL_INLINE mat33<T> operator+(typename math<T>::scalar_t, const mat33<T>&);  // add matrix to scalar
template<typename T> EGL_INLINE mat33<T> operator-(const mat33<T>&, const mat33<T>&);             // subtract matrix from matrix
template<typename T> EGL_INLINE mat33<T> operator-(const mat33<T>&, typename math<T>::scalar_t);  // subtract scalar from matrix
template<typename T> EGL_INLINE mat33<T> operator-(typename math<T>::scalar_t, const mat33<T>&);  // subtract matrix from scalar
template<typename T> EGL_INLINE mat33<T> operator-(const mat33<T>&);                              // negate the matrix
template<typename T> EGL_INLINE mat33<T> operator*(const mat33<T>&, const mat33<T>&);             // multiply matrix by matrix
template<typename T> EGL_INLINE mat33<T> operator*(const mat33<T>&, const quat<T>&);              // multiply matrix by quaternion
template<typename T> EGL_INLINE mat33<T> operator*(const quat<T>&, const mat33<T>&);              // multiply quaternion by matrix
template<typename T> EGL_INLINE mat33<T> operator*(const mat33<T>&, typename math<T>::scalar_t);  // multiply matrix by scalar
template<typename T> EGL_INLINE mat33<T> operator*(typename math<T>::scalar_t, const mat33<T>&);  // multiply matrix by scalar
template<typename T> EGL_INLINE mat33<T> operator/(const mat33<T>&, typename math<T>::scalar_t);  // divide matrix by scalar
template<typename T> EGL_INLINE mat33<T> mul(const mat33<T> &m0_, const mat33<T> &m1_);           // component-wise multiply: m0_*m1_
template<typename T> EGL_INLINE mat33<T> madd(const mat33<T> &m0_,                                // component-wise multiply-add: m0_*m1+m2_
                                              const mat33<T> &m1_,
                                              const mat33<T> &m2_);
template<typename T> EGL_INLINE mat33<T> madd(const mat33<T> &m_,                                 // matrix-scalar-scalar multiply-add: m_*mul_+add_
                                              typename math<T>::scalar_t mul_,
                                              typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE mat33<T> madd(const mat33<T> &m_,                                 // matrix-matrix-scalar multiply-add: m_*mul_+add_
                                              const mat33<T> &mul_,
                                              typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE mat33<T> madd(const mat33<T> &m_,                                 // matrix-scalar-matrix multiply-add: m_*mul_+add_
                                              typename math<T>::scalar_t mul_,
                                              const mat33<T> &add_);
template<typename T> EGL_INLINE typename math<T>::scalar_t det(const mat33<T>&);                  // determinant of the matrix
template<typename T> EGL_INLINE mat33<T> inv(const mat33<T>&, typename math<T>::scalar_t *det_=0);// inverse of the matrix
template<typename T> EGL_INLINE typename math<T>::scalar_t tr(const mat33<T>&);                   // trace of the matrix
template<typename T> EGL_INLINE void identity(mat33<T>&);                                         // set matrix to identity
template<typename T> EGL_INLINE void transpose(mat33<T>&);                                        // transpose of the matrix
template<typename T> EGL_INLINE void transpose(mat33<T> &res_, const mat33<T>&);                  // transpose of the matrix
template<typename T> EGL_INLINE void neg(mat33<T>&);                                              // negate the matrix
template<typename T> EGL_INLINE mat33<T> lerp(const mat33<T> &m0_,                                // linear interpolation of matrices: f(t=0)=m0, f(t=1)=m1
                                              const mat33<T> &m1_,
                                              typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE void lerp(mat33<T> &res_,                                         // linear interpolation of matrices: f(t=0)=m0, f(t=1)=m1
                                          const mat33<T> &m0_,
                                          const mat33<T> &m1_,
                                          typename math<T>::scalar_t t_);
template<typename T, typename U> EGL_INLINE mat33<T> to_mat33(const mat33<U>&);                   // convert between mat33 types
template<typename T> EGL_INLINE void normalize(mat33<T>&);                                        // normalize matrix vectors
template<typename T> EGL_INLINE void orthonormalize(mat33<T>&);                                   // ortho-normalize the matrix
template<typename T> EGL_INLINE void set_rotation_xyz(mat33<T>&,                                  // set rotation matrix with angles in order Rx*Ry*Rz
                                                      typename math<T>::scalar_t x_,
                                                      typename math<T>::scalar_t y_,
                                                      typename math<T>::scalar_t z_);
template<typename T> EGL_INLINE void set_rotation_zyx(mat33<T>&,                                  // set rotation matrix with angles in order Rz*Ry*Rx
                                                      typename math<T>::scalar_t x_,
                                                      typename math<T>::scalar_t y_,
                                                      typename math<T>::scalar_t z_);
template<typename T> EGL_INLINE void set_rotation_zxz(mat33<T>&,                                  // set rotation matrix with angles in order Rz*Rx*Rz (Euler)
                                                      typename math<T>::scalar_t x_,
                                                      typename math<T>::scalar_t z_);
// 4x4 matrix ops
template<typename T> EGL_INLINE bool is_zero(const mat44<T>&);                                    // test for zero-matrix, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const mat44<T>&);                                     // test for saturated matrix, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const mat44<T>&);                                    // test for signed saturated matrix, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const mat44<T>&, const mat44<T>&);                // test for equality of matrices, i.e. all components of the matrices are equal (exact)
template<typename T> EGL_INLINE bool operator==(const mat44<T>&, typename math<T>::scalar_t);     // test for equality of matrix and scalar, i.e. all components of the matrix equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const mat44<T>&);     // test for equality of matrix and scalar, i.e. all components of the matrix equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const mat44<T>&, const mat44<T>&);                // test for inequality of matrices, i.e. any of the components of the vectors are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const mat44<T>&, typename math<T>::scalar_t);     // test for inequality of matrix and scalar, i.e. any of the components of the matrix is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const mat44<T>&);     // test for inequality of matrix and scalar, i.e. any of the components of the matrix is unequal to the scalar (exact)
template<typename T> EGL_INLINE void operator+=(mat44<T>&, const mat44<T>&);                      // add matrix to matrix
template<typename T> EGL_INLINE void operator+=(mat44<T>&, typename math<T>::scalar_t);           // add scalar to matrix
template<typename T> EGL_INLINE void operator-=(mat44<T>&, const mat44<T>&);                      // subtract matrix from matrix
template<typename T> EGL_INLINE void operator-=(mat44<T>&, typename math<T>::scalar_t);           // subtract scalar from matrix
template<typename T> EGL_INLINE void operator*=(mat44<T>&, const mat44<T>&);                      // multiply matrix by matrix
template<typename T> EGL_INLINE void operator*=(mat44<T>&, typename math<T>::scalar_t);           // multiply matrix by scalar
template<typename T> EGL_INLINE void operator/=(mat44<T>&, typename math<T>::scalar_t);           // divide matrix by scalar
template<typename T> EGL_INLINE mat44<T> operator+(const mat44<T>&, const mat44<T>&);             // add matrix to matrix
template<typename T> EGL_INLINE mat44<T> operator+(const mat44<T>&, typename math<T>::scalar_t);  // add scalar to matrix
template<typename T> EGL_INLINE mat44<T> operator+(typename math<T>::scalar_t, const mat44<T>&);  // add matrix to scalar
template<typename T> EGL_INLINE mat44<T> operator-(const mat44<T>&, const mat44<T>&);             // subtract matrix from matrix
template<typename T> EGL_INLINE mat44<T> operator-(const mat44<T>&, typename math<T>::scalar_t);  // subtract scalar from matrix
template<typename T> EGL_INLINE mat44<T> operator-(typename math<T>::scalar_t, const mat44<T>&);  // subtract matrix from scalar
template<typename T> EGL_INLINE mat44<T> operator-(const mat44<T>&);                              // negate the matrix
template<typename T> EGL_INLINE mat44<T> operator*(const mat44<T>&, const mat44<T>&);             // multiply matrix by matrix
template<typename T> EGL_INLINE mat44<T> operator*(const mat44<T>&, typename math<T>::scalar_t);  // multiply matrix by scalar
template<typename T> EGL_INLINE mat44<T> operator*(typename math<T>::scalar_t, const mat44<T>&);  // multiply matrix by scalar
template<typename T> EGL_INLINE mat44<T> operator/(const mat44<T>&, typename math<T>::scalar_t);  // divide matrix by scalar
template<typename T> EGL_INLINE mat44<T> mul(const mat44<T> &m0_, const mat44<T> &m1_);           // component-wise multiply: m0_*m1_
template<typename T> EGL_INLINE mat44<T> madd(const mat44<T> &m0_,                                // component-wise multiply-add: m0_*m1+m2_
                                              const mat44<T> &m1_,
                                              const mat44<T> &m2_);
template<typename T> EGL_INLINE mat44<T> madd(const mat44<T> &m_,                                 // matrix scalar multiply-add: v_*mul_+add_
                                              typename math<T>::scalar_t mul_,
                                              typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE mat44<T> madd(const mat44<T> &m_,                                 // matrix-matrix-scalar multiply-add: m_*mul_+add_
                                              const mat44<T> &mul_,
                                              typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE mat44<T> madd(const mat44<T> &m_,                                 // matrix-scalar-matrix multiply-add: m_*mul_+add_
                                              typename math<T>::scalar_t mul_,
                                              const mat44<T> &add_);
template<typename T> EGL_INLINE typename math<T>::scalar_t det(const mat44<T>&);                  // determinant of the matrix
template<typename T> EGL_INLINE mat44<T> inv(const mat44<T>&, typename math<T>::scalar_t *det_=0);// inverse of the matrix
template<typename T> EGL_INLINE typename math<T>::scalar_t tr(const mat44<T>&);                   // trace of the matrix
template<typename T> EGL_INLINE void identity(mat44<T>&);                                         // set matrix to identity
template<typename T> EGL_INLINE void transpose(mat44<T>&);                                        // transpose of the matrix
template<typename T> EGL_INLINE void transpose(mat44<T> &res_, const mat44<T>&);                  // transpose of the matrix
template<typename T> EGL_INLINE void neg(mat44<T>&);                                              // negate the matrix
template<typename T> EGL_INLINE mat44<T> lerp(const mat44<T> &m0_,                                // linear interpolation of matrices: f(t=0)=m0, f(t=1)=m1
                                              const mat44<T> &m1_,
                                              typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE void lerp(mat44<T> &res_,                                         // linear interpolation of matrices: f(t=0)=m0, f(t=1)=m1
                                          const mat44<T> &m0_,
                                          const mat44<T> &m1_,
                                          typename math<T>::scalar_t t_); 
template<typename T> EGL_INLINE void normalize(mat44<T>&);                                        // normalize matrix vectors
template<typename T, typename U> EGL_INLINE mat44<T> to_mat44(const mat44<U>&);                   // convert between mat44 types
// quaternion ops
template<typename T> EGL_INLINE bool is_zero(const quat<T>&);                                     // test for zero-quaternion, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const quat<T>&);                                      // test for saturated quaternion, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const quat<T>&);                                     // test for signed saturated quaternion, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const quat<T>&, const quat<T>&);                  // test for equality of quaternions, i.e. all components of the quaternions are equal (exact)
template<typename T> EGL_INLINE bool operator==(const quat<T>&, typename math<T>::scalar_t);      // test for equality of quaternion and scalar, i.e. all components of the quaternion equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const quat<T>&);      // test for equality of quaternion and scalar, i.e. all components of the quaternion equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const quat<T>&, const quat<T>&);                  // test for inequality of quaternions, i.e. any of the components of the quaternions are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const quat<T>&, typename math<T>::scalar_t);      // test for inequality of quaternion and scalar, i.e. any of the components of the quaternion is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const quat<T>&);      // test for inequality of quaternion and scalar, i.e. any of the components of the quaternion is unequal to the scalar (exact)
template<typename T> EGL_INLINE quat<T> operator<(const quat<T>&, const quat<T>&);                // component-wise quaternion less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator<(const quat<T>&, typename math<T>::scalar_t);    // component-wise quaternion less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator<(typename math<T>::scalar_t, const quat<T>&);    // component-wise quaternion less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator>(const quat<T>&, const quat<T>&);                // component-wise quaternion greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator>(const quat<T>&, typename math<T>::scalar_t);    // component-wise quaternion greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator>(typename math<T>::scalar_t, const quat<T>&);    // component-wise quaternion greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator<=(const quat<T>&, const quat<T>&);               // component-wise quaternion less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator<=(const quat<T>&, typename math<T>::scalar_t);   // component-wise quaternion less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator<=(typename math<T>::scalar_t, const quat<T>&);   // component-wise quaternion less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator>=(const quat<T>&, const quat<T>&);               // component-wise quaternion greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator>=(const quat<T>&, typename math<T>::scalar_t);   // component-wise quaternion greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> operator>=(typename math<T>::scalar_t, const quat<T>&);   // component-wise quaternion greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE quat<T> sel_eq(const quat<T> &t0_, const quat<T> &t1_,            // component-wise equal quaternion select (t0_==t1_?q0_:q1_)
                                               const quat<T> &q0_, const quat<T> &q1_);
template<typename T> EGL_INLINE quat<T> sel_lt(const quat<T> &t0_, const quat<T> &t1_,            // component-wise less-than quaternion select (t0_<t1_?q0_:q1_)
                                               const quat<T> &q0_, const quat<T> &q1_);
template<typename T> EGL_INLINE quat<T> sel_le(const quat<T> &t0_, const quat<T> &t1_,            // component-wise less-or-equal quaternion select (t0_<=t1_?q0_:q1_)
                                               const quat<T> &q0_, const quat<T> &q1_);
template<typename T> EGL_INLINE quat<T> sel_eqz(const quat<T> &t_,                                // component-wise equal-to-zero quaternion select (t_==0?q0_:q1_)
                                                const quat<T> &q0_, const quat<T> &q1_);
template<typename T> EGL_INLINE quat<T> sel_ltz(const quat<T> &t_,                                // component-wise less-than-zero quaternion select (t_<0?q0_:q1_)
                                                const quat<T> &q0_, const quat<T> &q1_);
template<typename T> EGL_INLINE quat<T> sel_lez(const quat<T> &t_,                                // component-wise less-or-equal-to-zero quaternion select (t_<=0?q0_:q1_)
                                                const quat<T> &q0_, const quat<T> &q1_);
template<typename T> EGL_INLINE void operator+=(quat<T>&, const quat<T>&);                        // add quaternion to quaternion
template<typename T> EGL_INLINE void operator+=(quat<T>&, typename math<T>::scalar_t);            // add scalar to each component of the quaternion
template<typename T> EGL_INLINE void operator-=(quat<T>&, const quat<T>&);                        // subtract quaternion from quaternion
template<typename T> EGL_INLINE void operator-=(quat<T>&, typename math<T>::scalar_t);            // subtract scalar from each component of the quaternion
template<typename T> EGL_INLINE void operator*=(quat<T>&, const quat<T>&);                        // multiply quaternion by quaternion
template<typename T> EGL_INLINE void operator*=(quat<T>&, typename math<T>::scalar_t);            // quaternion by scalar multiplication
template<typename T> EGL_INLINE void operator/=(quat<T>&, typename math<T>::scalar_t);            // quaternion by scalar division
template<typename T> EGL_INLINE quat<T> operator+(const quat<T>&, const quat<T>&);                // add quaternion to quaternion
template<typename T> EGL_INLINE quat<T> operator+(const quat<T>&, typename math<T>::scalar_t);    // add scalar to each component of the quaternion
template<typename T> EGL_INLINE quat<T> operator+(typename math<T>::scalar_t, const quat<T>&);    // add each component of the quaternion to a scalar quaternion
template<typename T> EGL_INLINE quat<T> operator-(const quat<T>&, const quat<T>&);                // subtract quaternion from quaternion
template<typename T> EGL_INLINE quat<T> operator-(const quat<T>&, typename math<T>::scalar_t);    // subtract scalar from each component of the quaternion
template<typename T> EGL_INLINE quat<T> operator-(typename math<T>::scalar_t, const quat<T>&);    // subtract each component of the quaternion from a scalar quaternion
template<typename T> EGL_INLINE quat<T> operator-(const quat<T>&);                                // negate the quaternion
template<typename T> EGL_INLINE quat<T> operator*(const quat<T>&, const quat<T>&);                // multiply quaternion by quaternion
template<typename T> EGL_INLINE quat<T> operator*(const quat<T>&, typename math<T>::scalar_t);    // quaternion by scalar multiplication
template<typename T> EGL_INLINE quat<T> operator*(typename math<T>::scalar_t, const quat<T>&);    // quaternion by scalar multiplication
template<typename T> EGL_INLINE quat<T> operator/(const quat<T>&, typename math<T>::scalar_t);    // quaternion by scalar division
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const quat<T>&);                   // minimum component value
template<typename T> EGL_INLINE quat<T> min(const quat<T>&, const quat<T>&);                      // component-wise minimum of the quaternions
template<typename T> EGL_INLINE quat<T> min(const quat<T>&, const quat<T>&, const quat<T>&);
template<typename T> EGL_INLINE quat<T> min(const quat<T>&, const quat<T>&, const quat<T>&, const quat<T>&);
template<typename T> EGL_INLINE quat<T> min(const quat<T>&, typename math<T>::scalar_t);          // component-wise minimum of the quaternion and scalar
template<typename T> EGL_INLINE quat<T> min(typename math<T>::scalar_t, const quat<T>&);          // component-wise minimum of the quaternion and scalar
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const quat<T>&);                   // maximum component value
template<typename T> EGL_INLINE quat<T> max(const quat<T>&, const quat<T>&);                      // component-wise maximum of the quaternions
template<typename T> EGL_INLINE quat<T> max(const quat<T>&, const quat<T>&, const quat<T>&);
template<typename T> EGL_INLINE quat<T> max(const quat<T>&, const quat<T>&, const quat<T>&, const quat<T>&);
template<typename T> EGL_INLINE quat<T> max(const quat<T>&, typename math<T>::scalar_t);          // component-wise maximum of the quaternion and scalar
template<typename T> EGL_INLINE quat<T> max(typename math<T>::scalar_t, const quat<T>&);          // component-wise maximum of the quaternion and scalar
template<typename T> EGL_INLINE quat<T> mul(const quat<T> &q0_, const quat<T> &q1_);              // component-wise multiply: q0_*q1_
template<typename T> EGL_INLINE quat<T> madd(const quat<T> &q0_,                                  // component-wise multiply-add: q0_*q1+q2_
                                             const quat<T> &q1_,
                                             const quat<T> &q2_);
template<typename T> EGL_INLINE quat<T> madd(const quat<T> &q_,                                   // quaternion-scalar-scalar multiply-add: q_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE quat<T> madd(const quat<T> &q_,                                   // quaternion-quaternion-scalar multiply-add: q_*mul_+add_
                                             const quat<T> &mul_,
                                             typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE quat<T> madd(const quat<T> &q_,                                   // quaternion-scalar-quaternion multiply-add: q_*mul_+add_
                                             typename math<T>::scalar_t mul_,
                                             const quat<T> &add_);
template<typename T> EGL_INLINE quat<T> rcp(const quat<T>&);                                      // component-wise quaternion reciprocal
template<typename T> EGL_INLINE quat<T> rcp_z(const quat<T>&);                                    // component-wise quaternion reciprocal (set 0 for 0 components)
template<typename T> EGL_INLINE quat<T> floor(const quat<T>&);                                    // component-wise greatest integer less than or equal to the value
template<typename T> EGL_INLINE quat<T> ceil(const quat<T>&);                                     // component-wise smallest integer greater than or equal to the value
template<typename T> EGL_INLINE quat<T> trunc(const quat<T>&);                                    // component-wise truncate decimal part from the value
template<typename T> EGL_INLINE quat<T> frc(const quat<T>&);                                      // component-wise fractional part (for negative values returns 1-fractional part)
template<typename T> EGL_INLINE quat<T> mod(const quat<T>&, typename math<T>::scalar_t div_);     // component-wise modulo
template<typename T> EGL_INLINE quat<T> cycle(const quat<T>&, typename math<T>::scalar_t cycle_); // component-wise cycle
template<typename T> EGL_INLINE quat<T> cycle1(const quat<T>&);                                   // component-wise cycle1
template<typename T> EGL_INLINE quat<T> sat(const quat<T>&);                                      // component-wise quaternion saturate
template<typename T> EGL_INLINE quat<T> ssat(const quat<T>&);                                     // component-wise quaternion signed saturate
template<typename T> EGL_INLINE quat<T> clamp(const quat<T>&,                                     // component-wise clamp of the quaternion
                                              const quat<T> &min_,
                                              const quat<T> &max_);
template<typename T> EGL_INLINE quat<T> clamp(const quat<T>&,                                     // component-wise clamp of the quaternion with scalars
                                              typename math<T>::scalar_t min_,
                                              typename math<T>::scalar_t max_);
template<typename T> EGL_INLINE quat<T> abs(const quat<T>&);                                      // component-wise quaternion absolute
template<typename T> EGL_INLINE quat<T> sgn(const quat<T>&);                                      // component-wise signum (neg=-1, pos=+1, zero=0)
template<typename T> EGL_INLINE quat<T> sgn_zp(const quat<T>&);                                   // component-wise signum (neg=-1, pos&zero=+1)
template<typename T> EGL_INLINE quat<T> sqr(const quat<T>&);                                      // component-wise square: x*x
template<typename T> EGL_INLINE quat<T> cubic(const quat<T>&);                                    // component-wise cubic: x*x*x
template<typename T> EGL_INLINE quat<T> sqrt(const quat<T>&);                                     // component-wise quaternion square root
template<typename T> EGL_INLINE quat<T> sqrt_z(const quat<T>&);                                   // component-wise quaternion square root (set 0 for components less than zero)
template<typename T> EGL_INLINE quat<T> cbrt(const quat<T>&);                                     // component-wise cubic root
template<typename T> EGL_INLINE quat<T> rsqrt(const quat<T>&);                                    // component-wise quaternion reciprocal square root
template<typename T> EGL_INLINE quat<T> rsqrt_z(const quat<T>&);                                  // component-wise quaternion reciprocal square root (set 0 for components less than or equal to zero)
template<typename T> EGL_INLINE quat<T> rcbrt(const quat<T>&);                                    // component-wise reciprocal cubic root
template<typename T> EGL_INLINE quat<T> rcbrt_z(const quat<T>&);                                  // component-wise reciprocal cubic root (set 0 for components that are 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const quat<T>&);                  // quaternion norm (length): |q|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const quat<T>&);                 // reciprocal of the quaternion norm: 1/|q|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const quat<T>&);               // reciprocal of the quaternion norm: 1/|q| (return 0 if |q|=0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const quat<T>&);                 // squared quaternion norm: |q|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const quat<T>&);                // reciprocal of the squared vector norm: 1/|q|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const quat<T>&);              // reciprocal of the squared vector norm: 1/|q|^2 (return 0 if |q|=0)
template<typename T> EGL_INLINE quat<T> unit(const quat<T>&);                                     // unit quaternion of the quaternion: q/|q|
template<typename T> EGL_INLINE quat<T> unit_z(const quat<T>&);                                   // unit quaternion of the quaternion (if |q|=0, return q=[v=0, w=1])
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const quat<T>&, const quat<T>&);   // quaternion dot-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const quat<T>&);                  // quaternion dot-product with quaternion [1, 1, 1, 1] (component sum)
template<typename T> EGL_INLINE quat<T> cross(const quat<T>&, const quat<T>&);                    // cross-product of quaternions (or odd/Grassman outer product): q=[v0 x v1, 0]
template<typename T> EGL_INLINE quat<T> outer(const quat<T>&, const quat<T>&);                    // Euclidean outer-product of quaternions: q=[w0*v1-w1*v0-(v0 x v1), 0]
template<typename T> EGL_INLINE quat<T> even(const quat<T>&, const quat<T>&);                     // even-product (Grassman inner product) of quaternions: q=[w0*v1+w1*v0, w0*w1-dot(v0, v1)]
template<typename T> EGL_INLINE quat<T> conj(const quat<T>&);                                     // conjugate of the quaternion
template<typename T> EGL_INLINE quat<T> inv(const quat<T>&);                                      // inverse of the quaternion
template<typename T> EGL_INLINE quat<T> inv_u(const quat<T>&);                                    // inverse of the unit quaternion
template<typename T> EGL_INLINE quat<T> exp(const quat<T>&);                                      // natural exponent of the quaternion
template<typename T> EGL_INLINE quat<T> exp_u(const quat<T>&);                                    // natural exponent of the unit quaternion
template<typename T> EGL_INLINE quat<T> ln(const quat<T>&);                                       // natural logarithm of the quaternion
template<typename T> EGL_INLINE quat<T> ln_u(const quat<T>&);                                     // natural logarithm of the unit quaternion
template<typename T> EGL_INLINE quat<T> pow(const quat<T>&, typename math<T>::scalar_t);          // rise quaternion to a power
template<typename T> EGL_INLINE quat<T> pow_u(const quat<T>&, typename math<T>::scalar_t);        // rise unit quaternion to a power
template<typename T> EGL_INLINE typename math<T>::scalar_t arg(const quat<T>&);                   // argument of the quaternion
template<typename T> EGL_INLINE typename math<T>::scalar_t arg_u(const quat<T>&);                 // argument of the unit quaternion
template<typename T> EGL_INLINE void neg(quat<T>&);                                               // negate the quaternion
template<typename T, class Rng> EGL_INLINE void rand_u(quat<T>&, Rng&);                           // random unit quaternion
template<typename T, class Rng> EGL_INLINE void rand_real1(quat<T>&, Rng&);                       // random quaternion where each component is in range [-1, 1]
template<typename T, class Rng> EGL_INLINE void rand_ureal1(quat<T>&, Rng&);                      // random quaternion where each component is in range [0, 1]
template<typename T> EGL_INLINE quat<T> smoothstep(const quat<T>&);                               // component-wise smoothstep function
template<typename T> EGL_INLINE quat<T> smootherstep(const quat<T>&);                             // component-wise smootherstep function
template<typename T> EGL_INLINE quat<T> lerp(const quat<T> &q0_,                                  // linear interpolation of quaternions: f(t=0)=v0, f(t=1)=v1
                                             const quat<T> &q1_,
                                             typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE quat<T> nlerp(const quat<T> &q0_,                                 // normalized linear quaternion interpolation: f(t=0)=q0/|q0|, f(t=1)=q1/|q1|
                                              const quat<T> &q1_,
                                              typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE quat<T> nlerp_z(const quat<T> &q0_,                               // normalized linear quaternion interpolation: f(t=0)=q0/|q0|, f(t=1)=q1/|q1|. if |q|=0, return q=[v=0, w=1]
                                                const quat<T> &q1_,
                                                typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE quat<T> slerp(const quat<T> &q0_,                                 // spherical linear quaternion interpolation: f(t=0)=q0, f(t=1)=q1
                                              const quat<T> &q1_,
                                              typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE quat<T> rot_u(const vec3<T> &src_, const vec3<T> &dst_);          // generate quaternion rotation from unit source direction to unit destination direction
template<typename T> EGL_INLINE quat<T> zrot_u(const vec3<T> &dst_);                              // generate quaternion rotation from z-direction to unit destination direction
template<typename T> EGL_INLINE quat<T> axis_rot(const vec3<T> &axis_,                            // generate quaternion rotation from axis and angle (radians)
                                                 typename math<T>::scalar_t angle_);
template<typename T> EGL_INLINE quat<T> axis_rot_u(const vec3<T> &axis_,                          // generate quaternion rotation from unit axis and angle (radians)
                                                   typename math<T>::scalar_t angle_);
template<typename T> void convert_u(mat33<T>&, const quat<T>&);                                   // convert unit quaternion to left handed row-major 3x3 rotation matrix
template<typename T> void convert(mat33<T>&, const quat<T>&);                                     // convert (possibly non-unit) quaternion to left handed row-major 3x3 rotation matrix
template<typename T> void convert(quat<T>&, const mat33<T>&);                                     // convert left handed row-major 3x3 rotation matrix to quaternion
template<typename T, typename U> EGL_INLINE quat<T> to_quat(const quat<U>&);                      // convert between quat types
template<unsigned shuffle_, typename T> EGL_INLINE quat<T> shuffle(const quat<T>&);               // shuffle quaternion components (e.g. shuffle<2311>(q_) == quatf(v_.z, v_.w, v_.y, v_.y))
template<typename T> EGL_INLINE void pack_s1(quat_32s&, const quat<T>&);                          // pack quat (components in range [-1, 1]) to 32-bit quat
template<typename T> EGL_INLINE void pack_u1(quat_32u&, const quat<T>&);                          // pack quat (components in range [0, 1]) to 32-bit quat
template<typename T> EGL_INLINE void unpack_s1(quat<T>&, const quat_32s&);                        // unpack 32-bit quat to quat (components in range [-1, 1])
template<typename T> EGL_INLINE void unpack_u1(quat<T>&, const quat_32u&);                        // unpack 32-bit quat to quat (components in range [0, 1])
template<typename T> EGL_INLINE const vec3<T> &vec(const quat<T>&);                               // vector component of the quaternion
template<typename T> EGL_INLINE vec3<T> &vec(quat<T>&);                                           // vector component of the quaternion
// complex number ops
template<typename T> EGL_INLINE bool is_zero(const complex<T>&);                                        // test of zero-complex number, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const complex<T>&);                                         // test for saturated complex number, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const complex<T>&);                                        // test for signed saturated complex number, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const complex<T>&, const complex<T>&);                  // test for equality of complex numbers, i.e. all components of the complex numbers are equal (exact)
template<typename T> EGL_INLINE bool operator==(const complex<T>&, typename math<T>::scalar_t);         // test for equality of complex number and scalar, i.e. all components of the complex number equals the scalar (exact)
template<typename T> EGL_INLINE bool operator==(typename math<T>::scalar_t, const complex<T>&);         // test for equality of complex number and scalar, i.e. all components of the complex number equals the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(const complex<T>&, const complex<T>&);                  // test for inequality of complex numbers, i.e. any of the components of the complex numbers are unequal (exact)
template<typename T> EGL_INLINE bool operator!=(const complex<T>&, typename math<T>::scalar_t);         // test for inequality of complex number and scalar, i.e. any of the components of the complex number is unequal to the scalar (exact)
template<typename T> EGL_INLINE bool operator!=(typename math<T>::scalar_t, const complex<T>&);         // test for inequality of complex number and scalar, i.e. any of the components of the complex number is unequal to the scalar (exact)
template<typename T> EGL_INLINE complex<T> operator<(const complex<T>&, const complex<T>&);             // component-wise complex number less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator<(const complex<T>&, typename math<T>::scalar_t);    // component-wise complex number less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator<(typename math<T>::scalar_t, const complex<T>&);    // component-wise complex number less-than comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator>(const complex<T>&, const complex<T>&);             // component-wise complex number greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator>(const complex<T>&, typename math<T>::scalar_t);    // component-wise complex number greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator>(typename math<T>::scalar_t, const complex<T>&);    // component-wise complex number greater-than comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator<=(const complex<T>&, const complex<T>&);            // component-wise complex number less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator<=(const complex<T>&, typename math<T>::scalar_t);   // component-wise complex number less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator<=(typename math<T>::scalar_t, const complex<T>&);   // component-wise complex number less-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator>=(const complex<T>&, const complex<T>&);            // component-wise complex number greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator>=(const complex<T>&, typename math<T>::scalar_t);   // component-wise complex number greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> operator>=(typename math<T>::scalar_t, const complex<T>&);   // component-wise complex number greater-or-equal comparison (false=0, true=1)
template<typename T> EGL_INLINE complex<T> sel_eq(const complex<T> &t0_, const complex<T> &t1_,         // component-wise equal complex number select (t0_==t1_?c0_:c1_)
                                                  const complex<T> &c0_, const complex<T> &c1_);
template<typename T> EGL_INLINE complex<T> sel_lt(const complex<T> &t0_, const complex<T> &t1_,         // component-wise less-than complex number select (t0_<t1_?c0_:c1_)
                                                  const complex<T> &c0_, const complex<T> &c1_);
template<typename T> EGL_INLINE complex<T> sel_le(const complex<T> &t0_, const complex<T> &t1_,         // component-wise less-or-equal complex number select (t0_<=t1_?c0_:c1_)
                                                  const complex<T> &c0_, const complex<T> &c1_);
template<typename T> EGL_INLINE complex<T> sel_eqz(const complex<T> &t_,                                // component-wise equal-to-zero complex number select (t_==0?c0_:c1_)
                                                   const complex<T> &c0_, const complex<T> &c1_);
template<typename T> EGL_INLINE complex<T> sel_ltz(const complex<T> &t_,                                // component-wise less-than-zero complex number select (t_<0?c0_:c1_)
                                                   const complex<T> &c0_, const complex<T> &c1_);
template<typename T> EGL_INLINE complex<T> sel_lez(const complex<T> &t_,                                // component-wise less-or-equal-to-zero complex number select (t_<=0?c0_:c1_)
                                                   const complex<T> &c0_, const complex<T> &c1_);
template<typename T> EGL_INLINE void operator+=(complex<T>&, const complex<T>&);                        // add complex number to complex number
template<typename T> EGL_INLINE void operator+=(complex<T>&, typename math<T>::scalar_t);               // add scalar to each component of the complex number
template<typename T> EGL_INLINE void operator-=(complex<T>&, const complex<T>&);                        // subtract complex number from complex number
template<typename T> EGL_INLINE void operator-=(complex<T>&, typename math<T>::scalar_t);               // subtract scalar from each component of the complex number
template<typename T> EGL_INLINE void operator*=(complex<T>&, const complex<T>&);                        // multiply complex number by complex number
template<typename T> EGL_INLINE void operator*=(complex<T>&, typename math<T>::scalar_t);               // multiply complex number by scalar
template<typename T> EGL_INLINE void operator/=(complex<T>&, typename math<T>::scalar_t);               // divide complex number by scalar
template<typename T> EGL_INLINE complex<T> operator+(const complex<T>&, const complex<T>&);             // add complex number to complex number
template<typename T> EGL_INLINE complex<T> operator+(const complex<T>&, typename math<T>::scalar_t);    // add scalar to each component of the complex number
template<typename T> EGL_INLINE complex<T> operator+(typename math<T>::scalar_t, const complex<T>&);    // add each component of the complex number to a scalar complex number
template<typename T> EGL_INLINE complex<T> operator-(const complex<T>&, const complex<T>&);             // subtract complex number from complex number
template<typename T> EGL_INLINE complex<T> operator-(const complex<T>&, typename math<T>::scalar_t);    // subtract scalar from each component of the complex number
template<typename T> EGL_INLINE complex<T> operator-(typename math<T>::scalar_t, const complex<T>&);    // subtract each component of the complex number from a scalar complex number
template<typename T> EGL_INLINE complex<T> operator-(const complex<T>&);                                // negate the complex number
template<typename T> EGL_INLINE complex<T> operator*(const complex<T>&, const complex<T>&);             // multiply complex number by complex number
template<typename T> EGL_INLINE complex<T> operator*(const complex<T>&, typename math<T>::scalar_t);    // complex number by scalar multiplication
template<typename T> EGL_INLINE complex<T> operator*(typename math<T>::scalar_t, const complex<T>&);    // complex number by scalar multiplication
template<typename T> EGL_INLINE complex<T> operator/(const complex<T>&, typename math<T>::scalar_t);    // complex number by scalar division
template<typename T> EGL_INLINE typename math<T>::scalar_t min(const complex<T>&);                      // minimum component value
template<typename T> EGL_INLINE complex<T> min(const complex<T>&, const complex<T>&);                   // component-wise minimum of the complex numbers
template<typename T> EGL_INLINE complex<T> min(const complex<T>&, const complex<T>&, const complex<T>&);
template<typename T> EGL_INLINE complex<T> min(const complex<T>&, const complex<T>&, const complex<T>&, const complex<T>&);
template<typename T> EGL_INLINE complex<T> min(const complex<T>&, typename math<T>::scalar_t);          // component-wise minimum of the complex number and scalar
template<typename T> EGL_INLINE complex<T> min(typename math<T>::scalar_t, const complex<T>&);          // component-wise minimum of the complex number and scalar
template<typename T> EGL_INLINE typename math<T>::scalar_t max(const complex<T>&);                      // maximum component value
template<typename T> EGL_INLINE complex<T> max(const complex<T>&, const complex<T>&);                   // component-wise maximum of the complex numbers
template<typename T> EGL_INLINE complex<T> max(const complex<T>&, const complex<T>&, const complex<T>&);
template<typename T> EGL_INLINE complex<T> max(const complex<T>&, const complex<T>&, const complex<T>&, const complex<T>&);
template<typename T> EGL_INLINE complex<T> max(const complex<T>&, typename math<T>::scalar_t);          // component-wise maximum of the complex number and scalar
template<typename T> EGL_INLINE complex<T> max(typename math<T>::scalar_t, const complex<T>&);          // component-wise maximum of the complex number and scalar
template<typename T> EGL_INLINE complex<T> mul(const complex<T> &c0_, const complex<T> &c1_);           // component-wise multiply: c0_*c1_
template<typename T> EGL_INLINE complex<T> madd(const complex<T> &c0_,                                  // component-wise multiply-add: c0_*c1+c2_
                                                const complex<T> &c1_,
                                                const complex<T> &c2_);
template<typename T> EGL_INLINE complex<T> madd(const complex<T> &c_,                                   // complex-scalar-scalar multiply-add: c_*mul_+add_
                                                typename math<T>::scalar_t mul_,
                                                typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE complex<T> madd(const complex<T> &c_,                                   // complex-complex-scalar multiply-add: c_*mul_+add_
                                                const complex<T> &mul_,
                                                typename math<T>::scalar_t add_);
template<typename T> EGL_INLINE complex<T> madd(const complex<T> &c_,                                   // complex-scalar-complex multiply-add: c_*mul_+add_
                                                typename math<T>::scalar_t mul_,
                                                const complex<T> &add_);
template<typename T> EGL_INLINE complex<T> rcp(const complex<T>&);                                      // component-wise complex number reciprocal
template<typename T> EGL_INLINE complex<T> rcp_z(const complex<T>&);                                    // component-wise complex number reciprocal (set 0 for 0 components)
template<typename T> EGL_INLINE complex<T> floor(const complex<T>&);                                    // component-wise greatest integer less than or equal to the value
template<typename T> EGL_INLINE complex<T> ceil(const complex<T>&);                                     // component-wise smallest integer greater than or equal to the value
template<typename T> EGL_INLINE complex<T> trunc(const complex<T>&);                                    // component-wise truncate decimal part from the value
template<typename T> EGL_INLINE complex<T> frc(const complex<T>&);                                      // component-wise fractional part (for negative values returns 1-fractional part)
template<typename T> EGL_INLINE complex<T> mod(const complex<T>&, typename math<T>::scalar_t div_);     // component-wise modulo
template<typename T> EGL_INLINE complex<T> cycle(const complex<T>&, typename math<T>::scalar_t cycle_); // component-wise cycle
template<typename T> EGL_INLINE complex<T> cycle1(const complex<T>&);                                   // component-wise cycle1
template<typename T> EGL_INLINE complex<T> sat(const complex<T>&);                                      // component-wise complex number saturate
template<typename T> EGL_INLINE complex<T> ssat(const complex<T>&);                                     // component-wise complex number signed saturate
template<typename T> EGL_INLINE complex<T> clamp(const complex<T>&,                                     // component-wise clamp of the complex number
                                                 const complex<T> &min_,
                                                 const complex<T> &max_);
template<typename T> EGL_INLINE complex<T> clamp(const complex<T>&,                                     // component-wise clamp of the complex number with scalars
                                                 typename math<T>::scalar_t min_,
                                                 typename math<T>::scalar_t max_);
template<typename T> EGL_INLINE complex<T> abs(const complex<T>&);                                      // component-wise complex number absolute
template<typename T> EGL_INLINE complex<T> sgn(const complex<T>&);                                      // component-wise signum (neg=-1, pos=+1, zero=0)
template<typename T> EGL_INLINE complex<T> sgn_zp(const complex<T>&);                                   // component-wise signum (neg=-1, pos&zero=+1)
template<typename T> EGL_INLINE complex<T> sqr(const complex<T>&);                                      // component-wise square: x*x
template<typename T> EGL_INLINE complex<T> cubic(const complex<T>&);                                    // component-wise cubic: x*x*x
template<typename T> EGL_INLINE complex<T> sqrt(const complex<T>&);                                     // component-wise complex number square root
template<typename T> EGL_INLINE complex<T> sqrt_z(const complex<T>&);                                   // component-wise complex number square root (set 0 for components less than zero)
template<typename T> EGL_INLINE complex<T> cbrt(const complex<T>&);                                     // component-wise cubic root
template<typename T> EGL_INLINE complex<T> rsqrt(const complex<T>&);                                    // component-wise complex number reciprocal square root
template<typename T> EGL_INLINE complex<T> rsqrt_z(const complex<T>&);                                  // component-wise complex number reciprocal square root (set 0 for components less than or equal to zero)
template<typename T> EGL_INLINE complex<T> rcbrt(const complex<T>&);                                    // component-wise reciprocal cubic root
template<typename T> EGL_INLINE complex<T> rcbrt_z(const complex<T>&);                                  // component-wise reciprocal cubic root (set 0 for components that are 0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm(const complex<T>&);                     // complex number norm (length): |c| 
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm(const complex<T>&);                    // reciprocal of the complex number norm: 1/|c|
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm_z(const complex<T>&);                  // reciprocal of the complex number norm: 1/|c| (return 0 if |c|=0)
template<typename T> EGL_INLINE typename math<T>::scalar_t norm2(const complex<T>&);                    // squared complex number norm: |c|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2(const complex<T>&);                   // reciprocal of the squared vector norm: 1/|c|^2
template<typename T> EGL_INLINE typename math<T>::scalar_t rnorm2_z(const complex<T>&);                 // reciprocal of the squared vector norm: 1/|c|^2 (return 0 if |c|=0)
template<typename T> EGL_INLINE complex<T> unit(const complex<T>&);                                     // unit complex number of the complex number: c/|c| 
template<typename T> EGL_INLINE complex<T> unit_z(const complex<T>&);                                   // unit complex number of the complex number (if |c|=0, return c=[0, 0])
template<typename T> EGL_INLINE complex<T> conj(const complex<T>&);                                     // conjugate of the complex number
template<typename T> EGL_INLINE typename math<T>::scalar_t dot(const complex<T>&, const complex<T>&);   // complex number dot-product
template<typename T> EGL_INLINE typename math<T>::scalar_t dot1(const complex<T>&);                     // complex number dot-product with complex number [1, 1] (component sum)
template<typename T> EGL_INLINE typename math<T>::scalar_t arg(const complex<T>&);                      // argument of the complex number
template<typename T> EGL_INLINE void neg(complex<T>&);                                                  // negate the complex number
template<typename T, class Rng> EGL_INLINE void rand_u(complex<T>&, Rng&);                              // random unit complex number
template<typename T, class Rng> EGL_INLINE void rand_real1(complex<T>&, Rng&);                          // random complex number where each component is in range [-1, 1]
template<typename T, class Rng> EGL_INLINE void rand_ureal1(complex<T>&, Rng&);                         // random complex number where each component is in range [0, 1]
template<typename T> EGL_INLINE complex<T> smoothstep(const complex<T>&);                               // component-wise smoothstep function
template<typename T> EGL_INLINE complex<T> smootherstep(const complex<T>&);                             // component-wise smootherstep function
template<typename T> EGL_INLINE complex<T> lerp(const complex<T> &c0_,                                  // linear interpolation of complex numbers: f(t=0)=c0, f(t=1)=c1 
                                                const complex<T> &c1_,
                                                typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE complex<T> nlerp(const complex<T> &c0_,                                 // normalized linear complex number interpolation: f(t=0)=c0/|c0|, f(t=1)=c1/|c1|
                                                 const complex<T> &c1_,
                                                 typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE complex<T> nlerp_z(const complex<T> &c0_,                               // normalized linear complex number interpolation: f(t=0)=c0/|c0|, f(t=1)=c1/|c1|. if |c|=0, return c=[0, 0]
                                                   const complex<T> &c1_,
                                                   typename math<T>::scalar_t t_);
template<typename T, typename U> EGL_INLINE complex<T> to_complex(const complex<U>&);                   // convert between complex number types
template<unsigned shuffle_, typename T> EGL_INLINE complex<T> shuffle(const complex<T>&);               // shuffle complex components (e.g. shuffle<11>(c_) == complexf(v_.y, v_.y))
//----------------------------------------------------------------------------


//============================================================================
// math constants
//============================================================================
template<typename T>
struct math
{
  // pi variants
  static const T pi;          // 3.14159265358979323846 
  static const T quarter_pi;  // 0.25*3.14159265358979323846
  static const T half_pi;     // 0.5*3.14159265358979323846
  static const T two_pi;      // 2.0*3.14159265358979323846
  static const T four_pi;     // 4.0*3.14159265358979323846
  static const T rpi;         // 1.0/3.14159265358979323846 
  static const T quarter_rpi; // 0.25/3.14159265358979323846
  static const T half_rpi;    // 0.5/3.14159265358979323846
  static const T two_rpi;     // 2.0/3.14159265358979323846
  // square root variants
  static const T sqrt2;       // 1.4142135623730950488016887242097
  static const T sqrt3;       // 1.7320508075688772935274463415059
  static const T rsqrt2;      // 1.0/1.4142135623730950488016887242097
  static const T rsqrt3;      // 1.0/1.7320508075688772935274463415059
  // angle conversion factors
  static const T deg_to_rad;  // 0.01745329251994329577
  static const T rad_to_deg;  // 57.29577951308232087680
  // type definitions
  typedef T scalar_t;
};
//----

// partial specializations for basic math components
template<typename T> struct math<vec2<T> >:math<T>    {};
template<typename T> struct math<vec3<T> >:math<T>    {};
template<typename T> struct math<vec4<T> >:math<T>    {};
template<typename T> struct math<mat22<T> >:math<T>   {};
template<typename T> struct math<mat33<T> >:math<T>   {};
template<typename T> struct math<mat44<T> >:math<T>   {};
template<typename T> struct math<quat<T> >:math<T>    {};
template<typename T> struct math<complex<T> >:math<T> {};
//----------------------------------------------------------------------------


//============================================================================
// vec2
//============================================================================
template<typename T>
struct vec2
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim=2};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec2();
  EGL_INLINE vec2(T s_);
  EGL_INLINE vec2(T x_, T y_);
  EGL_INLINE vec2(const vec3<T>&);
  EGL_INLINE vec2(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T x_, T y_);
  EGL_INLINE void set(const vec2<T>&);
  EGL_INLINE void set(const vec3<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec2 zero();
  static EGL_INLINE vec2 one();
  static EGL_INLINE vec2 neg_one();
  //--------------------------------------------------------------------------

  T x, y;
};
//----------------------------------------------------------------------------


//============================================================================
// vec3
//============================================================================
template<typename T>
struct vec3
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim=3};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec3();
  EGL_INLINE vec3(T s_);
  EGL_INLINE vec3(T x_, T y_, T z_);
  EGL_INLINE vec3(const vec2<T>&, T z_);
  EGL_INLINE vec3(const vec4<T>&);
  EGL_INLINE vec3(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T x_, T y_, T z_);
  EGL_INLINE void set(const vec2<T>&, T z_);
  EGL_INLINE void set(const vec3<T>&);
  EGL_INLINE void set(const vec4<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec3 zero();
  static EGL_INLINE vec3 one();
  static EGL_INLINE vec3 neg_one();
  //--------------------------------------------------------------------------

  T x, y, z;
};
//----------------------------------------------------------------------------


//============================================================================
// vec3<vec32s_t>
//============================================================================
template<>
struct vec3<vec32s_t>
{
  typedef vec32s_t type_t;
  typedef int8_t scalar_t;
  enum {dim=3};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec3();
  EGL_INLINE vec3(scalar_t s_);
  EGL_INLINE vec3(scalar_t x_, scalar_t y_, scalar_t z_);
  EGL_INLINE vec3(const vec2<scalar_t>&, scalar_t z_);
  EGL_INLINE vec3(const scalar_t*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const scalar_t &operator[](unsigned idx_) const;
  EGL_INLINE scalar_t &operator[](unsigned idx_);
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t x_, scalar_t y_, scalar_t z_);
  EGL_INLINE void set(const vec2<scalar_t>&, scalar_t z_);
  EGL_INLINE void set(const vec3<type_t>&);
  EGL_INLINE void set(const scalar_t*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec3 zero();
  static EGL_INLINE vec3 one();
  static EGL_INLINE vec3 neg_one();
  //--------------------------------------------------------------------------

  EGL_ALIGN(4) scalar_t x;
  scalar_t y, z, dummy;
};
//----------------------------------------------------------------------------


//============================================================================
// vec3<vec32u_t>
//============================================================================
template<>
struct vec3<vec32u_t>
{
  typedef vec32u_t type_t;
  typedef uint8_t scalar_t;
  enum {dim=3};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec3();
  EGL_INLINE vec3(scalar_t s_);
  EGL_INLINE vec3(scalar_t x_, scalar_t y_, scalar_t z_);
  EGL_INLINE vec3(const vec2<scalar_t>&, scalar_t z_);
  EGL_INLINE vec3(const scalar_t*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const scalar_t &operator[](unsigned idx_) const;
  EGL_INLINE scalar_t &operator[](unsigned idx_);
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t x_, scalar_t y_, scalar_t z_);
  EGL_INLINE void set(const vec2<scalar_t>&, scalar_t z_);
  EGL_INLINE void set(const vec3<type_t>&);
  EGL_INLINE void set(const scalar_t*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec3 zero();
  static EGL_INLINE vec3 one();
  static EGL_INLINE vec3 neg_one();
  //--------------------------------------------------------------------------

  EGL_ALIGN(4) scalar_t x;
  scalar_t y, z, dummy;
};
//----------------------------------------------------------------------------


//============================================================================
// vec4
//============================================================================
template<typename T>
struct vec4
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec4();
  EGL_INLINE vec4(T s_);
  EGL_INLINE vec4(T x_, T y_, T z_, T w_);
  EGL_INLINE vec4(const vec3<T>&, T w_);
  EGL_INLINE vec4(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T x_, T y_, T z_, T w_);
  EGL_INLINE void set(const vec3<T>&, T w_);
  EGL_INLINE void set(const vec4<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec4 zero();
  static EGL_INLINE vec4 one();
  static EGL_INLINE vec4 neg_one();
  //--------------------------------------------------------------------------

  T x, y, z, w;
};
//----------------------------------------------------------------------------


//============================================================================
// vec4<vec32s_t>
//============================================================================
template<>
struct vec4<vec32s_t>
{
  typedef vec32s_t type_t;
  typedef int8_t scalar_t;
  enum {dim=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec4();
  EGL_INLINE vec4(scalar_t s_);
  EGL_INLINE vec4(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE vec4(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE vec4(const scalar_t*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const scalar_t &operator[](unsigned idx_) const;
  EGL_INLINE scalar_t &operator[](unsigned idx_);
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE void set(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE void set(const vec4<type_t>&);
  EGL_INLINE void set(const scalar_t*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec4 zero();
  static EGL_INLINE vec4 one();
  static EGL_INLINE vec4 neg_one();
  //--------------------------------------------------------------------------

  EGL_ALIGN(4) scalar_t x;
  scalar_t y, z, w;
};
//----------------------------------------------------------------------------


//============================================================================
// vec4<vec32u_t>
//============================================================================
template<>
struct vec4<vec32u_t>
{
  typedef vec32u_t type_t;
  typedef uint8_t scalar_t;
  enum {dim=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE vec4();
  EGL_INLINE vec4(scalar_t s_);
  EGL_INLINE vec4(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE vec4(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE vec4(const scalar_t*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const scalar_t &operator[](unsigned idx_) const;
  EGL_INLINE scalar_t &operator[](unsigned idx_);
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE void set(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE void set(const vec4<type_t>&);
  EGL_INLINE void set(const scalar_t*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE vec4 zero();
  static EGL_INLINE vec4 one();
  static EGL_INLINE vec4 neg_one();
  //--------------------------------------------------------------------------

  EGL_ALIGN(4) scalar_t x;
  scalar_t y, z, w;
};
//----------------------------------------------------------------------------


//============================================================================
// mat22
//============================================================================
template<typename T>
struct mat22
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim_rows=2, dim_columns=2};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE mat22();
  EGL_INLINE mat22(T s_);
  EGL_INLINE mat22(T d0_, T d1_);
  EGL_INLINE mat22(T m00_, T m01_,
                   T m10_, T m11_);
  EGL_INLINE mat22(const vec2<T> &diag_);
  EGL_INLINE mat22(const vec2<T> &row0_,
                   const vec2<T> &row1_);
  EGL_INLINE mat22(const mat33<T>&);
  EGL_INLINE mat22(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const vec2<T> &operator[](unsigned row_) const;
  EGL_INLINE vec2<T> &operator[](unsigned row_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T d0_, T d1_);
  EGL_INLINE void set(T m00_, T m01_,
                      T m10_, T m11_);
  EGL_INLINE void set(const vec2<T> &row0_,
                      const vec2<T> &row1_);
  EGL_INLINE void set(const mat22<T>&);
  EGL_INLINE void set(const mat33<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE mat22 zero();
  static EGL_INLINE mat22 one();
  static EGL_INLINE mat22 neg_one();
  static EGL_INLINE mat22 identity();
  //--------------------------------------------------------------------------

  vec2<T> x;
  vec2<T> y;
};
//----------------------------------------------------------------------------


//============================================================================
// mat33
//============================================================================
template<typename T>
struct mat33
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim_rows=3, dim_columns=3};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE mat33();
  EGL_INLINE mat33(T s_);
  EGL_INLINE mat33(T d0_, T d1_, T d2_);
  EGL_INLINE mat33(T m00_, T m01_, T m02_,
                   T m10_, T m11_, T m12_,
                   T m20_, T m21_, T m22_);
  EGL_INLINE mat33(const vec3<T> &diag_);
  EGL_INLINE mat33(const vec3<T> &row0_,
                   const vec3<T> &row1_,
                   const vec3<T> &row2_);
  EGL_INLINE mat33(const mat22<T>&);
  EGL_INLINE mat33(const mat44<T>&);
  EGL_INLINE mat33(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const vec3<T> &operator[](unsigned row_) const;
  EGL_INLINE vec3<T> &operator[](unsigned row_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T d0_, T d1_, T d2_);
  EGL_INLINE void set(T m00_, T m01_, T m02_,
                      T m10_, T m11_, T m12_,
                      T m20_, T m21_, T m22_);
  EGL_INLINE void set(const vec3<T> &row0_,
                      const vec3<T> &row1_,
                      const vec3<T> &row2_);
  EGL_INLINE void set(const mat22<T>&);
  EGL_INLINE void set(const mat33<T>&);
  EGL_INLINE void set(const mat44<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE mat33 zero();
  static EGL_INLINE mat33 one();
  static EGL_INLINE mat33 neg_one();
  static EGL_INLINE mat33 identity();
  //--------------------------------------------------------------------------

  vec3<T> x;
  vec3<T> y;
  vec3<T> z;
};
//----------------------------------------------------------------------------


//============================================================================
// mat44
//============================================================================
template<typename T>
struct mat44
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim_rows=4, dim_columns=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE mat44();
  EGL_INLINE mat44(T s_);
  EGL_INLINE mat44(T d0_, T d1_, T d2_, T d3_);
  EGL_INLINE mat44(T m00_, T m01_, T m02_, T m03_,
                   T m10_, T m11_, T m12_, T m13_,
                   T m20_, T m21_, T m22_, T m23_,
                   T m30_, T m31_, T m32_, T m33_);
  EGL_INLINE mat44(const vec4<T> &diag_);
  EGL_INLINE mat44(const vec4<T> &row0_,
                   const vec4<T> &row1_,
                   const vec4<T> &row2_,
                   const vec4<T> &row3_);
  EGL_INLINE mat44(const mat33<T>&);
  EGL_INLINE mat44(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const vec4<T> &operator[](unsigned row_) const;
  EGL_INLINE vec4<T> &operator[](unsigned row_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T d0_, T d1_, T d2_, T d3_);
  EGL_INLINE void set(T m00_, T m01_, T m02_, T m03_,
                      T m10_, T m11_, T m12_, T m13_,
                      T m20_, T m21_, T m22_, T m23_,
                      T m30_, T m31_, T m32_, T m33_);
  EGL_INLINE void set(const vec4<T> &row0_,
                      const vec4<T> &row1_,
                      const vec4<T> &row2_,
                      const vec4<T> &row3_);
  EGL_INLINE void set(const mat33<T>&);
  EGL_INLINE void set(const mat44<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE mat44 zero();
  static EGL_INLINE mat44 one();
  static EGL_INLINE mat44 neg_one();
  static EGL_INLINE mat44 identity();
  //--------------------------------------------------------------------------

  vec4<T> x;
  vec4<T> y;
  vec4<T> z;
  vec4<T> w;
};
//----------------------------------------------------------------------------


//============================================================================
// quat
//============================================================================
template<typename T>
struct quat
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE quat();
  EGL_INLINE quat(T s_);
  EGL_INLINE quat(T x_, T y_, T z_, T w_);
  EGL_INLINE quat(const vec3<T>&);
  EGL_INLINE quat(const vec3<T>&, T w_);
  EGL_INLINE quat(const vec4<T>&);
  EGL_INLINE quat(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T x_, T y_, T z_, T w_);
  EGL_INLINE void set(const vec3<T>&);
  EGL_INLINE void set(const vec3<T>&, T w_);
  EGL_INLINE void set(const vec4<T>&);
  EGL_INLINE void set(const quat<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE quat zero();
  static EGL_INLINE quat one();
  static EGL_INLINE quat neg_one();
  static EGL_INLINE quat identity();
  //--------------------------------------------------------------------------

  T x, y, z, w;
};
//----------------------------------------------------------------------------


//============================================================================
// quat<vec32s_t>
//============================================================================
template<>
struct quat<vec32s_t>
{
  typedef vec32s_t type_t;
  typedef int8_t scalar_t;
  enum {dim=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE quat();
  EGL_INLINE quat(scalar_t s_);
  EGL_INLINE quat(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE quat(const vec3<type_t>&);
  EGL_INLINE quat(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE quat(const vec4<type_t>&);
  EGL_INLINE quat(const scalar_t*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const scalar_t &operator[](unsigned idx_) const;
  EGL_INLINE scalar_t &operator[](unsigned idx_);
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE void set(const vec3<type_t>&);
  EGL_INLINE void set(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE void set(const vec4<type_t>&);
  EGL_INLINE void set(const quat<type_t>&);
  EGL_INLINE void set(const scalar_t*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE quat zero();
  static EGL_INLINE quat one();
  static EGL_INLINE quat neg_one();
  static EGL_INLINE quat identity();
  //--------------------------------------------------------------------------

  EGL_ALIGN(4) scalar_t x;
  scalar_t y, z, w;
};
//----------------------------------------------------------------------------


//============================================================================
// quat<vec32u_t>
//============================================================================
template<>
struct quat<vec32u_t>
{
  typedef vec32u_t type_t;
  typedef uint8_t scalar_t;
  enum {dim=4};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE quat();
  EGL_INLINE quat(scalar_t s_);
  EGL_INLINE quat(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE quat(const vec3<type_t>&);
  EGL_INLINE quat(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE quat(const vec4<type_t>&);
  EGL_INLINE quat(const scalar_t*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const scalar_t &operator[](unsigned idx_) const;
  EGL_INLINE scalar_t &operator[](unsigned idx_);
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t x_, scalar_t y_, scalar_t z_, scalar_t w_);
  EGL_INLINE void set(const vec3<type_t>&);
  EGL_INLINE void set(const vec3<type_t>&, scalar_t w_);
  EGL_INLINE void set(const vec4<type_t>&);
  EGL_INLINE void set(const quat<type_t>&);
  EGL_INLINE void set(const scalar_t*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE quat zero();
  static EGL_INLINE quat one();
  static EGL_INLINE quat neg_one();
  static EGL_INLINE quat identity();
  //--------------------------------------------------------------------------

  EGL_ALIGN(4) scalar_t x;
  scalar_t y, z, w;
};
//----------------------------------------------------------------------------


//============================================================================
// complex
//============================================================================
template<typename T>
struct complex
{
  typedef T type_t;
  typedef typename math<T>::scalar_t scalar_t;
  enum {dim=2};
  //--------------------------------------------------------------------------

  // construction
  EGL_INLINE complex();
  EGL_INLINE complex(T s_);
  EGL_INLINE complex(T real_, T imag_);
  EGL_INLINE complex(const T*);
  //--------------------------------------------------------------------------

  // accessors and mutators
  EGL_INLINE const T &operator[](unsigned idx_) const;
  EGL_INLINE T &operator[](unsigned idx_);
  EGL_INLINE void set(T s_);
  EGL_INLINE void set(T real_, T imag_);
  EGL_INLINE void set(const complex<T>&);
  EGL_INLINE void set(const T*);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE complex zero();
  static EGL_INLINE complex one();
  static EGL_INLINE complex neg_one();
  //--------------------------------------------------------------------------

  T real, imag;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// scalar ops
//============================================================================
template<typename T>
EGL_INLINE T mul(T a_, T b_)
{
  // multiply
  return a_*b_;
}
//----

template<typename T>
EGL_INLINE T madd(T a_, T b_, T c_)
{
  // multiply-add
  return a_*b_+c_;
}
//----

template<typename T>
EGL_INLINE T rcp(T v_)
{
  // reciprocal
  EGL_ASSERT_PEDANTIC(v_!=0);
  return T(1)/v_;
}
//----

template<typename T>
EGL_INLINE T rcp_z(T v_)
{
  // reciprocal (return 0 for 0)
  return v_?T(1)/v_:0;
}
//----

template<typename T>
EGL_INLINE T floor(T v_)
{
  // round to floor
  return MATH_NAMESPACE::floor(v_);
}
//----

template<typename T>
EGL_INLINE T ceil(T v_)
{
  // round to ceil
  return MATH_NAMESPACE::ceil(v_);
}
//----

template<typename T>
EGL_INLINE T trunc(T v_)
{
  // round towards zero
  return T(int(v_));
}
//----

template<typename T>
EGL_INLINE T frc(T v_)
{
  // fractional
  return v_-T(int(v_));
}
//----

template<typename T>
EGL_INLINE T mod(T v_, T div_)
{
  // modulo
  return MATH_NAMESPACE::fmod(v_, div_);
}
//----

template<typename T>
EGL_INLINE T cycle(T v_, T cycle_)
{
  // cycle
  T v=T(MATH_NAMESPACE::fmod(v_, cycle_));
  return v<0?cycle_+v:v;
}
//----

template<typename T>
EGL_INLINE T cycle1(T v_)
{
  // cycle1
  return v_-T(int(v_))+(v_<0?T(1):0);
}
//----

template<typename T>
EGL_INLINE T sat(T v_)
{
  // saturate to range [0, 1]
  return v_<0?0:v_>T(1)?T(1):v_;
}
//----

template<typename T>
EGL_INLINE T ssat(T v_)
{
  // signed saturate to range [-1, 1]
  return v_<T(-1)?T(-1):v_>T(1)?T(1):v_;
}
//----

template<typename T>
EGL_INLINE T clamp(T v_, T min_, T max_)
{
  // clamp to range [min, max]
  EGL_ASSERT_PEDANTIC(min_<=max_);
  return v_<min_?min_:v_>max_?max_:v_;
}
//----

template<typename T>
EGL_INLINE T abs(T v_)
{
  // absolule
  return v_<0?-v_:v_;
}
//----

template<typename T>
EGL_INLINE T sgn(T v_)
{
  // signum (v<0 => -1, v>0 => +1, v==0 => 0)
  return v_<0?T(-1):v_>0?T(1):0;
}
//----

template<typename T>
EGL_INLINE T sgn_zp(T v_)
{
  // signum (v<0 => -1, v>=0 => +1)
  return v_<0?T(-1):T(1);
}
//----

template<typename T>
EGL_INLINE T sqr(T v_)
{
  // square
  return v_*v_;
}
//----

template<typename T>
EGL_INLINE T cubic(T v_)
{
  // cubic
  return v_*v_*v_;
}
//----

template<typename T>
EGL_INLINE T sqrt(T v_)
{
  // square root
  EGL_ASSERT_PEDANTIC(v_>=0);
  return T(MATH_NAMESPACE::sqrt(v_));
}
//----

template<typename T>
EGL_INLINE T sqrt_z(T v_)
{
  // square root (return 0 for values <0)
  return v_>0?T(MATH_NAMESPACE::sqrt(v_)):0;
}
//----

template<typename T>
EGL_INLINE T cbrt(T v_)
{
  // cubic root
  return v_<0?-T(MATH_NAMESPACE::pow(-v_, T(1.0/3.0))):T(MATH_NAMESPACE::pow(v_, T(1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE T rsqrt(T v_)
{
  // reciprocal square root
  EGL_ASSERT_PEDANTIC(v_>0);
  return T(1)/MATH_NAMESPACE::sqrt(v_);
}
//----

template<typename T>
EGL_INLINE T rsqrt_z(T v_)
{
  // reciprocal square root (return 0 for values <=0)
  return v_>0?T(1)/MATH_NAMESPACE::sqrt(v_):0;
}
//----

template<typename T>
EGL_INLINE T rcbrt(T v_)
{
  // reciprocal cubic root
  return v_<0?-MATH_NAMESPACE::pow(-v_, T(-1.0/3.0)): MATH_NAMESPACE::pow(v_, T(-1.0/3.0));
}
//----

template<typename T>
EGL_INLINE T rcbrt_z(T v_)
{
  // reciprocal cubic root (return 0 for 0)
  return v_?v_<0?-MATH_NAMESPACE::pow(-v_, T(-1.0/3.0)): MATH_NAMESPACE::pow(v_, T(-1.0/3.0)):0;
}
//----

template<typename T>
EGL_INLINE T norm(T v_)
{
  // 1d norm (absolute)
  return v_<0?-v_:v_;
}
//----

template<typename T>
EGL_INLINE T rnorm(T v_)
{
  // 1d norm reciprocal
  EGL_ASSERT_PEDANTIC(v_);
  return T(1)/(v_<0?-v_:v_);
}
//----

template<typename T>
EGL_INLINE T rnorm_z(T v_)
{
  // 1d norm reciprocal (return 0 for 0)
  return v_?T(1)/(v_<0?-v_:v_):0;
}
//----

template<typename T>
EGL_INLINE T norm2(T v_)
{
  // squared norm (square)
  return v_*v_;
}
//----

template<typename T>
EGL_INLINE T rnorm2(T v_)
{
  // reciprocal squared norm
  EGL_ASSERT_PEDANTIC(v_);
  return T(1)/(v_*v_);
}
//----

template<typename T>
EGL_INLINE T rnorm2_z(T v_)
{
  // reciprocal squared norm (return 0 for 0)
  return v_?T(1)/(v_*v_):0;
}
//----

template<typename T>
EGL_INLINE T norm_l1(T v_)
{
  // 1d L1-norm (absolute)
  return v_<0?-v_:v_;
}
//----

template<typename T>
EGL_INLINE T rnorm_l1(T v_)
{
  // 1d L1-norm reciprocal
  return T(1)/(v_<0?-v_:v_);
}
//----

template<typename T>
EGL_INLINE T rnorm_l1_z(T v_)
{
  // 1d L1-norm reciprocal (return 0 for 0)
  return v_?T(1)/(v_<0?-v_:v_):0;
}
//----

template<typename T>
EGL_INLINE T exp(T v_)
{
  // natural exponent
  return T(MATH_NAMESPACE::exp(v_));
}
//----

template<typename T>
EGL_INLINE T exp2(T v_)
{
  // 2^x
  return T(MATH_NAMESPACE::pow(T(2), v_));
}
//----

template<typename T>
EGL_INLINE T ln(T v_)
{
  // nature logarithm
  EGL_ASSERT_PEDANTIC(v_>0);
  return T(MATH_NAMESPACE::log(v_));
}
//----

template<typename T>
EGL_INLINE T log2(T v_)
{
  // base-2 logarithm
  EGL_ASSERT_PEDANTIC(v_>0);
  return T(MATH_NAMESPACE::log(v_))*T(1.44269504089);
}
//----

template<typename T>
EGL_INLINE T log10(T v_)
{
  // base-10 logarithm
  EGL_ASSERT_PEDANTIC(v_>0);
  return T(MATH_NAMESPACE::log10(v_));
}
//----

template<typename T>
EGL_INLINE T pow(T s_, T p_)
{
  // rise value to power
  return T(MATH_NAMESPACE::pow(s_, p_));
}
//----

template<typename T>
EGL_INLINE T smoothstep(T t_)
{
  // smoothstep function: 3t^2-2t^3
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(t_>=0 && t_<=scalar_t(1));
  return t_*t_*(scalar_t(3)-scalar_t(2)*t_);
}
//----

template<typename T>
EGL_INLINE T smootherstep(T t_)
{
  // smootherstep function: 6t^5-15t^4+10t^3 (zero 2nd derivative at end points)
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(t_>=0 && t_<=scalar_t(1));
  return t_*t_*t_*(t_*(t_*scalar_t(6)-scalar_t(15))+scalar_t(10));
}
//----

template<typename T>
EGL_INLINE T lerp(T a_, T b_, T t_)
{
  // linear interpolate (f(t=0)=a, f(t=1)=b)
  return a_+(b_-a_)*t_;
}
//----

template<typename T>
EGL_INLINE T dot(T x_, T y_)
{
  // 1d dot product (multiply)
  return x_*y_;
}
//----

EGL_INLINE float32_t u16f_to_fp32(uint16_t u16f_)
{
  // convert unsigned 16-bit float to 32-bit float
  union convert
  {
    uint32_t u32;
    float32_t f32;
  } f2u;
  static const convert s_magic={(127+(127-15))<<23};
  f2u.u32=uint32_t(u16f_)<<13;
  f2u.f32*=s_magic.f32;
  return f2u.f32;
}
//----

EGL_INLINE float32_t s16f_to_fp32(uint16_t s16f_)
{
  // convert signed 16-bit float to 32-bit float
  union convert
  {
    uint32_t u32;
    float32_t f32;
  } f2u;
  static const convert s_magic={(127+(127-15))<<23};
  f2u.u32=uint32_t(s16f_)<<13;
  f2u.f32*=s_magic.f32;
  return f2u.f32;
}
//----

EGL_INLINE uint16_t fp32_to_u16f(float32_t v_)
{
  // convert 32-bit float to unsigned 16-bit float
  union
  {
    uint32_t u;
    float32_t f;
  } vc;
  vc.f=v_;
  uint16_t v=uint16_t(vc.u>>13);
  return v+(v?0x4000:0);
}
//----

EGL_INLINE uint16_t fp32_to_s16f(float32_t v_)
{
  // convert 32-bit float to signed 16-bit float
  union
  {
    uint32_t u;
    float32_t f;
  } vc;
  vc.f=v_;
  uint16_t v=uint16_t(vc.u>>13);
  return v+(v?0x4000:0);
}
//----------------------------------------------------------------------------


//============================================================================
// trigonometric scalar ops
//============================================================================
template<typename T>
EGL_INLINE T sin(T v_)
{
  // sine
  return MATH_NAMESPACE::sin(v_);
}
//----

template<typename T>
EGL_INLINE T cos(T v_)
{
  // cosine
  return MATH_NAMESPACE::cos(v_);
}
//----

template<typename T>
EGL_INLINE void sincos(T &sin_, T &cos_, T v_)
{
  // sine & cosine
  sin_=MATH_NAMESPACE::sin(v_);
  cos_=MATH_NAMESPACE::cos(v_);
}
//----

template<typename T>
EGL_INLINE T tan(T v_)
{
  // tangent
  return MATH_NAMESPACE::tan(v_);
}
//----

template<typename T>
EGL_INLINE T csc(T v_)
{
  // cosecant
  return T(1)/MATH_NAMESPACE::sin(v_);
}
//----

template<typename T>
EGL_INLINE T sec(T v_)
{
  // secant
  return T(1)/MATH_NAMESPACE::cos(v_);
}
//----

template<typename T>
EGL_INLINE T cot(T v_)
{
  // cotangent
  return T(1)/MATH_NAMESPACE::tan(v_);
}
//----

template<typename T>
EGL_INLINE T asin(T v_)
{
  // inverse sine
  EGL_ASSERT_PEDANTIC(v_>=T(-1) && v_<=T(1));
  return MATH_NAMESPACE::asin(v_);
}
//----

template<typename T>
EGL_INLINE T acos(T v_)
{
  // inverse cosine
  EGL_ASSERT_PEDANTIC(v_>=T(-1) && v_<=T(1));
  return MATH_NAMESPACE::acos(v_);
}
//----

template<typename T>
EGL_INLINE T atan(T v_)
{
  // inverse tangent
  return MATH_NAMESPACE::atan(v_);
}
//----

template<typename T>
EGL_INLINE T atan2(T y_, T x_)
{
  // four-quadrant inverse tangent
  return MATH_NAMESPACE::atan2(y_, x_);
}
//----

template<typename T>
EGL_INLINE T acsc(T v_)
{
  // inverse cosecant
  return MATH_NAMESPACE::asin(T(1)/v_);
}
//----

template<typename T>
EGL_INLINE T asec(T v_)
{
  // inverse secant
  return MATH_NAMESPACE::acos(T(1)/v_);
}
//----

template<typename T>
EGL_INLINE T acot(T v_)
{
  // inverse cotangent
  return MATH_NAMESPACE::atan(T(1)/v_);
}
//----

template<typename T>
EGL_INLINE T sinh(T v_)
{
  // hyperbolic sine
  return MATH_NAMESPACE::sinh(v_);
}
//----

template<typename T>
EGL_INLINE T cosh(T v_)
{
  // hyperbolic cosine
  return MATH_NAMESPACE::cosh(v_);
}
//----

template<typename T>
EGL_INLINE T tanh(T v_)
{
  // hyperbolic tangent
  return MATH_NAMESPACE::tanh(v_);
}
//----

template<typename T>
EGL_INLINE T csch(T v_)
{
  // hyperbolic cosecant
  return T(1)/MATH_NAMESPACE::sinh(v_);
}
//----

template<typename T>
EGL_INLINE T sech(T v_)
{
  // hyperbolic secant
  return T(1)/MATH_NAMESPACE::cosh(v_);
}
//----

template<typename T>
EGL_INLINE T coth(T v_)
{
  // hyperbolic cotangent
  return T(1)/MATH_NAMESPACE::tanh(v_);
}
//----

template<typename T>
EGL_INLINE T asinh(T v_)
{
  // inverse hyperbolic sine
  return MATH_NAMESPACE::log(v_+MATH_NAMESPACE::sqrt(v_*v_+T(1)));
}
//----

template<typename T>
EGL_INLINE T acosh(T v_)
{
  // inverse hyperbolic cosine
  EGL_ASSERT_PEDANTIC(v_>=1);
  return MATH_NAMESPACE::log(v_+MATH_NAMESPACE::sqrt(v_*v_-T(1)));
}
//----

template<typename T>
EGL_INLINE T atanh(T v_)
{
  // inverse hyperbolic tangent
  EGL_ASSERT_PEDANTIC(v_>T(-1) && v_<T(1));
  return T(0.5)*MATH_NAMESPACE::log((T(1)+v_)/(T(1)-v_));
}
//----

template<typename T>
EGL_INLINE T acsch(T v_)
{
  // inverse hyperbolic cosecant
  EGL_ASSERT_PEDANTIC(v_);
  T rv=T(1)/v_;
  return MATH_NAMESPACE::log(rv+MATH_NAMESPACE::sqrt(rv*rv+T(1)));
}
//----

template<typename T>
EGL_INLINE T asech(T v_)
{
  // inverse hyperbolic secant
  EGL_ASSERT_PEDANTIC(v_>0 && v_<=T(1));
  T rv=T(1)/v_;
  return MATH_NAMESPACE::log(rv+MATH_NAMESPACE::sqrt(rv*rv-T(1)));
}
//----

template<typename T>
EGL_INLINE T acoth(T v_)
{
  // inverse hyperbolic cotangent
  EGL_ASSERT_PEDANTIC(v_<T(-1) || v_>T(1));
  T rv=T(1)/v_;
  return T(0.5)*MATH_NAMESPACE::log((T(1)+rv)/(T(1)-rv));
}
//----------------------------------------------------------------------------


//============================================================================
// math constants
//============================================================================
// pi variants
template<typename T> const T math<T>::pi=T(3.14159265358979323846);
template<typename T> const T math<T>::quarter_pi=T(0.25*3.14159265358979323846);
template<typename T> const T math<T>::half_pi=T(0.5*3.14159265358979323846);
template<typename T> const T math<T>::two_pi=T(2.0*3.14159265358979323846);
template<typename T> const T math<T>::four_pi=T(4.0*3.14159265358979323846);
template<typename T> const T math<T>::rpi=T(1.0/3.14159265358979323846);
template<typename T> const T math<T>::quarter_rpi=T(0.25/3.14159265358979323846);
template<typename T> const T math<T>::half_rpi=T(0.5/3.14159265358979323846);
template<typename T> const T math<T>::two_rpi=T(2.0/3.14159265358979323846);
// square root variants
template<typename T> const T math<T>::sqrt2=T(1.4142135623730950488016887242097);
template<typename T> const T math<T>::sqrt3=T(1.7320508075688772935274463415059);
template<typename T> const T math<T>::rsqrt2=T(1.0/1.4142135623730950488016887242097);
template<typename T> const T math<T>::rsqrt3=T(1.0/1.7320508075688772935274463415059);
// angle conversion factors
template<typename T> const T math<T>::deg_to_rad=T(0.01745329251994329577);
template<typename T> const T math<T>::rad_to_deg=T(57.29577951308232087680);
//----------------------------------------------------------------------------


//============================================================================
// vec2
//============================================================================
template<typename T>
vec2<T> vec2<T>::zero()
{
  return vec2<T>(scalar_t(0));
}
//----

template<typename T>
vec2<T> vec2<T>::one()
{
  return vec2<T>(scalar_t(1));
}
//----

template<typename T>
vec2<T> vec2<T>::neg_one()
{
  return vec2<T>(scalar_t(-1));
}
//----------------------------------------------------------------------------

template<typename T>
vec2<T>::vec2()
{
}
//----

template<typename T>
vec2<T>::vec2(T s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
}
//----

template<typename T>
vec2<T>::vec2(T x_, T y_)
{
  // init vector with scalars
  x=x_;
  y=y_;
}
//----

template<typename T>
vec2<T>::vec2(const vec3<T> &v_)
{
  // init vector from 3d vector
  x=v_.x;
  y=v_.y;
}
//----

template<typename T>
vec2<T>::vec2(const T *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
}
//----------------------------------------------------------------------------

template<typename T>
const T &vec2<T>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1)
  EGL_ASSERT_PEDANTIC(idx_<2);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &vec2<T>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1)
  EGL_ASSERT_PEDANTIC(idx_<2);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void vec2<T>::set(T s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
}
//----

template<typename T>
void vec2<T>::set(T x_, T y_)
{
  // set vector with scalars
  x=x_;
  y=y_;
}
//----

template<typename T>
void vec2<T>::set(const vec2<T> &v_)
{
  // set vector from vector
  x=v_.x;
  y=v_.y;
}
//----

template<typename T>
void vec2<T>::set(const vec3<T> &v_)
{
  // set vector from 3d vector
  x=v_.x;
  y=v_.y;
}
//----

template<typename T>
void vec2<T>::set(const T *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const vec2<T> &v_)
{
  // test for zero-vector
  return v_.x==0 && v_.y==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const vec2<T> &v_)
{
  // test for saturated vector
  typedef typename math<T>::scalar_t scalar_t;
  return    v_.x>=0 && v_.x<=scalar_t(1)
         && v_.y>=0 && v_.y<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const vec2<T> &v_)
{
  // test for signed saturated vector
  typedef typename math<T>::scalar_t scalar_t;
  return    v_.x>=scalar_t(-1) && v_.x<=scalar_t(1)
         && v_.y>=scalar_t(-1) && v_.y<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool operator==(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // test for equality of vectors
  return v0_.x==v1_.x && v0_.y==v1_.y;
}
//----

template<typename T>
EGL_INLINE bool operator==(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // test for equality of vector and scalar
  return v_.x==s_ && v_.y==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // test for equality of vector and scalar
  return s_==v_.x && s_==v_.y;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // test for inequality of vectors
  return v0_.x!=v1_.x || v0_.y!=v1_.y;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // test for inequality of vector and scalar
  return v_.x!=s_ || v_.y!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // test for inequality of vector and scalar
  return s_!=v_.x || s_!=v_.y;
}
//----

template<typename T>
EGL_INLINE vec2<T> operator<(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v0_.x<v1_.x?scalar_t(1):0, v0_.y<v1_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator<(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<s_?scalar_t(1):0, v_.y<s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator<(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(s_<v_.x?scalar_t(1):0, s_<v_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator>(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v0_.x>v1_.x?scalar_t(1):0, v0_.y>v1_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator>(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x>s_?scalar_t(1):0, v_.y>s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator>(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(s_>v_.x?scalar_t(1):0, s_>v_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator<=(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v0_.x<=v1_.x?scalar_t(1):0, v0_.y<=v1_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator<=(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<=s_?scalar_t(1):0, v_.y<=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator<=(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(s_<=v_.x?scalar_t(1):0, s_<=v_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator>=(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v0_.x>=v1_.x?scalar_t(1):0, v0_.y>=v1_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator>=(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x>=s_?scalar_t(1):0, v_.y>=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator>=(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(s_>=v_.x?scalar_t(1):0, s_>=v_.y?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> sel_eq(const vec2<T> &t0_, const vec2<T> &t1_, const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise equal vector select (t0_==t1_?v0_:v1_)
  return vec2<T>(t0_.x==t1_.x?v0_.x:v1_.x,
                 t0_.y==t1_.y?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sel_lt(const vec2<T> &t0_, const vec2<T> &t1_, const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise less-than vector select (t0_<t1_?v0_:v1_)
  return vec2<T>(t0_.x<t1_.x?v0_.x:v1_.x,
                 t0_.y<t1_.y?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sel_le(const vec2<T> &t0_, const vec2<T> &t1_, const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise less-or-equal vector select (t0_<=t1_?v0_:v1_)
  return vec2<T>(t0_.x<=t1_.x?v0_.x:v1_.x,
                 t0_.y<=t1_.y?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sel_eqz(const vec2<T> &t_, const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise equal-to-zero vector select (t_==0?v0_:v1_)
  return vec2<T>(t_.x==0?v0_.x:v1_.x,
                 t_.y==0?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sel_ltz(const vec2<T> &t_, const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise less-than-zero vector select (t_<0?v0_:v1_)
  return vec2<T>(t_.x<0?v0_.x:v1_.x,
                 t_.y<0?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sel_lez(const vec2<T> &t_, const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise less-or-equal-to-zero vector select (t_<=0?v0_:v1_)
  return vec2<T>(t_.x<=0?v0_.x:v1_.x,
                 t_.y<=0?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE void operator+=(vec2<T> &vr_, const vec2<T> &v_)
{
  // add vector to vector
  vr_.x+=v_.x;
  vr_.y+=v_.y;
}
//----

template<typename T>
EGL_INLINE void operator+=(vec2<T> &vr_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the vector
  vr_.x+=s_;
  vr_.y+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(vec2<T> &vr_, const vec2<T> &v_)
{
  // subtract vector from vector
  vr_.x-=v_.x;
  vr_.y-=v_.y;
}
//----

template<typename T>
EGL_INLINE void operator-=(vec2<T> &vr_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the vector
  vr_.x-=s_;
  vr_.y-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec2<T> &vr_, const vec2<T> &v_)
{
  // component-wise vector by vector multiply
  vr_.x*=v_.x;
  vr_.y*=v_.y;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec2<T> &vr_, typename math<T>::scalar_t s_)
{
  // multiply vector by scalar
  vr_.x*=s_;
  vr_.y*=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec2<T> &vr_, const mat22<T> &m_)
{
  // multiply vector by matrix
  vec2<T> v(vr_);
  vr_.x=v.x*m_.x.x+v.y*m_.y.x;
  vr_.y=v.x*m_.x.y+v.y*m_.y.y;
}
//----

template<typename T>
EGL_INLINE void operator/=(vec2<T> &vr_, const vec2<T> &v_)
{
  // component-wise vector by vector division
  EGL_ASSERT_PEDANTIC(v_.x && v_.y);
  vr_.x/=v_.x;
  vr_.y/=v_.y;
}
//----

template<typename T>
EGL_INLINE void operator/=(vec2<T> &vr_, typename math<T>::scalar_t s_)
{
  // divide vector by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  vr_.x*=rs;
  vr_.y*=rs;
}
//----

template<typename T>
EGL_INLINE vec2<T> operator+(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // add vector to vector
  return vec2<T>(v0_.x+v1_.x, v0_.y+v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator+(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the vector
  return vec2<T>(v_.x+s_, v_.y+s_);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator+(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // add each component of the vector to a scalar vector
  return vec2<T>(s_+v_.x, s_+v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator-(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // subtract vector from vector
  return vec2<T>(v0_.x-v1_.x, v0_.y-v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator-(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the vector
  return vec2<T>(v_.x-s_, v_.y-s_);
}
//----

template<typename T> EGL_INLINE vec2<T> operator-(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // subtract each component of the vector from a scalar vector
  return vec2<T>(s_-v_.x, s_-v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator-(const vec2<T> &v_)
{
  // negate vector
  return vec2<T>(-v_.x, -v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator*(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise vector by vector multiply
  return vec2<T>(v0_.x*v1_.x, v0_.y*v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator*(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // multiply vector by scalar
  return vec2<T>(v_.x*s_, v_.y*s_);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator*(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // multiply vector by scalar
  return vec2<T>(v_.x*s_, v_.y*s_);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator*(const vec2<T> &v_, const mat22<T> &m_)
{
  // multiply vector my matrix
  return vec2<T>(v_.x*m_.x.x+v_.y*m_.y.x,
                 v_.x*m_.x.y+v_.y*m_.y.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator*(const mat22<T> &m_, const vec2<T> &v_)
{
  // multiply matrix by transposed vector
  return vec2<T>(v_.x*m_.x.x+v_.y*m_.x.y,
                 v_.x*m_.y.x+v_.y*m_.y.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator/(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise vector by vector division
  EGL_ASSERT_PEDANTIC(v1_.x && v1_.y);
  return vec2<T>(v0_.x/v1_.x, v0_.y/v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> operator/(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // divide vector by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return vec2<T>(v_.x*rs, v_.y*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t min(const vec2<T> &v_)
{
  // minimum component value
  return min(v_.x, v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> min(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise min vector
  return vec2<T>(v0_.x<v1_.x?v0_.x:v1_.x,
                 v0_.y<v1_.y?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> min(const vec2<T> &v0_, const vec2<T> &v1_, const vec2<T> &v2_)
{
  // component-wise min vector
  return vec2<T>(min(v0_.x, v1_.x, v2_.x),
                 min(v0_.y, v1_.y, v2_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> min(const vec2<T> &v0_, const vec2<T> &v1_, const vec2<T> &v2_, const vec2<T> &v3_)
{
  // component-wise min vector
  return vec2<T>(min(v0_.x, v1_.x, v2_.x, v3_.x),
                 min(v0_.y, v1_.y, v2_.y, v3_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> min(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise min vector
  return vec2<T>(v_.x<s_?v_.x:s_,
                 v_.y<s_?v_.y:s_);
}
//----

template<typename T>
EGL_INLINE vec2<T> min(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // component-wise min vector
  return vec2<T>(s_<v_.x?s_:v_.x,
                 s_<v_.y?s_:v_.y);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t max(const vec2<T> &v_)
{
  // maximum component value
  return max(v_.x, v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> max(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise max vector
  return vec2<T>(v0_.x>v1_.x?v0_.x:v1_.x,
                 v0_.y>v1_.y?v0_.y:v1_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> max(const vec2<T> &v0_, const vec2<T> &v1_, const vec2<T> &v2_)
{
  // component-wise max vector
  return vec2<T>(max(v0_.x, v1_.x, v2_.x),
                 max(v0_.y, v1_.y, v2_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> max(const vec2<T> &v0_, const vec2<T> &v1_, const vec2<T> &v2_, const vec2<T> &v3_)
{
  // component-wise max vector
  return vec2<T>(max(v0_.x, v1_.x, v2_.x, v3_.x),
                 max(v0_.y, v1_.y, v2_.y, v3_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> max(const vec2<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise max vector
  return vec2<T>(v_.x>s_?v_.x:s_,
                 v_.y>s_?v_.y:s_);
}
//----

template<typename T>
EGL_INLINE vec2<T> max(typename math<T>::scalar_t s_, const vec2<T> &v_)
{
  // component-wise max vector
  return vec2<T>(s_>v_.x?s_:v_.x,
                 s_>v_.y?s_:v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> mul(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // component-wise multiply: v0_*v1
  return vec2<T>(v0_.x*v1_.x, v0_.y*v1_.y);
}

template<typename T>
EGL_INLINE vec2<T> madd(const vec2<T> &vm0_, const vec2<T> &vm1_, const vec2<T> &va_)
{
  // component-wise multiply-add: vm0_*vm1_+va_
  return vec2<T>(vm0_.x*vm1_.x+va_.x, vm0_.y*vm1_.y+va_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> madd(const vec2<T> &vm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: vm_*sm_+sa_
  return vec2<T>(vm_.x*sm_+sa_, vm_.y*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE vec2<T> madd(const vec2<T> &vm0_, const vec2<T> &vm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: vm0_*vm1_+sa_
  return vec2<T>(vm0_.x*vm1_.x+sa_, vm0_.y*vm1_.y+sa_);
}
//----

template<typename T>
EGL_INLINE vec2<T> madd(const vec2<T> &vm_, typename math<T>::scalar_t sm_, const vec2<T> &va_)
{
  // component-wise multiply-add: vm_*sm_+va_
  return vec2<T>(vm_.x*sm_+va_.x, vm_.y*sm_+va_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> rcp(const vec2<T> &v_)
{
  // component-wise reciprocal
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x && v_.y);
  return vec2<T>(scalar_t(1)/v_.x, scalar_t(1)/v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> rcp_z(const vec2<T> &v_)
{
  // component-wise reciprocal (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x?scalar_t(1)/v_.x:0, v_.y?scalar_t(1)/v_.y:0);
}
//----

template<typename T>
EGL_INLINE vec2<T> floor(const vec2<T> &v_)
{
  // component-wise floor
  return vec2<T>(MATH_NAMESPACE::floor(v_.x), MATH_NAMESPACE::floor(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> ceil(const vec2<T> &v_)
{
  // component-wise ceil
  return vec2<T>(MATH_NAMESPACE::ceil(v_.x), MATH_NAMESPACE::ceil(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> trunc(const vec2<T> &v_)
{
  // component-wise truncate
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(scalar_t(int(v_.x)), scalar_t(int(v_.y)));
}
//----

template<typename T>
EGL_INLINE vec2<T> frc(const vec2<T> &v_)
{
  // component-wise fractional
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x-scalar_t(int(v_.x)), v_.y-scalar_t(int(v_.y)));
}
//----

template<typename T>
EGL_INLINE vec2<T> mod(const vec2<T> &v_, typename math<T>::scalar_t div_)
{
  // component-wise modulo
  return vec2<T>(MATH_NAMESPACE::fmod(v_.x, div_), MATH_NAMESPACE::fmod(v_.y, div_));
}
//----

template<typename T>
EGL_INLINE vec2<T> cycle(const vec2<T> &v_, typename math<T>::scalar_t cycle_)
{
  // component-wise cycle
  return vec2<T>(MATH_NAMESPACE::fmod(v_.x, cycle_)+(v_.x<0?cycle_:0),
                 MATH_NAMESPACE::fmod(v_.y, cycle_)+(v_.y<0?cycle_:0));
}
//----

template<typename T>
EGL_INLINE vec2<T> cycle1(const vec2<T> &v_)
{
  // cycle1
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x-scalar_t(int(v_.x))+(v_.x<0?scalar_t(1):0),
                 v_.y-scalar_t(int(v_.y))+(v_.y<0?scalar_t(1):0));
}
//----

template<typename T>
EGL_INLINE vec2<T> sat(const vec2<T> &v_)
{
  // component-wise saturate
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<0?0:v_.x>scalar_t(1)?scalar_t(1):v_.x,
                 v_.y<0?0:v_.y>scalar_t(1)?scalar_t(1):v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> ssat(const vec2<T> &v_)
{
  // component-wise signed saturate
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<scalar_t(-1)?scalar_t(-1):v_.x>scalar_t(1)?scalar_t(1):v_.x,
                 v_.y<scalar_t(-1)?scalar_t(-1):v_.y>scalar_t(1)?scalar_t(1):v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> clamp(const vec2<T> &v_, const vec2<T> &min_, const vec2<T> &max_)
{
  // component-wise clamp (to vectors)
  return vec2<T>(v_.x<min_.x?min_.x:v_.x>max_.x?max_.x:v_.x,
                 v_.y<min_.y?min_.y:v_.y>max_.y?max_.y:v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> clamp(const vec2<T> &v_, typename math<T>::scalar_t min_, typename math<T>::scalar_t max_)
{
  // component-wise clamp (to scalars)
  return vec2<T>(v_.x<min_?min_:v_.x>max_?max_:v_.x,
                 v_.y<min_?min_:v_.y>max_?max_:v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> abs(const vec2<T> &v_)
{
  // component-wise absolute
  return vec2<T>(v_.x<0?-v_.x:v_.x, v_.y<0?-v_.y:v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sgn(const vec2<T> &v_)
{
  // component-wise signum (neg=-1, pos=+1, zero=0)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<0?scalar_t(-1):v_.x>0?scalar_t(1):0,
                 v_.y<0?scalar_t(-1):v_.y>0?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> sgn_zp(const vec2<T> &v_)
{
  // component-wise signum (neg=-1, pos&zero=+1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<0?scalar_t(-1):scalar_t(1), v_.y<0?scalar_t(-1):scalar_t(1));
}
//----

template<typename T>
EGL_INLINE vec2<T> sqr(const vec2<T> &v_)
{
  // component-wise square
  return vec2<T>(v_.x*v_.x, v_.y*v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> cubic(const vec2<T> &v_)
{
  // component-wise cubic
  return vec2<T>(v_.x*v_.x*v_.x, v_.y*v_.y*v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> sqrt(const vec2<T> &v_)
{
  // component-wise square root
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.y>=0);
  return vec2<T>(MATH_NAMESPACE::sqrt(v_.x), MATH_NAMESPACE::sqrt(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> sqrt_z(const vec2<T> &v_)
{
  // component-wise square root (set to 0 for components less than zero)
  return vec2<T>(v_.x>0?MATH_NAMESPACE::sqrt(v_.x):0,
                 v_.y>0?MATH_NAMESPACE::sqrt(v_.y):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> cbrt(const vec2<T> &v_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(1.0/3.0)),
                 v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE vec2<T> rsqrt(const vec2<T> &v_)
{
  // component-wise reciprocal square root
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>0 && v_.y>0);
  return vec2<T>(scalar_t(1)/MATH_NAMESPACE::sqrt(v_.x),
                 scalar_t(1)/MATH_NAMESPACE::sqrt(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> rsqrt_z(const vec2<T> &v_)
{
  // component-wise reciprocal square root (set to 0 for components less than or equal to zero)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.x):0,
                 v_.y>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.y):0);
}
//----

template<typename T>
EGL_INLINE vec2<T> rcbrt(const vec2<T> &v_)
{
  // component-wise reciprocal cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(-1.0/3.0)),
                 v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(-1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE vec2<T> rcbrt_z(const vec2<T> &v_)
{
  // component-wise reciprocal cubic root (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(v_.x?v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(-1.0/3.0)):0,
                 v_.y?v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(-1.0/3.0)):0);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm(const vec2<T> &v_)
{
  // norm of the vector (=vector length)
  return MATH_NAMESPACE::sqrt(v_.x*v_.x+v_.y*v_.y);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm(const vec2<T> &v_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_z(const vec2<T> &v_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y;
  return n2?scalar_t(1)/MATH_NAMESPACE::sqrt(n2):0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm2(const vec2<T> &v_)
{
  // squared norm of the vector
  return v_.x*v_.x+v_.y*v_.y;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2(const vec2<T> &v_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/n2;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2_z(const vec2<T> &v_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=v_.x*v_.x+v_.y*v_.y;
  return v?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm_l1(const vec2<T> &v_)
{
  // L1-norm of the vector (Manhattan norm)
  return (v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_l1(const vec2<T> &v_)
{
  // reciprocal L1-norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  return scalar_t(1)/((v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y));
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const vec2<T> &v_)
{
  // reciprocal L1-norm of the vector (return 0 if |v|=0)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=(v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y);
  return v?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE vec2<T> exp(const vec2<T> &v_)
{
  // natural exponent
  return vec2<T>(MATH_NAMESPACE::exp(v_.x), MATH_NAMESPACE::exp(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> exp2(const vec2<T> &v_)
{
  // 2^x
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(MATH_NAMESPACE::pow(scalar_t(2), v_.x), MATH_NAMESPACE::pow(scalar_t(2), v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> ln(const vec2<T> &v_)
{
  // natural logarithm
  return vec2<T>(MATH_NAMESPACE::log(v_.x), MATH_NAMESPACE::log(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> log2(const vec2<T> &v_)
{
  // base-2 logarithm
  return vec2<T>(T(MATH_NAMESPACE::log(v_.x))*T(1.44269504089), T(MATH_NAMESPACE::log(v_.x))*T(1.44269504089));
}
//----

template<typename T>
EGL_INLINE vec2<T> log10(const vec2<T> &v_)
{
  // base-10 logarithm
  return vec2<T>(MATH_NAMESPACE::log10(v_.x), MATH_NAMESPACE::log10(v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> pow(const vec2<T> &v_, typename math<T>::scalar_t power_)
{
  // raises value to the given power
  return vec2<T>(MATH_NAMESPACE::pow(v_.x, power_), MATH_NAMESPACE::pow(v_.y, power_));
}
//----

template<typename T>
EGL_INLINE vec2<T> unit(const vec2<T> &v_)
{
  // unit vector of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return vec2<T>(v_.x*rs, v_.y*rs);
}
//----

template<typename T>
EGL_INLINE vec2<T> unit_z(const vec2<T> &v_)
{
  // unit vector of the vector. if |v|=0, return v=[0, 0]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs=v_.x*v_.x+v_.y*v_.y;
  if(!rs)
    return vec2<T>(0, 0);
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return vec2<T>(v_.x*rs, v_.y*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t cross(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // cross-product of vectors
  return v0_.x*v1_.y-v0_.y*v1_.x;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot(const vec2<T> &v0_, const vec2<T> &v1_)
{
  // dot-product of vectors
  return v0_.x*v1_.x+v0_.y*v1_.y;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot1(const vec2<T> &v_)
{
  // dot-product with [1, 1]
  return v_.x+v_.y;
}
//----

template<typename T>
EGL_INLINE void neg(vec2<T> &v_)
{
  // negate the vector
  v_.x=-v_.x;
  v_.y=-v_.y;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_u(vec2<T> &vr_, Rng &rng_)
{
  // random unit vector (even distribution on unit circle)
  sincos(vr_.x, vr_.y, typename math<T>::scalar_t(rng_.rand_ureal1())*math<T>::two_pi);
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_real1(vec2<T> &vr_, Rng &rng_)
{
  // random vector where each component is in range [-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=scalar_t(rng_.rand_real1());
  vr_.y=scalar_t(rng_.rand_real1());
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_ureal1(vec2<T> &vr_, Rng &rng_)
{
  // random vector where each component is in range [0, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=scalar_t(rng_.rand_ureal1());
  vr_.y=scalar_t(rng_.rand_ureal1());
}
//----

template<typename T>
EGL_INLINE vec2<T> smoothstep(const vec2<T> &v_)
{
  // component-wise smoothstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.x<=scalar_t(1) && v_.y>=0 && v_.y<=scalar_t(1));
  return vec2<T>(v_.x*v_.x*(scalar_t(3)-scalar_t(2)*v_.x),
                 v_.y*v_.y*(scalar_t(3)-scalar_t(2)*v_.y));
}
//----

template<typename T>
EGL_INLINE vec2<T> smootherstep(const vec2<T> &v_)
{
  // component-wise smootherstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.x<=scalar_t(1) && v_.y>=0 && v_.y<=scalar_t(1));
  return vec2<T>(v_.x*v_.x*v_.x*(v_.x*(v_.x*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 v_.y*v_.y*v_.y*(v_.y*(v_.y*scalar_t(6)-scalar_t(15))+scalar_t(10)));
}
//----

template<typename T>
EGL_INLINE vec2<T> lerp(const vec2<T> &v0_, const vec2<T> &v1_, typename math<T>::scalar_t t_)
{
  // linear vector interpolation. f(t=0)=v0, f(t=1)=v1
  return vec2<T>(v0_.x+(v1_.x-v0_.x)*t_,
                 v0_.y+(v1_.y-v0_.y)*t_);
}
//----

template<typename T>
EGL_INLINE vec2<T> reflect(const vec2<T> &v_, const vec2<T> &n_)
{
  // reflect vector about normal: v'=2*n*(v.n)/(n.n)-v
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=scalar_t(2)*(v_.x*n_.x+v_.y*n_.y)*rcp(n_.x*n_.x+n_.y*n_.y);
  return vec2<T>(d*n_.x-v_.x,
                 d*n_.y-v_.y);
}
//----

template<typename T>
EGL_INLINE vec2<T> reflect_u(const vec2<T> &v_, const vec2<T> &n_)
{
  // reflect vector about unit normal: v'=2*n*(v.n)-v
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=scalar_t(2)*(v_.x*n_.x+v_.y*n_.y);
  return vec2<T>(d*n_.x-v_.x,
                 d*n_.y-v_.y);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t perspective_project(const vec2<T> &v_)
{
  // perspective project vector
  return v_.x/v_.y;
}
//----

template<typename T, typename U>
EGL_INLINE vec2<T> to_vec2(const vec2<U> &v_)
{
  // convert between vec2 types
  typedef typename math<T>::scalar_t scalar_t;
  return vec2<T>(scalar_t(v_.x), scalar_t(v_.y));
}
//----

namespace priv
{
  template<unsigned shuffle_, typename T>
  struct shuffle_vec2_helper
  {
    static vec2<T> shuffle(const vec2<T>&) {EGL_STATIC_ERROR(T, invalid_vector_component_shuffle);}
  };
  #define EGL_VECTOR_SHUFFLE(shuffle__, x__, y__)\
  template<typename T> struct shuffle_vec2_helper<shuffle__, T>\
  {\
    static EGL_INLINE vec2<T> shuffle(const vec2<T> &v_) {return vec2<T>(v_.x__, v_.y__);}\
  };
  EGL_VECTOR_SHUFFLE(00, x, x);
  EGL_VECTOR_SHUFFLE(01, x, y);
  EGL_VECTOR_SHUFFLE(10, y, x);
  EGL_VECTOR_SHUFFLE(11, y, y);
  #undef EGL_VECTOR_SHUFFLE
} // namespace priv
//----

template<unsigned shuffle_, typename T>
EGL_INLINE vec2<T> shuffle(const vec2<T> &v_)
{
  return priv::shuffle_vec2_helper<shuffle_, T>::shuffle(v_);
}
//----------------------------------------------------------------------------


//============================================================================
// vec3
//============================================================================
template<typename T>
vec3<T> vec3<T>::zero()
{
  return vec3<T>(scalar_t(0));
}
//----

template<typename T>
vec3<T> vec3<T>::one()
{
  return vec3<T>(scalar_t(1));
}
//----

template<typename T>
vec3<T> vec3<T>::neg_one()
{
  return vec3<T>(scalar_t(-1));
}
//----------------------------------------------------------------------------

template<typename T>
vec3<T>::vec3()
{
}
//----

template<typename T>
vec3<T>::vec3(T s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
  z=s_;
}
//----

template<typename T>
vec3<T>::vec3(T x_, T y_, T z_)
{
  // init vector with scalars
  x=x_;
  y=y_;
  z=z_;
}
//----

template<typename T>
vec3<T>::vec3(const vec2<T> &v_, T z_)
{
  // init vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=z_;
}
//----

template<typename T>
vec3<T>::vec3(const vec4<T> &v_)
{
  // init vector with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
}
//----

template<typename T>
vec3<T>::vec3(const T *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
}
//----------------------------------------------------------------------------

template<typename T>
const T &vec3<T>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &vec3<T>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void vec3<T>::set(T s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
  z=s_;
}
//----

template<typename T>
void vec3<T>::set(T x_, T y_, T z_)
{
  // set vector with scalars
  x=x_;
  y=y_;
  z=z_;
}
//----

template<typename T>
void vec3<T>::set(const vec2<T> &v_, T z_)
{
  // set vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=z_;
}
//----

template<typename T>
void vec3<T>::set(const vec3<T> &v_)
{
  // set vector with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
}
//----

template<typename T>
void vec3<T>::set(const vec4<T> &v_)
{
  // set vector with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
}
//----

template<typename T>
void vec3<T>::set(const T *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
}
//----------------------------------------------------------------------------


//============================================================================
// vec3<vec32s_t>
//============================================================================
vec3<vec32s_t> vec3<vec32s_t>::zero()
{
  return vec3<vec32s_t>(scalar_t(0));
}
//----

vec3<vec32s_t> vec3<vec32s_t>::one()
{
  return vec3<vec32s_t>(scalar_t(1));
}
//----

vec3<vec32s_t> vec3<vec32s_t>::neg_one()
{
  return vec3<vec32s_t>(scalar_t(-1));
}
//----------------------------------------------------------------------------

vec3<vec32s_t>::vec3()
{
}
//----

vec3<vec32s_t>::vec3(int8_t s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
  z=s_;
}
//----

vec3<vec32s_t>::vec3(int8_t x_, int8_t y_, int8_t z_)
{
  // init vector with scalars
  x=x_;
  y=y_;
  z=z_;
}
//----

vec3<vec32s_t>::vec3(const vec2<int8_t> &v_, int8_t z_)
{
  // init vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=z_;
}
//----

vec3<vec32s_t>::vec3(const int8_t *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
}
//----------------------------------------------------------------------------

const int8_t &vec3<vec32s_t>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const int8_t*>(this)[idx_];
}
//----

int8_t &vec3<vec32s_t>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<int8_t*>(this)[idx_];
}
//----

void vec3<vec32s_t>::set(int8_t s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
  z=s_;
}
//----

void vec3<vec32s_t>::set(int8_t x_, int8_t y_, int8_t z_)
{
  // set vector with scalars
  x=x_;
  y=y_;
  z=z_;
}
//----

void vec3<vec32s_t>::set(const vec2<int8_t> &v_, int8_t z_)
{
  // set vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=z_;
}
//----

void vec3<vec32s_t>::set(const vec3<vec32s_t> &v_)
{
  // set vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=v_.z;
}
//----

void vec3<vec32s_t>::set(const int8_t *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
}
//----------------------------------------------------------------------------


//============================================================================
// vec3<vec32u_t>
//============================================================================
vec3<vec32u_t> vec3<vec32u_t>::zero()
{
  return vec3<vec32u_t>(scalar_t(0));
}
//----

vec3<vec32u_t> vec3<vec32u_t>::one()
{
  return vec3<vec32u_t>(scalar_t(1));
}
//----

vec3<vec32u_t> vec3<vec32u_t>::neg_one()
{
  return vec3<vec32u_t>(scalar_t(0xff));
}
//----------------------------------------------------------------------------

vec3<vec32u_t>::vec3()
{
}
//----

vec3<vec32u_t>::vec3(uint8_t s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
  z=s_;
}
//----

vec3<vec32u_t>::vec3(uint8_t x_, uint8_t y_, uint8_t z_)
{
  // init vector with scalars
  x=x_;
  y=y_;
  z=z_;
}
//----

vec3<vec32u_t>::vec3(const vec2<uint8_t> &v_, uint8_t z_)
{
  // init vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=z_;
}
//----

vec3<vec32u_t>::vec3(const uint8_t *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
}
//----------------------------------------------------------------------------

const uint8_t &vec3<vec32u_t>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<const uint8_t*>(this)[idx_];
}
//----

uint8_t &vec3<vec32u_t>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(idx_<3);
  return reinterpret_cast<uint8_t*>(this)[idx_];
}
//----

void vec3<vec32u_t>::set(uint8_t s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
  z=s_;
}
//----

void vec3<vec32u_t>::set(uint8_t x_, uint8_t y_, uint8_t z_)
{
  // set vector with scalars
  x=x_;
  y=y_;
  z=z_;
}
//----

void vec3<vec32u_t>::set(const vec2<uint8_t> &v_, uint8_t z_)
{
  // set vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=z_;
}
//----

void vec3<vec32u_t>::set(const vec3<vec32u_t> &v_)
{
  // set vector with 2d vector and z
  x=v_.x;
  y=v_.y;
  z=v_.z;
}
//----

void vec3<vec32u_t>::set(const uint8_t *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
}
//----------------------------------------------------------------------------


//============================================================================
// vec3 free functions
//============================================================================
template<typename T>
EGL_INLINE bool is_zero(const vec3<T> &v_)
{
  // test for zero-vector
  return v_.x==0 && v_.y==0 && v_.z==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const vec3<T> &v_)
{
  // test for saturated vector
  typedef typename math<T>::scalar_t scalar_t;
  return    v_.x>=0 && v_.x<=scalar_t(1)
         && v_.y>=0 && v_.y<=scalar_t(1)
         && v_.z>=0 && v_.z<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const vec3<T> &v_)
{
  // test for signed saturated vector
  typedef typename math<T>::scalar_t scalar_t;
  return    v_.x>=scalar_t(-1) && v_.x<=scalar_t(1)
         && v_.y>=scalar_t(-1) && v_.y<=scalar_t(1)
         && v_.z>=scalar_t(-1) && v_.z<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool operator==(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // test for equality of vectors
  return v0_.x==v1_.x && v0_.y==v1_.y && v0_.z==v1_.z;
}
//----

template<typename T>
EGL_INLINE bool operator==(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // test for equality of vector and scalar
  return v_.x==s_ && v_.y==s_ && v_.z==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // test for equality of vector and scalar
  return s_==v_.x && s_==v_.y && s_==v_.z;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // test for inequality of vectors
  return v0_.x!=v1_.x || v0_.y!=v1_.y || v0_.z!=v1_.z;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // test for inequality of vector and scalar
  return v_.x!=s_ || v_.y!=s_ || v_.z!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // test for inequality of vector and scalar
  return s_!=v_.x || s_!=v_.y || s_!=v_.z;
}
//----

template<typename T>
EGL_INLINE vec3<T> operator<(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v0_.x<v1_.x?scalar_t(1):0, v0_.y<v1_.y?scalar_t(1):0, v0_.z<v1_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator<(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<s_?scalar_t(1):0, v_.y<s_?scalar_t(1):0, v_.z<s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator<(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(s_<v_.x?scalar_t(1):0, s_<v_.y?scalar_t(1):0, s_<v_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator>(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v0_.x>v1_.x?scalar_t(1):0, v0_.y>v1_.y?scalar_t(1):0, v0_.z>v1_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator>(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x>s_?scalar_t(1):0, v_.y>s_?scalar_t(1):0, v_.z>s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator>(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(s_>v_.x?scalar_t(1):0, s_>v_.y?scalar_t(1):0, s_>v_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator<=(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v0_.x<=v1_.x?scalar_t(1):0, v0_.y<=v1_.y?scalar_t(1):0, v0_.z<=v1_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator<=(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<=s_?scalar_t(1):0, v_.y<=s_?scalar_t(1):0, v_.z<=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator<=(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(s_<=v_.x?scalar_t(1):0, s_<=v_.y?scalar_t(1):0, s_<=v_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator>=(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v0_.x>=v1_.x?scalar_t(1):0, v0_.y>=v1_.y?scalar_t(1):0, v0_.z>=v1_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator>=(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x>=s_?scalar_t(1):0, v_.y>=s_?scalar_t(1):0, v_.z>=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator>=(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(s_>=v_.x?scalar_t(1):0, s_>=v_.y?scalar_t(1):0, s_>=v_.z?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> sel_eq(const vec3<T> &t0_, const vec3<T> &t1_, const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise equal vector select (t0_==t1_?v0_:v1_)
  return vec3<T>(t0_.x==t1_.x?v0_.x:v1_.x,
                 t0_.y==t1_.y?v0_.y:v1_.y,
                 t0_.z==t1_.z?v0_.z:v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> sel_lt(const vec3<T> &t0_, const vec3<T> &t1_, const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise less-than vector select (t0_<t1_?v0_:v1_)
  return vec3<T>(t0_.x<t1_.x?v0_.x:v1_.x,
                 t0_.y<t1_.y?v0_.y:v1_.y,
                 t0_.z<t1_.z?v0_.z:v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> sel_le(const vec3<T> &t0_, const vec3<T> &t1_, const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise less-or-equal vector select (t0_<=t1_?v0_:v1_)
  return vec3<T>(t0_.x<=t1_.x?v0_.x:v1_.x,
                 t0_.y<=t1_.y?v0_.y:v1_.y,
                 t0_.z<=t1_.z?v0_.z:v1_.z);
}
//----

template<typename T> EGL_INLINE vec3<T> sel_eqz(const vec3<T> &t_, const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise equal-to-zero vector select (t_==0?v0_:v1_)
  return vec3<T>(t_.x==0?v0_.x:v1_.x,
                 t_.y==0?v0_.y:v1_.y,
                 t_.z==0?v0_.z:v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> sel_ltz(const vec3<T> &t_, const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise less-than-zero vector select (t_<0?v0_:v1_)
  return vec3<T>(t_.x<0?v0_.x:v1_.x,
                 t_.y<0?v0_.y:v1_.y,
                 t_.z<0?v0_.z:v1_.z);
}
//----

template<typename T> EGL_INLINE vec3<T> sel_lez(const vec3<T> &t_, const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise less-or-equal-to-zero vector select (t_<=0?v0_:v1_)
  return vec3<T>(t_.x<=0?v0_.x:v1_.x,
                 t_.y<=0?v0_.y:v1_.y,
                 t_.z<=0?v0_.z:v1_.z);
}
//----

template<typename T>
EGL_INLINE void operator+=(vec3<T> &vr_, const vec3<T> &v_)
{
  // add vector to vector
  vr_.x+=v_.x;
  vr_.y+=v_.y;
  vr_.z+=v_.z;
}
//----

template<typename T>
EGL_INLINE void operator+=(vec3<T> &vr_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the vector
  vr_.x+=s_;
  vr_.y+=s_;
  vr_.z+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(vec3<T> &vr_, const vec3<T> &v_)
{
  // subtract vector from vector
  vr_.x-=v_.x;
  vr_.y-=v_.y;
  vr_.z-=v_.z;
}
//----

template<typename T>
EGL_INLINE void operator-=(vec3<T> &vr_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the vector
  vr_.x-=s_;
  vr_.y-=s_;
  vr_.z-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, const vec3<T> &v_)
{
  // component-wise vector by vector multiply
  vr_.x*=v_.x;
  vr_.y*=v_.y;
  vr_.z*=v_.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, typename math<T>::scalar_t s_)
{
  // multiply vector by scalar
  vr_.x*=s_;
  vr_.y*=s_;
  vr_.z*=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, const mat33<T> &m_)
{
  // multiply vector by matrix
  vec3<T> v(vr_);
  vr_.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x;
  vr_.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y;
  vr_.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, const mat44<T> &m_)
{
  // multiply vector by 4x4 matrix (assume vector w=1)
  vec3<T> v(vr_);
  vr_.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x+m_.w.x;
  vr_.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y+m_.w.y;
  vr_.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z+m_.w.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, const quat<T> &q_)
{
  // transform the vector by quaternion: v'=2(qv x (qv x v + wv)) + v
  typedef typename math<T>::scalar_t scalar_t;
  vec3<T> s(q_.y*vr_.z-q_.z*vr_.y+q_.w*vr_.x,
            q_.z*vr_.x-q_.x*vr_.z+q_.w*vr_.y,
            q_.x*vr_.y-q_.y*vr_.x+q_.w*vr_.z);
  vr_.x+=scalar_t(2)*(q_.y*s.z-q_.z*s.y);
  vr_.y+=scalar_t(2)*(q_.z*s.x-q_.x*s.z);
  vr_.z+=scalar_t(2)*(q_.x*s.y-q_.y*s.x);
}
//----

template<typename T>
EGL_INLINE void operator/=(vec3<T> &vr_, const vec3<T> &v_)
{
  // component-wise vector by vector division
  EGL_ASSERT_PEDANTIC(v_.x && v_.y && v_.z);
  vr_.x/=v_.x;
  vr_.y/=v_.y;
  vr_.z/=v_.z;
}
//----

template<typename T>
EGL_INLINE void operator/=(vec3<T> &vr_, typename math<T>::scalar_t s_)
{
  // divide vector by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  vr_.x*=rs;
  vr_.y*=rs;
  vr_.z*=rs;
}
//----

template<typename T>
EGL_INLINE vec3<T> operator+(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // add vector to vector
  return vec3<T>(v0_.x+v1_.x, v0_.y+v1_.y, v0_.z+v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator+(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the vector
  return vec3<T>(v_.x+s_, v_.y+s_, v_.z+s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator+(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // add each component of the vector to a scalar vector
  return vec3<T>(s_+v_.x, s_+v_.y, s_+v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator-(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // subtract vector from vector
  return vec3<T>(v0_.x-v1_.x, v0_.y-v1_.y, v0_.z-v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator-(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the vector
  return vec3<T>(v_.x-s_, v_.y-s_, v_.z-s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator-(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // subtract each component of the vector from a scalar vector
  return vec3<T>(s_-v_.x, s_-v_.y, s_-v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator-(const vec3<T> &v_)
{
  // negate vector
  return vec3<T>(-v_.x, -v_.y, -v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise vector by vector multiply
  return vec3<T>(v0_.x*v1_.x, v0_.y*v1_.y, v0_.z*v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // multiply vector by scalar
  return vec3<T>(v_.x*s_, v_.y*s_, v_.z*s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // multiply vector by scalar
  return vec3<T>(v_.x*s_, v_.y*s_, v_.z*s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v_, const mat33<T> &m_)
{
  // multiply vector my matrix
  return vec3<T>(v_.x*m_.x.x+v_.y*m_.y.x+v_.z*m_.z.x,
                 v_.x*m_.x.y+v_.y*m_.y.y+v_.z*m_.z.y,
                 v_.x*m_.x.z+v_.y*m_.y.z+v_.z*m_.z.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const mat33<T> &m_, const vec3<T> &v_)
{
  // multiply matrix by transposed vector
  return vec3<T>(v_.x*m_.x.x+v_.y*m_.x.y+v_.z*m_.x.z,
                 v_.x*m_.y.x+v_.y*m_.y.y+v_.z*m_.y.z,
                 v_.x*m_.z.x+v_.y*m_.z.y+v_.z*m_.z.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v_, const mat44<T> &m_)
{
  // multiply vector by 4x4 matrix (assume vector w=1)
  return vec3<T>(v_.x*m_.x.x+v_.y*m_.y.x+v_.z*m_.z.x+m_.w.x,
                 v_.x*m_.x.y+v_.y*m_.y.y+v_.z*m_.z.y+m_.w.y,
                 v_.x*m_.x.z+v_.y*m_.y.z+v_.z*m_.z.z+m_.w.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const mat44<T> &m_, const vec3<T> &v_)
{
  // multiply 4x4 matrix by transposed vector (assume vector w=1)
  return vec3<T>(v_.x*m_.x.x+v_.y*m_.x.y+v_.z*m_.x.z+m_.x.w,
                 v_.x*m_.y.x+v_.y*m_.y.y+v_.z*m_.y.z+m_.y.w,
                 v_.x*m_.z.x+v_.y*m_.z.y+v_.z*m_.z.z+m_.z.w);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v_, const quat<T> &q_)
{
  // transform the vector by quaternion: v'=2(qv x (qv x v + wv)) + v
  typedef typename math<T>::scalar_t scalar_t;
  vec3<T> s(q_.y*v_.z-q_.z*v_.y+q_.w*v_.x,
            q_.z*v_.x-q_.x*v_.z+q_.w*v_.y,
            q_.x*v_.y-q_.y*v_.x+q_.w*v_.z);
  return vec3<T>(scalar_t(2)*(q_.y*s.z-q_.z*s.y)+v_.x,
                 scalar_t(2)*(q_.z*s.x-q_.x*s.z)+v_.y,
                 scalar_t(2)*(q_.x*s.y-q_.y*s.x)+v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator/(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise vector by vector division
  EGL_ASSERT_PEDANTIC(v1_.x && v1_.y && v1_.z);
  return vec3<T>(v0_.x/v1_.x, v0_.y/v1_.y, v0_.z/v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator/(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // divide vector by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return vec3<T>(v_.x*rs, v_.y*rs, v_.z*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t min(const vec3<T> &v_)
{
  // minimum component value
  return min(v_.x, v_.y, v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> min(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise min vector
  return vec3<T>(v0_.x<v1_.x?v0_.x:v1_.x,
                 v0_.y<v1_.y?v0_.y:v1_.y,
                 v0_.z<v1_.z?v0_.z:v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> min(const vec3<T> &v0_, const vec3<T> &v1_, const vec3<T> &v2_)
{
  // component-wise min vector
  return vec3<T>(min(v0_.x, v1_.x, v2_.x),
                 min(v0_.y, v1_.y, v2_.y),
                 min(v0_.z, v1_.z, v2_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> min(const vec3<T> &v0_, const vec3<T> &v1_, const vec3<T> &v2_, const vec3<T> &v3_)
{
  // component-wise min vector
  return vec3<T>(min(v0_.x, v1_.x, v2_.x, v3_.x),
                 min(v0_.y, v1_.y, v2_.y, v3_.y),
                 min(v0_.z, v1_.z, v2_.z, v3_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> min(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise min vector
  return vec3<T>(v_.x<s_?v_.x:s_,
                 v_.y<s_?v_.y:s_,
                 v_.z<s_?v_.z:s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> min(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // component-wise min vector
  return vec3<T>(s_<v_.x?s_:v_.x,
                 s_<v_.y?s_:v_.y,
                 s_<v_.z?s_:v_.z);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t max(const vec3<T> &v_)
{
  // maximum component value
  return max(v_.x, v_.y, v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> max(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise max vector
  return vec3<T>(v0_.x>v1_.x?v0_.x:v1_.x,
                 v0_.y>v1_.y?v0_.y:v1_.y,
                 v0_.z>v1_.z?v0_.z:v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> max(const vec3<T> &v0_, const vec3<T> &v1_, const vec3<T> &v2_)
{
  // component-wise max vector
  return vec3<T>(max(v0_.x, v1_.x, v2_.x),
                 max(v0_.y, v1_.y, v2_.y),
                 max(v0_.z, v1_.z, v2_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> max(const vec3<T> &v0_, const vec3<T> &v1_, const vec3<T> &v2_, const vec3<T> &v3_)
{
  // component-wise max vector
  return vec3<T>(max(v0_.x, v1_.x, v2_.x, v3_.x),
                 max(v0_.y, v1_.y, v2_.y, v3_.y),
                 max(v0_.z, v1_.z, v2_.z, v3_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> max(const vec3<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise max vector
  return vec3<T>(v_.x>s_?v_.x:s_,
                 v_.y>s_?v_.y:s_,
                 v_.z>s_?v_.z:s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> max(typename math<T>::scalar_t s_, const vec3<T> &v_)
{
  // component-wise max vector
  return vec3<T>(s_>v_.x?s_:v_.x,
                 s_>v_.y?s_:v_.y,
                 s_>v_.z?s_:v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> mul(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // component-wise multiply: v0_*v1_
  return vec3<T>(v0_.x*v1_.x, v0_.y*v1_.y, v0_.z*v1_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> madd(const vec3<T> &vm0_, const vec3<T> &vm1_, const vec3<T> &va_)
{
  // component-wise multiply-add: vm0_*vm1_+va_
  return vec3<T>(vm0_.x*vm1_.x+va_.x, vm0_.y*vm1_.y+va_.y, vm0_.z*vm1_.z+va_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> madd(const vec3<T> &vm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: vm_*sm_+sa_
  return vec3<T>(vm_.x*sm_+sa_, vm_.y*sm_+sa_, vm_.z*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE vec3<T> madd(const vec3<T> &vm0_, const vec3<T> &vm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: vm_*vm1_+sa_
  return vec3<T>(vm0_.x*vm1_.x+sa_, vm0_.y*vm1_.y+sa_, vm0_.z*vm1_.z+sa_);
}
//----

template<typename T>
EGL_INLINE vec3<T> madd(const vec3<T> &vm_, typename math<T>::scalar_t sm_, const vec3<T> &va_)
{
  // component-wise multiply-add: vm_*sm_+va_
  return vec3<T>(vm_.x*sm_+va_.x, vm_.y*sm_+va_.y, vm_.z*sm_+va_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> rcp(const vec3<T> &v_)
{
  // component-wise reciprocal
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x && v_.y && v_.z);
  return vec3<T>(scalar_t(1)/v_.x, scalar_t(1)/v_.y, scalar_t(1)/v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> rcp_z(const vec3<T> &v_)
{
  // component-wise reciprocal (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x?scalar_t(1)/v_.x:0, v_.y?scalar_t(1)/v_.y:0, v_.z?scalar_t(1)/v_.z:0);
}
//----

template<typename T>
EGL_INLINE vec3<T> floor(const vec3<T> &v_)
{
  // component-wise floor
  return vec3<T>(MATH_NAMESPACE::floor(v_.x), MATH_NAMESPACE::floor(v_.y), MATH_NAMESPACE::floor(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> ceil(const vec3<T> &v_)
{
  // component-wise ceil
  return vec3<T>(MATH_NAMESPACE::ceil(v_.x), MATH_NAMESPACE::ceil(v_.y), MATH_NAMESPACE::ceil(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> trunc(const vec3<T> &v_)
{
  // component-wise truncate
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(scalar_t(int(v_.x)), scalar_t(int(v_.y)), scalar_t(int(v_.z)));
}
//----

template<typename T>
EGL_INLINE vec3<T> frc(const vec3<T> &v_)
{
  // component-wise fractional
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x-scalar_t(int(v_.x)), v_.y-scalar_t(int(v_.y)), v_.z-scalar_t(int(v_.z)));
}
//----

template<typename T>
EGL_INLINE vec3<T> mod(const vec3<T> &v_, typename math<T>::scalar_t div_)
{
  // component-wise modulo
  return vec3<T>(MATH_NAMESPACE::fmod(v_.x, div_), MATH_NAMESPACE::fmod(v_.y, div_), MATH_NAMESPACE::fmod(v_.z, div_));
}
//----

template<typename T>
EGL_INLINE vec3<T> cycle(const vec3<T> &v_, typename math<T>::scalar_t cycle_)
{
  // component-wise cycle
  return vec3<T>(MATH_NAMESPACE::fmod(v_.x, cycle_)+(v_.x<0?cycle_:0),
                 MATH_NAMESPACE::fmod(v_.y, cycle_)+(v_.y<0?cycle_:0),
                 MATH_NAMESPACE::fmod(v_.z, cycle_)+(v_.z<0?cycle_:0));
}
//----

template<typename T>
EGL_INLINE vec3<T> cycle1(const vec3<T> &v_)
{
  // cycle1
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x-scalar_t(int(v_.x))+(v_.x<0?scalar_t(1):0),
                 v_.y-scalar_t(int(v_.y))+(v_.y<0?scalar_t(1):0),
                 v_.z-scalar_t(int(v_.z))+(v_.z<0?scalar_t(1):0));
}
//----

template<typename T>
EGL_INLINE vec3<T> sat(const vec3<T> &v_)
{
  // component-wise saturate
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<0?0:v_.x>scalar_t(1)?scalar_t(1):v_.x,
                 v_.y<0?0:v_.y>scalar_t(1)?scalar_t(1):v_.y,
                 v_.z<0?0:v_.z>scalar_t(1)?scalar_t(1):v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> ssat(const vec3<T> &v_)
{
  // component-wise signed saturate
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<scalar_t(-1)?scalar_t(-1):v_.x>scalar_t(1)?scalar_t(1):v_.x,
                 v_.y<scalar_t(-1)?scalar_t(-1):v_.y>scalar_t(1)?scalar_t(1):v_.y,
                 v_.z<scalar_t(-1)?scalar_t(-1):v_.z>scalar_t(1)?scalar_t(1):v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> clamp(const vec3<T> &v_, const vec3<T> &min_, const vec3<T> &max_)
{
  // component-wise clamp (to vectors)
  return vec3<T>(v_.x<min_.x?min_.x:v_.x>max_.x?max_.x:v_.x,
                 v_.y<min_.y?min_.y:v_.y>max_.y?max_.y:v_.y,
                 v_.z<min_.z?min_.z:v_.z>max_.z?max_.z:v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> clamp(const vec3<T> &v_, typename math<T>::scalar_t min_, typename math<T>::scalar_t max_)
{
  // component-wise clamp (to scalars)
  return vec3<T>(v_.x<min_?min_:v_.x>max_?max_:v_.x,
                 v_.y<min_?min_:v_.y>max_?max_:v_.y,
                 v_.z<min_?min_:v_.z>max_?max_:v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> abs(const vec3<T> &v_)
{
  // component-wise absolute
  return vec3<T>(v_.x<0?-v_.x:v_.x, v_.y<0?-v_.y:v_.y, v_.z<0?-v_.z:v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> sgn(const vec3<T> &v_)
{
  // component-wise signum (neg=-1, pos=+1, zero=0)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<0?scalar_t(-1):v_.x>0?scalar_t(1):0, v_.y<0?scalar_t(-1):v_.y>0?scalar_t(1):0, v_.z<0?scalar_t(-1):v_.z>0?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> sgn_zp(const vec3<T> &v_)
{
  // component-wise signum (neg=-1, pos&zero=+1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<0?scalar_t(-1):scalar_t(1), v_.y<0?scalar_t(-1):scalar_t(1), v_.z<0?scalar_t(-1):scalar_t(1));
}
//----

template<typename T>
EGL_INLINE vec3<T> sqr(const vec3<T> &v_)
{
  // component-wise square
  return vec3<T>(v_.x*v_.x, v_.y*v_.y, v_.z*v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> cubic(const vec3<T> &v_)
{
  // component-wise cubic
  return vec3<T>(v_.x*v_.x*v_.x, v_.y*v_.y*v_.y, v_.z*v_.z*v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> sqrt(const vec3<T> &v_)
{
  // component-wise square root
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.y>=0 && v_.z>=0);
  return vec3<T>(MATH_NAMESPACE::sqrt(v_.x), MATH_NAMESPACE::sqrt(v_.y), MATH_NAMESPACE::sqrt(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> sqrt_z(const vec3<T> &v_)
{
  // component-wise square root (set to 0 for components less than zero)
  return vec3<T>(sqrt_z(v_.x), sqrt_z(v_.y), sqrt_z(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> cbrt(const vec3<T> &v_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(1.0/3.0)),
                 v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(1.0/3.0)),
                 v_.z<0?-MATH_NAMESPACE::pow(-v_.z, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.z, scalar_t(1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE vec3<T> rsqrt(const vec3<T> &v_)
{
  // component-wise reciprocal square root
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>0 && v_.y>0 && v_.z>0);
  return vec3<T>(scalar_t(1)/MATH_NAMESPACE::sqrt(v_.x), scalar_t(1)/MATH_NAMESPACE::sqrt(v_.y), scalar_t(1)/MATH_NAMESPACE::sqrt(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> rsqrt_z(const vec3<T> &v_)
{
  // component-wise reciprocal square root (set to 0 for components less than or equal to zero)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.x):0,
                 v_.y>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.y):0,
                 v_.z>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.z):0);
}
//----

template<typename T>
EGL_INLINE vec3<T> rcbrt(const vec3<T> &v_)
{
  // component-wise reciprocal cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(-1.0/3.0)),
                 v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(-1.0/3.0)),
                 v_.z<0?-MATH_NAMESPACE::pow(-v_.z, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.z, scalar_t(-1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE vec3<T> rcbrt_z(const vec3<T> &v_)
{
  // component-wise reciprocal cubic root (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(v_.x?v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(-1.0/3.0)):0,
                 v_.y?v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(-1.0/3.0)):0,
                 v_.z?v_.z<0?-MATH_NAMESPACE::pow(-v_.z, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.z, scalar_t(-1.0/3.0)):0);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm(const vec3<T> &v_)
{
  // norm of the vector (=vector length)
  return MATH_NAMESPACE::sqrt(v_.x*v_.x+v_.y*v_.y+v_.z*v_.z);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm(const vec3<T> &v_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_z(const vec3<T> &v_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
  return n2?scalar_t(1)/MATH_NAMESPACE::sqrt(n2):0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm2(const vec3<T> &v_)
{
  // squared norm of the vector
  return v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2(const vec3<T> &v_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/n2;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2_z(const vec3<T> &v_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
  return v?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm_l1(const vec3<T> &v_)
{
  // L1-norm of the vector (Manhattan norm)
  return (v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y)+(v_.z<0?-v_.z:v_.z);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_l1(const vec3<T> &v_)
{
  // reciprocal L1-norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  return scalar_t(1)/((v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y)+(v_.z<0?-v_.z:v_.z));
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const vec3<T> &v_)
{
  // reciprocal L1-norm of the vector (return 0 if |v|=0)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=(v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y)+(v_.z<0?-v_.z:v_.z);
  return v?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE vec3<T> exp(const vec3<T> &v_)
{
  // natural exponent
  return vec3<T>(MATH_NAMESPACE::exp(v_.x), MATH_NAMESPACE::exp(v_.y), MATH_NAMESPACE::exp(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> exp2(const vec3<T> &v_)
{
  // 2^x
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(MATH_NAMESPACE::pow(scalar_t(2), v_.x), MATH_NAMESPACE::pow(scalar_t(2), v_.y), MATH_NAMESPACE::pow(scalar_t(2), v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> ln(const vec3<T> &v_)
{
  // natural logarithm
  return vec3<T>(MATH_NAMESPACE::log(v_.x), MATH_NAMESPACE::log(v_.y), MATH_NAMESPACE::log(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> log2(const vec3<T> &v_)
{
  // base-2 logarithm
  return vec3<T>(T(MATH_NAMESPACE::log(v_.x))*T(1.44269504089), T(MATH_NAMESPACE::log(v_.y))*T(1.44269504089), T(MATH_NAMESPACE::log(v_.z))*T(1.44269504089));
}
//----

template<typename T>
EGL_INLINE vec3<T> log10(const vec3<T> &v_)
{
  // base-10 logarithm
  return vec3<T>(MATH_NAMESPACE::log10(v_.x), MATH_NAMESPACE::log10(v_.y), MATH_NAMESPACE::log10(v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> pow(const vec3<T> &v_, typename math<T>::scalar_t power_)
{
  // raises value to the given power
  return vec3<T>(MATH_NAMESPACE::pow(v_.x, power_), MATH_NAMESPACE::pow(v_.y, power_), MATH_NAMESPACE::pow(v_.z, power_));
}
//----

template<typename T>
EGL_INLINE vec3<T> unit(const vec3<T> &v_)
{
  // unit vector of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return vec3<T>(v_.x*rs, v_.y*rs, v_.z*rs);
}
//----

template<typename T>
EGL_INLINE vec3<T> unit_z(const vec3<T> &v_)
{
  // unit vector of the vector. if |v|=0, return v=[0, 0, 0]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z;
  if(!rs)
    return vec3<T>(0, 0, 0);
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return vec3<T>(v_.x*rs, v_.y*rs, v_.z*rs);
}
//----

template<typename T>
EGL_INLINE vec3<T> cross(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // cross-product of vectors
  return vec3<T>(v0_.y*v1_.z-v0_.z*v1_.y,
                 v0_.z*v1_.x-v0_.x*v1_.z,
                 v0_.x*v1_.y-v0_.y*v1_.x);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot(const vec3<T> &v0_, const vec3<T> &v1_)
{
  // dot-product of vectors
  return v0_.x*v1_.x+v0_.y*v1_.y+v0_.z*v1_.z;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot1(const vec3<T> &v_)
{
  // dot-product with [1, 1, 1]
  return v_.x+v_.y+v_.z;
}
//----

template<typename T>
EGL_INLINE void neg(vec3<T> &v_)
{
  // negate the vector
  v_.x=-v_.x;
  v_.y=-v_.y;
  v_.z=-v_.z;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_u(vec3<T> &vr_, Rng &rng_)
{
  // random unit vector (even distribution on unit sphere)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t s, c;
  sincos(s, c, scalar_t(rng_.rand_ureal1())*math<T>::two_pi);
  scalar_t z=scalar_t(rng_.rand_real1());
  vr_.z=z;
  scalar_t rad=MATH_NAMESPACE::sqrt(scalar_t(1)-z*z);
  vr_.x=s*rad;
  vr_.y=c*rad;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_real1(vec3<T> &vr_, Rng &rng_)
{
  // random vector where each component is in range [-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=scalar_t(rng_.rand_real1());
  vr_.y=scalar_t(rng_.rand_real1());
  vr_.z=scalar_t(rng_.rand_real1());
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_ureal1(vec3<T> &vr_, Rng &rng_)
{
  // random vector where each component is in range [0, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=scalar_t(rng_.rand_ureal1());
  vr_.y=scalar_t(rng_.rand_ureal1());
  vr_.z=scalar_t(rng_.rand_ureal1());
}
//----

template<typename T>
EGL_INLINE vec3<T> smoothstep(const vec3<T> &v_)
{
  // component-wise smoothstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.x<=scalar_t(1) && v_.y>=0 && v_.y<=scalar_t(1) && v_.z>=0 && v_.z<=scalar_t(1));
  return vec3<T>(v_.x*v_.x*(scalar_t(3)-scalar_t(2)*v_.x),
                 v_.y*v_.y*(scalar_t(3)-scalar_t(2)*v_.y),
                 v_.z*v_.z*(scalar_t(3)-scalar_t(2)*v_.z));
}
//----

template<typename T>
EGL_INLINE vec3<T> smootherstep(const vec3<T> &v_)
{
  // component-wise smootherstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.x<=scalar_t(1) && v_.y>=0 && v_.y<=scalar_t(1) && v_.z>=0 && v_.z<=scalar_t(1));
  return vec3<T>(v_.x*v_.x*v_.x*(v_.x*(v_.x*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 v_.y*v_.y*v_.y*(v_.y*(v_.y*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 v_.z*v_.z*v_.z*(v_.z*(v_.z*scalar_t(6)-scalar_t(15))+scalar_t(10)));
}
//----

template<typename T>
EGL_INLINE vec3<T> lerp(const vec3<T> &v0_, const vec3<T> &v1_, typename math<T>::scalar_t t_)
{
  // linear vector interpolation. f(t=0)=v0, f(t=1)=v1
  return vec3<T>(v0_.x+(v1_.x-v0_.x)*t_,
                 v0_.y+(v1_.y-v0_.y)*t_,
                 v0_.z+(v1_.z-v0_.z)*t_);
}
//----

template<typename T> EGL_INLINE vec3<T> reflect(const vec3<T> &v_, const vec3<T> &n_)
{
  // reflect vector about normal: v'=2*n*(v.n)/(n.n)-v
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=scalar_t(2)*(v_.x*n_.x+v_.y*n_.y+v_.z*n_.z)*rcp(n_.x*n_.x+n_.y*n_.y+n_.z*n_.z);
  return vec3<T>(d*n_.x-v_.x,
                 d*n_.y-v_.y,
                 d*n_.z-v_.z);
}
//----

template<typename T>
EGL_INLINE vec3<T> reflect_u(const vec3<T> &v_, const vec3<T> &n_)
{
  // reflect vector about unit normal: v'=2*n*(v.n)-v
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=scalar_t(2)*(v_.x*n_.x+v_.y*n_.y+v_.z*n_.z);
  return vec3<T>(d*n_.x-v_.x,
                 d*n_.y-v_.y,
                 d*n_.z-v_.z);
}
//----

template<typename T>
EGL_INLINE vec2<T> perspective_project(const vec3<T> &v_)
{
  // perspective project vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t s=scalar_t(1)/v_.z;
  return vec2<T>(v_.x*s, v_.y*s);
}
//----

template<typename T, typename U>
EGL_INLINE vec3<T> to_vec3(const vec3<U> &v_)
{
  // convert between vec3 types
  typedef typename math<T>::scalar_t scalar_t;
  return vec3<T>(scalar_t(v_.x), scalar_t(v_.y), scalar_t(v_.z));
}
//----

namespace priv
{
  template<unsigned shuffle_, typename T>
  struct shuffle_vec3_helper
  {
    static vec3<T> shuffle(const vec3<T>&) {EGL_STATIC_ERROR(T, invalid_vector_component_shuffle);}
  };
  #define EGL_VECTOR_SHUFFLE(shuffle__, x__, y__, z__)\
  template<typename T> struct shuffle_vec3_helper<shuffle__, T>\
  {\
    static EGL_INLINE vec3<T> shuffle(const vec3<T> &v_) {return vec3<T>(v_.x__, v_.y__, v_.z__);}\
  };
  EGL_VECTOR_SHUFFLE(000, x, x, x); EGL_VECTOR_SHUFFLE(001, x, x, y); EGL_VECTOR_SHUFFLE(002, x, x, z); EGL_VECTOR_SHUFFLE(010, x, y, x); EGL_VECTOR_SHUFFLE(011, x, y, y); EGL_VECTOR_SHUFFLE(012, x, y, z); EGL_VECTOR_SHUFFLE(020, x, z, x); EGL_VECTOR_SHUFFLE(021, x, z, y); EGL_VECTOR_SHUFFLE(022, x, z, z);
  EGL_VECTOR_SHUFFLE(100, y, x, x); EGL_VECTOR_SHUFFLE(101, y, x, y); EGL_VECTOR_SHUFFLE(102, y, x, z); EGL_VECTOR_SHUFFLE(110, y, y, x); EGL_VECTOR_SHUFFLE(111, y, y, y); EGL_VECTOR_SHUFFLE(112, y, y, z); EGL_VECTOR_SHUFFLE(120, y, z, x); EGL_VECTOR_SHUFFLE(121, y, z, y); EGL_VECTOR_SHUFFLE(122, y, z, z);
  EGL_VECTOR_SHUFFLE(200, z, x, x); EGL_VECTOR_SHUFFLE(201, z, x, y); EGL_VECTOR_SHUFFLE(202, z, x, z); EGL_VECTOR_SHUFFLE(210, z, y, x); EGL_VECTOR_SHUFFLE(211, z, y, y); EGL_VECTOR_SHUFFLE(212, z, y, z); EGL_VECTOR_SHUFFLE(220, z, z, x); EGL_VECTOR_SHUFFLE(221, z, z, y); EGL_VECTOR_SHUFFLE(222, z, z, z);
  #undef EGL_VECTOR_SHUFFLE
} // namespace priv
//----

template<unsigned shuffle_, typename T>
EGL_INLINE vec3<T> shuffle(const vec3<T> &v_)
{
  return priv::shuffle_vec3_helper<shuffle_, T>::shuffle(v_);
}
//----

template<typename T>
EGL_INLINE void pack_s1(vec3_32s &vr_, const vec3<T> &v_)
{
  // pack 3d vector (components in range [-1, 1]) to 32-bit 3d vector
  EGL_ASSERT_PEDANTIC_MSG(is_ssat(v_), "All vector components [%f, %f, %f] must be in range [-1, 1]\r\n", float(v_.x), float(v_.y), float(v_.z));
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=int8_t(v_.x*scalar_t(127.999));
  vr_.y=int8_t(v_.y*scalar_t(127.999));
  vr_.z=int8_t(v_.z*scalar_t(127.999));
}
//----

template<typename T>
EGL_INLINE void pack_u1(vec3_32u &vr_, const vec3<T> &v_)
{
  // pack 3d vector (components in range [0, 1]) to 32-bit 3d vector
  EGL_ASSERT_PEDANTIC_MSG(is_sat(v_), "All vector components [%f, %f, %f] must be in range [0, 1]\r\n", float(v_.x), float(v_.y), float(v_.z));
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=uint8_t(v_.x*scalar_t(255.999));
  vr_.y=uint8_t(v_.y*scalar_t(255.999));
  vr_.z=uint8_t(v_.z*scalar_t(255.999));
}
//----

template<typename T>
EGL_INLINE void unpack_s1(vec3<T> &vr_, const vec3_32s &v_)
{
  // unpack 3d vector (components in range [-1, 1]) from 32-bit 3d vector
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=v_.x*scalar_t(1.0/127.0);
  vr_.y=v_.y*scalar_t(1.0/127.0);
  vr_.z=v_.z*scalar_t(1.0/127.0);
}
//----

template<typename T>
EGL_INLINE void unpack_u1(vec3<T> &vr_, const vec3_32u &v_)
{
  // unpack 3d vector (components in range [0, 1]) from 32-bit 3d vector
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=v_.x*scalar_t(1.0/255.0);
  vr_.y=v_.y*scalar_t(1.0/255.0);
  vr_.z=v_.z*scalar_t(1.0/255.0);
}
//----------------------------------------------------------------------------


//============================================================================
// vec4
//============================================================================
template<typename T>
vec4<T> vec4<T>::zero()
{
  return vec4<T>(scalar_t(0));
}
//----

template<typename T>
vec4<T> vec4<T>::one()
{
  return vec4<T>(scalar_t(1));
}
//----

template<typename T>
vec4<T> vec4<T>::neg_one()
{
  return vec4<T>(scalar_t(-1));
}
//----------------------------------------------------------------------------

template<typename T>
vec4<T>::vec4()
{
}
//----

template<typename T>
vec4<T>::vec4(T s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

template<typename T>
vec4<T>::vec4(T x_, T y_, T z_, T w_)
{
  // init vector with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

template<typename T>
vec4<T>::vec4(const vec3<T> &v_, T w_)
{
  // init vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

template<typename T>
vec4<T>::vec4(const T *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------

template<typename T>
const T &vec4<T>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &vec4<T>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void vec4<T>::set(T s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

template<typename T>
void vec4<T>::set(T x_, T y_, T z_, T w_)
{
  // set vector with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

template<typename T>
void vec4<T>::set(const vec3<T> &v_, T w_)
{
  // set vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

template<typename T>
void vec4<T>::set(const vec4<T> &v_)
{
  // set vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

template<typename T>
void vec4<T>::set(const T *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------


//============================================================================
// vec4<vec32s_t>
//============================================================================
vec4<vec32s_t> vec4<vec32s_t>::zero()
{
  return vec4<vec32s_t>(scalar_t(0));
}
//----

vec4<vec32s_t> vec4<vec32s_t>::one()
{
  return vec4<vec32s_t>(scalar_t(1));
}
//----

vec4<vec32s_t> vec4<vec32s_t>::neg_one()
{
  return vec4<vec32s_t>(scalar_t(-1));
}
//----------------------------------------------------------------------------

vec4<vec32s_t>::vec4()
{
}
//----

vec4<vec32s_t>::vec4(int8_t s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

vec4<vec32s_t>::vec4(int8_t x_, int8_t y_, int8_t z_, int8_t w_)
{
  // init vector with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

vec4<vec32s_t>::vec4(const vec3<vec32s_t> &v_, int8_t w_)
{
  // init vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

vec4<vec32s_t>::vec4(const int8_t *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------

const int8_t &vec4<vec32s_t>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const int8_t*>(this)[idx_];
}
//----

int8_t &vec4<vec32s_t>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<int8_t*>(this)[idx_];
}
//----

void vec4<vec32s_t>::set(int8_t s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

void vec4<vec32s_t>::set(int8_t x_, int8_t y_, int8_t z_, int8_t w_)
{
  // set vector with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

void vec4<vec32s_t>::set(const vec3<vec32s_t> &v_, int8_t w_)
{
  // set vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

void vec4<vec32s_t>::set(const vec4<vec32s_t> &v_)
{
  // set vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

void vec4<vec32s_t>::set(const int8_t *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------


//============================================================================
// vec4<vec32u_t>
//============================================================================
vec4<vec32u_t> vec4<vec32u_t>::zero()
{
  return vec4<vec32u_t>(scalar_t(0));
}
//----

vec4<vec32u_t> vec4<vec32u_t>::one()
{
  return vec4<vec32u_t>(scalar_t(1));
}
//----

vec4<vec32u_t> vec4<vec32u_t>::neg_one()
{
  return vec4<vec32u_t>(scalar_t(0xff));
}
//----------------------------------------------------------------------------

vec4<vec32u_t>::vec4()
{
}
//----

vec4<vec32u_t>::vec4(uint8_t s_)
{
  // init vector with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

vec4<vec32u_t>::vec4(uint8_t x_, uint8_t y_, uint8_t z_, uint8_t w_)
{
  // init vector with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

vec4<vec32u_t>::vec4(const vec3<vec32u_t> &v_, uint8_t w_)
{
  // init vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

vec4<vec32u_t>::vec4(const uint8_t *a_)
{
  // init vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------

const uint8_t &vec4<vec32u_t>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const uint8_t*>(this)[idx_];
}
//----

uint8_t &vec4<vec32u_t>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<uint8_t*>(this)[idx_];
}
//----

void vec4<vec32u_t>::set(uint8_t s_)
{
  // set vector with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

void vec4<vec32u_t>::set(uint8_t x_, uint8_t y_, uint8_t z_, uint8_t w_)
{
  // set vector with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

void vec4<vec32u_t>::set(const vec3<vec32u_t> &v_, uint8_t w_)
{
  // set vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

void vec4<vec32u_t>::set(const vec4<vec32u_t> &v_)
{
  // set vector with 3d vector and w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

void vec4<vec32u_t>::set(const uint8_t *a_)
{
  // set vector with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------


//============================================================================
// vec4 free functions
//============================================================================
template<typename T>
EGL_INLINE bool is_zero(const vec4<T> &v_)
{
  // test for zero-vector
  return v_.x==0 && v_.y==0 && v_.z==0 && v_.w==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const vec4<T> &v_)
{
  // test for saturated vector
  typedef typename math<T>::scalar_t scalar_t;
  return    v_.x>=0 && v_.x<=scalar_t(1)
         && v_.y>=0 && v_.y<=scalar_t(1)
         && v_.z>=0 && v_.z<=scalar_t(1)
         && v_.w>=0 && v_.w<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const vec4<T> &v_)
{
  // test for signed saturated vector
  typedef typename math<T>::scalar_t scalar_t;
  return    v_.x>=scalar_t(-1) && v_.x<=scalar_t(1)
         && v_.y>=scalar_t(-1) && v_.y<=scalar_t(1)
         && v_.z>=scalar_t(-1) && v_.z<=scalar_t(1)
         && v_.w>=scalar_t(-1) && v_.w<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool operator==(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // test for equality of vectors
  return v0_.x==v1_.x && v0_.y==v1_.y && v0_.z==v1_.z && v0_.w==v1_.w;
}
//----

template<typename T>
EGL_INLINE bool operator==(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // test for equality of vector and scalar
  return v_.x==s_ && v_.y==s_ && v_.z==s_ && v_.w==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // test for equality of vector and scalar
  return s_==v_.x && s_==v_.y && s_==v_.z && s_==v_.w;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // test for inequality of vectors
  return v0_.x!=v1_.x || v0_.y!=v1_.y || v0_.z!=v1_.z || v0_.w!=v1_.w;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // test for inequality of vector and scalar
  return v_.x!=s_ || v_.y!=s_ || v_.z!=s_ || v_.w!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // test for inequality of vector and scalar
  return s_!=v_.x || s_!=v_.y || s_!=v_.z || s_!=v_.w;
}
//----

template<typename T>
EGL_INLINE vec4<T> operator<(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v0_.x<v1_.x?scalar_t(1):0, v0_.y<v1_.y?scalar_t(1):0, v0_.z<v1_.z?scalar_t(1):0, v0_.w<v1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator<(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<s_?scalar_t(1):0, v_.y<s_?scalar_t(1):0, v_.z<s_?scalar_t(1):0, v_.w<s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator<(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(s_<v_.x?scalar_t(1):0, s_<v_.y?scalar_t(1):0, s_<v_.z?scalar_t(1):0, s_<v_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator>(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v0_.x>v1_.x?scalar_t(1):0, v0_.y>v1_.y?scalar_t(1):0, v0_.z>v1_.z?scalar_t(1):0, v0_.w>v1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator>(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x>s_?scalar_t(1):0, v_.y>s_?scalar_t(1):0, v_.z>s_?scalar_t(1):0, v_.w>s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator>(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(s_>v_.x?scalar_t(1):0, s_>v_.y?scalar_t(1):0, s_>v_.z?scalar_t(1):0, s_>v_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator<=(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v0_.x<=v1_.x?scalar_t(1):0, v0_.y<=v1_.y?scalar_t(1):0, v0_.z<=v1_.z?scalar_t(1):0, v0_.w<=v1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator<=(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<=s_?scalar_t(1):0, v_.y<=s_?scalar_t(1):0, v_.z<=s_?scalar_t(1):0, v_.w<=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator<=(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(s_<=v_.x?scalar_t(1):0, s_<=v_.y?scalar_t(1):0, s_<=v_.z?scalar_t(1):0, s_<=v_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator>=(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v0_.x>=v1_.x?scalar_t(1):0, v0_.y>=v1_.y?scalar_t(1):0, v0_.z>=v1_.z?scalar_t(1):0, v0_.w>=v1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator>=(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x>=s_?scalar_t(1):0, v_.y>=s_?scalar_t(1):0, v_.z>=s_?scalar_t(1):0, v_.w>=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator>=(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(s_>=v_.x?scalar_t(1):0, s_>=v_.y?scalar_t(1):0, s_>=v_.z?scalar_t(1):0, s_>=v_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> sel_eq(const vec4<T> &t0_, const vec4<T> &t1_, const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise equal vector select (t0_==t1_?v0_:v1_)
  return vec4<T>(t0_.x==t1_.x?v0_.x:v1_.x,
                 t0_.y==t1_.y?v0_.y:v1_.y,
                 t0_.z==t1_.z?v0_.z:v1_.z,
                 t0_.w==t1_.w?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sel_lt(const vec4<T> &t0_, const vec4<T> &t1_, const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise less-than vector select (t0_<t1_?v0_:v1_)
  return vec4<T>(t0_.x<t1_.x?v0_.x:v1_.x,
                 t0_.y<t1_.y?v0_.y:v1_.y,
                 t0_.z<t1_.z?v0_.z:v1_.z,
                 t0_.w<t1_.w?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sel_le(const vec4<T> &t0_, const vec4<T> &t1_, const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise less-or-equal vector select (t0_<=t1_?v0_:v1_)
  return vec4<T>(t0_.x<=t1_.x?v0_.x:v1_.x,
                 t0_.y<=t1_.y?v0_.y:v1_.y,
                 t0_.z<=t1_.z?v0_.z:v1_.z,
                 t0_.w<=t1_.w?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sel_eqz(const vec4<T> &t_, const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise equal-to-zero vector select (t_==0?v0_:v1_)
  return vec4<T>(t_.x==0?v0_.x:v1_.x,
                 t_.y==0?v0_.y:v1_.y,
                 t_.z==0?v0_.z:v1_.z,
                 t_.w==0?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sel_ltz(const vec4<T> &t_, const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise less-than-zero vector select (t_<0?v0_:v1_)
  return vec4<T>(t_.x<0?v0_.x:v1_.x,
                 t_.y<0?v0_.y:v1_.y,
                 t_.z<0?v0_.z:v1_.z,
                 t_.w<0?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sel_lez(const vec4<T> &t_, const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise less-or-equal-to-zero vector select (t_<=0?v0_:v1_)
  return vec4<T>(t_.x<=0?v0_.x:v1_.x,
                 t_.y<=0?v0_.y:v1_.y,
                 t_.z<=0?v0_.z:v1_.z,
                 t_.w<=0?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE void operator+=(vec4<T> &vr_, const vec4<T> &v_)
{
  // add vector to vector
  vr_.x+=v_.x;
  vr_.y+=v_.y;
  vr_.z+=v_.z;
  vr_.w+=v_.w;
}
//----

template<typename T>
EGL_INLINE void operator+=(vec4<T> &vr_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the vector
  vr_.x+=s_;
  vr_.y+=s_;
  vr_.z+=s_;
  vr_.w+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(vec4<T> &vr_, const vec4<T> &v_)
{
  // subtract vector from vector
  vr_.x-=v_.x;
  vr_.y-=v_.y;
  vr_.z-=v_.z;
  vr_.w-=v_.w;
}
//----

template<typename T>
EGL_INLINE void operator-=(vec4<T> &vr_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the vector
  vr_.x-=s_;
  vr_.y-=s_;
  vr_.z-=s_;
  vr_.w-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec4<T> &vr_, const vec4<T> &v_)
{
  // component-wise vector by vector multiply
  vr_.x*=v_.x;
  vr_.y*=v_.y;
  vr_.z*=v_.z;
  vr_.w*=v_.w;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec4<T> &vr_, typename math<T>::scalar_t s_)
{
  // multiply vector by scalar
  vr_.x*=s_;
  vr_.y*=s_;
  vr_.z*=s_;
  vr_.w*=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec4<T> &vr_, const mat44<T> &m_)
{
  // multiply vector by matrix
  vec4<T> v(vr_);
  vr_.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x+v.w*m_.w.x;
  vr_.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y+v.w*m_.w.y;
  vr_.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z+v.w*m_.w.z;
  vr_.w=v.x*m_.x.w+v.y*m_.y.w+v.z*m_.z.w+v.w*m_.w.w;
}
//----

template<typename T>
EGL_INLINE void operator/=(vec4<T> &vr_, const vec4<T> &v_)
{
  // component-wise vector by vector division
  EGL_ASSERT_PEDANTIC(v_.x && v_.y && v_.z && v_.w);
  vr_.x/=v_.x;
  vr_.y/=v_.y;
  vr_.z/=v_.z;
  vr_.w/=v_.w;
}
//----

template<typename T>
EGL_INLINE void operator/=(vec4<T> &vr_, typename math<T>::scalar_t s_)
{
  // divide vector by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  vr_.x*=rs;
  vr_.y*=rs;
  vr_.z*=rs;
  vr_.w*=rs;
}
//----

template<typename T>
EGL_INLINE vec4<T> operator+(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // add vector to vector
  return vec4<T>(v0_.x+v1_.x, v0_.y+v1_.y, v0_.z+v1_.z, v0_.w+v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator+(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the vector
  return vec4<T>(v_.x+s_, v_.y+s_, v_.z+s_, v_.w+s_);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator+(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // add each component of the vector to a scalar vector
  return vec4<T>(s_+v_.x, s_+v_.y, s_+v_.z, s_+v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator-(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // subtract vector from vector
  return vec4<T>(v0_.x-v1_.x, v0_.y-v1_.y, v0_.z-v1_.z, v0_.w-v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator-(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the vector
  return vec4<T>(v_.x-s_, v_.y-s_, v_.z-s_, v_.w-s_);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator-(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // subtract each component of the vector from a scalar vector
  return vec4<T>(s_-v_.x, s_-v_.y, s_-v_.z, s_-v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator-(const vec4<T> &v_)
{
  // negate vector
  return vec4<T>(-v_.x, -v_.y, -v_.z, -v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise vector by vector multiply
  return vec4<T>(v0_.x*v1_.x, v0_.y*v1_.y, v0_.z*v1_.z, v0_.w*v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // multiply vector by scalar
  return vec4<T>(v_.x*s_, v_.y*s_, v_.z*s_, v_.w*s_);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // multiply vector by scalar
  return vec4<T>(v_.x*s_, v_.y*s_, v_.z*s_, v_.w*s_);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const vec4<T> &v_, const mat44<T> &m_)
{
  // multiply vector by matrix
  return vec4<T>(v_.x*m_.x.x+v_.y*m_.y.x+v_.z*m_.z.x+v_.w*m_.w.x,
                 v_.x*m_.x.y+v_.y*m_.y.y+v_.z*m_.z.y+v_.w*m_.w.y,
                 v_.x*m_.x.z+v_.y*m_.y.z+v_.z*m_.z.z+v_.w*m_.w.z,
                 v_.x*m_.x.w+v_.y*m_.y.w+v_.z*m_.z.w+v_.w*m_.w.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const mat44<T> &m_, const vec4<T> &v_)
{
  // multiply matrix by transposed vector
  return vec4<T>(v_.x*m_.x.x+v_.y*m_.x.y+v_.z*m_.x.z+v_.w*m_.x.w,
                 v_.x*m_.y.x+v_.y*m_.y.y+v_.z*m_.y.z+v_.w*m_.y.w,
                 v_.x*m_.z.x+v_.y*m_.z.y+v_.z*m_.z.z+v_.w*m_.z.w,
                 v_.x*m_.w.x+v_.y*m_.w.y+v_.z*m_.w.z+v_.w*m_.w.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator/(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise vector by vector division
  EGL_ASSERT_PEDANTIC(v1_.x && v1_.y && v1_.z && v1_.w);
  return vec4<T>(v0_.x/v1_.x, v0_.y/v1_.y, v0_.z/v1_.z, v0_.w/v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator/(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // divide vector by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return vec4<T>(v_.x*rs, v_.y*rs, v_.z*rs, v_.w*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t min(const vec4<T> &v_)
{
  // minimum component value
  return min(v_.x, v_.y, v_.z, v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> min(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise min vector
  return vec4<T>(v0_.x<v1_.x?v0_.x:v1_.x,
                 v0_.y<v1_.y?v0_.y:v1_.y,
                 v0_.z<v1_.z?v0_.z:v1_.z,
                 v0_.w<v1_.w?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> min(const vec4<T> &v0_, const vec4<T> &v1_, const vec4<T> &v2_)
{
  // component-wise min vector
  return vec4<T>(min(v0_.x, v1_.x, v2_.x),
                 min(v0_.y, v1_.y, v2_.y),
                 min(v0_.z, v1_.z, v2_.z),
                 min(v0_.w, v1_.w, v2_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> min(const vec4<T> &v0_, const vec4<T> &v1_, const vec4<T> &v2_, const vec4<T> &v3_)
{
  // component-wise min vector
  return vec4<T>(min(v0_.x, v1_.x, v2_.x, v3_.x),
                 min(v0_.y, v1_.y, v2_.y, v3_.y),
                 min(v0_.z, v1_.z, v2_.z, v3_.z),
                 min(v0_.w, v1_.w, v2_.w, v3_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> min(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise min vector
  return vec4<T>(v_.x<s_?v_.x:s_,
                 v_.y<s_?v_.y:s_,
                 v_.z<s_?v_.z:s_,
                 v_.w<s_?v_.w:s_);
}
//----

template<typename T>
EGL_INLINE vec4<T> min(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // component-wise min vector
  return vec4<T>(s_<v_.x?s_:v_.x,
                 s_<v_.y?s_:v_.y,
                 s_<v_.z?s_:v_.z,
                 s_<v_.w?s_:v_.w);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t max(const vec4<T> &v_)
{
  // maximum component value
  return max(v_.x, v_.y, v_.z, v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> max(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise max vector
  return vec4<T>(v0_.x>v1_.x?v0_.x:v1_.x,
                 v0_.y>v1_.y?v0_.y:v1_.y,
                 v0_.z>v1_.z?v0_.z:v1_.z,
                 v0_.w>v1_.w?v0_.w:v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> max(const vec4<T> &v0_, const vec4<T> &v1_, const vec4<T> &v2_)
{
  // component-wise max vector
  return vec4<T>(max(v0_.x, v1_.x, v2_.x),
                 max(v0_.y, v1_.y, v2_.y),
                 max(v0_.z, v1_.z, v2_.z),
                 max(v0_.w, v1_.w, v2_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> max(const vec4<T> &v0_, const vec4<T> &v1_, const vec4<T> &v2_, const vec4<T> &v3_)
{
  // component-wise max vector
  return vec4<T>(max(v0_.x, v1_.x, v2_.x, v3_.x),
                 max(v0_.y, v1_.y, v2_.y, v3_.y),
                 max(v0_.z, v1_.z, v2_.z, v3_.z),
                 max(v0_.w, v1_.w, v2_.w, v3_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> max(const vec4<T> &v_, typename math<T>::scalar_t s_)
{
  // component-wise max vector
  return vec4<T>(v_.x>s_?v_.x:s_,
                 v_.y>s_?v_.y:s_,
                 v_.z>s_?v_.z:s_,
                 v_.w>s_?v_.w:s_);
}
//----

template<typename T>
EGL_INLINE vec4<T> max(typename math<T>::scalar_t s_, const vec4<T> &v_)
{
  // component-wise max vector
  return vec4<T>(s_>v_.x?s_:v_.x,
                 s_>v_.y?s_:v_.y,
                 s_>v_.z?s_:v_.z,
                 s_>v_.w?s_:v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> mul(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // component-wise multiply: v0_*v1_
  return vec4<T>(v0_.x*v1_.x, v0_.y*v1_.y, v0_.z*v1_.z, v0_.w*v1_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> madd(const vec4<T> &vm0_, const vec4<T> &vm1_, const vec4<T> &va_)
{
  // component-wise multiply-add: vm0_*vm1+va_
  return vec4<T>(vm0_.x*vm1_.x+va_.x, vm0_.y*vm1_.y+va_.y, vm0_.z*vm1_.z+va_.z, vm0_.w*vm1_.w+va_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> madd(const vec4<T> &vm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: vm_*sm_+sa_
  return vec4<T>(vm_.x*sm_+sa_, vm_.y*sm_+sa_, vm_.z*sm_+sa_, vm_.w*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE vec4<T> madd(const vec4<T> &vm0_, const vec4<T> &vm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: vm0_*vm1_+sa_
  return vec4<T>(vm0_.x*vm1_.x+sa_, vm0_.y*vm1_.y+sa_, vm0_.z*vm1_.z+sa_, vm0_.w*vm1_.w+sa_);
}
//----

template<typename T>
EGL_INLINE vec4<T> madd(const vec4<T> &vm_, typename math<T>::scalar_t sm_, const vec4<T> &va_)
{
  // component-wise multiply-add: vm_*sm_+va_
  return vec4<T>(vm_.x*sm_+va_.x, vm_.y*sm_+va_.y, vm_.z*sm_+va_.z, vm_.w*sm_+va_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> rcp(const vec4<T> &v_)
{
  // component-wise reciprocal
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x && v_.y && v_.z && v_.w);
  return vec4<T>(scalar_t(1)/v_.x, scalar_t(1)/v_.y, scalar_t(1)/v_.z, scalar_t(1)/v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> rcp_z(const vec4<T> &v_)
{
  // component-wise reciprocal (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x?scalar_t(1)/v_.x:0, v_.y?scalar_t(1)/v_.y:0, v_.z?scalar_t(1)/v_.z:0, v_.w?scalar_t(1)/v_.w:0);
}
//----

template<typename T>
EGL_INLINE vec4<T> floor(const vec4<T> &v_)
{
  // component-wise floor
  return vec4<T>(MATH_NAMESPACE::floor(v_.x), MATH_NAMESPACE::floor(v_.y), MATH_NAMESPACE::floor(v_.z), MATH_NAMESPACE::floor(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> ceil(const vec4<T> &v_)
{
  // component-wise ceil
  return vec4<T>(MATH_NAMESPACE::ceil(v_.x), MATH_NAMESPACE::ceil(v_.y), MATH_NAMESPACE::ceil(v_.z), MATH_NAMESPACE::ceil(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> trunc(const vec4<T> &v_)
{
  // component-wise truncate
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(scalar_t(int(v_.x)), scalar_t(int(v_.y)), scalar_t(int(v_.z)), scalar_t(int(v_.w)));
}
//----

template<typename T>
EGL_INLINE vec4<T> frc(const vec4<T> &v_)
{
  // component-wise fractional
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x-scalar_t(int(v_.x)), v_.y-scalar_t(int(v_.y)), v_.z-scalar_t(int(v_.z)), v_.w-scalar_t(int(v_.w)));
}
//----

template<typename T>
EGL_INLINE vec4<T> mod(const vec4<T> &v_, typename math<T>::scalar_t div_)
{
  // component-wise modulo
  return vec4<T>(MATH_NAMESPACE::fmod(v_.x, div_), MATH_NAMESPACE::fmod(v_.y, div_), MATH_NAMESPACE::fmod(v_.z, div_), MATH_NAMESPACE::fmod(v_.w, div_));
}
//----

template<typename T>
EGL_INLINE vec4<T> cycle(const vec4<T> &v_, typename math<T>::scalar_t cycle_)
{
  // component-wise cycle
  return vec4<T>(MATH_NAMESPACE::fmod(v_.x, cycle_)+(v_.x<0?cycle_:0),
                 MATH_NAMESPACE::fmod(v_.y, cycle_)+(v_.y<0?cycle_:0),
                 MATH_NAMESPACE::fmod(v_.z, cycle_)+(v_.z<0?cycle_:0),
                 MATH_NAMESPACE::fmod(v_.w, cycle_)+(v_.w<0?cycle_:0));
}
//----

template<typename T>
EGL_INLINE vec4<T> cycle1(const vec4<T> &v_)
{
  // cycle1
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x-scalar_t(int(v_.x))+(v_.x<0?scalar_t(1):0),
                 v_.y-scalar_t(int(v_.y))+(v_.y<0?scalar_t(1):0),
                 v_.z-scalar_t(int(v_.z))+(v_.z<0?scalar_t(1):0),
                 v_.w-scalar_t(int(v_.w))+(v_.w<0?scalar_t(1):0));
}
//----

template<typename T>
EGL_INLINE vec4<T> sat(const vec4<T> &v_)
{
  // component-wise saturate
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<0?0:v_.x>scalar_t(1)?scalar_t(1):v_.x,
                 v_.y<0?0:v_.y>scalar_t(1)?scalar_t(1):v_.y,
                 v_.z<0?0:v_.z>scalar_t(1)?scalar_t(1):v_.z,
                 v_.w<0?0:v_.w>scalar_t(1)?scalar_t(1):v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> ssat(const vec4<T> &v_)
{
  // component-wise signed saturate
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<scalar_t(-1)?scalar_t(-1):v_.x>scalar_t(1)?scalar_t(1):v_.x,
                 v_.y<scalar_t(-1)?scalar_t(-1):v_.y>scalar_t(1)?scalar_t(1):v_.y,
                 v_.z<scalar_t(-1)?scalar_t(-1):v_.z>scalar_t(1)?scalar_t(1):v_.z,
                 v_.w<scalar_t(-1)?scalar_t(-1):v_.w>scalar_t(1)?scalar_t(1):v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> clamp(const vec4<T> &v_, const vec4<T> &min_, const vec4<T> &max_)
{
  // component-wise clamp (to vectors)
  return vec4<T>(v_.x<min_.x?min_.x:v_.x>max_.x?max_.x:v_.x,
                 v_.y<min_.y?min_.y:v_.y>max_.y?max_.y:v_.y,
                 v_.z<min_.z?min_.z:v_.z>max_.z?max_.z:v_.z,
                 v_.w<min_.w?min_.w:v_.w>max_.w?max_.w:v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> clamp(const vec4<T> &v_, typename math<T>::scalar_t min_, typename math<T>::scalar_t max_)
{
  // component-wise clamp (to scalars)
  return vec4<T>(v_.x<min_?min_:v_.x>max_?max_:v_.x,
                 v_.y<min_?min_:v_.y>max_?max_:v_.y,
                 v_.z<min_?min_:v_.z>max_?max_:v_.z,
                 v_.w<min_?min_:v_.w>max_?max_:v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> abs(const vec4<T> &v_)
{
  // component-wise absolute
  return vec4<T>(v_.x<0?-v_.x:v_.x, v_.y<0?-v_.y:v_.y, v_.z<0?-v_.z:v_.z, v_.w<0?-v_.w:v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sgn(const vec4<T> &v_)
{
  // component-wise signum (neg=-1, pos=+1, zero=0)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<0?scalar_t(-1):v_.x>0?scalar_t(1):0, v_.y<0?scalar_t(-1):v_.y>0?scalar_t(1):0, v_.z<0?scalar_t(-1):v_.z>0?scalar_t(1):0, v_.w<0?scalar_t(-1):v_.w>0?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> sgn_zp(const vec4<T> &v_)
{
  // component-wise signum (neg=-1, pos&zero=+1)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<0?scalar_t(-1):scalar_t(1), v_.y<0?scalar_t(-1):scalar_t(1), v_.z<0?scalar_t(-1):scalar_t(1), v_.w<0?scalar_t(-1):scalar_t(1));
}
//----

template<typename T>
EGL_INLINE vec4<T> sqr(const vec4<T> &v_)
{
  // component-wise square
  return vec4<T>(v_.x*v_.x, v_.y*v_.y, v_.z*v_.z, v_.w*v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> cubic(const vec4<T> &v_)
{
  // component-wise cubic
  return vec4<T>(v_.x*v_.x*v_.x, v_.y*v_.y*v_.y, v_.z*v_.z*v_.z, v_.w*v_.w*v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> sqrt(const vec4<T> &v_)
{
  // component-wise square root
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.y>=0 && v_.z>=0 && v_.w>=0);
  return vec4<T>(MATH_NAMESPACE::sqrt(v_.x), MATH_NAMESPACE::sqrt(v_.y), MATH_NAMESPACE::sqrt(v_.z), MATH_NAMESPACE::sqrt(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> sqrt_z(const vec4<T> &v_)
{
  // component-wise square root (set to 0 for components less than zero)
  return vec4<T>(sqrt_z(v_.x), sqrt_z(v_.y), sqrt_z(v_.z), sqrt_z(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> cbrt(const vec4<T> &v_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(1.0/3.0)),
                 v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(1.0/3.0)),
                 v_.z<0?-MATH_NAMESPACE::pow(-v_.z, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.z, scalar_t(1.0/3.0)),
                 v_.w<0?-MATH_NAMESPACE::pow(-v_.w, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(v_.w, scalar_t(1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE vec4<T> rsqrt(const vec4<T> &v_)
{
  // component-wise reciprocal square root
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>0 && v_.y>0 && v_.z>0 && v_.w>0);
  return vec4<T>(scalar_t(1)/MATH_NAMESPACE::sqrt(v_.x), scalar_t(1)/MATH_NAMESPACE::sqrt(v_.y), scalar_t(1)/MATH_NAMESPACE::sqrt(v_.z), scalar_t(1)/MATH_NAMESPACE::sqrt(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> rsqrt_z(const vec4<T> &v_)
{
  // component-wise reciprocal square root (set to 0 for components less than or equal to zero)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.x):0,
                 v_.y>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.y):0,
                 v_.z>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.z):0,
                 v_.w>0?scalar_t(1)/MATH_NAMESPACE::sqrt(v_.w):0);
}
//----

template<typename T>
EGL_INLINE vec4<T> rcbrt(const vec4<T> &v_)
{
  // component-wise reciprocal cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(-1.0/3.0)),
                 v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(-1.0/3.0)),
                 v_.z<0?-MATH_NAMESPACE::pow(-v_.z, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.z, scalar_t(-1.0/3.0)),
                 v_.w<0?-MATH_NAMESPACE::pow(-v_.w, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.w, scalar_t(-1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE vec4<T> rcbrt_z(const vec4<T> &v_)
{
  // component-wise reciprocal cubic root (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(v_.x?v_.x<0?-MATH_NAMESPACE::pow(-v_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.x, scalar_t(-1.0/3.0)):0,
                 v_.y?v_.y<0?-MATH_NAMESPACE::pow(-v_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.y, scalar_t(-1.0/3.0)):0,
                 v_.z?v_.z<0?-MATH_NAMESPACE::pow(-v_.z, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.z, scalar_t(-1.0/3.0)):0,
                 v_.w?v_.w<0?-MATH_NAMESPACE::pow(-v_.w, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(v_.w, scalar_t(-1.0/3.0)):0);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm(const vec4<T> &v_)
{
  // norm of the vector (=vector length)
  return MATH_NAMESPACE::sqrt(v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm(const vec4<T> &v_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_z(const vec4<T> &v_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
  return n2?scalar_t(1)/MATH_NAMESPACE::sqrt(n2):0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm2(const vec4<T> &v_)
{
  // squared norm of the vector
  return v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2(const vec4<T> &v_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/n2;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2_z(const vec4<T> &v_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
  return v?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm_l1(const vec4<T> &v_)
{
  // L1-norm of the vector (Manhattan norm)
  return (v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y)+(v_.z<0?-v_.z:v_.z)+(v_.w<0?-v_.w:v_.w);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_l1(const vec4<T> &v_)
{
  // reciprocal L1-norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  return scalar_t(1)/((v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y)+(v_.z<0?-v_.z:v_.z)+(v_.w<0?-v_.w:v_.w));
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_l1_z(const vec4<T> &v_)
{
  // reciprocal L1-norm of the vector (return 0 if |v|=0)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=(v_.x<0?-v_.x:v_.x)+(v_.y<0?-v_.y:v_.y)+(v_.z<0?-v_.z:v_.z)+(v_.w<0?-v_.w:v_.w);
  return v?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE vec4<T> exp(const vec4<T> &v_)
{
  // natural exponent
  return vec4<T>(MATH_NAMESPACE::exp(v_.x), MATH_NAMESPACE::exp(v_.y), MATH_NAMESPACE::exp(v_.z), MATH_NAMESPACE::exp(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> exp2(const vec4<T> &v_)
{
  // 2^x
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(MATH_NAMESPACE::pow(scalar_t(2), v_.x), MATH_NAMESPACE::pow(scalar_t(2), v_.y), MATH_NAMESPACE::pow(scalar_t(2), v_.z), MATH_NAMESPACE::pow(scalar_t(2), v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> ln(const vec4<T> &v_)
{
  // natural logarithm
  return vec4<T>(MATH_NAMESPACE::log(v_.x), MATH_NAMESPACE::log(v_.y), MATH_NAMESPACE::log(v_.z), MATH_NAMESPACE::log(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> log2(const vec4<T> &v_)
{
  // base-2 logarithm
  return vec4<T>(T(MATH_NAMESPACE::log(v_.x))*T(1.44269504089), T(MATH_NAMESPACE::log(v_.y))*T(1.44269504089), T(MATH_NAMESPACE::log(v_.z))*T(1.44269504089), T(MATH_NAMESPACE::log(v_.w))*T(1.44269504089));
}
//----

template<typename T>
EGL_INLINE vec4<T> log10(const vec4<T> &v_)
{
  // base-10 logarithm
  return vec4<T>(MATH_NAMESPACE::log10(v_.x), MATH_NAMESPACE::log10(v_.y), MATH_NAMESPACE::log10(v_.z), MATH_NAMESPACE::log10(v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> pow(const vec4<T> &v_, typename math<T>::scalar_t power_)
{
  // raises value to the given power
  return vec4<T>(MATH_NAMESPACE::pow(v_.x, power_), MATH_NAMESPACE::pow(v_.y, power_), MATH_NAMESPACE::pow(v_.z, power_), MATH_NAMESPACE::pow(v_.w, power_));
}
//----

template<typename T>
EGL_INLINE vec4<T> unit(const vec4<T> &v_)
{
  // unit vector of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return vec4<T>(v_.x*rs, v_.y*rs, v_.z*rs, v_.w*rs);
}
//----

template<typename T>
EGL_INLINE vec4<T> unit_z(const vec4<T> &v_)
{
  // unit vector of the vector. if |v|=0, return v=[0, 0, 0, 0]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs=v_.x*v_.x+v_.y*v_.y+v_.z*v_.z+v_.w*v_.w;
  if(!rs)
    return vec4<T>(0, 0, 0, 0);
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return vec4<T>(v_.x*rs, v_.y*rs, v_.z*rs, v_.w*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot(const vec4<T> &v0_, const vec4<T> &v1_)
{
  // dot-product of vectors
  return v0_.x*v1_.x+v0_.y*v1_.y+v0_.z*v1_.z+v0_.w*v1_.w;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot1(const vec4<T> &v_)
{
  // dot-product with [1, 1, 1, 1]
  return v_.x+v_.y+v_.z+v_.w;
}
//----

template<typename T>
EGL_INLINE void neg(vec4<T> &v_)
{
  // negate the vector
  v_.x=-v_.x;
  v_.y=-v_.y;
  v_.z=-v_.z;
  v_.w=-v_.w;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_u(vec4<T> &vr_, Rng &rng_)
{
  // random unit vector (even distribution on unit hypersphere)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs, rc, vs, vc;
  sincos(rs, rc, scalar_t(rng_.rand_ureal1())*math<T>::pi);
  sincos(vs, vc, scalar_t(rng_.rand_ureal1())*math<T>::two_pi);
  scalar_t z=scalar_t(rng_.rand_real1());
  vr_.z=z*rs;
  vr_.w=rc;
  scalar_t rad=MATH_NAMESPACE::sqrt(scalar_t(1)-z*z)*rs;
  vr_.x=vs*rad;
  vr_.y=vc*rad;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_real1(vec4<T> &vr_, Rng &rng_)
{
  // random vector where each component is in range [-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=scalar_t(rng_.rand_real1());
  vr_.y=scalar_t(rng_.rand_real1());
  vr_.z=scalar_t(rng_.rand_real1());
  vr_.w=scalar_t(rng_.rand_real1());
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_ureal1(vec4<T> &vr_, Rng &rng_)
{
  // random vector where each component is in range [0, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=scalar_t(rng_.rand_ureal1());
  vr_.y=scalar_t(rng_.rand_ureal1());
  vr_.z=scalar_t(rng_.rand_ureal1());
  vr_.w=scalar_t(rng_.rand_ureal1());
}
//----

template<typename T>
EGL_INLINE vec4<T> smoothstep(const vec4<T> &v_)
{
  // component-wise smoothstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.x<=scalar_t(1) && v_.y>=0 && v_.y<=scalar_t(1) && v_.z>=0 && v_.z<=scalar_t(1) && v_.w>=0 && v_.w<=scalar_t(1));
  return vec4<T>(v_.x*v_.x*(scalar_t(3)-scalar_t(2)*v_.x),
                 v_.y*v_.y*(scalar_t(3)-scalar_t(2)*v_.y),
                 v_.z*v_.z*(scalar_t(3)-scalar_t(2)*v_.z),
                 v_.w*v_.w*(scalar_t(3)-scalar_t(2)*v_.w));
}
//----

template<typename T>
EGL_INLINE vec4<T> smootherstep(const vec4<T> &v_)
{
  // component-wise smootherstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(v_.x>=0 && v_.x<=scalar_t(1) && v_.y>=0 && v_.y<=scalar_t(1) && v_.z>=0 && v_.z<=scalar_t(1) && v_.w>=0 && v_.w<=scalar_t(1));
  return vec4<T>(v_.x*v_.x*v_.x*(v_.x*(v_.x*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 v_.y*v_.y*v_.y*(v_.y*(v_.y*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 v_.z*v_.z*v_.z*(v_.z*(v_.z*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 v_.w*v_.w*v_.w*(v_.w*(v_.w*scalar_t(6)-scalar_t(15))+scalar_t(10)));
}
//----

template<typename T>
EGL_INLINE vec4<T> lerp(const vec4<T> &v0_, const vec4<T> &v1_, typename math<T>::scalar_t t_)
{
  // linear vector interpolation. f(t=0)=v0, f(t=1)=v1
  return vec4<T>(v0_.x+(v1_.x-v0_.x)*t_,
                 v0_.y+(v1_.y-v0_.y)*t_,
                 v0_.z+(v1_.z-v0_.z)*t_,
                 v0_.w+(v1_.w-v0_.w)*t_);
}
//----

template<typename T> EGL_INLINE vec4<T> reflect(const vec4<T> &v_, const vec4<T> &n_)
{
  // reflect vector about normal: v'=2*n*(v.n)/(n.n)-v
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=scalar_t(2)*(v_.x*n_.x+v_.y*n_.y+v_.z*n_.z+v_.w*n_.w)*rcp(n_.x*n_.x+n_.y*n_.y+n_.z*n_.z+n_.w*n_.w);
  return vec4<T>(d*n_.x-v_.x,
                 d*n_.y-v_.y,
                 d*n_.z-v_.z,
                 d*n_.w-v_.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> reflect_u(const vec4<T> &v_, const vec4<T> &n_)
{
  // reflect vector about unit normal: v'=2*n*(v.n)-v
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=scalar_t(2)*(v_.x*n_.x+v_.y*n_.y+v_.z*n_.z+v_.w*n_.w);
  return vec4<T>(d*n_.x-v_.x,
                 d*n_.y-v_.y,
                 d*n_.z-v_.z,
                 d*n_.w-v_.w);
}
//----

template<typename T>
EGL_INLINE vec3<T> perspective_project(const vec4<T> &v_)
{
  // perspective project vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t s=scalar_t(1)/v_.w;
  return vec3<T>(v_.x*s, v_.y*s, v_.z*s);
}
//----

template<typename T, typename U>
EGL_INLINE vec4<T> to_vec4(const vec4<U> &v_)
{
  // convert between vec4 types
  typedef typename math<T>::scalar_t scalar_t;
  return vec4<T>(scalar_t(v_.x), scalar_t(v_.y), scalar_t(v_.z), scalar_t(v_.w));
}
//----

namespace priv
{
  template<unsigned shuffle_, typename T>
  struct shuffle_vec4_helper
  {
    static vec4<T> shuffle(const vec4<T>&) {EGL_STATIC_ERROR(T, invalid_vector_component_shuffle);}
  };
  #define EGL_VECTOR_SHUFFLE(shuffle__, x__, y__, z__, w__)\
  template<typename T> struct shuffle_vec4_helper<shuffle__, T>\
  {\
    static EGL_INLINE vec4<T> shuffle(const vec4<T> &v_) {return vec4<T>(v_.x__, v_.y__, v_.z__, v_.w__);}\
  };
  EGL_VECTOR_SHUFFLE(0000, x, x, x, x); EGL_VECTOR_SHUFFLE(0001, x, x, x, y); EGL_VECTOR_SHUFFLE(0002, x, x, x, z); EGL_VECTOR_SHUFFLE(0003, x, x, x, w); EGL_VECTOR_SHUFFLE(0010, x, x, y, x); EGL_VECTOR_SHUFFLE(0011, x, x, y, y); EGL_VECTOR_SHUFFLE(0012, x, x, y, z); EGL_VECTOR_SHUFFLE(0013, x, x, y, w); EGL_VECTOR_SHUFFLE(0020, x, x, z, x); EGL_VECTOR_SHUFFLE(0021, x, x, z, y); EGL_VECTOR_SHUFFLE(0022, x, x, z, z); EGL_VECTOR_SHUFFLE(0023, x, x, z, w); EGL_VECTOR_SHUFFLE(0030, x, x, w, x); EGL_VECTOR_SHUFFLE(0031, x, x, w, y); EGL_VECTOR_SHUFFLE(0032, x, x, w, z); EGL_VECTOR_SHUFFLE(0033, x, x, w, w);
  EGL_VECTOR_SHUFFLE(0100, x, y, x, x); EGL_VECTOR_SHUFFLE(0101, x, y, x, y); EGL_VECTOR_SHUFFLE(0102, x, y, x, z); EGL_VECTOR_SHUFFLE(0103, x, y, x, w); EGL_VECTOR_SHUFFLE(0110, x, y, y, x); EGL_VECTOR_SHUFFLE(0111, x, y, y, y); EGL_VECTOR_SHUFFLE(0112, x, y, y, z); EGL_VECTOR_SHUFFLE(0113, x, y, y, w); EGL_VECTOR_SHUFFLE(0120, x, y, z, x); EGL_VECTOR_SHUFFLE(0121, x, y, z, y); EGL_VECTOR_SHUFFLE(0122, x, y, z, z); EGL_VECTOR_SHUFFLE(0123, x, y, z, w); EGL_VECTOR_SHUFFLE(0130, x, y, w, x); EGL_VECTOR_SHUFFLE(0131, x, y, w, y); EGL_VECTOR_SHUFFLE(0132, x, y, w, z); EGL_VECTOR_SHUFFLE(0133, x, y, w, w);
  EGL_VECTOR_SHUFFLE(0200, x, z, x, x); EGL_VECTOR_SHUFFLE(0201, x, z, x, y); EGL_VECTOR_SHUFFLE(0202, x, z, x, z); EGL_VECTOR_SHUFFLE(0203, x, z, x, w); EGL_VECTOR_SHUFFLE(0210, x, z, y, x); EGL_VECTOR_SHUFFLE(0211, x, z, y, y); EGL_VECTOR_SHUFFLE(0212, x, z, y, z); EGL_VECTOR_SHUFFLE(0213, x, z, y, w); EGL_VECTOR_SHUFFLE(0220, x, z, z, x); EGL_VECTOR_SHUFFLE(0221, x, z, z, y); EGL_VECTOR_SHUFFLE(0222, x, z, z, z); EGL_VECTOR_SHUFFLE(0223, x, z, z, w); EGL_VECTOR_SHUFFLE(0230, x, z, w, x); EGL_VECTOR_SHUFFLE(0231, x, z, w, y); EGL_VECTOR_SHUFFLE(0232, x, z, w, z); EGL_VECTOR_SHUFFLE(0233, x, z, w, w);
  EGL_VECTOR_SHUFFLE(0300, x, w, x, x); EGL_VECTOR_SHUFFLE(0301, x, w, x, y); EGL_VECTOR_SHUFFLE(0302, x, w, x, z); EGL_VECTOR_SHUFFLE(0303, x, w, x, w); EGL_VECTOR_SHUFFLE(0310, x, w, y, x); EGL_VECTOR_SHUFFLE(0311, x, w, y, y); EGL_VECTOR_SHUFFLE(0312, x, w, y, z); EGL_VECTOR_SHUFFLE(0313, x, w, y, w); EGL_VECTOR_SHUFFLE(0320, x, w, z, x); EGL_VECTOR_SHUFFLE(0321, x, w, z, y); EGL_VECTOR_SHUFFLE(0322, x, w, z, z); EGL_VECTOR_SHUFFLE(0323, x, w, z, w); EGL_VECTOR_SHUFFLE(0330, x, w, w, x); EGL_VECTOR_SHUFFLE(0331, x, w, w, y); EGL_VECTOR_SHUFFLE(0332, x, w, w, z); EGL_VECTOR_SHUFFLE(0333, x, w, w, w);
  EGL_VECTOR_SHUFFLE(1000, y, x, x, x); EGL_VECTOR_SHUFFLE(1001, y, x, x, y); EGL_VECTOR_SHUFFLE(1002, y, x, x, z); EGL_VECTOR_SHUFFLE(1003, y, x, x, w); EGL_VECTOR_SHUFFLE(1010, y, x, y, x); EGL_VECTOR_SHUFFLE(1011, y, x, y, y); EGL_VECTOR_SHUFFLE(1012, y, x, y, z); EGL_VECTOR_SHUFFLE(1013, y, x, y, w); EGL_VECTOR_SHUFFLE(1020, y, x, z, x); EGL_VECTOR_SHUFFLE(1021, y, x, z, y); EGL_VECTOR_SHUFFLE(1022, y, x, z, z); EGL_VECTOR_SHUFFLE(1023, y, x, z, w); EGL_VECTOR_SHUFFLE(1030, y, x, w, x); EGL_VECTOR_SHUFFLE(1031, y, x, w, y); EGL_VECTOR_SHUFFLE(1032, y, x, w, z); EGL_VECTOR_SHUFFLE(1033, y, x, w, w);
  EGL_VECTOR_SHUFFLE(1100, y, y, x, x); EGL_VECTOR_SHUFFLE(1101, y, y, x, y); EGL_VECTOR_SHUFFLE(1102, y, y, x, z); EGL_VECTOR_SHUFFLE(1103, y, y, x, w); EGL_VECTOR_SHUFFLE(1110, y, y, y, x); EGL_VECTOR_SHUFFLE(1111, y, y, y, y); EGL_VECTOR_SHUFFLE(1112, y, y, y, z); EGL_VECTOR_SHUFFLE(1113, y, y, y, w); EGL_VECTOR_SHUFFLE(1120, y, y, z, x); EGL_VECTOR_SHUFFLE(1121, y, y, z, y); EGL_VECTOR_SHUFFLE(1122, y, y, z, z); EGL_VECTOR_SHUFFLE(1123, y, y, z, w); EGL_VECTOR_SHUFFLE(1130, y, y, w, x); EGL_VECTOR_SHUFFLE(1131, y, y, w, y); EGL_VECTOR_SHUFFLE(1132, y, y, w, z); EGL_VECTOR_SHUFFLE(1133, y, y, w, w);
  EGL_VECTOR_SHUFFLE(1200, y, z, x, x); EGL_VECTOR_SHUFFLE(1201, y, z, x, y); EGL_VECTOR_SHUFFLE(1202, y, z, x, z); EGL_VECTOR_SHUFFLE(1203, y, z, x, w); EGL_VECTOR_SHUFFLE(1210, y, z, y, x); EGL_VECTOR_SHUFFLE(1211, y, z, y, y); EGL_VECTOR_SHUFFLE(1212, y, z, y, z); EGL_VECTOR_SHUFFLE(1213, y, z, y, w); EGL_VECTOR_SHUFFLE(1220, y, z, z, x); EGL_VECTOR_SHUFFLE(1221, y, z, z, y); EGL_VECTOR_SHUFFLE(1222, y, z, z, z); EGL_VECTOR_SHUFFLE(1223, y, z, z, w); EGL_VECTOR_SHUFFLE(1230, y, z, w, x); EGL_VECTOR_SHUFFLE(1231, y, z, w, y); EGL_VECTOR_SHUFFLE(1232, y, z, w, z); EGL_VECTOR_SHUFFLE(1233, y, z, w, w);
  EGL_VECTOR_SHUFFLE(1300, y, w, x, x); EGL_VECTOR_SHUFFLE(1301, y, w, x, y); EGL_VECTOR_SHUFFLE(1302, y, w, x, z); EGL_VECTOR_SHUFFLE(1303, y, w, x, w); EGL_VECTOR_SHUFFLE(1310, y, w, y, x); EGL_VECTOR_SHUFFLE(1311, y, w, y, y); EGL_VECTOR_SHUFFLE(1312, y, w, y, z); EGL_VECTOR_SHUFFLE(1313, y, w, y, w); EGL_VECTOR_SHUFFLE(1320, y, w, z, x); EGL_VECTOR_SHUFFLE(1321, y, w, z, y); EGL_VECTOR_SHUFFLE(1322, y, w, z, z); EGL_VECTOR_SHUFFLE(1323, y, w, z, w); EGL_VECTOR_SHUFFLE(1330, y, w, w, x); EGL_VECTOR_SHUFFLE(1331, y, w, w, y); EGL_VECTOR_SHUFFLE(1332, y, w, w, z); EGL_VECTOR_SHUFFLE(1333, y, w, w, w);
  EGL_VECTOR_SHUFFLE(2000, z, x, x, x); EGL_VECTOR_SHUFFLE(2001, z, x, x, y); EGL_VECTOR_SHUFFLE(2002, z, x, x, z); EGL_VECTOR_SHUFFLE(2003, z, x, x, w); EGL_VECTOR_SHUFFLE(2010, z, x, y, x); EGL_VECTOR_SHUFFLE(2011, z, x, y, y); EGL_VECTOR_SHUFFLE(2012, z, x, y, z); EGL_VECTOR_SHUFFLE(2013, z, x, y, w); EGL_VECTOR_SHUFFLE(2020, z, x, z, x); EGL_VECTOR_SHUFFLE(2021, z, x, z, y); EGL_VECTOR_SHUFFLE(2022, z, x, z, z); EGL_VECTOR_SHUFFLE(2023, z, x, z, w); EGL_VECTOR_SHUFFLE(2030, z, x, w, x); EGL_VECTOR_SHUFFLE(2031, z, x, w, y); EGL_VECTOR_SHUFFLE(2032, z, x, w, z); EGL_VECTOR_SHUFFLE(2033, z, x, w, w);
  EGL_VECTOR_SHUFFLE(2100, z, y, x, x); EGL_VECTOR_SHUFFLE(2101, z, y, x, y); EGL_VECTOR_SHUFFLE(2102, z, y, x, z); EGL_VECTOR_SHUFFLE(2103, z, y, x, w); EGL_VECTOR_SHUFFLE(2110, z, y, y, x); EGL_VECTOR_SHUFFLE(2111, z, y, y, y); EGL_VECTOR_SHUFFLE(2112, z, y, y, z); EGL_VECTOR_SHUFFLE(2113, z, y, y, w); EGL_VECTOR_SHUFFLE(2120, z, y, z, x); EGL_VECTOR_SHUFFLE(2121, z, y, z, y); EGL_VECTOR_SHUFFLE(2122, z, y, z, z); EGL_VECTOR_SHUFFLE(2123, z, y, z, w); EGL_VECTOR_SHUFFLE(2130, z, y, w, x); EGL_VECTOR_SHUFFLE(2131, z, y, w, y); EGL_VECTOR_SHUFFLE(2132, z, y, w, z); EGL_VECTOR_SHUFFLE(2133, z, y, w, w);
  EGL_VECTOR_SHUFFLE(2200, z, z, x, x); EGL_VECTOR_SHUFFLE(2201, z, z, x, y); EGL_VECTOR_SHUFFLE(2202, z, z, x, z); EGL_VECTOR_SHUFFLE(2203, z, z, x, w); EGL_VECTOR_SHUFFLE(2210, z, z, y, x); EGL_VECTOR_SHUFFLE(2211, z, z, y, y); EGL_VECTOR_SHUFFLE(2212, z, z, y, z); EGL_VECTOR_SHUFFLE(2213, z, z, y, w); EGL_VECTOR_SHUFFLE(2220, z, z, z, x); EGL_VECTOR_SHUFFLE(2221, z, z, z, y); EGL_VECTOR_SHUFFLE(2222, z, z, z, z); EGL_VECTOR_SHUFFLE(2223, z, z, z, w); EGL_VECTOR_SHUFFLE(2230, z, z, w, x); EGL_VECTOR_SHUFFLE(2231, z, z, w, y); EGL_VECTOR_SHUFFLE(2232, z, z, w, z); EGL_VECTOR_SHUFFLE(2233, z, z, w, w);
  EGL_VECTOR_SHUFFLE(2300, z, w, x, x); EGL_VECTOR_SHUFFLE(2301, z, w, x, y); EGL_VECTOR_SHUFFLE(2302, z, w, x, z); EGL_VECTOR_SHUFFLE(2303, z, w, x, w); EGL_VECTOR_SHUFFLE(2310, z, w, y, x); EGL_VECTOR_SHUFFLE(2311, z, w, y, y); EGL_VECTOR_SHUFFLE(2312, z, w, y, z); EGL_VECTOR_SHUFFLE(2313, z, w, y, w); EGL_VECTOR_SHUFFLE(2320, z, w, z, x); EGL_VECTOR_SHUFFLE(2321, z, w, z, y); EGL_VECTOR_SHUFFLE(2322, z, w, z, z); EGL_VECTOR_SHUFFLE(2323, z, w, z, w); EGL_VECTOR_SHUFFLE(2330, z, w, w, x); EGL_VECTOR_SHUFFLE(2331, z, w, w, y); EGL_VECTOR_SHUFFLE(2332, z, w, w, z); EGL_VECTOR_SHUFFLE(2333, z, w, w, w);
  EGL_VECTOR_SHUFFLE(3000, w, x, x, x); EGL_VECTOR_SHUFFLE(3001, w, x, x, y); EGL_VECTOR_SHUFFLE(3002, w, x, x, z); EGL_VECTOR_SHUFFLE(3003, w, x, x, w); EGL_VECTOR_SHUFFLE(3010, w, x, y, x); EGL_VECTOR_SHUFFLE(3011, w, x, y, y); EGL_VECTOR_SHUFFLE(3012, w, x, y, z); EGL_VECTOR_SHUFFLE(3013, w, x, y, w); EGL_VECTOR_SHUFFLE(3020, w, x, z, x); EGL_VECTOR_SHUFFLE(3021, w, x, z, y); EGL_VECTOR_SHUFFLE(3022, w, x, z, z); EGL_VECTOR_SHUFFLE(3023, w, x, z, w); EGL_VECTOR_SHUFFLE(3030, w, x, w, x); EGL_VECTOR_SHUFFLE(3031, w, x, w, y); EGL_VECTOR_SHUFFLE(3032, w, x, w, z); EGL_VECTOR_SHUFFLE(3033, w, x, w, w);
  EGL_VECTOR_SHUFFLE(3100, w, y, x, x); EGL_VECTOR_SHUFFLE(3101, w, y, x, y); EGL_VECTOR_SHUFFLE(3102, w, y, x, z); EGL_VECTOR_SHUFFLE(3103, w, y, x, w); EGL_VECTOR_SHUFFLE(3110, w, y, y, x); EGL_VECTOR_SHUFFLE(3111, w, y, y, y); EGL_VECTOR_SHUFFLE(3112, w, y, y, z); EGL_VECTOR_SHUFFLE(3113, w, y, y, w); EGL_VECTOR_SHUFFLE(3120, w, y, z, x); EGL_VECTOR_SHUFFLE(3121, w, y, z, y); EGL_VECTOR_SHUFFLE(3122, w, y, z, z); EGL_VECTOR_SHUFFLE(3123, w, y, z, w); EGL_VECTOR_SHUFFLE(3130, w, y, w, x); EGL_VECTOR_SHUFFLE(3131, w, y, w, y); EGL_VECTOR_SHUFFLE(3132, w, y, w, z); EGL_VECTOR_SHUFFLE(3133, w, y, w, w);
  EGL_VECTOR_SHUFFLE(3200, w, z, x, x); EGL_VECTOR_SHUFFLE(3201, w, z, x, y); EGL_VECTOR_SHUFFLE(3202, w, z, x, z); EGL_VECTOR_SHUFFLE(3203, w, z, x, w); EGL_VECTOR_SHUFFLE(3210, w, z, y, x); EGL_VECTOR_SHUFFLE(3211, w, z, y, y); EGL_VECTOR_SHUFFLE(3212, w, z, y, z); EGL_VECTOR_SHUFFLE(3213, w, z, y, w); EGL_VECTOR_SHUFFLE(3220, w, z, z, x); EGL_VECTOR_SHUFFLE(3221, w, z, z, y); EGL_VECTOR_SHUFFLE(3222, w, z, z, z); EGL_VECTOR_SHUFFLE(3223, w, z, z, w); EGL_VECTOR_SHUFFLE(3230, w, z, w, x); EGL_VECTOR_SHUFFLE(3231, w, z, w, y); EGL_VECTOR_SHUFFLE(3232, w, z, w, z); EGL_VECTOR_SHUFFLE(3233, w, z, w, w);
  EGL_VECTOR_SHUFFLE(3300, w, w, x, x); EGL_VECTOR_SHUFFLE(3301, w, w, x, y); EGL_VECTOR_SHUFFLE(3302, w, w, x, z); EGL_VECTOR_SHUFFLE(3303, w, w, x, w); EGL_VECTOR_SHUFFLE(3310, w, w, y, x); EGL_VECTOR_SHUFFLE(3311, w, w, y, y); EGL_VECTOR_SHUFFLE(3312, w, w, y, z); EGL_VECTOR_SHUFFLE(3313, w, w, y, w); EGL_VECTOR_SHUFFLE(3320, w, w, z, x); EGL_VECTOR_SHUFFLE(3321, w, w, z, y); EGL_VECTOR_SHUFFLE(3322, w, w, z, z); EGL_VECTOR_SHUFFLE(3323, w, w, z, w); EGL_VECTOR_SHUFFLE(3330, w, w, w, x); EGL_VECTOR_SHUFFLE(3331, w, w, w, y); EGL_VECTOR_SHUFFLE(3332, w, w, w, z); EGL_VECTOR_SHUFFLE(3333, w, w, w, w);
  #undef EGL_VECTOR_SHUFFLE
} // namespace priv
//----

template<unsigned shuffle_, typename T>
EGL_INLINE vec4<T> shuffle(const vec4<T> &v_)
{
  return priv::shuffle_vec4_helper<shuffle_, T>::shuffle(v_);
}
//----

template<typename T>
EGL_INLINE void pack_s1(vec4_32s &vr_, const vec4<T> &v_)
{
  // pack 4d vector (components in range [-1, 1]) to 32-bit 4d vector
  EGL_ASSERT_PEDANTIC_MSG(is_ssat(v_), "All vector components [%f, %f, %f, %f] must be in range [-1, 1]\r\n", float(v_.x), float(v_.y), float(v_.z), float(v_.w));
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=int8_t(v_.x*scalar_t(127.999));
  vr_.y=int8_t(v_.y*scalar_t(127.999));
  vr_.z=int8_t(v_.z*scalar_t(127.999));
  vr_.w=int8_t(v_.w*scalar_t(127.999));
}
//----

template<typename T>
EGL_INLINE void pack_u1(vec4_32u &vr_, const vec4<T> &v_)
{
  // pack 4d vector (components in range [0, 1]) to 32-bit 4d vector
  EGL_ASSERT_PEDANTIC_MSG(is_sat(v_), "All vector components [%f, %f, %f, %f] must be in range [0, 1]\r\n", float(v_.x), float(v_.y), float(v_.z), float(v_.w));
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=uint8_t(v_.x*scalar_t(255.999));
  vr_.y=uint8_t(v_.y*scalar_t(255.999));
  vr_.z=uint8_t(v_.z*scalar_t(255.999));
  vr_.w=uint8_t(v_.w*scalar_t(255.999));
}
//----

template<typename T>
EGL_INLINE void unpack_s1(vec4<T> &vr_, const vec4_32s &v_)
{
  // unpack 4d vector (components in range [-1, 1]) from 32-bit 4d vector
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=v_.x*scalar_t(1.0/127.0);
  vr_.y=v_.y*scalar_t(1.0/127.0);
  vr_.z=v_.z*scalar_t(1.0/127.0);
  vr_.w=v_.w*scalar_t(1.0/127.0);
}
//----

template<typename T>
EGL_INLINE void unpack_u1(vec4<T> &vr_, const vec4_32u &v_)
{
  // unpack 4d vector (components in range [0, 1]) from 32-bit 4d vector
  typedef typename math<T>::scalar_t scalar_t;
  vr_.x=v_.x*scalar_t(1.0/255.0);
  vr_.y=v_.y*scalar_t(1.0/255.0);
  vr_.z=v_.z*scalar_t(1.0/255.0);
  vr_.w=v_.w*scalar_t(1.0/255.0);
}
//----------------------------------------------------------------------------


//============================================================================
// mat22
//============================================================================
template<typename T>
mat22<T> mat22<T>::zero()
{
  return mat22<T>(scalar_t(0));
}
//----

template<typename T>
mat22<T> mat22<T>::one()
{
  return mat22<T>(scalar_t(1));
}
//----

template<typename T>
mat22<T> mat22<T>::neg_one()
{
  return mat22<T>(scalar_t(-1));
}
//----

template<typename T>
mat22<T> mat22<T>::identity()
{
  return mat22<T>(scalar_t(1),           0,
                            0, scalar_t(1));
}
//----------------------------------------------------------------------------

template<typename T>
mat22<T>::mat22()
{
}
//----

template<typename T>
mat22<T>::mat22(T s_)
{
  // init matrix with scalar
  x.x=s_; x.y=s_;
  y.x=s_; y.y=s_;
}
//----

template<typename T>
mat22<T>::mat22(T d0_, T d1_)
{
  // init matrix with diagonal scalars
  x.x=d0_; x.y=0;
  y.x=0;   y.y=d1_;
}
//----

template<typename T>
mat22<T>::mat22(T m00_, T m01_,
                T m10_, T m11_)
{
  // init matrix with scalars
  x.x=m00_; x.y=m01_;
  y.x=m10_; y.y=m11_;
}
//----

template<typename T>
mat22<T>::mat22(const vec2<T> &diag_)
{
  // init matrix with diagonal vector
  x.x=diag_.x; x.y=0;
  y.x=0;       y.y=diag_.y;
}
//----

template<typename T>
mat22<T>::mat22(const vec2<T> &row0_,
                const vec2<T> &row1_)
{
  // init matrix with row-vectors
  x.x=row0_.x; x.y=row0_.y;
  y.x=row1_.x; y.y=row1_.y;
}
//----

template<typename T>
mat22<T>::mat22(const mat33<T> &m_)
{
  // init matrix with 3d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y;
  y.x=m_.y.x; y.y=m_.y.y;
}
//----

template<typename T>
mat22<T>::mat22(const T *a_)
{
  // init matrix with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x.x=a_[0]; x.y=a_[1];
  y.x=a_[2]; y.y=a_[3];
}
//----------------------------------------------------------------------------

template<typename T>
const vec2<T> &mat22<T>::operator[](unsigned row_) const
{
  // return reference to nth vector (x=0, y=1)
  EGL_ASSERT_PEDANTIC(row_<2);
  return reinterpret_cast<const vec2<T>*>(this)[row_];
}
//----

template<typename T>
vec2<T> &mat22<T>::operator[](unsigned row_)
{
  // return reference to nth vector (x=0, y=1)
  EGL_ASSERT_PEDANTIC(row_<2);
  return reinterpret_cast<vec2<T>*>(this)[row_];
}
//----

template<typename T>
void mat22<T>::set(T s_)
{
  // set matrix with scalar
  x.x=s_; x.y=s_;
  y.x=s_; y.y=s_;
}
//----

template<typename T>
void mat22<T>::set(T d0_, T d1_)
{
  // set matrix with diagonal scalars
  x.x=d0_; x.y=0;
  y.x=0;   y.y=d1_;
}
//----

template<typename T>
void mat22<T>::set(T m00_, T m01_,
                   T m10_, T m11_)
{
  // set matrix with scalars
  x.x=m00_; x.y=m01_;
  y.x=m10_; y.y=m11_;
}
//----

template<typename T>
void mat22<T>::set(const vec2<T> &row0_,
                   const vec2<T> &row1_)
{
  // set matrix with row-vectors
  x.x=row0_.x; x.y=row0_.y;
  y.x=row1_.x; y.y=row1_.y;
}
//----

template<typename T>
void mat22<T>::set(const mat22<T> &m_)
{
  // set matrix with matrix
  x.x=m_.x.x; x.y=m_.x.y;
  y.x=m_.y.x; y.y=m_.y.y;
}
//----

template<typename T>
void mat22<T>::set(const mat33<T> &m_)
{
  // set matrix with 3d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y;
  y.x=m_.y.x; y.y=m_.y.y;
}
//----

template<typename T>
void mat22<T>::set(const T *a_)
{
  // set matrix with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x.x=a_[0]; x.y=a_[1];
  y.x=a_[2]; y.y=a_[3];
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const mat22<T> &m_)
{
  // test for zero-matrix
  return    m_.x.x==0 && m_.x.y==0
         && m_.y.x==0 && m_.y.y==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const mat22<T> &m_)
{
  // test for saturated matrix
  return is_sat(m_.x) && is_sat(m_.y);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const mat22<T> &m_)
{
  // test for signed saturated matrix
  return is_ssat(m_.x) && is_ssat(m_.y);
}
//----

template<typename T>
EGL_INLINE bool operator==(const mat22<T> &m0_, const mat22<T> &m1_)
{
  // test for equality of matrices
  return    m0_.x.x==m1_.x.x && m0_.x.y==m1_.x.y
         && m0_.y.x==m1_.y.x && m0_.y.y==m1_.y.y;
}
//----

template<typename T>
EGL_INLINE bool operator==(const mat22<T> &m_, typename math<T>::scalar_t s_)
{
  // test for equality of matrix and scalar
  return    m_.x.x==s_ && m_.x.y==s_
         && m_.y.x==s_ && m_.y.y==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const mat22<T> &m_)
{
  // test for equality of matrix and scalar
  return    s_==m_.x.x && s_==m_.x.y
         && s_==m_.y.x && s_==m_.y.y;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const mat22<T> &m0_, const mat22<T> &m1_)
{
  // test for inequality of matrices
  return    m0_.x.x!=m1_.x.x || m0_.x.y!=m1_.x.y
         || m0_.y.x!=m1_.y.x || m0_.y.y!=m1_.y.y;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const mat22<T> &m_, typename math<T>::scalar_t s_)
{
  // test for inequality of matrix and scalar
  return    m_.x.x!=s_ || m_.x.y!=s_
         || m_.y.x!=s_ || m_.y.y!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const mat22<T> &m_)
{
  // test for inequality of matrix and scalar
  return    s_!=m_.x.x || s_!=m_.x.y
         || s_!=m_.y.x || s_!=m_.y.y;
}
//----

template<typename T>
EGL_INLINE void operator+=(mat22<T> &mr_, const mat22<T> &m_)
{
  // add matrix to matrix
  mr_.x.x+=m_.x.x; mr_.x.y+=m_.x.y;
  mr_.y.x+=m_.y.x; mr_.y.y+=m_.y.y;
}
//----

template<typename T>
EGL_INLINE void operator+=(mat22<T> &mr_, typename math<T>::scalar_t s_)
{
  // add matrix to matrix
  mr_.x.x+=s_; mr_.x.y+=s_;
  mr_.y.x+=s_; mr_.y.y+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(mat22<T> &mr_, const mat22<T> &m_)
{
  // subtract matrix from matrix
  mr_.x.x-=m_.x.x; mr_.x.y-=m_.x.y;
  mr_.y.x-=m_.y.x; mr_.y.y-=m_.y.y;
}
//----

template<typename T>
EGL_INLINE void operator-=(mat22<T> &mr_, typename math<T>::scalar_t s_)
{
  // subtract matrix from matrix
  mr_.x.x-=s_; mr_.x.y-=s_;
  mr_.y.x-=s_; mr_.y.y-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat22<T> &mr_, const mat22<T> &m_)
{
  // multiply matrix by matrix
  vec2<T> v=mr_.x;
  mr_.x.x=v.x*m_.x.x+v.y*m_.y.x; mr_.x.y=v.x*m_.x.y+v.y*m_.y.y;
  v=mr_.y;
  mr_.y.x=v.x*m_.x.x+v.y*m_.y.x; mr_.y.y=v.x*m_.x.y+v.y*m_.y.y;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat22<T> &mr_, typename math<T>::scalar_t s_)
{
  // multiply matrix by scalar
  mr_.x.x*=s_; mr_.x.y*=s_;
  mr_.y.x*=s_; mr_.y.y*=s_;
}
//----

template<typename T>
EGL_INLINE void operator/=(mat22<T> &mr_, typename math<T>::scalar_t s_)
{
  // divide matrix by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  mr_.x.x*=rs; mr_.x.y*=rs;
  mr_.y.x*=rs; mr_.y.y*=rs;
}
//----

template<typename T>
EGL_INLINE mat22<T> operator+(const mat22<T> &m0_, const mat22<T> &m1_)
{
  // add matrix to matrix
  return mat22<T>(m0_.x.x+m1_.x.x, m0_.x.y+m1_.x.y,
                  m0_.y.x+m1_.y.x, m0_.y.y+m1_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator+(const mat22<T> &m_, typename math<T>::scalar_t s_)
{
  // add scalar to matrix
  return mat22<T>(m_.x.x+s_, m_.x.y+s_,
                  m_.y.x+s_, m_.y.y+s_);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator+(typename math<T>::scalar_t s_, const mat22<T> &m_)
{
  // add matrix to scalar
  return mat22<T>(s_+m_.x.x, s_+m_.x.y,
                  s_+m_.y.x, s_+m_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator-(const mat22<T> &m0_, const mat22<T> &m1_)
{
  // subtract matrix from matrix
  return mat22<T>(m0_.x.x-m1_.x.x, m0_.x.y-m1_.x.y,
                  m0_.y.x-m1_.y.x, m0_.y.y-m1_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator-(const mat22<T> &m_, typename math<T>::scalar_t s_)
{
  // subtract matrix from matrix
  return mat22<T>(m_.x.x-s_, m_.x.y-s_,
                  m_.y.x-s_, m_.y.y-s_);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator-(typename math<T>::scalar_t s_, const mat22<T> &m_)
{
  // subtract matrix from matrix
  return mat22<T>(s_-m_.x.x, s_-m_.x.y,
                  s_-m_.y.x, s_-m_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator-(const mat22<T> &m_)
{
  // negate matrix
  return mat22<T>(-m_.x.x, -m_.x.y,
                  -m_.y.x, -m_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator*(const mat22<T> &m0_, const mat22<T> &m1_)
{
  // multiply matrix by matrix
  return mat22<T>(m0_.x.x*m1_.x.x+m0_.x.y*m1_.y.x, m0_.x.x*m1_.x.y+m0_.x.y*m1_.y.y,
                  m0_.y.x*m1_.x.x+m0_.y.y*m1_.y.x, m0_.y.x*m1_.x.y+m0_.y.y*m1_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator*(const mat22<T> &m_, typename math<T>::scalar_t s_)
{
  // multiply matrix by scalar
  return mat22<T>(m_.x.x*s_, m_.x.y*s_,
                  m_.y.x*s_, m_.y.y*s_);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator*(typename math<T>::scalar_t s_, const mat22<T> &m_)
{
  // multiply matrix by scalar
  return mat22<T>(s_*m_.x.x, s_*m_.x.y,
                  s_*m_.y.x, s_*m_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> operator/(const mat22<T> &m_, typename math<T>::scalar_t s_)
{
  // divide matrix by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return mat22<T>(m_.x.x*rs, m_.x.y*rs,
                  m_.y.x*rs, m_.y.y*rs);
}
//----

template<typename T>
EGL_INLINE mat22<T> mul(const mat22<T> &m0_, const mat22<T> &m1_)
{
  // component-wise matrix multiply: m0_*m1_
  return mat22<T>(m0_.x.x*m1_.x.x, m0_.x.y*m1_.x.y,
                  m0_.y.x*m1_.y.x, m0_.y.y*m1_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> madd(const mat22<T> &mm0_, const mat22<T> &mm1_, const mat22<T> &ma_)
{
  // component-wise multiply-add: mm0_*mm1_+ma_
  return mat22<T>(mm0_.x.x*mm1_.x.x+ma_.x.x, mm0_.x.y*mm1_.x.y+ma_.x.y,
                  mm0_.y.x*mm1_.y.x+ma_.y.x, mm0_.y.y*mm1_.y.y+ma_.y.y);
}
//----

template<typename T>
EGL_INLINE mat22<T> madd(const mat22<T> &mm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: mm_*sm_+sa_
  return mat22<T>(mm_.x.x*sm_+sa_, mm_.x.y*sm_+sa_,
                  mm_.y.x*sm_+sa_, mm_.y.y*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE mat22<T> madd(const mat22<T> &mm0_, const mat22<T> &mm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: mm0_*mm1_+sa_
  return mat22<T>(mm0_.x.x*mm1_.x.x+sa_, mm0_.x.y*mm1_.x.y+sa_,
                  mm0_.y.x*mm1_.y.x+sa_, mm0_.y.y*mm1_.y.y+sa_);
}
//----

template<typename T>
EGL_INLINE mat22<T> madd(const mat22<T> &mm_, typename math<T>::scalar_t sm_, const mat22<T> &ma_)
{
  // component-wise multiply-add: mm_*sm_+ma_
  return mat22<T>(mm_.x.x*sm_+ma_.x.x, mm_.x.y*sm_+ma_.x.y,
                  mm_.y.x*sm_+ma_.y.x, mm_.y.y*sm_+ma_.y.y);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t det(const mat22<T> &m_)
{
  // determinant of the matrix
  return m_.x.x*m_.y.y-m_.x.y*m_.y.x;
}
//----

template<typename T>
EGL_INLINE mat22<T> inv(const mat22<T> &m_, typename math<T>::scalar_t *det_)
{
  // calculate matrix determinant
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=m_.x.x*m_.y.y-m_.x.y*m_.y.x;
  if(det_)
    *det_=d;
  if(!d)
    return mat22<T>(scalar_t(0));

  // invert the matrix
  scalar_t rd=scalar_t(1)/d;
  return mat22<T>( m_.y.y*rd, -m_.x.y*rd,
                  -m_.y.x*rd,  m_.x.x*rd);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t tr(const mat22<T> &m_)
{
  // trace of the matrix
  return m_.x.x+m_.y.y;
}
//----

template<typename T>
EGL_INLINE void identity(mat22<T> &m_)
{
  // set identity
  typedef typename math<T>::scalar_t scalar_t;
  m_.x.x=scalar_t(1); m_.x.y=0;
  m_.y.x=0;           m_.y.y=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE void transpose(mat22<T> &m_)
{
  // transpose the matrix
  typename math<T>::scalar_t t=m_.y.x;
  m_.y.x=m_.x.y;
  m_.x.y=t;
}
//----

template<typename T>
EGL_INLINE void transpose(mat22<T> &res_, const mat22<T> &m_)
{
  // transpose the matrix
  res_.x.x=m_.x.x; res_.x.y=m_.y.x;
  res_.y.x=m_.x.y; res_.y.y=m_.y.y;
}
//----

template<typename T>
EGL_INLINE void neg(mat22<T> &m_)
{
  // negate the matrix
  m_.x.x=-m_.x.x; m_.x.y=-m_.x.y;
  m_.y.x=-m_.y.x; m_.y.y=-m_.y.y;
}
//----

template<typename T>
EGL_INLINE mat22<T> lerp(const mat22<T> &m0_, const mat22<T> &m1_, typename math<T>::scalar_t t_)
{
  // linear matrix interpolation. f(t=0)=v0, f(t=1)=v1
  return mat22<T>(m0_.x.x+(m1_.x.x-m0_.x.x)*t_, m0_.x.y+(m1_.x.y-m0_.x.y)*t_,
                  m0_.y.x+(m1_.y.x-m0_.y.x)*t_, m0_.y.y+(m1_.y.y-m0_.y.y)*t_);
}
//----

template<typename T>
EGL_INLINE void lerp(mat22<T> &res_, const mat22<T> &m0_, const mat22<T> &m1_, typename math<T>::scalar_t t_)
{
  // linear matrix interpolation. f(t=0)=v0, f(t=1)=v1
  res_.x.x=m0_.x.x+(m1_.x.x-m0_.x.x)*t_; res_.x.y=m0_.x.y+(m1_.x.y-m0_.x.y)*t_;
  res_.y.x=m0_.y.x+(m1_.y.x-m0_.y.x)*t_; res_.y.y=m0_.y.y+(m1_.y.y-m0_.y.y)*t_;
}
//-----

template<typename T, typename U>
EGL_INLINE mat22<T> to_mat22(const mat22<U> &m_)
{
  // convert between mat22 types
  typedef typename math<T>::scalar_t scalar_t;
  return mat22<T>(scalar_t(m_.x.x), scalar_t(m_.x.y),
                  scalar_t(m_.y.x), scalar_t(m_.y.y));
}
//----

template<typename T>
EGL_INLINE void normalize(mat22<T> &m_)
{
  // normalize matrix vectors
  m_.x=unit(m_.x);
  m_.y=unit(m_.y);
}
//----------------------------------------------------------------------------


//============================================================================
// mat33
//============================================================================
template<typename T>
mat33<T> mat33<T>::zero()
{
  return mat33<T>(scalar_t(0));
}
//----

template<typename T>
mat33<T> mat33<T>::one()
{
  return mat33<T>(scalar_t(1));
}
//----

template<typename T>
mat33<T> mat33<T>::neg_one()
{
  return mat33<T>(scalar_t(-1));
}
//----

template<typename T>
mat33<T> mat33<T>::identity()
{
  return mat33<T>(scalar_t(1),           0,           0,
                            0, scalar_t(1),           0,
                            0,           0, scalar_t(1));
}
//----------------------------------------------------------------------------

template<typename T>
mat33<T>::mat33()
{
}
//----

template<typename T>
mat33<T>::mat33(T s_)
{
  // init matrix with scalar
  x.x=s_; x.y=s_; x.z=s_;
  y.x=s_; y.y=s_; y.z=s_;
  z.x=s_; z.y=s_; z.z=s_;
}
//----

template<typename T>
mat33<T>::mat33(T d0_, T d1_, T d2_)
{
  // init matrix with diagonal scalars
  x.x=d0_; x.y=0;   x.z=0;
  y.x=0;   y.y=d1_; y.z=0;
  z.x=0;   z.y=0;   z.z=d2_;
}
//----

template<typename T>
mat33<T>::mat33(T m00_, T m01_, T m02_,
                T m10_, T m11_, T m12_,
                T m20_, T m21_, T m22_)
{
  // init matrix with scalars
  x.x=m00_; x.y=m01_; x.z=m02_;
  y.x=m10_; y.y=m11_; y.z=m12_;
  z.x=m20_; z.y=m21_; z.z=m22_;
}
//----

template<typename T>
mat33<T>::mat33(const vec3<T> &diag_)
{
  // init matrix with row-vectors
  x.x=diag_.x; x.y=0;       x.z=0;
  y.x=0;       y.y=diag_.y; y.z=0;
  z.x=0;       z.y=0;       z.z=diag_.z;
}
//----

template<typename T>
mat33<T>::mat33(const vec3<T> &row0_,
                const vec3<T> &row1_,
                const vec3<T> &row2_)
{
  // init matrix with row-vectors
  x.x=row0_.x; x.y=row0_.y; x.z=row0_.z;
  y.x=row1_.x; y.y=row1_.y; y.z=row1_.z;
  z.x=row2_.x; z.y=row2_.y; z.z=row2_.z;
}
//----

template<typename T>
mat33<T>::mat33(const mat22<T> &m_)
{
  // init matrix with 2d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y; x.z=0;
  y.x=m_.y.x; y.y=m_.y.y; y.z=0;
  z.x=0;      z.y=0;      z.z=T(1);
}
//----

template<typename T>
mat33<T>::mat33(const mat44<T> &m_)
{
  // init matrix with 4d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y; x.z=m_.x.z;
  y.x=m_.y.x; y.y=m_.y.y; y.z=m_.y.z;
  z.x=m_.z.x; z.y=m_.z.y; z.z=m_.z.z;
}
//----

template<typename T>
mat33<T>::mat33(const T *a_)
{
  // init matrix with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x.x=a_[0]; x.y=a_[1]; x.z=a_[2];
  y.x=a_[3]; y.y=a_[4]; y.z=a_[5];
  z.x=a_[6]; z.y=a_[7]; z.z=a_[8];
}
//----------------------------------------------------------------------------

template<typename T>
const vec3<T> &mat33<T>::operator[](unsigned row_) const
{
  // return reference to nth vector (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(row_<3);
  return reinterpret_cast<const vec3<T>*>(this)[row_];
}
//----

template<typename T>
vec3<T> &mat33<T>::operator[](unsigned row_)
{
  // return reference to nth vector (x=0, y=1, z=2)
  EGL_ASSERT_PEDANTIC(row_<3);
  return reinterpret_cast<vec3<T>*>(this)[row_];
}
//----

template<typename T>
void mat33<T>::set(T s_)
{
  // set matrix with scalar
  x.x=s_; x.y=s_; x.z=s_;
  y.x=s_; y.y=s_; y.z=s_;
  z.x=s_; z.y=s_; z.z=s_;
}
//----

template<typename T>
void mat33<T>::set(T d0_, T d1_, T d2_)
{
  // set matrix with diagonal scalars
  x.x=d0_; x.y=0;   x.z=0;
  y.x=0;   y.y=d1_; y.z=0;
  z.x=0;   z.y=0;   z.z=d2_;
}
//----

template<typename T>
void mat33<T>::set(T m00_, T m01_, T m02_,
                   T m10_, T m11_, T m12_,
                   T m20_, T m21_, T m22_)
{
  // set matrix with scalars
  x.x=m00_; x.y=m01_; x.z=m02_;
  y.x=m10_; y.y=m11_; y.z=m12_;
  z.x=m20_; z.y=m21_; z.z=m22_;
}
//----

template<typename T>
void mat33<T>::set(const vec3<T> &row0_,
                   const vec3<T> &row1_,
                   const vec3<T> &row2_)
{
  // set matrix with row-vectors
  x.x=row0_.x; x.y=row0_.y; x.z=row0_.z;
  y.x=row1_.x; y.y=row1_.y; y.z=row1_.z;
  z.x=row2_.x; z.y=row2_.y; z.z=row2_.z;
}
//----

template<typename T>
void mat33<T>::set(const mat22<T> &m_)
{
  // set matrix with 2d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y; x.z=0;
  y.x=m_.y.x; y.y=m_.y.y; y.z=0;
  z.x=0;      z.y=0;      z.z=T(1);
}
//----

template<typename T>
void mat33<T>::set(const mat33<T> &m_)
{
  // set matrix with matrix
  x.x=m_.x.x; x.y=m_.x.y; x.z=m_.x.z;
  y.x=m_.y.x; y.y=m_.y.y; y.z=m_.y.z;
  z.x=m_.z.x; z.y=m_.z.y; z.z=m_.z.z;
}
//----

template<typename T>
void mat33<T>::set(const mat44<T> &m_)
{
  // set matrix with 4d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y; x.z=m_.x.z;
  y.x=m_.y.x; y.y=m_.y.y; y.z=m_.y.z;
  z.x=m_.z.x; z.y=m_.z.y; z.z=m_.z.z;
}
//----

template<typename T>
void mat33<T>::set(const T *a_)
{
  // set matrix with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x.x=a_[0]; x.y=a_[1]; x.z=a_[2];
  y.x=a_[3]; y.y=a_[4]; y.z=a_[5];
  z.x=a_[6]; z.y=a_[7]; z.z=a_[8];
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const mat33<T> &m_)
{
  // test for zero-matrix
  return    m_.x.x==0 && m_.x.y==0 && m_.x.z==0
         && m_.y.x==0 && m_.y.y==0 && m_.y.z==0
         && m_.z.x==0 && m_.z.y==0 && m_.z.z==0;
}
//----

template<typename T>
EGL_INLINE bool is_orthogonal(const mat33<T> &m_)
{
  return    dot(m_.x, m_.y)<0.0001f
         && dot(m_.x, m_.z)<0.0001f
         && dot(m_.y, m_.z)<0.0001f;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const mat33<T> &m_)
{
  // test for saturated matrix
  return is_sat(m_.x) && is_sat(m_.y) && is_sat(m_.z);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const mat33<T> &m_)
{
  // test for signed saturated matrix
  return is_ssat(m_.x) && is_ssat(m_.y) && is_ssat(m_.z);
}
//----

template<typename T>
EGL_INLINE bool operator==(const mat33<T> &m0_, const mat33<T> &m1_)
{
  // test for equality of matrices
  return    m0_.x.x==m1_.x.x && m0_.x.y==m1_.x.y && m0_.x.z==m1_.x.z
         && m0_.y.x==m1_.y.x && m0_.y.y==m1_.y.y && m0_.y.z==m1_.y.z
         && m0_.z.x==m1_.z.x && m0_.z.y==m1_.z.y && m0_.z.z==m1_.z.z;
}
//----

template<typename T>
EGL_INLINE bool operator==(const mat33<T> &m_, typename math<T>::scalar_t s_)
{
  // test for equality of matrix and scalar
  return    m_.x.x==s_ && m_.x.y==s_ && m_.x.z==s_
         && m_.y.x==s_ && m_.y.y==s_ && m_.y.z==s_
         && m_.z.x==s_ && m_.z.y==s_ && m_.z.z==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const mat33<T> &m_)
{
  // test for equality of matrix and scalar
  return    s_==m_.x.x && s_==m_.x.y && s_==m_.x.z
         && s_==m_.y.x && s_==m_.y.y && s_==m_.y.z
         && s_==m_.z.x && s_==m_.z.y && s_==m_.z.z;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const mat33<T> &m0_, const mat33<T> &m1_)
{
  // test for inequality of matrices
  return    m0_.x.x!=m1_.x.x || m0_.x.y!=m1_.x.y || m0_.x.z!=m1_.x.z
         || m0_.y.x!=m1_.y.x || m0_.y.y!=m1_.y.y || m0_.y.z!=m1_.y.z
         || m0_.z.x!=m1_.z.x || m0_.z.y!=m1_.z.y || m0_.z.z!=m1_.z.z;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const mat33<T> &m_, typename math<T>::scalar_t s_)
{
  // test for inequality of matrix and scalar
  return    m_.x.x!=s_ || m_.x.y!=s_ || m_.x.z!=s_
         || m_.y.x!=s_ || m_.y.y!=s_ || m_.y.z!=s_
         || m_.z.x!=s_ || m_.z.y!=s_ || m_.z.z!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const mat33<T> &m_)
{
  // test for inequality of matrix and scalar
  return    s_!=m_.x.x || s_!=m_.x.y || s_!=m_.x.z
         || s_!=m_.y.x || s_!=m_.y.y || s_!=m_.y.z
         || s_!=m_.z.x || s_!=m_.z.y || s_!=m_.z.z;
}
//----

template<typename T>
EGL_INLINE void operator+=(mat33<T> &mr_, const mat33<T> &m_)
{
  // add matrix to matrix
  mr_.x.x+=m_.x.x; mr_.x.y+=m_.x.y; mr_.x.z+=m_.x.z;
  mr_.y.x+=m_.y.x; mr_.y.y+=m_.y.y; mr_.y.z+=m_.y.z;
  mr_.z.x+=m_.z.x; mr_.z.y+=m_.z.y; mr_.z.z+=m_.z.z;
}
//----

template<typename T>
EGL_INLINE void operator+=(mat33<T> &mr_, typename math<T>::scalar_t s_)
{
  // add matrix to matrix
  mr_.x.x+=s_; mr_.x.y+=s_; mr_.x.z+=s_;
  mr_.y.x+=s_; mr_.y.y+=s_; mr_.y.z+=s_;
  mr_.z.x+=s_; mr_.z.y+=s_; mr_.z.z+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(mat33<T> &mr_, const mat33<T> &m_)
{
  // subtract matrix from matrix
  mr_.x.x-=m_.x.x; mr_.x.y-=m_.x.y; mr_.x.z-=m_.x.z;
  mr_.y.x-=m_.y.x; mr_.y.y-=m_.y.y; mr_.y.z-=m_.y.z;
  mr_.z.x-=m_.z.x; mr_.z.y-=m_.z.y; mr_.z.z-=m_.z.z;
}
//----

template<typename T>
EGL_INLINE void operator-=(mat33<T> &mr_, typename math<T>::scalar_t s_)
{
  // subtract matrix from matrix
  mr_.x.x-=s_; mr_.x.y-=s_; mr_.x.z-=s_;
  mr_.y.x-=s_; mr_.y.y-=s_; mr_.y.z-=s_;
  mr_.z.x-=s_; mr_.z.y-=s_; mr_.z.z-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat33<T> &mr_, const mat33<T> &m_)
{
  // multiply matrix by matrix
  vec3<T> v=mr_.x;
  mr_.x.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x; mr_.x.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y; mr_.x.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z;
  v=mr_.y;
  mr_.y.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x; mr_.y.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y; mr_.y.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z;
  v=mr_.z;
  mr_.z.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x; mr_.z.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y; mr_.z.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat33<T> &mr_, const quat<T> &q_)
{
  // multiply matrix by quaternion
  mr_.x*=q_;
  mr_.y*=q_;
  mr_.z*=q_;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat33<T> &mr_, typename math<T>::scalar_t s_)
{
  // multiply matrix by scalar
  mr_.x.x*=s_; mr_.x.y*=s_; mr_.x.z*=s_;
  mr_.y.x*=s_; mr_.y.y*=s_; mr_.y.z*=s_;
  mr_.z.x*=s_; mr_.z.y*=s_; mr_.z.z*=s_;
}
//----

template<typename T>
EGL_INLINE void operator/=(mat33<T> &mr_, typename math<T>::scalar_t s_)
{
  // divide matrix by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  mr_.x.x*=rs; mr_.x.y*=rs; mr_.x.z*=rs;
  mr_.y.x*=rs; mr_.y.y*=rs; mr_.y.z*=rs;
  mr_.z.x*=rs; mr_.z.y*=rs; mr_.z.z*=rs;
}
//----

template<typename T>
EGL_INLINE mat33<T> operator+(const mat33<T> &m0_, const mat33<T> &m1_)
{
  // add matrix to matrix
  return mat33<T>(m0_.x.x+m1_.x.x, m0_.x.y+m1_.x.y, m0_.x.z+m1_.x.z,
                  m0_.y.x+m1_.y.x, m0_.y.y+m1_.y.y, m0_.y.z+m1_.y.z,
                  m0_.z.x+m1_.z.x, m0_.z.y+m1_.z.y, m0_.z.z+m1_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator+(const mat33<T> &m_, typename math<T>::scalar_t s_)
{
  // add matrix to matrix
  return mat33<T>(m_.x.x+s_, m_.x.y+s_, m_.x.z+s_,
                  m_.y.x+s_, m_.y.y+s_, m_.y.z+s_,
                  m_.z.x+s_, m_.z.y+s_, m_.z.z+s_);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator+(typename math<T>::scalar_t s_, const mat33<T> &m_)
{
  // add matrix to matrix
  return mat33<T>(s_+m_.x.x, s_+m_.x.y, s_+m_.x.z,
                  s_+m_.y.x, s_+m_.y.y, s_+m_.y.z,
                  s_+m_.z.x, s_+m_.z.y, s_+m_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator-(const mat33<T> &m0_, const mat33<T> &m1_)
{
  // subtract matrix from matrix
  return mat33<T>(m0_.x.x-m1_.x.x, m0_.x.y-m1_.x.y, m0_.x.z-m1_.x.z,
                  m0_.y.x-m1_.y.x, m0_.y.y-m1_.y.y, m0_.y.z-m1_.y.z,
                  m0_.z.x-m1_.z.x, m0_.z.y-m1_.z.y, m0_.z.z-m1_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator-(const mat33<T> &m_, typename math<T>::scalar_t s_)
{
  // subtract matrix from matrix
  return mat33<T>(m_.x.x-s_, m_.x.y-s_, m_.x.z-s_,
                  m_.y.x-s_, m_.y.y-s_, m_.y.z-s_,
                  m_.z.x-s_, m_.z.y-s_, m_.z.z-s_);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator-(typename math<T>::scalar_t s_, const mat33<T> &m_)
{
  // subtract matrix from matrix
  return mat33<T>(s_-m_.x.x, s_-m_.x.y, s_-m_.x.z,
                  s_-m_.y.x, s_-m_.y.y, s_-m_.y.z,
                  s_-m_.z.x, s_-m_.z.y, s_-m_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator-(const mat33<T> &m_)
{
  // negate matrix
  return mat33<T>(-m_.x.x, -m_.x.y, -m_.x.z,
                  -m_.y.x, -m_.y.y, -m_.y.z,
                  -m_.z.x, -m_.z.y, -m_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator*(const mat33<T> &m0_, const mat33<T> &m1_)
{
  // multiply matrix by matrix
  return mat33<T>(m0_.x.x*m1_.x.x+m0_.x.y*m1_.y.x+m0_.x.z*m1_.z.x, m0_.x.x*m1_.x.y+m0_.x.y*m1_.y.y+m0_.x.z*m1_.z.y, m0_.x.x*m1_.x.z+m0_.x.y*m1_.y.z+m0_.x.z*m1_.z.z,
                  m0_.y.x*m1_.x.x+m0_.y.y*m1_.y.x+m0_.y.z*m1_.z.x, m0_.y.x*m1_.x.y+m0_.y.y*m1_.y.y+m0_.y.z*m1_.z.y, m0_.y.x*m1_.x.z+m0_.y.y*m1_.y.z+m0_.y.z*m1_.z.z,
                  m0_.z.x*m1_.x.x+m0_.z.y*m1_.y.x+m0_.z.z*m1_.z.x, m0_.z.x*m1_.x.y+m0_.z.y*m1_.y.y+m0_.z.z*m1_.z.y, m0_.z.x*m1_.x.z+m0_.z.y*m1_.y.z+m0_.z.z*m1_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator*(const mat33<T> &m_, const quat<T> &q_)
{
  // multiply matrix by quaternion
  mat33<T> m=m_;
  m.x*=q_;
  m.y*=q_;
  m.z*=q_;
  return m;
}
//----

template<typename T>
EGL_INLINE mat33<T> operator*(const quat<T> &q_, const mat33<T> &m_)
{
  // multiply quaternion by matrix
  mat33<T> m;
  convert(m, q_);
  m*=m_;
  return m;
}
//----

template<typename T>
EGL_INLINE mat33<T> operator*(const mat33<T> &m_, typename math<T>::scalar_t s_)
{
  // multiply matrix by scalar
  return mat33<T>(m_.x.x*s_, m_.x.y*s_, m_.x.z*s_,
                  m_.y.x*s_, m_.y.y*s_, m_.y.z*s_,
                  m_.z.x*s_, m_.z.y*s_, m_.z.z*s_);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator*(typename math<T>::scalar_t s_, const mat33<T> &m_)
{
  // multiply matrix by scalar
  return mat33<T>(s_*m_.x.x, s_*m_.x.y, s_*m_.x.z,
                  s_*m_.y.x, s_*m_.y.y, s_*m_.y.z,
                  s_*m_.z.x, s_*m_.z.y, s_*m_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> operator/(const mat33<T> &m_, typename math<T>::scalar_t s_)
{
  // divide matrix by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return mat33<T>(m_.x.x*rs, m_.x.y*rs, m_.x.z*rs,
                  m_.y.x*rs, m_.y.y*rs, m_.y.z*rs,
                  m_.z.x*rs, m_.z.y*rs, m_.z.z*rs);
}
//----

template<typename T>
EGL_INLINE mat33<T> mul(const mat33<T> &m0_, const mat33<T> &m1_)
{
  // component-wise multiply: m0_*m1
  return mat33<T>(m0_.x.x*m1_.x.x, m0_.x.y*m1_.x.y, m0_.x.z*m1_.x.z,
                  m0_.y.x*m1_.y.x, m0_.y.y*m1_.y.y, m0_.y.z*m1_.y.z,
                  m0_.z.x*m1_.z.x, m0_.z.y*m1_.z.y, m0_.z.z*m1_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> madd(const mat33<T> &mm0_, const mat33<T> &mm1_, const mat33<T> &ma_)
{
  // component-wise multiply-add: mm0_*mm1_+ma_
  return mat33<T>(mm0_.x.x*mm1_.x.x+ma_.x.x, mm0_.x.y*mm1_.x.y+ma_.x.y, mm0_.x.z*mm1_.x.z+ma_.x.z,
                  mm0_.y.x*mm1_.y.x+ma_.y.x, mm0_.y.y*mm1_.y.y+ma_.y.y, mm0_.y.z*mm1_.y.z+ma_.y.z,
                  mm0_.z.x*mm1_.z.x+ma_.z.x, mm0_.z.y*mm1_.z.y+ma_.z.y, mm0_.z.z*mm1_.z.z+ma_.z.z);
}
//----

template<typename T>
EGL_INLINE mat33<T> madd(const mat33<T> &mm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: mm_*sm_+sa_
  return mat33<T>(mm_.x.x*sm_+sa_, mm_.x.y*sm_+sa_, mm_.x.z*sm_+sa_,
                  mm_.y.x*sm_+sa_, mm_.y.y*sm_+sa_, mm_.y.z*sm_+sa_,
                  mm_.z.x*sm_+sa_, mm_.z.y*sm_+sa_, mm_.z.z*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE mat33<T> madd(const mat33<T> &mm0_, const mat33<T> &mm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: mm0_*mm1_+sa_
  return mat33<T>(mm0_.x.x*mm1_.x.x+sa_, mm0_.x.y*mm1_.x.y+sa_, mm0_.x.z*mm1_.x.z+sa_,
                  mm0_.y.x*mm1_.y.x+sa_, mm0_.y.y*mm1_.y.y+sa_, mm0_.y.z*mm1_.y.z+sa_,
                  mm0_.z.x*mm1_.z.x+sa_, mm0_.z.y*mm1_.z.y+sa_, mm0_.z.z*mm1_.z.z+sa_);
}
//----

template<typename T>
EGL_INLINE mat33<T> madd(const mat33<T> &mm_, typename math<T>::scalar_t sm_, mat33<T> &ma_)
{
  // component-wise multiply-add: mm_*sm_+ma_
  return mat33<T>(mm_.x.x*sm_+ma_.x.x, mm_.x.y*sm_+ma_.x.y, mm_.x.z*sm_+ma_.x.z,
                  mm_.y.x*sm_+ma_.y.x, mm_.y.y*sm_+ma_.y.y, mm_.y.z*sm_+ma_.y.z,
                  mm_.z.x*sm_+ma_.z.x, mm_.z.y*sm_+ma_.z.y, mm_.z.z*sm_+ma_.z.z);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t det(const mat33<T> &m_)
{
  // determinant of the matrix
  return  m_.x.x*(m_.y.y*m_.z.z-m_.y.z*m_.z.y)
         +m_.y.x*(m_.z.y*m_.x.z-m_.z.z*m_.x.y)
         +m_.z.x*(m_.x.y*m_.y.z-m_.x.z*m_.y.y);
}
//----

template<typename T>
EGL_INLINE mat33<T> inv(const mat33<T> &m_, typename math<T>::scalar_t *det_)
{
  // calculate determinant
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t dx=m_.y.y*m_.z.z-m_.y.z*m_.z.y;
  scalar_t dy=m_.z.y*m_.x.z-m_.z.z*m_.x.y;
  scalar_t dz=m_.x.y*m_.y.z-m_.x.z*m_.y.y;
  scalar_t d=m_.x.x*dx+m_.y.x*dy+m_.z.x*dz;
  if(det_)
    *det_=d;
  if(!d)
    return mat33<T>(scalar_t(0));

  // calculate inverse of the matrix
  scalar_t rd=scalar_t(1)/d;
  return mat33<T>(                           rd*dx,                            rd*dy,                            rd*dz,
                  rd*(m_.y.z*m_.z.x-m_.y.x*m_.z.z), rd*(m_.z.z*m_.x.x-m_.z.x*m_.x.z), rd*(m_.x.z*m_.y.x-m_.x.x*m_.y.z),
                  rd*(m_.y.x*m_.z.y-m_.y.y*m_.z.x), rd*(m_.z.x*m_.x.y-m_.z.y*m_.x.x), rd*(m_.x.x*m_.y.y-m_.x.y*m_.y.x));
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t tr(const mat33<T> &m_)
{
  // trace of the matrix
  return m_.x.x+m_.y.y+m_.z.z;
}
//----

template<typename T>
EGL_INLINE void identity(mat33<T> &m_)
{
  // set identity
  typedef typename math<T>::scalar_t scalar_t;
  m_.x.x=scalar_t(1); m_.x.y=0;           m_.x.z=0;
  m_.y.x=0;           m_.y.y=scalar_t(1); m_.y.z=0;
  m_.z.x=0;           m_.z.y=0;           m_.z.z=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE void transpose(mat33<T> &m_)
{
  // transpose the matrix
  typename math<T>::scalar_t t=m_.x.y;
  m_.x.y=m_.y.x;
  m_.y.x=t;
  t=m_.x.z;
  m_.x.z=m_.z.x;
  m_.z.x=t;
  t=m_.y.z;
  m_.y.z=m_.z.y;
  m_.z.y=t;
}
//----

template<typename T>
EGL_INLINE void transpose(mat33<T> &res_, const mat33<T> &m_)
{
  // transpose the matrix
  res_.x.x=m_.x.x; res_.x.y=m_.y.x; res_.x.z=m_.z.x;
  res_.y.x=m_.x.y; res_.y.y=m_.y.y; res_.y.z=m_.z.y;
  res_.z.x=m_.x.z; res_.z.y=m_.y.z; res_.z.z=m_.z.z;
}
//----

template<typename T>
EGL_INLINE void neg(mat33<T> &m_)
{
  // negate the matrix
  m_.x.x=-m_.x.x; m_.x.y=-m_.x.y; m_.x.z=-m_.x.z;
  m_.y.x=-m_.y.x; m_.y.y=-m_.y.y; m_.y.z=-m_.y.z;
  m_.z.x=-m_.z.x; m_.z.y=-m_.z.y; m_.z.z=-m_.z.z;
}
//----

template<typename T>
EGL_INLINE mat33<T> lerp(const mat33<T> &m0_, const mat33<T> &m1_, typename math<T>::scalar_t t_)
{
  // linear matrix interpolation. f(t=0)=v0, f(t=1)=v1
  return mat33<T>(m0_.x.x+(m1_.x.x-m0_.x.x)*t_, m0_.x.y+(m1_.x.y-m0_.x.y)*t_, m0_.x.z+(m1_.x.z-m0_.x.z)*t_,
                  m0_.y.x+(m1_.y.x-m0_.y.x)*t_, m0_.y.y+(m1_.y.y-m0_.y.y)*t_, m0_.y.z+(m1_.y.z-m0_.y.z)*t_,
                  m0_.z.x+(m1_.z.x-m0_.z.x)*t_, m0_.z.y+(m1_.z.y-m0_.z.y)*t_, m0_.z.z+(m1_.z.z-m0_.z.z)*t_);
}
//----

template<typename T>
EGL_INLINE void lerp(mat33<T> &res_, const mat33<T> &m0_, const mat33<T> &m1_, typename math<T>::scalar_t t_)
{
  // linear matrix interpolation. f(t=0)=v0, f(t=1)=v1
  res_.x.x=m0_.x.x+(m1_.x.x-m0_.x.x)*t_; res_.x.y=m0_.x.y+(m1_.x.y-m0_.x.y)*t_; res_.x.z=m0_.x.z+(m1_.x.z-m0_.x.z)*t_;
  res_.y.x=m0_.y.x+(m1_.y.x-m0_.y.x)*t_; res_.y.y=m0_.y.y+(m1_.y.y-m0_.y.y)*t_; res_.y.z=m0_.y.z+(m1_.y.z-m0_.y.z)*t_;
  res_.z.x=m0_.z.x+(m1_.z.x-m0_.z.x)*t_; res_.z.y=m0_.z.y+(m1_.z.y-m0_.z.y)*t_; res_.z.z=m0_.z.z+(m1_.z.z-m0_.z.z)*t_;
}
//----

template<typename T, typename U>
EGL_INLINE mat33<T> to_mat33(const mat33<U> &m_)
{
  // convert between mat33 types
  typedef typename math<T>::scalar_t scalar_t;
  return mat33<T>(scalar_t(m_.x.x), scalar_t(m_.x.y), scalar_t(m_.x.z),
                  scalar_t(m_.y.x), scalar_t(m_.y.y), scalar_t(m_.y.z),
                  scalar_t(m_.z.x), scalar_t(m_.z.y), scalar_t(m_.z.z));
}
//----

template<typename T>
EGL_INLINE void normalize(mat33<T> &m_)
{
  // normalize matrix vectors
  m_.x=unit(m_.x);
  m_.y=unit(m_.y);
  m_.z=unit(m_.z);
}
//----

template<typename T>
EGL_INLINE void orthonormalize(mat33<T> &res_)
{
  // ortho-normalize the matrix
  res_.x=unit(res_.x);
  res_.y=unit(cross(res_.z, res_.x));
  res_.z=cross(res_.x, res_.y);
}
//----

template<typename T> 
EGL_INLINE void set_rotation_xyz(mat33<T> &res_, typename math<T>::scalar_t x_, typename math<T>::scalar_t y_, typename math<T>::scalar_t z_)
{
  // set rotation matrix with angles in order Rx*Ry*Rz
  typename math<T>::scalar_t sx, cx, sy, cy, sz, cz;
  sincos(sx, cx, x_);
  sincos(sy, cy, y_);
  sincos(sz, cz, z_);
  res_.set(         cy*cz,          cy*sz,   -sy,
           sx*sy*cz-cx*sz, sx*sy*sz+cx*cz, sx*cy,
           cx*sy*cz+sx*sz, cx*sy*sz-sx*cz, cx*cy);
}
//----

template<typename T> 
EGL_INLINE void set_rotation_zyx(mat33<T> &res_, typename math<T>::scalar_t x_, typename math<T>::scalar_t y_, typename math<T>::scalar_t z_)
{
  // set rotation matrix with angles in order Rz*Ry*Rx
  typename math<T>::scalar_t sx, cx, sy, cy, sz, cz;
  sincos(sx, cx, x_);
  sincos(sy, cy, y_);
  sincos(sz, cz, z_);
  res_.set( cz*cy, sz*cx+cz*sy*sx, sz*sx-cz*sy*cx,
           -sz*cy, cz*cx-sz*sy*sx, cz*sx+sz*sy*cx,
               sy,         -cy*sx, cy*cx);
}
//----

template<typename T> 
EGL_INLINE void set_rotation_zxz(mat33<T> &res_, typename math<T>::scalar_t x_, typename math<T>::scalar_t z_)
{
  // set rotation matrix with angles in order Rz*Rx*Rz (Euler)
  typename math<T>::scalar_t sx, cx, sz, cz;
  sincos(sx, cx, x_);
  sincos(sz, cz, z_);
  typename math<T>::scalar_t sz2=sz*sz, cz2=cz*cz, czsz=cz*sz, czszcx=czsz*cx, szsx=sz*sx, sxcz=sx*cz;
  res_.set(   cz2-sz2*cx, czsz+czszcx, szsx,
            -czsz-czszcx, -sz2+cz2*cx, sxcz,
                    szsx,       -sxcz, cx);
}
//----------------------------------------------------------------------------


//============================================================================
// mat44
//============================================================================
template<typename T>
mat44<T> mat44<T>::zero()
{
  return mat44<T>(scalar_t(0));
}
//----

template<typename T>
mat44<T> mat44<T>::one()
{
  return mat44<T>(scalar_t(1));
}
//----

template<typename T>
mat44<T> mat44<T>::neg_one()
{
  return mat44<T>(scalar_t(-1));
}
//----

template<typename T>
mat44<T> mat44<T>::identity()
{
  return mat44<T>(scalar_t(1),           0,           0,           0,
                            0, scalar_t(1),           0,           0,
                            0,           0, scalar_t(1),           0,
                            0,           0,           0, scalar_t(1));
}
//----------------------------------------------------------------------------

template<typename T>
mat44<T>::mat44()
{
}
//----

template<typename T>
mat44<T>::mat44(T s_)
{
  // init matrix with scalar
  x.x=s_; x.y=s_; x.z=s_; x.w=s_;
  y.x=s_; y.y=s_; y.z=s_; y.w=s_;
  z.x=s_; z.y=s_; z.z=s_; z.w=s_;
  w.x=s_; w.y=s_; w.z=s_; w.w=s_;
}
//----

template<typename T>
mat44<T>::mat44(T d0_, T d1_, T d2_, T d3_)
{
  // init matrix with diagonal scalars
  x.x=d0_; x.y=0;   x.z=0;   x.w=0;
  y.x=0;   y.y=d1_; y.z=0;   y.w=0;
  z.x=0;   z.y=0;   z.z=d2_; z.w=0;
  w.x=0;   w.y=0;   w.z=0;   w.w=d3_;
}
//----

template<typename T>
mat44<T>::mat44(T m00_, T m01_, T m02_, T m03_,
                T m10_, T m11_, T m12_, T m13_,
                T m20_, T m21_, T m22_, T m23_,
                T m30_, T m31_, T m32_, T m33_)
{
  // init matrix with scalars
  x.x=m00_; x.y=m01_; x.z=m02_; x.w=m03_;
  y.x=m10_; y.y=m11_; y.z=m12_; y.w=m13_;
  z.x=m20_; z.y=m21_; z.z=m22_; z.w=m23_;
  w.x=m30_; w.y=m31_; w.z=m32_; w.w=m33_;
}
//----

template<typename T>
mat44<T>::mat44(const vec4<T> &diag_)
{
  // init matrix with row-vectors
  x.x=diag_.x; x.y=0;       x.z=0;       x.w=0;
  y.x=0;       y.y=diag_.y; y.z=0;       y.w=0;
  z.x=0;       z.y=0;       z.z=diag_.z; z.w=0;
  w.x=0;       w.y=0;       w.z=0;       w.w=diag_.w;
}
//----

template<typename T>
mat44<T>::mat44(const vec4<T> &row0_,
                const vec4<T> &row1_,
                const vec4<T> &row2_,
                const vec4<T> &row3_)
{
  // init matrix with row-vectors
  x.x=row0_.x; x.y=row0_.y; x.z=row0_.z; x.w=row0_.w;
  y.x=row1_.x; y.y=row1_.y; y.z=row1_.z; y.w=row1_.w;
  z.x=row2_.x; z.y=row2_.y; z.z=row2_.z; z.w=row2_.w;
  w.x=row3_.x; w.y=row3_.y; w.z=row3_.z; w.w=row3_.w;
}
//----

template<typename T>
mat44<T>::mat44(const mat33<T> &m_)
{
  // init matrix with 3d matrix (upper-left matrix)
  x.x=m_.x.x; x.y=m_.x.y; x.z=m_.x.z; x.w=0;
  y.x=m_.y.x; y.y=m_.y.y; y.z=m_.y.z; y.w=0;
  z.x=m_.z.x; z.y=m_.z.y; z.z=m_.z.z; z.w=0;
  w.x=0;      w.y=0;      w.z=0;      w.w=T(1);
}
//----

template<typename T>
mat44<T>::mat44(const T *a_)
{
  // init matrix with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x.x=a_[ 0]; x.y=a_[ 1]; x.z=a_[ 2]; x.w=a_[ 3];
  y.x=a_[ 4]; y.y=a_[ 5]; y.z=a_[ 6]; y.w=a_[ 7];
  z.x=a_[ 8]; z.y=a_[ 9]; z.z=a_[10]; z.w=a_[11];
  w.x=a_[12]; w.y=a_[13]; w.z=a_[14]; w.w=a_[15];
}
//----------------------------------------------------------------------------

template<typename T>
const vec4<T> &mat44<T>::operator[](unsigned row_) const
{
  // return reference to nth vector (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(row_<4);
  return reinterpret_cast<const vec4<T>*>(this)[row_];
}
//----

template<typename T>
vec4<T> &mat44<T>::operator[](unsigned row_)
{
  // return reference to nth vector (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(row_<4);
  return reinterpret_cast<vec4<T>*>(this)[row_];
}
//----

template<typename T>
void mat44<T>::set(T s_)
{
  // set matrix with scalar
  x.x=s_; x.y=s_; x.z=s_; x.w=s_;
  y.x=s_; y.y=s_; y.z=s_; y.w=s_;
  z.x=s_; z.y=s_; z.z=s_; z.w=s_;
  w.x=s_; w.y=s_; w.z=s_; w.w=s_;
}
//----

template<typename T>
void mat44<T>::set(T d0_, T d1_, T d2_, T d3_)
{
  // set matrix with diagonal scalars
  x.x=d0_; x.y=0;   x.z=0;   x.w=0;
  y.x=0;   y.y=d1_; y.z=0;   y.w=0;
  z.x=0;   z.y=0;   z.z=d2_; z.w=0;
  w.x=0;   w.y=0;   w.z=0;   w.w=d3_;
}
//----

template<typename T>
void mat44<T>::set(T m00_, T m01_, T m02_, T m03_,
                   T m10_, T m11_, T m12_, T m13_,
                   T m20_, T m21_, T m22_, T m23_,
                   T m30_, T m31_, T m32_, T m33_)
{
  // set matrix with scalars
  x.x=m00_; x.y=m01_; x.z=m02_; x.w=m03_;
  y.x=m10_; y.y=m11_; y.z=m12_; y.w=m13_;
  z.x=m20_; z.y=m21_; z.z=m22_; z.w=m23_;
  w.x=m30_; w.y=m31_; w.z=m32_; w.w=m33_;
}
//----

template<typename T>
void mat44<T>::set(const vec4<T> &row0_,
                   const vec4<T> &row1_,
                   const vec4<T> &row2_,
                   const vec4<T> &row3_)
{
  // set matrix with row-vectors
  x.x=row0_.x; x.y=row0_.y; x.z=row0_.z; x.w=row0_.w;
  y.x=row1_.x; y.y=row1_.y; y.z=row1_.z; y.w=row1_.w;
  z.x=row2_.x; z.y=row2_.y; z.z=row2_.z; z.w=row2_.w;
  w.x=row3_.x; w.y=row3_.y; w.z=row3_.z; w.w=row3_.w;
}
//----

template<typename T>
void mat44<T>::set(const mat33<T> &m_)
{
  // set matrix with one lower dimensional matrix
  x.x=m_.x.x; x.y=m_.x.y; x.z=m_.x.z; x.w=0;
  y.x=m_.y.x; y.y=m_.y.y; y.z=m_.y.z; y.w=0;
  z.x=m_.z.x; z.y=m_.z.y; z.z=m_.z.z; z.w=0;
  w.x=0;      w.y=0;      w.z=0;      w.w=T(1);
}
//----

template<typename T>
void mat44<T>::set(const mat44<T> &m_)
{
  // set matrix with one lower dimensional matrix
  x.x=m_.x.x; x.y=m_.x.y; x.z=m_.x.z; x.w=m_.x.w;
  y.x=m_.y.x; y.y=m_.y.y; y.z=m_.y.z; y.w=m_.y.w;
  z.x=m_.z.x; z.y=m_.z.y; z.z=m_.z.z; z.w=m_.z.w;
  w.x=m_.w.x; w.y=m_.w.y; w.z=m_.w.z; w.w=m_.w.w;
}
//----

template<typename T>
void mat44<T>::set(const T *a_)
{
  // set matrix with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x.x=a_[ 0]; x.y=a_[ 1]; x.z=a_[ 2]; x.w=a_[ 3];
  y.x=a_[ 4]; y.y=a_[ 5]; y.z=a_[ 6]; y.w=a_[ 7];
  z.x=a_[ 8]; z.y=a_[ 9]; z.z=a_[10]; z.w=a_[11];
  w.x=a_[12]; w.y=a_[13]; w.z=a_[14]; w.w=a_[15];
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const mat44<T> &m_)
{
  // test for zero-matrix
  return    m_.x.x==0 && m_.x.y==0 && m_.x.z==0 && m_.x.w==0
         && m_.y.x==0 && m_.y.y==0 && m_.y.z==0 && m_.y.w==0
         && m_.z.x==0 && m_.z.y==0 && m_.z.z==0 && m_.z.w==0
         && m_.w.x==0 && m_.w.y==0 && m_.w.z==0 && m_.w.w==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const mat44<T> &m_)
{
  // test for saturated matrix
  return is_sat(m_.x) && is_sat(m_.y) && is_sat(m_.z) && is_sat(m_.w);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const mat44<T> &m_)
{
  // test for signed saturated matrix
  return is_ssat(m_.x) && is_ssat(m_.y) && is_ssat(m_.z) && is_ssat(m_.w);
}
//----

template<typename T>
EGL_INLINE bool operator==(const mat44<T> &m0_, const mat44<T> &m1_)
{
  // test for equality of matrices
  return    m0_.x.x==m1_.x.x && m0_.x.y==m1_.x.y && m0_.x.z==m1_.x.z && m0_.x.w==m1_.x.w
         && m0_.y.x==m1_.y.x && m0_.y.y==m1_.y.y && m0_.y.z==m1_.y.z && m0_.y.w==m1_.y.w
         && m0_.z.x==m1_.z.x && m0_.z.y==m1_.z.y && m0_.z.z==m1_.z.z && m0_.z.w==m1_.z.w
         && m0_.w.x==m1_.w.x && m0_.w.y==m1_.w.y && m0_.w.z==m1_.w.z && m0_.w.w==m1_.w.w;
}
//----

template<typename T>
EGL_INLINE bool operator==(const mat44<T> &m_, typename math<T>::scalar_t s_)
{
  // test for equality of matrix and scalar
  return    m_.x.x==s_ && m_.x.y==s_ && m_.x.z==s_ && m_.x.w==s_
         && m_.y.x==s_ && m_.y.y==s_ && m_.y.z==s_ && m_.y.w==s_
         && m_.z.x==s_ && m_.z.y==s_ && m_.z.z==s_ && m_.z.w==s_
         && m_.w.x==s_ && m_.w.y==s_ && m_.w.z==s_ && m_.w.w==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const mat44<T> &m_)
{
  // test for equality of matrix and scalar
  return    s_==m_.x.x && s_==m_.x.y && s_==m_.x.z && s_==m_.x.w
         && s_==m_.y.x && s_==m_.y.y && s_==m_.y.z && s_==m_.y.w
         && s_==m_.z.x && s_==m_.z.y && s_==m_.z.z && s_==m_.z.w
         && s_==m_.w.x && s_==m_.w.y && s_==m_.w.z && s_==m_.w.w;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const mat44<T> &m0_, const mat44<T> &m1_)
{
  // test for inequality of matrices
  return    m0_.x.x!=m1_.x.x || m0_.x.y!=m1_.x.y || m0_.x.z!=m1_.x.z || m0_.x.w!=m1_.x.w
         || m0_.y.x!=m1_.y.x || m0_.y.y!=m1_.y.y || m0_.y.z!=m1_.y.z || m0_.y.w!=m1_.y.w
         || m0_.z.x!=m1_.z.x || m0_.z.y!=m1_.z.y || m0_.z.z!=m1_.z.z || m0_.z.w!=m1_.z.w
         || m0_.w.x!=m1_.w.x || m0_.w.y!=m1_.w.y || m0_.w.z!=m1_.w.z || m0_.w.w!=m1_.w.w;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const mat44<T> &m_, typename math<T>::scalar_t s_)
{
  // test for inequality of matrix and scalar
  return    m_.x.x!=s_ || m_.x.y!=s_ || m_.x.z!=s_ || m_.x.w!=s_
         || m_.y.x!=s_ || m_.y.y!=s_ || m_.y.z!=s_ || m_.y.w!=s_
         || m_.z.x!=s_ || m_.z.y!=s_ || m_.z.z!=s_ || m_.z.w!=s_
         || m_.w.x!=s_ || m_.w.y!=s_ || m_.w.z!=s_ || m_.w.w!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const mat44<T> &m_)
{
  // test for inequality of matrix and scalar
  return    s_!=m_.x.x || s_!=m_.x.y || s_!=m_.x.z || s_!=m_.x.w
         || s_!=m_.y.x || s_!=m_.y.y || s_!=m_.y.z || s_!=m_.y.w
         || s_!=m_.z.x || s_!=m_.z.y || s_!=m_.z.z || s_!=m_.z.w
         || s_!=m_.w.x || s_!=m_.w.y || s_!=m_.w.z || s_!=m_.w.w;
}
//----

template<typename T>
EGL_INLINE void operator+=(mat44<T> &mr_, const mat44<T> &m_)
{
  // add matrix to matrix
  mr_.x.x+=m_.x.x; mr_.x.y+=m_.x.y; mr_.x.z+=m_.x.z; mr_.x.w+=m_.x.w;
  mr_.y.x+=m_.y.x; mr_.y.y+=m_.y.y; mr_.y.z+=m_.y.z; mr_.y.w+=m_.y.w;
  mr_.z.x+=m_.z.x; mr_.z.y+=m_.z.y; mr_.z.z+=m_.z.z; mr_.z.w+=m_.z.w;
  mr_.w.x+=m_.w.x; mr_.w.y+=m_.w.y; mr_.w.z+=m_.w.z; mr_.w.w+=m_.w.w;
}
//----

template<typename T>
EGL_INLINE void operator+=(mat44<T> &mr_, typename math<T>::scalar_t s_)
{
  // add matrix to matrix
  mr_.x.x+=s_; mr_.x.y+=s_; mr_.x.z+=s_; mr_.x.w+=s_;
  mr_.y.x+=s_; mr_.y.y+=s_; mr_.y.z+=s_; mr_.y.w+=s_;
  mr_.z.x+=s_; mr_.z.y+=s_; mr_.z.z+=s_; mr_.z.w+=s_;
  mr_.w.x+=s_; mr_.w.y+=s_; mr_.w.z+=s_; mr_.w.w+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(mat44<T> &mr_, const mat44<T> &m_)
{
  // subtract matrix from matrix
  mr_.x.x-=m_.x.x; mr_.x.y-=m_.x.y; mr_.x.z-=m_.x.z; mr_.x.w-=m_.x.w;
  mr_.y.x-=m_.y.x; mr_.y.y-=m_.y.y; mr_.y.z-=m_.y.z; mr_.y.w-=m_.y.w;
  mr_.z.x-=m_.z.x; mr_.z.y-=m_.z.y; mr_.z.z-=m_.z.z; mr_.z.w-=m_.z.w;
  mr_.w.x-=m_.w.x; mr_.w.y-=m_.w.y; mr_.w.z-=m_.w.z; mr_.w.w-=m_.w.w;
}
//----

template<typename T>
EGL_INLINE void operator-=(mat44<T> &mr_, typename math<T>::scalar_t s_)
{
  // subtract matrix from matrix
  mr_.x.x-=s_; mr_.x.y-=s_; mr_.x.z-=s_; mr_.x.w-=s_;
  mr_.y.x-=s_; mr_.y.y-=s_; mr_.y.z-=s_; mr_.y.w-=s_;
  mr_.z.x-=s_; mr_.z.y-=s_; mr_.z.z-=s_; mr_.z.w-=s_;
  mr_.w.x-=s_; mr_.w.y-=s_; mr_.w.z-=s_; mr_.w.w-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat44<T> &mr_, const mat44<T> &m_)
{
  // multiply matrix by matrix
  vec4<T> v=mr_.x;
  mr_.x.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x+v.w*m_.w.x; mr_.x.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y+v.w*m_.w.y; mr_.x.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z+v.w*m_.w.z; mr_.x.w=v.x*m_.x.w+v.y*m_.y.w+v.z*m_.z.w+v.w*m_.w.w;
  v=mr_.y;
  mr_.y.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x+v.w*m_.w.x; mr_.y.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y+v.w*m_.w.y; mr_.y.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z+v.w*m_.w.z; mr_.y.w=v.x*m_.x.w+v.y*m_.y.w+v.z*m_.z.w+v.w*m_.w.w;
  v=mr_.z;
  mr_.z.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x+v.w*m_.w.x; mr_.z.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y+v.w*m_.w.y; mr_.z.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z+v.w*m_.w.z; mr_.z.w=v.x*m_.x.w+v.y*m_.y.w+v.z*m_.z.w+v.w*m_.w.w;
  v=mr_.w;
  mr_.w.x=v.x*m_.x.x+v.y*m_.y.x+v.z*m_.z.x+v.w*m_.w.x; mr_.w.y=v.x*m_.x.y+v.y*m_.y.y+v.z*m_.z.y+v.w*m_.w.y; mr_.w.z=v.x*m_.x.z+v.y*m_.y.z+v.z*m_.z.z+v.w*m_.w.z; mr_.w.w=v.x*m_.x.w+v.y*m_.y.w+v.z*m_.z.w+v.w*m_.w.w;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat44<T> &mr_, typename math<T>::scalar_t s_)
{
  // multiply matrix by scalar
  mr_.x.x*=s_; mr_.x.y*=s_; mr_.x.z*=s_; mr_.x.w*=s_;
  mr_.y.x*=s_; mr_.y.y*=s_; mr_.y.z*=s_; mr_.y.w*=s_;
  mr_.z.x*=s_; mr_.z.y*=s_; mr_.z.z*=s_; mr_.z.w*=s_;
  mr_.w.x*=s_; mr_.w.y*=s_; mr_.w.z*=s_; mr_.w.w*=s_;
}
//----

template<typename T>
EGL_INLINE void operator/=(mat44<T> &mr_, typename math<T>::scalar_t s_)
{
  // divide matrix by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  mr_.x.x*=rs; mr_.x.y*=rs; mr_.x.z*=rs; mr_.x.w*=rs;
  mr_.y.x*=rs; mr_.y.y*=rs; mr_.y.z*=rs; mr_.y.w*=rs;
  mr_.z.x*=rs; mr_.z.y*=rs; mr_.z.z*=rs; mr_.z.w*=rs;
  mr_.w.x*=rs; mr_.w.y*=rs; mr_.w.z*=rs; mr_.w.w*=rs;
}
//----

template<typename T>
EGL_INLINE mat44<T> operator+(const mat44<T> &m0_, const mat44<T> &m1_)
{
  // add matrix to matrix
  return mat44<T>(m0_.x.x+m1_.x.x, m0_.x.y+m1_.x.y, m0_.x.z+m1_.x.z, m0_.x.w+m1_.x.w,
                  m0_.y.x+m1_.y.x, m0_.y.y+m1_.y.y, m0_.y.z+m1_.y.z, m0_.y.w+m1_.y.w,
                  m0_.z.x+m1_.z.x, m0_.z.y+m1_.z.y, m0_.z.z+m1_.z.z, m0_.z.w+m1_.z.w,
                  m0_.w.x+m1_.w.x, m0_.w.y+m1_.w.y, m0_.w.z+m1_.w.z, m0_.w.w+m1_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator+(const mat44<T> &m_, typename math<T>::scalar_t s_)
{
  // add matrix to matrix
  return mat44<T>(m_.x.x+s_, m_.x.y+s_, m_.x.z+s_, m_.x.w+s_,
                  m_.y.x+s_, m_.y.y+s_, m_.y.z+s_, m_.y.w+s_,
                  m_.z.x+s_, m_.z.y+s_, m_.z.z+s_, m_.z.w+s_,
                  m_.w.x+s_, m_.w.y+s_, m_.w.z+s_, m_.w.w+s_);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator+(typename math<T>::scalar_t s_, const mat44<T> &m_)
{
  // add matrix to matrix
  return mat44<T>(s_+m_.x.x, s_+m_.x.y, s_+m_.x.z, s_+m_.x.w,
                  s_+m_.y.x, s_+m_.y.y, s_+m_.y.z, s_+m_.y.w,
                  s_+m_.z.x, s_+m_.z.y, s_+m_.z.z, s_+m_.z.w,
                  s_+m_.w.x, s_+m_.w.y, s_+m_.w.z, s_+m_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator-(const mat44<T> &m0_, const mat44<T> &m1_)
{
  // subtract matrix from matrix
  return mat44<T>(m0_.x.x-m1_.x.x, m0_.x.y-m1_.x.y, m0_.x.z-m1_.x.z, m0_.x.w-m1_.x.w,
                  m0_.y.x-m1_.y.x, m0_.y.y-m1_.y.y, m0_.y.z-m1_.y.z, m0_.y.w-m1_.y.w,
                  m0_.z.x-m1_.z.x, m0_.z.y-m1_.z.y, m0_.z.z-m1_.z.z, m0_.z.w-m1_.z.w,
                  m0_.w.x-m1_.w.x, m0_.w.y-m1_.w.y, m0_.w.z-m1_.w.z, m0_.w.w-m1_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator-(const mat44<T> &m_, typename math<T>::scalar_t s_)
{
  // subtract matrix from matrix
  return mat44<T>(m_.x.x-s_, m_.x.y-s_, m_.x.z-s_, m_.x.w-s_,
                  m_.y.x-s_, m_.y.y-s_, m_.y.z-s_, m_.y.w-s_,
                  m_.z.x-s_, m_.z.y-s_, m_.z.z-s_, m_.z.w-s_,
                  m_.w.x-s_, m_.w.y-s_, m_.w.z-s_, m_.w.w-s_);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator-(typename math<T>::scalar_t s_, const mat44<T> &m_)
{
  // subtract matrix from matrix
  return mat44<T>(s_-m_.x.x, s_-m_.x.y, s_-m_.x.z, s_-m_.x.w,
                  s_-m_.y.x, s_-m_.y.y, s_-m_.y.z, s_-m_.y.w,
                  s_-m_.z.x, s_-m_.z.y, s_-m_.z.z, s_-m_.z.w,
                  s_-m_.w.x, s_-m_.w.y, s_-m_.w.z, s_-m_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator-(const mat44<T> &m_)
{
  // negate matrix
  return mat44<T>(-m_.x.x, -m_.x.y, -m_.x.z, -m_.x.w,
                  -m_.y.x, -m_.y.y, -m_.y.z, -m_.y.w,
                  -m_.z.x, -m_.z.y, -m_.z.z, -m_.z.w,
                  -m_.w.x, -m_.w.y, -m_.w.z, -m_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(const mat44<T> &m0_, const mat44<T> &m1_)
{
  // multiply matrix by matrix
  return mat44<T>(m0_.x.x*m1_.x.x+m0_.x.y*m1_.y.x+m0_.x.z*m1_.z.x+m0_.x.w*m1_.w.x, m0_.x.x*m1_.x.y+m0_.x.y*m1_.y.y+m0_.x.z*m1_.z.y+m0_.x.w*m1_.w.y, m0_.x.x*m1_.x.z+m0_.x.y*m1_.y.z+m0_.x.z*m1_.z.z+m0_.x.w*m1_.w.z, m0_.x.x*m1_.x.w+m0_.x.y*m1_.y.w+m0_.x.z*m1_.z.w+m0_.x.w*m1_.w.w,
                  m0_.y.x*m1_.x.x+m0_.y.y*m1_.y.x+m0_.y.z*m1_.z.x+m0_.y.w*m1_.w.x, m0_.y.x*m1_.x.y+m0_.y.y*m1_.y.y+m0_.y.z*m1_.z.y+m0_.y.w*m1_.w.y, m0_.y.x*m1_.x.z+m0_.y.y*m1_.y.z+m0_.y.z*m1_.z.z+m0_.y.w*m1_.w.z, m0_.y.x*m1_.x.w+m0_.y.y*m1_.y.w+m0_.y.z*m1_.z.w+m0_.y.w*m1_.w.w,
                  m0_.z.x*m1_.x.x+m0_.z.y*m1_.y.x+m0_.z.z*m1_.z.x+m0_.z.w*m1_.w.x, m0_.z.x*m1_.x.y+m0_.z.y*m1_.y.y+m0_.z.z*m1_.z.y+m0_.z.w*m1_.w.y, m0_.z.x*m1_.x.z+m0_.z.y*m1_.y.z+m0_.z.z*m1_.z.z+m0_.z.w*m1_.w.z, m0_.z.x*m1_.x.w+m0_.z.y*m1_.y.w+m0_.z.z*m1_.z.w+m0_.z.w*m1_.w.w,
                  m0_.w.x*m1_.x.x+m0_.w.y*m1_.y.x+m0_.w.z*m1_.z.x+m0_.w.w*m1_.w.x, m0_.w.x*m1_.x.y+m0_.w.y*m1_.y.y+m0_.w.z*m1_.z.y+m0_.w.w*m1_.w.y, m0_.w.x*m1_.x.z+m0_.w.y*m1_.y.z+m0_.w.z*m1_.z.z+m0_.w.w*m1_.w.z, m0_.w.x*m1_.x.w+m0_.w.y*m1_.y.w+m0_.w.z*m1_.z.w+m0_.w.w*m1_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(const mat44<T> &m_, typename math<T>::scalar_t s_)
{
  // multiply matrix by scalar
  return mat44<T>(m_.x.x*s_, m_.x.y*s_, m_.x.z*s_, m_.x.w*s_,
                  m_.y.x*s_, m_.y.y*s_, m_.y.z*s_, m_.y.w*s_,
                  m_.z.x*s_, m_.z.y*s_, m_.z.z*s_, m_.z.w*s_,
                  m_.w.x*s_, m_.w.y*s_, m_.w.z*s_, m_.w.w*s_);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(typename math<T>::scalar_t s_, const mat44<T> &m_)
{
  // multiply matrix by scalar
  return mat44<T>(s_*m_.x.x, s_*m_.x.y, s_*m_.x.z, s_*m_.x.w,
                  s_*m_.y.x, s_*m_.y.y, s_*m_.y.z, s_*m_.y.w,
                  s_*m_.z.x, s_*m_.z.y, s_*m_.z.z, s_*m_.z.w,
                  s_*m_.w.x, s_*m_.w.y, s_*m_.w.z, s_*m_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator/(const mat44<T> &m_, typename math<T>::scalar_t s_)
{
  // divide matrix by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return mat44<T>(m_.x.x*rs, m_.x.y*rs, m_.x.z*rs, m_.x.w*rs,
                  m_.y.x*rs, m_.y.y*rs, m_.y.z*rs, m_.y.w*rs,
                  m_.z.x*rs, m_.z.y*rs, m_.z.z*rs, m_.z.w*rs,
                  m_.w.x*rs, m_.w.y*rs, m_.w.z*rs, m_.w.w*rs);
}
//----

template<typename T>
EGL_INLINE mat44<T> mul(const mat44<T> &m0_, const mat44<T> &m1_)
{
  // component-wise multiply: m0_*m1
  return mat44<T>(m0_.x.x*m1_.x.x, m0_.x.y*m1_.x.y, m0_.x.z*m1_.x.z, m0_.x.w*m1_.x.w,
                  m0_.y.x*m1_.y.x, m0_.y.y*m1_.y.y, m0_.y.z*m1_.y.z, m0_.y.w*m1_.y.w,
                  m0_.z.x*m1_.z.x, m0_.z.y*m1_.z.y, m0_.z.z*m1_.z.z, m0_.z.w*m1_.z.w,
                  m0_.w.x*m1_.w.x, m0_.w.y*m1_.w.y, m0_.w.z*m1_.w.z, m0_.w.w*m1_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> madd(const mat44<T> &mm0_, const mat44<T> &mm1_, const mat44<T> &ma_)
{
  // component-wise multiply-add: mm0_*mm1_+ma_
  return mat44<T>(mm0_.x.x*mm1_.x.x+ma_.x.x, mm0_.x.y*mm1_.x.y+ma_.x.y, mm0_.x.z*mm1_.x.z+ma_.x.z, mm0_.x.w*mm1_.x.w+ma_.x.w,
                  mm0_.y.x*mm1_.y.x+ma_.y.x, mm0_.y.y*mm1_.y.y+ma_.y.y, mm0_.y.z*mm1_.y.z+ma_.y.z, mm0_.y.w*mm1_.y.w+ma_.y.w,
                  mm0_.z.x*mm1_.z.x+ma_.z.x, mm0_.z.y*mm1_.z.y+ma_.z.y, mm0_.z.z*mm1_.z.z+ma_.z.z, mm0_.z.w*mm1_.z.w+ma_.z.w,
                  mm0_.w.x*mm1_.w.x+ma_.w.x, mm0_.w.y*mm1_.w.y+ma_.w.y, mm0_.w.z*mm1_.w.z+ma_.w.z, mm0_.w.w*mm1_.w.w+ma_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> madd(const mat44<T> &mm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: mm_*sm_+sa_
  return mat44<T>(mm_.x.x*sm_+sa_, mm_.x.y*sm_+sa_, mm_.x.z*sm_+sa_, mm_.x.w*sm_+sa_,
                  mm_.y.x*sm_+sa_, mm_.y.y*sm_+sa_, mm_.y.z*sm_+sa_, mm_.y.w*sm_+sa_,
                  mm_.z.x*sm_+sa_, mm_.z.y*sm_+sa_, mm_.z.z*sm_+sa_, mm_.z.w*sm_+sa_,
                  mm_.w.x*sm_+sa_, mm_.w.y*sm_+sa_, mm_.w.z*sm_+sa_, mm_.w.w*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE mat44<T> madd(const mat44<T> &mm0_, const mat44<T> &mm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: mm0_*mm1_+sa_
  return mat44<T>(mm0_.x.x*mm1_.x.x+sa_, mm0_.x.y*mm1_.x.y+sa_, mm0_.x.z*mm1_.x.z+sa_, mm0_.x.w*mm1_.x.w+sa_,
                  mm0_.y.x*mm1_.y.x+sa_, mm0_.y.y*mm1_.y.y+sa_, mm0_.y.z*mm1_.y.z+sa_, mm0_.y.w*mm1_.y.w+sa_,
                  mm0_.z.x*mm1_.z.x+sa_, mm0_.z.y*mm1_.z.y+sa_, mm0_.z.z*mm1_.z.z+sa_, mm0_.z.w*mm1_.z.w+sa_,
                  mm0_.w.x*mm1_.w.x+sa_, mm0_.w.y*mm1_.w.y+sa_, mm0_.w.z*mm1_.w.z+sa_, mm0_.w.w*mm1_.w.w+sa_);
}
//----

template<typename T>
EGL_INLINE mat44<T> madd(const mat44<T> &mm_, typename math<T>::scalar_t sm_, const mat44<T> &ma_)
{
  // component-wise multiply-add: mm_*sm_+ma_
  return mat44<T>(mm_.x.x*sm_+ma_.x.x, mm_.x.y*sm_+ma_.x.y, mm_.x.z*sm_+ma_.x.z, mm_.x.w*sm_+ma_.x.w,
                  mm_.y.x*sm_+ma_.y.x, mm_.y.y*sm_+ma_.y.y, mm_.y.z*sm_+ma_.y.z, mm_.y.w*sm_+ma_.y.w,
                  mm_.z.x*sm_+ma_.z.x, mm_.z.y*sm_+ma_.z.y, mm_.z.z*sm_+ma_.z.z, mm_.z.w*sm_+ma_.z.w,
                  mm_.w.x*sm_+ma_.w.x, mm_.w.y*sm_+ma_.w.y, mm_.w.z*sm_+ma_.w.z, mm_.w.w*sm_+ma_.w.w);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t det(const mat44<T> &m_)
{
  // determinant of the matrix
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t dzw=m_.z.z*m_.w.w-m_.z.w*m_.w.z;
  scalar_t dwx=m_.w.z*m_.x.w-m_.w.w*m_.x.z;
  scalar_t dxy=m_.x.z*m_.y.w-m_.x.w*m_.y.z;
  scalar_t dyz=m_.y.z*m_.z.w-m_.y.w*m_.z.z;
  return  m_.x.x*(m_.y.y*dzw+m_.z.y*(m_.w.z*m_.y.w-m_.w.w*m_.y.z)+m_.w.y*dyz)
         -m_.y.x*(m_.z.y*dwx+m_.w.y*(m_.x.z*m_.z.w-m_.x.w*m_.z.z)+m_.x.y*dzw)
         +m_.z.x*(m_.w.y*dxy+m_.x.y*(m_.y.z*m_.w.w-m_.y.w*m_.w.z)+m_.y.y*dwx)
         -m_.w.x*(m_.x.y*dyz+m_.y.y*(m_.z.z*m_.x.w-m_.z.w*m_.x.z)+m_.z.y*dxy);
}
//----

template<typename T>
EGL_INLINE mat44<T> inv(const mat44<T> &m_, typename math<T>::scalar_t *det_)
{
  // calculate determinant of the matrix
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t dzw=m_.z.z*m_.w.w-m_.z.w*m_.w.z;
  scalar_t dwx=m_.w.z*m_.x.w-m_.w.w*m_.x.z;
  scalar_t dxy=m_.x.z*m_.y.w-m_.x.w*m_.y.z;
  scalar_t dyz=m_.y.z*m_.z.w-m_.y.w*m_.z.z;
  scalar_t dx= m_.y.y*dzw+m_.z.y*(m_.w.z*m_.y.w-m_.w.w*m_.y.z)+m_.w.y*dyz;
  scalar_t dy=-m_.z.y*dwx-m_.w.y*(m_.x.z*m_.z.w-m_.x.w*m_.z.z)-m_.x.y*dzw;
  scalar_t dz= m_.w.y*dxy+m_.x.y*(m_.y.z*m_.w.w-m_.y.w*m_.w.z)+m_.y.y*dwx;
  scalar_t dw=-m_.x.y*dyz-m_.y.y*(m_.z.z*m_.x.w-m_.z.w*m_.x.z)-m_.z.y*dxy;
  scalar_t d=m_.x.x*dx+m_.y.x*dy+m_.z.x*dz+m_.w.x*dw;
  if(det_)
    *det_=d;
  if(!d)
    return mat44<T>(scalar_t(0));

  // calculate inverse of the matrix x-row
  mat44<T> res;
  scalar_t rd=scalar_t(1)/d;
  res.x.x=rd*dx;
  res.x.y=rd*dy;
  res.x.z=rd*dz;
  res.x.w=rd*dw;

  // calculate inverse of the matrix y-row
  dzw=m_.z.w*m_.w.x-m_.z.x*m_.w.w;
  dwx=m_.w.w*m_.x.x-m_.w.x*m_.x.w;
  dxy=m_.x.w*m_.y.x-m_.x.x*m_.y.w;
  dyz=m_.y.w*m_.z.x-m_.y.x*m_.z.w;
  res.y.x=rd*(-m_.y.z*dzw-m_.z.z*(m_.w.w*m_.y.x-m_.w.x*m_.y.w)-m_.w.z*dyz);
  res.y.y=rd*( m_.z.z*dwx+m_.w.z*(m_.x.w*m_.z.x-m_.x.x*m_.z.w)+m_.x.z*dzw);
  res.y.z=rd*(-m_.w.z*dxy-m_.x.z*(m_.y.w*m_.w.x-m_.y.x*m_.w.w)-m_.y.z*dwx);
  res.y.w=rd*( m_.x.z*dyz+m_.y.z*(m_.z.w*m_.x.x-m_.z.x*m_.x.w)+m_.z.z*dxy);

  // calculate inverse of the matrix z-row
  dzw=m_.z.x*m_.w.y-m_.z.y*m_.w.x;
  dwx=m_.w.x*m_.x.y-m_.w.y*m_.x.x;
  dxy=m_.x.x*m_.y.y-m_.x.y*m_.y.x;
  dyz=m_.y.x*m_.z.y-m_.y.y*m_.z.x;
  res.z.x=rd*( m_.y.w*dzw+m_.z.w*(m_.w.x*m_.y.y-m_.w.y*m_.y.x)+m_.w.w*dyz);
  res.z.y=rd*(-m_.z.w*dwx-m_.w.w*(m_.x.x*m_.z.y-m_.x.y*m_.z.x)-m_.x.w*dzw);
  res.z.z=rd*( m_.w.w*dxy+m_.x.w*(m_.y.x*m_.w.y-m_.y.y*m_.w.x)+m_.y.w*dwx);
  res.z.w=rd*(-m_.x.w*dyz-m_.y.w*(m_.z.x*m_.x.y-m_.z.y*m_.x.x)-m_.z.w*dxy);

  // calculate inverse of the matrix w-row
  dzw=m_.z.y*m_.w.z-m_.z.z*m_.w.y;
  dwx=m_.w.y*m_.x.z-m_.w.z*m_.x.y;
  dxy=m_.x.y*m_.y.z-m_.x.z*m_.y.y;
  dyz=m_.y.y*m_.z.z-m_.y.z*m_.z.y;
  res.w.x=rd*(-m_.y.x*dzw-m_.z.x*(m_.w.y*m_.y.z-m_.w.z*m_.y.y)-m_.w.x*dyz);
  res.w.y=rd*( m_.z.x*dwx+m_.w.x*(m_.x.y*m_.z.z-m_.x.z*m_.z.y)+m_.x.x*dzw);
  res.w.z=rd*(-m_.w.x*dxy-m_.x.x*(m_.y.y*m_.w.z-m_.y.z*m_.w.y)-m_.y.x*dwx);
  res.w.w=rd*( m_.x.x*dyz+m_.y.x*(m_.z.y*m_.x.z-m_.z.z*m_.x.y)+m_.z.x*dxy);
  return res;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t tr(const mat44<T> &m_)
{
  // trace of the matrix
  return m_.x.x+m_.y.y+m_.z.z+m_.w.w;
}
//----

template<typename T>
EGL_INLINE void identity(mat44<T> &m_)
{
  // set identity
  typedef typename math<T>::scalar_t scalar_t;
  m_.x.x=scalar_t(1); m_.x.y=0;           m_.x.z=0;           m_.x.w=0;
  m_.y.x=0;           m_.y.y=scalar_t(1); m_.y.z=0;           m_.y.w=0;
  m_.z.x=0;           m_.z.y=0;           m_.z.z=scalar_t(1); m_.z.w=0;
  m_.w.x=0;           m_.w.y=0;           m_.w.z=0;           m_.w.w=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE void transpose(mat44<T> &m_)
{
  // transpose the matrix
  typename math<T>::scalar_t t=m_.x.y;
  m_.x.y=m_.y.x;
  m_.y.x=t;
  t=m_.x.z;
  m_.x.z=m_.z.x;
  m_.z.x=t;
  t=m_.x.w;
  m_.x.w=m_.w.x;
  m_.w.x=t;
  t=m_.y.z;
  m_.y.z=m_.z.y;
  m_.z.y=t;
  t=m_.y.w;
  m_.y.w=m_.w.y;
  m_.w.y=t;
  t=m_.z.w;
  m_.z.w=m_.w.z;
  m_.w.z=t;
}
//----

template<typename T>
EGL_INLINE void transpose(mat44<T> &res_, const mat44<T> &m_)
{
  // transpose the matrix
  res_.x.x=m_.x.x; res_.x.y=m_.y.x; res_.x.z=m_.z.x; res_.x.w=m_.w.x;
  res_.y.x=m_.x.y; res_.y.y=m_.y.y; res_.y.z=m_.z.y; res_.y.w=m_.w.y;
  res_.z.x=m_.x.z; res_.z.y=m_.y.z; res_.z.z=m_.z.z; res_.z.w=m_.w.z;
  res_.w.x=m_.x.w; res_.w.y=m_.y.w; res_.w.z=m_.z.w; res_.w.w=m_.w.w;
}
//----

template<typename T>
EGL_INLINE void neg(mat44<T> &m_)
{
  // negate the matrix
  m_.x.x=-m_.x.x; m_.x.y=-m_.x.y; m_.x.z=-m_.x.z; m_.x.w=-m_.x.w;
  m_.y.x=-m_.y.x; m_.y.y=-m_.y.y; m_.y.z=-m_.y.z; m_.y.w=-m_.y.w;
  m_.z.x=-m_.z.x; m_.z.y=-m_.z.y; m_.z.z=-m_.z.z; m_.z.w=-m_.z.w;
  m_.w.x=-m_.w.x; m_.w.y=-m_.w.y; m_.w.z=-m_.w.z; m_.w.w=-m_.w.w;
}
//----

template<typename T>
EGL_INLINE mat44<T> lerp(const mat44<T> &m0_, const mat44<T> &m1_, typename math<T>::scalar_t t_)
{
  // linear matrix interpolation. f(t=0)=v0, f(t=1)=v1
  return mat44<T>(m0_.x.x+(m1_.x.x-m0_.x.x)*t_, m0_.x.y+(m1_.x.y-m0_.x.y)*t_, m0_.x.z+(m1_.x.z-m0_.x.z)*t_, m0_.x.w+(m1_.x.w-m0_.x.w)*t_,
                  m0_.y.x+(m1_.y.x-m0_.y.x)*t_, m0_.y.y+(m1_.y.y-m0_.y.y)*t_, m0_.y.z+(m1_.y.z-m0_.y.z)*t_, m0_.y.w+(m1_.y.w-m0_.y.w)*t_,
                  m0_.z.x+(m1_.z.x-m0_.z.x)*t_, m0_.z.y+(m1_.z.y-m0_.z.y)*t_, m0_.z.z+(m1_.z.z-m0_.z.z)*t_, m0_.z.w+(m1_.z.w-m0_.z.w)*t_,
                  m0_.w.x+(m1_.w.x-m0_.w.x)*t_, m0_.w.y+(m1_.w.y-m0_.w.y)*t_, m0_.w.z+(m1_.w.z-m0_.w.z)*t_, m0_.w.w+(m1_.w.w-m0_.w.w)*t_);
}
//----

template<typename T>
EGL_INLINE void lerp(mat44<T> &res_, const mat44<T> &m0_, const mat44<T> &m1_, typename math<T>::scalar_t t_)
{
  // linear matrix interpolation. f(t=0)=v0, f(t=1)=v1
  res_.x.x=m0_.x.x+(m1_.x.x-m0_.x.x)*t_; res_.x.y=m0_.x.y+(m1_.x.y-m0_.x.y)*t_; res_.x.z=m0_.x.z+(m1_.x.z-m0_.x.z)*t_; res_.x.w=m0_.x.w+(m1_.x.w-m0_.x.w)*t_;
  res_.y.x=m0_.y.x+(m1_.y.x-m0_.y.x)*t_; res_.y.y=m0_.y.y+(m1_.y.y-m0_.y.y)*t_; res_.y.z=m0_.y.z+(m1_.y.z-m0_.y.z)*t_; res_.y.w=m0_.y.w+(m1_.y.w-m0_.y.w)*t_;
  res_.z.x=m0_.z.x+(m1_.z.x-m0_.z.x)*t_; res_.z.y=m0_.z.y+(m1_.z.y-m0_.z.y)*t_; res_.z.z=m0_.z.z+(m1_.z.z-m0_.z.z)*t_; res_.z.w=m0_.z.w+(m1_.z.w-m0_.z.w)*t_;
  res_.w.x=m0_.w.x+(m1_.w.x-m0_.w.x)*t_; res_.w.y=m0_.w.y+(m1_.w.y-m0_.w.y)*t_; res_.w.z=m0_.w.z+(m1_.w.z-m0_.w.z)*t_; res_.w.w=m0_.w.w+(m1_.w.w-m0_.w.w)*t_;
}
//----

template<typename T, typename U>
EGL_INLINE mat44<T> to_mat44(const mat44<U> &m_)
{
  // convert between mat44 types
  typedef typename math<T>::scalar_t scalar_t;
  return mat44<T>(scalar_t(m_.x.x), scalar_t(m_.x.y), scalar_t(m_.x.z), scalar_t(m_.x.w),
                  scalar_t(m_.y.x), scalar_t(m_.y.y), scalar_t(m_.y.z), scalar_t(m_.y.w),
                  scalar_t(m_.z.x), scalar_t(m_.z.y), scalar_t(m_.z.z), scalar_t(m_.z.w),
                  scalar_t(m_.w.x), scalar_t(m_.w.y), scalar_t(m_.w.z), scalar_t(m_.w.w));
}
//----

template<typename T> EGL_INLINE void normalize(mat44<T> &m_)
{
  // normalize matrix vectors
  m_.x=unit(m_.x);
  m_.y=unit(m_.y);
  m_.z=unit(m_.z);
  m_.w=unit(m_.w);
}
//----------------------------------------------------------------------------


//============================================================================
// quat
//============================================================================
template<typename T>
quat<T> quat<T>::zero()
{
  return quat<T>(scalar_t(0));
}
//----

template<typename T>
quat<T> quat<T>::one()
{
  return quat<T>(scalar_t(1));
}
//----

template<typename T>
quat<T> quat<T>::neg_one()
{
  return quat<T>(scalar_t(-1));
}
//----

template<typename T>
quat<T> quat<T>::identity()
{
  return quat<T>(0, 0, 0, scalar_t(1));
}
//----------------------------------------------------------------------------

template<typename T>
quat<T>::quat()
{
}
//----

template<typename T>
quat<T>::quat(T s_)
{
  // init quat with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

template<typename T>
quat<T>::quat(T x_, T y_, T z_, T w_)
{
  // init quat with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

template<typename T>
quat<T>::quat(const vec3<T> &v_)
{
  // init quat with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=0;
}
//----

template<typename T>
quat<T>::quat(const vec3<T> &v_, T w_)
{
  // init quat with 3d vector & w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

template<typename T>
quat<T>::quat(const vec4<T> &v_)
{
  // init quat with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

template<typename T>
quat<T>::quat(const T *a_)
{
  // init quat with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------

template<typename T>
const T &quat<T>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &quat<T>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void quat<T>::set(T s_)
{
  // set quat with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

template<typename T>
void quat<T>::set(T x_, T y_, T z_, T w_)
{
  // set quat with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

template<typename T>
void quat<T>::set(const vec3<T> &v_)
{
  // set quat with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=0;
}
//----

template<typename T>
void quat<T>::set(const vec3<T> &v_, T w_)
{
  // set quat with 3d vector & w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

template<typename T>
void quat<T>::set(const vec4<T> &v_)
{
  // set quat with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

template<typename T>
void quat<T>::set(const quat<T> &q_)
{
  // set quat with quat
  x=q_.x;
  y=q_.y;
  z=q_.z;
  w=q_.w;
}
//----

template<typename T>
void quat<T>::set(const T *a_)
{
  // set quat with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------


//============================================================================
// quat<vec32s_t>
//============================================================================
quat<vec32s_t> quat<vec32s_t>::zero()
{
  return quat<vec32s_t>(scalar_t(0));
}
//----

quat<vec32s_t> quat<vec32s_t>::one()
{
  return quat<vec32s_t>(scalar_t(1));
}
//----

quat<vec32s_t> quat<vec32s_t>::neg_one()
{
  return quat<vec32s_t>(scalar_t(-1));
}
//----

quat<vec32s_t> quat<vec32s_t>::identity()
{
  return quat<vec32s_t>(0, 0, 0, scalar_t(1));
}
//----------------------------------------------------------------------------

quat<vec32s_t>::quat()
{
}
//----

quat<vec32s_t>::quat(int8_t s_)
{
  // init quat with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

quat<vec32s_t>::quat(int8_t x_, int8_t y_, int8_t z_, int8_t w_)
{
  // init quat with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

quat<vec32s_t>::quat(const vec3<vec32s_t> &v_)
{
  // init quat with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=0;
}
//----

quat<vec32s_t>::quat(const vec3<vec32s_t> &v_, int8_t w_)
{
  // init quat with 3d vector & w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

quat<vec32s_t>::quat(const vec4<vec32s_t> &v_)
{
  // init quat with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

quat<vec32s_t>::quat(const int8_t *a_)
{
  // init quat with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------

const int8_t &quat<vec32s_t>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const int8_t*>(this)[idx_];
}
//----

int8_t &quat<vec32s_t>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<int8_t*>(this)[idx_];
}
//----

void quat<vec32s_t>::set(int8_t s_)
{
  // set quat with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

void quat<vec32s_t>::set(int8_t x_, int8_t y_, int8_t z_, int8_t w_)
{
  // set quat with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

void quat<vec32s_t>::set(const vec3<vec32s_t> &v_)
{
  // set quat with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=0;
}
//----

void quat<vec32s_t>::set(const vec3<vec32s_t> &v_, int8_t w_)
{
  // set quat with 3d vector & w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

void quat<vec32s_t>::set(const vec4<vec32s_t> &v_)
{
  // set quat with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

void quat<vec32s_t>::set(const quat<vec32s_t> &q_)
{
  // set quat with quat
  x=q_.x;
  y=q_.y;
  z=q_.z;
  w=q_.w;
}
//----

void quat<vec32s_t>::set(const int8_t *a_)
{
  // set quat with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------


//============================================================================
// quat<vec32u_t>
//============================================================================
quat<vec32u_t> quat<vec32u_t>::zero()
{
  return quat<vec32u_t>(scalar_t(0));
}
//----

quat<vec32u_t> quat<vec32u_t>::one()
{
  return quat<vec32u_t>(scalar_t(1));
}
//----

quat<vec32u_t> quat<vec32u_t>::neg_one()
{
  return quat<vec32u_t>(scalar_t(0xff));
}
//----

quat<vec32u_t> quat<vec32u_t>::identity()
{
  return quat<vec32u_t>(0, 0, 0, scalar_t(1));
}
//----------------------------------------------------------------------------

quat<vec32u_t>::quat()
{
}
//----

quat<vec32u_t>::quat(uint8_t s_)
{
  // init quat with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

quat<vec32u_t>::quat(uint8_t x_, uint8_t y_, uint8_t z_, uint8_t w_)
{
  // init quat with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

quat<vec32u_t>::quat(const vec3<vec32u_t> &v_)
{
  // init quat with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=0;
}
//----

quat<vec32u_t>::quat(const vec3<vec32u_t> &v_, uint8_t w_)
{
  // init quat with 3d vector & w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

quat<vec32u_t>::quat(const vec4<vec32u_t> &v_)
{
  // init quat with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

quat<vec32u_t>::quat(const uint8_t *a_)
{
  // init quat with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------

const uint8_t &quat<vec32u_t>::operator[](unsigned idx_) const
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<const uint8_t*>(this)[idx_];
}
//----

uint8_t &quat<vec32u_t>::operator[](unsigned idx_)
{
  // return reference to nth component (x=0, y=1, z=2, w=3)
  EGL_ASSERT_PEDANTIC(idx_<4);
  return reinterpret_cast<uint8_t*>(this)[idx_];
}
//----

void quat<vec32u_t>::set(uint8_t s_)
{
  // set quat with scalar
  x=s_;
  y=s_;
  z=s_;
  w=s_;
}
//----

void quat<vec32u_t>::set(uint8_t x_, uint8_t y_, uint8_t z_, uint8_t w_)
{
  // set quat with scalars
  x=x_;
  y=y_;
  z=z_;
  w=w_;
}
//----

void quat<vec32u_t>::set(const vec3<vec32u_t> &v_)
{
  // set quat with 3d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=0;
}
//----

void quat<vec32u_t>::set(const vec3<vec32u_t> &v_, uint8_t w_)
{
  // set quat with 3d vector & w
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=w_;
}
//----

void quat<vec32u_t>::set(const vec4<vec32u_t> &v_)
{
  // set quat with 4d vector
  x=v_.x;
  y=v_.y;
  z=v_.z;
  w=v_.w;
}
//----

void quat<vec32u_t>::set(const quat<vec32u_t> &q_)
{
  // set quat with quat
  x=q_.x;
  y=q_.y;
  z=q_.z;
  w=q_.w;
}
//----

void quat<vec32u_t>::set(const uint8_t *a_)
{
  // set quat with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  x=a_[0];
  y=a_[1];
  z=a_[2];
  w=a_[3];
}
//----------------------------------------------------------------------------


//============================================================================
// quat free functions
//============================================================================
template<typename T>
EGL_INLINE bool is_zero(const quat<T> &q_)
{
  // test for zero-quaternion
  return q_.x==0 && q_.y==0 && q_.z==0 && q_.w==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const quat<T> &q_)
{
  // test for saturated quaternion
  typedef typename math<T>::scalar_t scalar_t;
  return    q_.x>=0 && q_.x<=scalar_t(1)
         && q_.y>=0 && q_.y<=scalar_t(1)
         && q_.z>=0 && q_.z<=scalar_t(1)
         && q_.w>=0 && q_.w<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const quat<T> &q_)
{
  // test for signed saturated quaternion
  typedef typename math<T>::scalar_t scalar_t;
  return    q_.x>=scalar_t(-1) && q_.x<=scalar_t(1)
         && q_.y>=scalar_t(-1) && q_.y<=scalar_t(1)
         && q_.z>=scalar_t(-1) && q_.z<=scalar_t(1)
         && q_.w>=scalar_t(-1) && q_.w<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool operator==(const quat<T> &q0_, const quat<T> &q1_)
{
  // test for equality of quats
  return q0_.x==q1_.x && q0_.y==q1_.y && q0_.z==q1_.z && q0_.w==q1_.w;
}
//----

template<typename T>
EGL_INLINE bool operator==(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // test for equality of quat and scalar
  return q_.x==s_ && q_.y==s_ && q_.z==s_ && q_.w==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // test for equality of quat and scalar
  return s_==q_.x && s_==q_.y && s_==q_.z && s_==q_.w;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const quat<T> &q0_, const quat<T> &q1_)
{
  // test for inequality of quats
  return q0_.x!=q1_.x || q0_.y!=q1_.y || q0_.z!=q1_.z || q0_.w!=q1_.w;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // test for inequality of quat and scalar
  return q_.x!=s_ || q_.y!=s_ || q_.z!=s_ || q_.w!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // test for inequality of quat and scalar
  return s_!=q_.x || s_!=q_.y || s_!=q_.z || s_!=q_.w;
}
//----

template<typename T>
EGL_INLINE quat<T> operator<(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q0_.x<q1_.x?scalar_t(1):0, q0_.y<q1_.y?scalar_t(1):0, q0_.z<q1_.z?scalar_t(1):0, q0_.w<q1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator<(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<s_?scalar_t(1):0, q_.y<s_?scalar_t(1):0, q_.z<s_?scalar_t(1):0, q_.w<s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator<(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(s_<q_.x?scalar_t(1):0, s_<q_.y?scalar_t(1):0, s_<q_.z?scalar_t(1):0, s_<q_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator>(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q0_.x>q1_.x?scalar_t(1):0, q0_.y>q1_.y?scalar_t(1):0, q0_.z>q1_.z?scalar_t(1):0, q0_.w>q1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator>(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x>s_?scalar_t(1):0, q_.y>s_?scalar_t(1):0, q_.z>s_?scalar_t(1):0, q_.w>s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator>(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(s_>q_.x?scalar_t(1):0, s_>q_.y?scalar_t(1):0, s_>q_.z?scalar_t(1):0, s_>q_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator<=(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q0_.x<=q1_.x?scalar_t(1):0, q0_.y<=q1_.y?scalar_t(1):0, q0_.z<=q1_.z?scalar_t(1):0, q0_.w<=q1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator<=(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<=s_?scalar_t(1):0, q_.y<=s_?scalar_t(1):0, q_.z<=s_?scalar_t(1):0, q_.w<=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator<=(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(s_<=q_.x?scalar_t(1):0, s_<=q_.y?scalar_t(1):0, s_<=q_.z?scalar_t(1):0, s_<=q_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator>=(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q0_.x>=q1_.x?scalar_t(1):0, q0_.y>=q1_.y?scalar_t(1):0, q0_.z>=q1_.z?scalar_t(1):0, q0_.w>=q1_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator>=(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x>=s_?scalar_t(1):0, q_.y>=s_?scalar_t(1):0, q_.z>=s_?scalar_t(1):0, q_.w>=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> operator>=(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(s_>=q_.x?scalar_t(1):0, s_>=q_.y?scalar_t(1):0, s_>=q_.z?scalar_t(1):0, s_>=q_.w?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> sel_eq(const quat<T> &t0_, const quat<T> &t1_, const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise equal quaternion select (t0_==t1_?q0_:q1_)
  return quat<T>(t0_.x==t1_.x?q0_.x:q1_.x,
                 t0_.y==t1_.y?q0_.y:q1_.y,
                 t0_.z==t1_.z?q0_.z:q1_.z,
                 t0_.w==t1_.w?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sel_lt(const quat<T> &t0_, const quat<T> &t1_, const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise less-than quaternion select (t0_<t1_?q0_:q1_)
  return quat<T>(t0_.x<t1_.x?q0_.x:q1_.x,
                 t0_.y<t1_.y?q0_.y:q1_.y,
                 t0_.z<t1_.z?q0_.z:q1_.z,
                 t0_.w<t1_.w?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sel_le(const quat<T> &t0_, const quat<T> &t1_, const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise less-or-equal quaternion select (t0_<=t1_?q0_:q1_)
  return quat<T>(t0_.x<=t1_.x?q0_.x:q1_.x,
                 t0_.y<=t1_.y?q0_.y:q1_.y,
                 t0_.z<=t1_.z?q0_.z:q1_.z,
                 t0_.w<=t1_.w?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sel_eqz(const quat<T> &t_, const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise equal-to-zero quaternion select (t_==0?q0_:q1_)
  return quat<T>(t_.x==0?q0_.x:q1_.x,
                 t_.y==0?q0_.y:q1_.y,
                 t_.z==0?q0_.z:q1_.z,
                 t_.w==0?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sel_ltz(const quat<T> &t_, const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise less-than-zero quaternion select (t_<0?q0_:q1_)
  return quat<T>(t_.x<0?q0_.x:q1_.x,
                 t_.y<0?q0_.y:q1_.y,
                 t_.z<0?q0_.z:q1_.z,
                 t_.w<0?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sel_lez(const quat<T> &t_, const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise less-or-equal-to-zero quaternion select (t_<=0?q0_:q1_)
  return quat<T>(t_.x<=0?q0_.x:q1_.x,
                 t_.y<=0?q0_.y:q1_.y,
                 t_.z<=0?q0_.z:q1_.z,
                 t_.w<=0?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE void operator+=(quat<T> &qr_, const quat<T> &q_)
{
  // add quat to quat
  qr_.x+=q_.x;
  qr_.y+=q_.y;
  qr_.z+=q_.z;
  qr_.w+=q_.w;
}
//----

template<typename T>
EGL_INLINE void operator+=(quat<T> &qr_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the quat
  qr_.x+=s_;
  qr_.y+=s_;
  qr_.z+=s_;
  qr_.w+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(quat<T> &qr_, const quat<T> &q_)
{
  // subtract quat from quat
  qr_.x-=q_.x;
  qr_.y-=q_.y;
  qr_.z-=q_.z;
  qr_.w-=q_.w;
}
//----

template<typename T>
EGL_INLINE void operator-=(quat<T> &qr_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the quat
  qr_.x-=s_;
  qr_.y-=s_;
  qr_.z-=s_;
  qr_.w-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(quat<T> &qr_, const quat<T> &q_)
{
  // multiply quat by quat
  quat<T> q(qr_);
  qr_.x=q.y*q_.z-q.z*q_.y+q.w*q_.x+q.x*q_.w;
  qr_.y=q.z*q_.x-q.x*q_.z+q.w*q_.y+q.y*q_.w;
  qr_.z=q.x*q_.y-q.y*q_.x+q.w*q_.z+q.z*q_.w;
  qr_.w=q.w*q_.w-q.x*q_.x-q.y*q_.y-q.z*q_.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(quat<T> &qr_, typename math<T>::scalar_t s_)
{
  // multiply quat by scalar
  qr_.x*=s_;
  qr_.y*=s_;
  qr_.z*=s_;
  qr_.w*=s_;
}
//----

template<typename T>
EGL_INLINE void operator/=(quat<T> &qr_, typename math<T>::scalar_t s_)
{
  // divide quat by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  qr_.x*=rs;
  qr_.y*=rs;
  qr_.z*=rs;
  qr_.w*=rs;
}
//----

template<typename T>
EGL_INLINE quat<T> operator+(const quat<T> &q0_, const quat<T> &q1_)
{
  // add quat to quat
  return quat<T>(q0_.x+q1_.x, q0_.y+q1_.y, q0_.z+q1_.z, q0_.w+q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> operator+(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the quat
  return quat<T>(q_.x+s_, q_.y+s_, q_.z+s_, q_.w+s_);
}
//----

template<typename T>
EGL_INLINE quat<T> operator+(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // add each component of the quat to a scalar quat
  return quat<T>(s_+q_.x, s_+q_.y, s_+q_.z, s_+q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> operator-(const quat<T> &q0_, const quat<T> &q1_)
{
  // subtract quat from quat
  return quat<T>(q0_.x-q1_.x, q0_.y-q1_.y, q0_.z-q1_.z, q0_.w-q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> operator-(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the quat
  return quat<T>(q_.x-s_, q_.y-s_, q_.z-s_, q_.w-s_);
}
//----

template<typename T>
EGL_INLINE quat<T> operator-(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // subtract each component of the quat from a scalar quat
  return quat<T>(s_-q_.x, s_-q_.y, s_-q_.z, s_-q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> operator-(const quat<T> &q_)
{
  // negate quat
  return quat<T>(-q_.x, -q_.y, -q_.z, -q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> operator*(const quat<T> &q0_, const quat<T> &q1_)
{
  // multiply quat by quat
  return quat<T>(q0_.y*q1_.z-q0_.z*q1_.y+q0_.w*q1_.x+q0_.x*q1_.w,
                 q0_.z*q1_.x-q0_.x*q1_.z+q0_.w*q1_.y+q0_.y*q1_.w,
                 q0_.x*q1_.y-q0_.y*q1_.x+q0_.w*q1_.z+q0_.z*q1_.w,
                 q0_.w*q1_.w-q0_.x*q1_.x-q0_.y*q1_.y-q0_.z*q1_.z);
}
//----

template<typename T>
EGL_INLINE quat<T> operator*(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // multiply quat by scalar
  return quat<T>(q_.x*s_, q_.y*s_, q_.z*s_, q_.w*s_);
}
//----

template<typename T>
EGL_INLINE quat<T> operator*(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // multiply quat by scalar
  return quat<T>(q_.x*s_, q_.y*s_, q_.z*s_, q_.w*s_);
}
//----

template<typename T>
EGL_INLINE quat<T> operator/(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // divide quat by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return quat<T>(q_.x*rs, q_.y*rs, q_.z*rs, q_.w*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t min(const quat<T> &q_)
{
  // minimum component value
  return min(q_.x, q_.y, q_.z, q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> min(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise min quat
  return quat<T>(q0_.x<q1_.x?q0_.x:q1_.x,
                 q0_.y<q1_.y?q0_.y:q1_.y,
                 q0_.z<q1_.z?q0_.z:q1_.z,
                 q0_.w<q1_.w?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> min(const quat<T> &q0_, const quat<T> &q1_, const quat<T> &q2_)
{
  // component-wise min quat
  return quat<T>(min(q0_.x, q1_.x, q2_.x),
                 min(q0_.y, q1_.y, q2_.y),
                 min(q0_.z, q1_.z, q2_.z),
                 min(q0_.w, q1_.w, q2_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> min(const quat<T> &q0_, const quat<T> &q1_, const quat<T> &q2_, const quat<T> &q3_)
{
  // component-wise min quat
  return quat<T>(min(q0_.x, q1_.x, q2_.x, q3_.x),
                 min(q0_.y, q1_.y, q2_.y, q3_.y),
                 min(q0_.z, q1_.z, q2_.z, q3_.z),
                 min(q0_.w, q1_.w, q2_.w, q3_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> min(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // component-wise min quat
  return quat<T>(q_.x<s_?q_.x:s_,
                 q_.y<s_?q_.y:s_,
                 q_.z<s_?q_.z:s_,
                 q_.w<s_?q_.w:s_);
}
//----

template<typename T>
EGL_INLINE quat<T> min(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // component-wise min quat
  return quat<T>(s_<q_.x?s_:q_.x,
                 s_<q_.y?s_:q_.y,
                 s_<q_.z?s_:q_.z,
                 s_<q_.w?s_:q_.w);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t max(const quat<T> &q_)
{
  // maximum component value
  return max(q_.x, q_.y, q_.z, q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> max(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise max quat
  return quat<T>(q0_.x>q1_.x?q0_.x:q1_.x,
                 q0_.y>q1_.y?q0_.y:q1_.y,
                 q0_.z>q1_.z?q0_.z:q1_.z,
                 q0_.w>q1_.w?q0_.w:q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> max(const quat<T> &q0_, const quat<T> &q1_, const quat<T> &q2_)
{
  // component-wise max quat
  return quat<T>(max(q0_.x, q1_.x, q2_.x),
                 max(q0_.y, q1_.y, q2_.y),
                 max(q0_.z, q1_.z, q2_.z),
                 max(q0_.w, q1_.w, q2_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> max(const quat<T> &q0_, const quat<T> &q1_, const quat<T> &q2_, const quat<T> &q3_)
{
  // component-wise max quat
  return quat<T>(max(q0_.x, q1_.x, q2_.x, q3_.x),
                 max(q0_.y, q1_.y, q2_.y, q3_.y),
                 max(q0_.z, q1_.z, q2_.z, q3_.z),
                 max(q0_.w, q1_.w, q2_.w, q3_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> max(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // component-wise max quat
  return quat<T>(q_.x>s_?q_.x:s_,
                 q_.y>s_?q_.y:s_,
                 q_.z>s_?q_.z:s_,
                 q_.w>s_?q_.w:s_);
}
//----

template<typename T>
EGL_INLINE quat<T> max(typename math<T>::scalar_t s_, const quat<T> &q_)
{
  // component-wise max quat
  return quat<T>(s_>q_.x?s_:q_.x,
                 s_>q_.y?s_:q_.y,
                 s_>q_.z?s_:q_.z,
                 s_>q_.w?s_:q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> mul(const quat<T> &q0_, const quat<T> &q1_)
{
  // component-wise multiply: q0_*q1
  return quat<T>(q0_.x*q1_.x, q0_.y*q1_.y, q0_.z*q1_.z, q0_.w*q1_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> madd(const quat<T> &qm0_, const quat<T> &qm1_, const quat<T> &qa_)
{
  // component-wise multiply-add: qm0_*qm1_+qa_
  return quat<T>(qm0_.x*qm1_.x+qa_.x, qm0_.y*qm1_.y+qa_.y, qm0_.z*qm1_.z+qa_.z, qm0_.w*qm1_.w+qa_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> madd(const quat<T> &qm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: qm_*sm_+sa_
  return quat<T>(qm_.x*sm_+sa_, qm_.y*sm_+sa_, qm_.z*sm_+sa_, qm_.w*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE quat<T> madd(const quat<T> &qm0_, const quat<T> &qm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: qm0_*qm1_+sa_
  return quat<T>(qm0_.x*qm1_.x+sa_, qm0_.y*qm1_.y+sa_, qm0_.z*qm1_.z+sa_, qm0_.w*qm1_.w+sa_);
}
//----

template<typename T>
EGL_INLINE quat<T> madd(const quat<T> &qm_, typename math<T>::scalar_t sm_, const quat<T> &qa_)
{
  // component-wise multiply-add: qm_*sm_+qa_
  return quat<T>(qm_.x*sm_+qa_.x, qm_.y*sm_+qa_.y, qm_.z*sm_+qa_.z, qm_.w*sm_+qa_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> rcp(const quat<T> &q_)
{
  // component-wise reciprocal
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(q_.x && q_.y && q_.z && q_.w);
  return quat<T>(scalar_t(1)/q_.x, scalar_t(1)/q_.y, scalar_t(1)/q_.z, scalar_t(1)/q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> rcp_z(const quat<T> &q_)
{
  // component-wise reciprocal (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x?scalar_t(1)/q_.x:0, q_.y?scalar_t(1)/q_.y:0, q_.z?scalar_t(1)/q_.z:0, q_.w?scalar_t(1)/q_.w:0);
}
//----

template<typename T>
EGL_INLINE quat<T> floor(const quat<T> &q_)
{
  // component-wise floor
  return quat<T>(MATH_NAMESPACE::floor(q_.x), MATH_NAMESPACE::floor(q_.y), MATH_NAMESPACE::floor(q_.z), MATH_NAMESPACE::floor(q_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> ceil(const quat<T> &q_)
{
  // component-wise ceil
  return quat<T>(MATH_NAMESPACE::ceil(q_.x), MATH_NAMESPACE::ceil(q_.y), MATH_NAMESPACE::ceil(q_.z), MATH_NAMESPACE::ceil(q_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> trunc(const quat<T> &q_)
{
  // component-wise truncate
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(scalar_t(int(q_.x)), scalar_t(int(q_.y)), scalar_t(int(q_.z)), scalar_t(int(q_.w)));
}
//----

template<typename T>
EGL_INLINE quat<T> frc(const quat<T> &q_)
{
  // component-wise fractional
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x-scalar_t(int(q_.x)), q_.y-scalar_t(int(q_.y)), q_.z-scalar_t(int(q_.z)), q_.w-scalar_t(int(q_.w)));
}
//----

template<typename T>
EGL_INLINE quat<T> mod(const quat<T> &q_, typename math<T>::scalar_t div_)
{
  // component-wise modulo
  return quat<T>(MATH_NAMESPACE::fmod(q_.x, div_), MATH_NAMESPACE::fmod(q_.y, div_), MATH_NAMESPACE::fmod(q_.z, div_), MATH_NAMESPACE::fmod(q_.w, div_));
}
//----

template<typename T>
EGL_INLINE quat<T> cycle(const quat<T> &q_, typename math<T>::scalar_t cycle_)
{
  // component-wise cycle
  return quat<T>(MATH_NAMESPACE::fmod(q_.x, cycle_)+(q_.x<0?cycle_:0),
                 MATH_NAMESPACE::fmod(q_.y, cycle_)+(q_.y<0?cycle_:0),
                 MATH_NAMESPACE::fmod(q_.z, cycle_)+(q_.z<0?cycle_:0),
                 MATH_NAMESPACE::fmod(q_.w, cycle_)+(q_.w<0?cycle_:0));
}
//----

template<typename T>
EGL_INLINE quat<T> cycle1(const quat<T> &q_)
{
  // cycle1
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x-scalar_t(int(q_.x))+(q_.x<0?scalar_t(1):0),
                 q_.y-scalar_t(int(q_.y))+(q_.y<0?scalar_t(1):0),
                 q_.z-scalar_t(int(q_.z))+(q_.z<0?scalar_t(1):0),
                 q_.w-scalar_t(int(q_.w))+(q_.w<0?scalar_t(1):0));
}
//----

template<typename T>
EGL_INLINE quat<T> sat(const quat<T> &q_)
{
  // component-wise saturate
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<0?0:q_.x>scalar_t(1)?scalar_t(1):q_.x,
                 q_.y<0?0:q_.y>scalar_t(1)?scalar_t(1):q_.y,
                 q_.z<0?0:q_.z>scalar_t(1)?scalar_t(1):q_.z,
                 q_.w<0?0:q_.w>scalar_t(1)?scalar_t(1):q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> ssat(const quat<T> &q_)
{
  // component-wise signed saturate
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<scalar_t(-1)?scalar_t(-1):q_.x>scalar_t(1)?scalar_t(1):q_.x,
                 q_.y<scalar_t(-1)?scalar_t(-1):q_.y>scalar_t(1)?scalar_t(1):q_.y,
                 q_.z<scalar_t(-1)?scalar_t(-1):q_.z>scalar_t(1)?scalar_t(1):q_.z,
                 q_.w<scalar_t(-1)?scalar_t(-1):q_.w>scalar_t(1)?scalar_t(1):q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> clamp(const quat<T> &q_, const quat<T> &min_, const quat<T> &max_)
{
  // component-wise clamp (to quats)
  return quat<T>(q_.x<min_.x?min_.x:q_.x>max_.x?max_.x:q_.x,
                 q_.y<min_.y?min_.y:q_.y>max_.y?max_.y:q_.y,
                 q_.z<min_.z?min_.z:q_.z>max_.z?max_.z:q_.z,
                 q_.w<min_.w?min_.w:q_.w>max_.w?max_.w:q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> clamp(const quat<T> &q_, typename math<T>::scalar_t min_, typename math<T>::scalar_t max_)
{
  // component-wise clamp (to scalars)
  return quat<T>(q_.x<min_?min_:q_.x>max_?max_:q_.x,
                 q_.y<min_?min_:q_.y>max_?max_:q_.y,
                 q_.z<min_?min_:q_.z>max_?max_:q_.z,
                 q_.w<min_?min_:q_.w>max_?max_:q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> abs(const quat<T> &q_)
{
  // component-wise absolute
  return quat<T>(q_.x<0?-q_.x:q_.x, q_.y<0?-q_.y:q_.y, q_.z<0?-q_.z:q_.z, q_.w<0?-q_.w:q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sgn(const quat<T> &q_)
{
  // component-wise signum (neg=-1, pos=+1, zero=0)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<0?scalar_t(-1):q_.x>0?scalar_t(1):0, q_.y<0?scalar_t(-1):q_.y>0?scalar_t(1):0, q_.z<0?scalar_t(-1):q_.z>0?scalar_t(1):0, q_.w<0?scalar_t(-1):q_.w>0?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE quat<T> sgn_zp(const quat<T> &q_)
{
  // component-wise signum (neg=-1, pos&zero=+1)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<0?scalar_t(-1):scalar_t(1), q_.y<0?scalar_t(-1):scalar_t(1), q_.z<0?scalar_t(-1):scalar_t(1), q_.w<0?scalar_t(-1):scalar_t(1));
}
//----

template<typename T>
EGL_INLINE quat<T> sqr(const quat<T> &q_)
{
  // component-wise square
  return quat<T>(q_.x*q_.x, q_.y*q_.y, q_.z*q_.z, q_.w*q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> cubic(const quat<T> &q_)
{
  // component-wise cubic
  return quat<T>(q_.x*q_.x*q_.x, q_.y*q_.y*q_.y, q_.z*q_.z*q_.z, q_.w*q_.w*q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> sqrt(const quat<T> &q_)
{
  // component-wise square root
  EGL_ASSERT_PEDANTIC(q_.x>=0 && q_.y>=0 && q_.z>=0 && q_.w>=0);
  return quat<T>(MATH_NAMESPACE::sqrt(q_.x), MATH_NAMESPACE::sqrt(q_.y), MATH_NAMESPACE::sqrt(q_.z), MATH_NAMESPACE::sqrt(q_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> sqrt_z(const quat<T> &q_)
{
  // component-wise square root (set to 0 for components less than zero)
  return quat<T>(sqrt_z(q_.x), sqrt_z(q_.y), sqrt_z(q_.z), sqrt_z(q_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> cbrt(const quat<T> &q_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<0?-MATH_NAMESPACE::pow(-q_.x, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(q_.x, scalar_t(1.0/3.0)),
                 q_.y<0?-MATH_NAMESPACE::pow(-q_.y, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(q_.y, scalar_t(1.0/3.0)),
                 q_.z<0?-MATH_NAMESPACE::pow(-q_.z, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(q_.z, scalar_t(1.0/3.0)),
                 q_.w<0?-MATH_NAMESPACE::pow(-q_.w, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(q_.w, scalar_t(1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE quat<T> rsqrt(const quat<T> &q_)
{
  // component-wise reciprocal square root
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(q_.x>0 && q_.y>0 && q_.z>0 && q_.w>0);
  return quat<T>(scalar_t(1)/MATH_NAMESPACE::sqrt(q_.x), scalar_t(1)/MATH_NAMESPACE::sqrt(q_.y), scalar_t(1)/MATH_NAMESPACE::sqrt(q_.z), scalar_t(1)/MATH_NAMESPACE::sqrt(q_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> rsqrt_z(const quat<T> &q_)
{
  // component-wise reciprocal square root (set to 0 for components less than or equal to zero)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x>0?scalar_t(1)/MATH_NAMESPACE::sqrt(q_.x):0,
                 q_.y>0?scalar_t(1)/MATH_NAMESPACE::sqrt(q_.y):0,
                 q_.z>0?scalar_t(1)/MATH_NAMESPACE::sqrt(q_.z):0,
                 q_.w>0?scalar_t(1)/MATH_NAMESPACE::sqrt(q_.w):0);
}
//----

template<typename T>
EGL_INLINE quat<T> rcbrt(const quat<T> &q_)
{
  // component-wise reciprocal cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x<0?-MATH_NAMESPACE::pow(-q_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.x, scalar_t(-1.0/3.0)),
                 q_.y<0?-MATH_NAMESPACE::pow(-q_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.y, scalar_t(-1.0/3.0)),
                 q_.z<0?-MATH_NAMESPACE::pow(-q_.z, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.z, scalar_t(-1.0/3.0)),
                 q_.w<0?-MATH_NAMESPACE::pow(-q_.w, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.w, scalar_t(-1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE quat<T> rcbrt_z(const quat<T> &q_)
{
  // component-wise reciprocal cubic root (set to 0 for 0 component)
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(q_.x?q_.x<0?-MATH_NAMESPACE::pow(-q_.x, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.x, scalar_t(-1.0/3.0)):0,
                 q_.y?q_.y<0?-MATH_NAMESPACE::pow(-q_.y, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.y, scalar_t(-1.0/3.0)):0,
                 q_.z?q_.z<0?-MATH_NAMESPACE::pow(-q_.z, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.z, scalar_t(-1.0/3.0)):0,
                 q_.w?q_.w<0?-MATH_NAMESPACE::pow(-q_.w, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(q_.w, scalar_t(-1.0/3.0)):0);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm(const quat<T> &q_)
{
  // norm of the quat (=quat length)
  return MATH_NAMESPACE::sqrt(q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm(const quat<T> &q_)
{
  // reciprocal norm of the quat
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_z(const quat<T> &q_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  return n2>0?scalar_t(1)/MATH_NAMESPACE::sqrt(n2):0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm2(const quat<T> &q_)
{
  // squared norm of the quat
  return q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2(const quat<T> &q_)
{
  // reciprocal squared norm of the quat
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/n2;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2_z(const quat<T> &q_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  return v>0?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE quat<T> unit(const quat<T> &q_)
{
  // unit quat of the quat
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return quat<T>(q_.x*rs, q_.y*rs, q_.z*rs, q_.w*rs);
}
//----

template<typename T>
EGL_INLINE quat<T> unit_z(const quat<T> &q_)
{
  // unit quat of the quat. if |q|=0, return q=[v=0, w=1]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  if(!rs)
    return quat<T>(0, 0, 0, scalar_t(1));
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return quat<T>(q_.x*rs, q_.y*rs, q_.z*rs, q_.w*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot(const quat<T> &q0_, const quat<T> &q1_)
{
  // dot-product of quats
  return q0_.x*q1_.x+q0_.y*q1_.y+q0_.z*q1_.z+q0_.w*q1_.w;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot1(const quat<T> &q_)
{
  // dot-product with [1, 1, 1, 1]
  return q_.x+q_.y+q_.z+q_.w;
}
//----

template<typename T>
EGL_INLINE quat<T> cross(const quat<T> &q0_, const quat<T> &q1_)
{
  // cross-product of quats: cross(q0, q1)=[v0 x v1, 0]
  return quat<T>(q0_.y*q1_.z-q0_.z*q1_.y,
                 q0_.z*q1_.x-q0_.x*q1_.z,
                 q0_.x*q1_.y-q0_.y*q1_.x,
                 0);
}
//----

template<typename T>
EGL_INLINE quat<T> outer(const quat<T> &q0_, const quat<T> &q1_)
{
  // Euclidean outer-product of quats: outer(q0, q1)=[w0*v1-w1*v0-(v0 x v1), 0]
  return quat<T>(q0_.w*q1_.x-q1_.w*q0_.x-q0_.y*q1_.z+q0_.z*q1_.y,
                 q0_.w*q1_.y-q1_.w*q0_.y-q0_.z*q1_.x+q0_.x*q1_.z,
                 q0_.w*q1_.z-q1_.w*q0_.z-q0_.x*q1_.y+q0_.y*q1_.x,
                 0);
}
//----

template<typename T>
EGL_INLINE quat<T> even(const quat<T> &q0_, const quat<T> &q1_)
{
  // even-product of quats: even(q0, q1)=[w0*v1+w1*v0, w0*w1-dot(v0, v1)]
  return quat<T>(q0_.w*q1_.x+q1_.w*q0_.x,
                 q0_.w*q1_.y+q1_.w*q0_.y,
                 q0_.w*q1_.z+q1_.w*q0_.z,
                 q0_.w*q1_.w-q0_.x*q1_.x-q0_.y*q1_.y-q0_.z*q1_.z);
}
//----

template<typename T>
EGL_INLINE quat<T> conj(const quat<T> &q_)
{
  // conjugate of the quat
  return quat<T>(-q_.x, -q_.y, -q_.z, q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> inv(const quat<T> &q_)
{
  // inverse of the quat
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rn2=scalar_t(1)/n2;
  return quat<T>(-q_.x*rn2, -q_.y*rn2, -q_.z*rn2, q_.w*rn2);
}
//----

template<typename T>
EGL_INLINE quat<T> inv_u(const quat<T> &q_)
{
  // inverse of the unit-quat
  return quat<T>(-q_.x, -q_.y, -q_.z, q_.w);
}
//----

template<typename T>
EGL_INLINE quat<T> exp(const quat<T> &q_)
{
  // quat natural exponent: exp(w)*[sin(|v|)*v/|v|, cos(|v|)]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t vn=MATH_NAMESPACE::sqrt(q_.x*q_.x+q_.y*q_.y+q_.z*q_.z);
  EGL_ASSERT_PEDANTIC(vn);
  scalar_t s, c;
  sincos(s, c, vn);
  scalar_t ew=exp(q_.w);
  scalar_t vf=ew*s/vn;
  return quat<T>(q_.x*vf, q_.y*vf, q_.z*vf, ew*c);
}
//----

template<typename T>
EGL_INLINE quat<T> exp_u(const quat<T> &q_)
{
  // unit quat natural exponent: exp(w)*[sin(|v|)*v/|v|, cos(|v|)]
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(q_.w<=scalar_t(1));
  scalar_t vn=MATH_NAMESPACE::sqrt(scalar_t(1)-q_.w*q_.w);
  EGL_ASSERT_PEDANTIC(vn);
  scalar_t s, c;
  sincos(s, c, vn);
  scalar_t ew=exp(q_.w);
  scalar_t vf=ew*s/vn;
  return quat<T>(q_.x*vf, q_.y*vf, q_.z*vf, ew*c);
}
//----

template<typename T>
EGL_INLINE quat<T> ln(const quat<T> &q_)
{
  // quat natural logarithm: [acos(w/|q|)*v/|v|, ln(|q|)]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t vn2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z;
  scalar_t n2=vn2+q_.w*q_.w;
  EGL_ASSERT_PEDANTIC(vn2 && n2);
  scalar_t rqn=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  scalar_t vf=scalar_t(1)/MATH_NAMESPACE::sqrt(vn2)*acos(q_.w*rqn);
  return quat<T>(q_.x*vf, q_.y*vf, q_.z*vf, -ln(rqn));
}
//----

template<typename T>
EGL_INLINE quat<T> ln_u(const quat<T> &q_)
{
  // unit quat natural logarithm: [acos(w)*v/|v|, 0]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t vn2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z;
  EGL_ASSERT_PEDANTIC(vn2);
  scalar_t vf=scalar_t(1)/MATH_NAMESPACE::sqrt(vn2)*acos(q_.w);
  return quat<T>(q_.x*vf, q_.y*vf, q_.z*vf, 0);
}
//----

template<typename T>
EGL_INLINE quat<T> pow(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // quat power: exp(ln(q)*s_)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t s, c;
  scalar_t vn2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z;
  scalar_t qn=MATH_NAMESPACE::sqrt(vn2+q_.w*q_.w);
  EGL_ASSERT_PEDANTIC(vn2 && qn);
  sincos(s, c, acos(q_.w/qn)*s_);
  scalar_t ew=pow(qn, s_);
  scalar_t vf=ew*s/MATH_NAMESPACE::sqrt(vn2);
  return quat<T>(q_.x*vf, q_.y*vf, q_.z*vf, ew*c);
}
//----

template<typename T>
EGL_INLINE quat<T> pow_u(const quat<T> &q_, typename math<T>::scalar_t s_)
{
  // unit quat power: exp(ln_u(q)*s_)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t vn=s_*acos(q_.w);
  EGL_ASSERT_PEDANTIC(vn);
  scalar_t s, c;
  sincos(s, c, vn);
  scalar_t vf=s/vn;
  return quat<T>(q_.x*vf, q_.y*vf, q_.z*vf, c);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t arg(const quat<T> &q_)
{
  // argument of the quat
  typename math<T>::scalar_t n2=q_.x*q_.x+q_.y*q_.y+q_.z*q_.z+q_.w*q_.w;
  EGL_ASSERT_PEDANTIC(n2);
  return acos(q_.w/MATH_NAMESPACE::sqrt(n2));
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t arg_u(const quat<T> &q_)
{
  // argument of the unit quat
  return acos(q_.w);
}
//----

template<typename T>
EGL_INLINE void neg(quat<T> &q_)
{
  // negate the quat
  q_.x=-q_.x;
  q_.y=-q_.y;
  q_.z=-q_.z;
  q_.w=-q_.w;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_u(quat<T> &qr_, Rng &rng_)
{
  // random unit quaternion (even distribution on unit hypersphere)
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs, rc, vs, vc;
  sincos(rs, rc, scalar_t(rng_.rand_ureal1())*math<T>::pi);
  sincos(vs, vc, scalar_t(rng_.rand_ureal1())*math<T>::two_pi);
  scalar_t z=scalar_t(rng_.rand_real1());
  qr_.z=z*rs;
  qr_.w=rc;
  scalar_t rad=MATH_NAMESPACE::sqrt(scalar_t(1)-z*z)*rs;
  qr_.x=vs*rad;
  qr_.y=vc*rad;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_real1(quat<T> &qr_, Rng &rng_)
{
  // random quaternion where each component is in range [-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  qr_.x=scalar_t(rng_.rand_real1());
  qr_.y=scalar_t(rng_.rand_real1());
  qr_.z=scalar_t(rng_.rand_real1());
  qr_.w=scalar_t(rng_.rand_real1());
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_ureal1(quat<T> &qr_, Rng &rng_)
{
  // random quaternion where each component is in range [0, 1]
  typedef typename math<T>::scalar_t scalar_t;
  qr_.x=scalar_t(rng_.rand_ureal1());
  qr_.y=scalar_t(rng_.rand_ureal1());
  qr_.z=scalar_t(rng_.rand_ureal1());
  qr_.w=scalar_t(rng_.rand_ureal1());
}
//----

template<typename T>
EGL_INLINE quat<T> smoothstep(const quat<T> &q_)
{
  // component-wise smoothstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(q_.x>=0 && q_.x<=scalar_t(1) && q_.y>=0 && q_.y<=scalar_t(1) && q_.z>=0 && q_.z<=scalar_t(1) && q_.w>=0 && q_.w<=scalar_t(1));
  return quat<T>(q_.x*q_.x*(scalar_t(3)-scalar_t(2)*q_.x),
                 q_.y*q_.y*(scalar_t(3)-scalar_t(2)*q_.y),
                 q_.z*q_.z*(scalar_t(3)-scalar_t(2)*q_.z),
                 q_.w*q_.w*(scalar_t(3)-scalar_t(2)*q_.w));
}
//----

template<typename T>
EGL_INLINE quat<T> smootherstep(const quat<T> &q_)
{
  // component-wise smootherstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(q_.x>=0 && q_.x<=scalar_t(1) && q_.y>=0 && q_.y<=scalar_t(1) && q_.z>=0 && q_.z<=scalar_t(1) && q_.w>=0 && q_.w<=scalar_t(1));
  return quat<T>(q_.x*q_.x*q_.x*(q_.x*(q_.x*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 q_.y*q_.y*q_.y*(q_.y*(q_.y*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 q_.z*q_.z*q_.z*(q_.z*(q_.z*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                 q_.w*q_.w*q_.w*(q_.w*(q_.w*scalar_t(6)-scalar_t(15))+scalar_t(10)));
}
//----

template<typename T>
EGL_INLINE quat<T> lerp(const quat<T> &q0_, const quat<T> &q1_, typename math<T>::scalar_t t_)
{
  // linear quat interpolation. f(t=0)=q0, f(t=1)=q1
  return quat<T>(q0_.x+(q1_.x-q0_.x)*t_, q0_.y+(q1_.y-q0_.y)*t_, q0_.z+(q1_.z-q0_.z)*t_, q0_.w+(q1_.w-q0_.w)*t_);
}
//----

template<typename T>
EGL_INLINE quat<T> nlerp(const quat<T> &q0_, const quat<T> &q1_, typename math<T>::scalar_t t_)
{
  // normalized linear quat interpolation. f(t=0)=q0/|q0|, f(t=1)=q1/|q1|
  typedef typename math<T>::scalar_t scalar_t;
  quat<T> q(q0_.x+(q1_.x-q0_.x)*t_, q0_.y+(q1_.y-q0_.y)*t_, q0_.z+(q1_.z-q0_.z)*t_, q0_.w+(q1_.w-q0_.w)*t_);
  scalar_t n2=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return quat<T>(q.x*rs, q.y*rs, q.z*rs, q.w*rs);
}
//----

template<typename T>
EGL_INLINE quat<T> nlerp_z(const quat<T> &q0_, const quat<T> &q1_, typename math<T>::scalar_t t_)
{
  // normalized linear quat interpolation. f(t=0)=q0/|q0|, f(t=1)=q1/|q1|. if interpolated |q|=0, return [0, 0, 0, 1]
  typedef typename math<T>::scalar_t scalar_t;
  quat<T> q(q0_.x+(q1_.x-q0_.x)*t_, q0_.y+(q1_.y-q0_.y)*t_, q0_.z+(q1_.z-q0_.z)*t_, q0_.w+(q1_.w-q0_.w)*t_);
  scalar_t rs=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
  if(!rs)
    return quat<T>(0, 0, 0, scalar_t(1));
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return quat<T>(q.x*rs, q.y*rs, q.z*rs, q.w*rs);
}
//----

template<typename T>
EGL_INLINE quat<T> slerp(const quat<T> &q0_, const quat<T> &q1_, typename math<T>::scalar_t t_)
{
  // spherical linear quat interpolation. f(t=0)=q0, f(t=1)=q1
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t a=acos(q0_.x*q1_.x+q0_.y*q1_.y+q0_.z*q1_.z+q0_.w*q1_.w);
  scalar_t rs=csc(a);
  return q0_*(sin((scalar_t(1)-t_)*a)*rs)+q1_*(sin(t_*a)*rs);
}
//----

template<typename T>
EGL_INLINE quat<T> rot_u(const vec3<T> &src_, const vec3<T> &dst_)
{
  // generate rotation from source to destination direction
  vec3<T> v=unit_z(src_+dst_);
  return quat<T>(cross(v, dst_), dot(v, dst_));
}
//----

template<typename T>
EGL_INLINE quat<T> zrot_u(const vec3<T> &dst_)
{
  // generate rotation from z-axis to destination direction
  typedef typename math<T>::scalar_t scalar_t;
  vec3<T> v=unit_z(dst_+vec3<T>(0, 0, scalar_t(1)));
  if(is_zero(v))
    v.set(scalar_t(1), 0, 0);
  return quat<T>(cross(vec3<T>(0, 0, scalar_t(1)), v), v.z);
}
//----

template<typename T>
EGL_INLINE quat<T> axis_rot(const vec3<T> &axis_, typename math<T>::scalar_t angle_)
{
  // generate rotation from axis and angle (radians)
  typedef typename math<T>::scalar_t scalar_t;
  if(!norm2(axis_))
    return quat<T>(0, 0, 0, scalar_t(1));
  scalar_t s, c;
  sincos(s, c, angle_*scalar_t(0.5));
  return quat<T>(unit(axis_)*s, c);
}
//----

template<typename T>
EGL_INLINE quat<T> axis_rot_u(const vec3<T> &axis_, typename math<T>::scalar_t angle_)
{
  // generate rotation from unit axis and angle (radians)
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC_MSG(abs(norm2(axis_)-scalar_t(1))<scalar_t(0.0005), "Axis vector length must be 1.0 (%f)\r\n", norm(axis_));
  scalar_t s, c;
  sincos(s, c, angle_*scalar_t(0.5));
  return quat<T>(axis_*s, c);
}
//----

template<typename T>
void convert_u(mat33<T> &m_, const quat<T> &q_)
{
  // convert the unit quat to 3x3 row major rotation matrix
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t xs=q_.x+q_.x;
  scalar_t ys=q_.y+q_.y;
  scalar_t zs=q_.z+q_.z;
  scalar_t xx=q_.x*xs;
  scalar_t yy=q_.y*ys;
  scalar_t zz=q_.z*zs;
  m_.x.x=scalar_t(1)-yy-zz;
  m_.y.y=scalar_t(1)-xx-zz;
  m_.z.z=scalar_t(1)-xx-yy;
  scalar_t xy=q_.x*ys;
  scalar_t wz=q_.w*zs;
  m_.x.y=xy+wz;
  m_.y.x=xy-wz;
  scalar_t xz=q_.x*zs;
  scalar_t wy=q_.w*ys;
  m_.x.z=xz-wy;
  m_.z.x=xz+wy;
  scalar_t yz=q_.y*zs;
  scalar_t wx=q_.w*xs;
  m_.y.z=yz+wx;
  m_.z.y=yz-wx;
}
//----

template<typename T>
void convert(mat33<T> &m_, const quat<T> &q_)
{
  // convert the quat to 3x3 row major rotation matrix
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t s=rnorm2(q_);
  s+=s;
  scalar_t xs=q_.x*s;
  scalar_t ys=q_.y*s;
  scalar_t zs=q_.z*s;
  scalar_t xx=q_.x*xs;
  scalar_t yy=q_.y*ys;
  scalar_t zz=q_.z*zs;
  m_.x.x=scalar_t(1)-yy-zz;
  m_.y.y=scalar_t(1)-xx-zz;
  m_.z.z=scalar_t(1)-xx-yy;
  scalar_t xy=q_.x*ys;
  scalar_t wz=q_.w*zs;
  m_.x.y=xy+wz;
  m_.y.x=xy-wz;
  scalar_t xz=q_.x*zs;
  scalar_t wy=q_.w*ys;
  m_.x.z=xz-wy;
  m_.z.x=xz+wy;
  scalar_t yz=q_.y*zs;
  scalar_t wx=q_.w*xs;
  m_.y.z=yz+wx;
  m_.z.y=yz-wx;
}
//----

template<typename T>
void convert(quat<T> &q_, const mat33<T> &m_)
{
  // check for positive matrix trace
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC_MSG(abs(det(m_)-scalar_t(1))<scalar_t(0.01), "Matrix determinant must be 1 for quaternion conversion (det = %f)\r\n", float(det(m_)));
  scalar_t tr=m_.x.x+m_.y.y+m_.z.z;
  if(tr>0)
  {
    scalar_t s=MATH_NAMESPACE::sqrt(tr+scalar_t(1));
    q_.w=s*scalar_t(0.5);
    s=scalar_t(0.5)/s;
    q_.x=(m_.y.z-m_.z.y)*s;
    q_.y=(m_.z.x-m_.x.z)*s;
    q_.z=(m_.x.y-m_.y.x)*s;
  }
  else
  {
    // find largest diagonal value and setup element indices
    unsigned i=m_.y.y>m_.x.x?1:0;
    if(m_.z.z>m_[i][i])
      i=2;
    const unsigned next[3]={1, 2, 0};
    unsigned j=next[i], k=next[j];

    // convert the matrix
    scalar_t s=MATH_NAMESPACE::sqrt(m_[i][i]-m_[j][j]-m_[k][k]+scalar_t(1));
    q_[i]=s*scalar_t(0.5);
    s=scalar_t(0.5)/s;
    q_.w=(m_[j][k]-m_[k][j])*s;
    q_[j]=(m_[i][j]+m_[j][i])*s;
    q_[k]=(m_[i][k]+m_[k][i])*s;
  }
}
//----

template<typename T, typename U>
EGL_INLINE quat<T> to_quat(const quat<U> &q_)
{
  // convert between quat types
  typedef typename math<T>::scalar_t scalar_t;
  return quat<T>(scalar_t(q_.x), scalar_t(q_.y), scalar_t(q_.z), scalar_t(q_.w));
}
//----

namespace priv
{
  template<unsigned shuffle_, typename T>
  struct shuffle_quat_helper
  {
    static quat<T> shuffle(const quat<T>&) {EGL_STATIC_ERROR(T, invalid_quaternion_component_shuffle);}
  };
  #define EGL_QUAT_SHUFFLE(shuffle__, x__, y__, z__, w__)\
  template<typename T> struct shuffle_quat_helper<shuffle__, T>\
  {\
    static EGL_INLINE quat<T> shuffle(const quat<T> &q_) {return quat<T>(q_.x__, q_.y__, q_.z__, q_.w__);}\
  };
  EGL_QUAT_SHUFFLE(0000, x, x, x, x); EGL_QUAT_SHUFFLE(0001, x, x, x, y); EGL_QUAT_SHUFFLE(0002, x, x, x, z); EGL_QUAT_SHUFFLE(0003, x, x, x, w); EGL_QUAT_SHUFFLE(0010, x, x, y, x); EGL_QUAT_SHUFFLE(0011, x, x, y, y); EGL_QUAT_SHUFFLE(0012, x, x, y, z); EGL_QUAT_SHUFFLE(0013, x, x, y, w); EGL_QUAT_SHUFFLE(0020, x, x, z, x); EGL_QUAT_SHUFFLE(0021, x, x, z, y); EGL_QUAT_SHUFFLE(0022, x, x, z, z); EGL_QUAT_SHUFFLE(0023, x, x, z, w); EGL_QUAT_SHUFFLE(0030, x, x, w, x); EGL_QUAT_SHUFFLE(0031, x, x, w, y); EGL_QUAT_SHUFFLE(0032, x, x, w, z); EGL_QUAT_SHUFFLE(0033, x, x, w, w);
  EGL_QUAT_SHUFFLE(0100, x, y, x, x); EGL_QUAT_SHUFFLE(0101, x, y, x, y); EGL_QUAT_SHUFFLE(0102, x, y, x, z); EGL_QUAT_SHUFFLE(0103, x, y, x, w); EGL_QUAT_SHUFFLE(0110, x, y, y, x); EGL_QUAT_SHUFFLE(0111, x, y, y, y); EGL_QUAT_SHUFFLE(0112, x, y, y, z); EGL_QUAT_SHUFFLE(0113, x, y, y, w); EGL_QUAT_SHUFFLE(0120, x, y, z, x); EGL_QUAT_SHUFFLE(0121, x, y, z, y); EGL_QUAT_SHUFFLE(0122, x, y, z, z); EGL_QUAT_SHUFFLE(0123, x, y, z, w); EGL_QUAT_SHUFFLE(0130, x, y, w, x); EGL_QUAT_SHUFFLE(0131, x, y, w, y); EGL_QUAT_SHUFFLE(0132, x, y, w, z); EGL_QUAT_SHUFFLE(0133, x, y, w, w);
  EGL_QUAT_SHUFFLE(0200, x, z, x, x); EGL_QUAT_SHUFFLE(0201, x, z, x, y); EGL_QUAT_SHUFFLE(0202, x, z, x, z); EGL_QUAT_SHUFFLE(0203, x, z, x, w); EGL_QUAT_SHUFFLE(0210, x, z, y, x); EGL_QUAT_SHUFFLE(0211, x, z, y, y); EGL_QUAT_SHUFFLE(0212, x, z, y, z); EGL_QUAT_SHUFFLE(0213, x, z, y, w); EGL_QUAT_SHUFFLE(0220, x, z, z, x); EGL_QUAT_SHUFFLE(0221, x, z, z, y); EGL_QUAT_SHUFFLE(0222, x, z, z, z); EGL_QUAT_SHUFFLE(0223, x, z, z, w); EGL_QUAT_SHUFFLE(0230, x, z, w, x); EGL_QUAT_SHUFFLE(0231, x, z, w, y); EGL_QUAT_SHUFFLE(0232, x, z, w, z); EGL_QUAT_SHUFFLE(0233, x, z, w, w);
  EGL_QUAT_SHUFFLE(0300, x, w, x, x); EGL_QUAT_SHUFFLE(0301, x, w, x, y); EGL_QUAT_SHUFFLE(0302, x, w, x, z); EGL_QUAT_SHUFFLE(0303, x, w, x, w); EGL_QUAT_SHUFFLE(0310, x, w, y, x); EGL_QUAT_SHUFFLE(0311, x, w, y, y); EGL_QUAT_SHUFFLE(0312, x, w, y, z); EGL_QUAT_SHUFFLE(0313, x, w, y, w); EGL_QUAT_SHUFFLE(0320, x, w, z, x); EGL_QUAT_SHUFFLE(0321, x, w, z, y); EGL_QUAT_SHUFFLE(0322, x, w, z, z); EGL_QUAT_SHUFFLE(0323, x, w, z, w); EGL_QUAT_SHUFFLE(0330, x, w, w, x); EGL_QUAT_SHUFFLE(0331, x, w, w, y); EGL_QUAT_SHUFFLE(0332, x, w, w, z); EGL_QUAT_SHUFFLE(0333, x, w, w, w);
  EGL_QUAT_SHUFFLE(1000, y, x, x, x); EGL_QUAT_SHUFFLE(1001, y, x, x, y); EGL_QUAT_SHUFFLE(1002, y, x, x, z); EGL_QUAT_SHUFFLE(1003, y, x, x, w); EGL_QUAT_SHUFFLE(1010, y, x, y, x); EGL_QUAT_SHUFFLE(1011, y, x, y, y); EGL_QUAT_SHUFFLE(1012, y, x, y, z); EGL_QUAT_SHUFFLE(1013, y, x, y, w); EGL_QUAT_SHUFFLE(1020, y, x, z, x); EGL_QUAT_SHUFFLE(1021, y, x, z, y); EGL_QUAT_SHUFFLE(1022, y, x, z, z); EGL_QUAT_SHUFFLE(1023, y, x, z, w); EGL_QUAT_SHUFFLE(1030, y, x, w, x); EGL_QUAT_SHUFFLE(1031, y, x, w, y); EGL_QUAT_SHUFFLE(1032, y, x, w, z); EGL_QUAT_SHUFFLE(1033, y, x, w, w);
  EGL_QUAT_SHUFFLE(1100, y, y, x, x); EGL_QUAT_SHUFFLE(1101, y, y, x, y); EGL_QUAT_SHUFFLE(1102, y, y, x, z); EGL_QUAT_SHUFFLE(1103, y, y, x, w); EGL_QUAT_SHUFFLE(1110, y, y, y, x); EGL_QUAT_SHUFFLE(1111, y, y, y, y); EGL_QUAT_SHUFFLE(1112, y, y, y, z); EGL_QUAT_SHUFFLE(1113, y, y, y, w); EGL_QUAT_SHUFFLE(1120, y, y, z, x); EGL_QUAT_SHUFFLE(1121, y, y, z, y); EGL_QUAT_SHUFFLE(1122, y, y, z, z); EGL_QUAT_SHUFFLE(1123, y, y, z, w); EGL_QUAT_SHUFFLE(1130, y, y, w, x); EGL_QUAT_SHUFFLE(1131, y, y, w, y); EGL_QUAT_SHUFFLE(1132, y, y, w, z); EGL_QUAT_SHUFFLE(1133, y, y, w, w);
  EGL_QUAT_SHUFFLE(1200, y, z, x, x); EGL_QUAT_SHUFFLE(1201, y, z, x, y); EGL_QUAT_SHUFFLE(1202, y, z, x, z); EGL_QUAT_SHUFFLE(1203, y, z, x, w); EGL_QUAT_SHUFFLE(1210, y, z, y, x); EGL_QUAT_SHUFFLE(1211, y, z, y, y); EGL_QUAT_SHUFFLE(1212, y, z, y, z); EGL_QUAT_SHUFFLE(1213, y, z, y, w); EGL_QUAT_SHUFFLE(1220, y, z, z, x); EGL_QUAT_SHUFFLE(1221, y, z, z, y); EGL_QUAT_SHUFFLE(1222, y, z, z, z); EGL_QUAT_SHUFFLE(1223, y, z, z, w); EGL_QUAT_SHUFFLE(1230, y, z, w, x); EGL_QUAT_SHUFFLE(1231, y, z, w, y); EGL_QUAT_SHUFFLE(1232, y, z, w, z); EGL_QUAT_SHUFFLE(1233, y, z, w, w);
  EGL_QUAT_SHUFFLE(1300, y, w, x, x); EGL_QUAT_SHUFFLE(1301, y, w, x, y); EGL_QUAT_SHUFFLE(1302, y, w, x, z); EGL_QUAT_SHUFFLE(1303, y, w, x, w); EGL_QUAT_SHUFFLE(1310, y, w, y, x); EGL_QUAT_SHUFFLE(1311, y, w, y, y); EGL_QUAT_SHUFFLE(1312, y, w, y, z); EGL_QUAT_SHUFFLE(1313, y, w, y, w); EGL_QUAT_SHUFFLE(1320, y, w, z, x); EGL_QUAT_SHUFFLE(1321, y, w, z, y); EGL_QUAT_SHUFFLE(1322, y, w, z, z); EGL_QUAT_SHUFFLE(1323, y, w, z, w); EGL_QUAT_SHUFFLE(1330, y, w, w, x); EGL_QUAT_SHUFFLE(1331, y, w, w, y); EGL_QUAT_SHUFFLE(1332, y, w, w, z); EGL_QUAT_SHUFFLE(1333, y, w, w, w);
  EGL_QUAT_SHUFFLE(2000, z, x, x, x); EGL_QUAT_SHUFFLE(2001, z, x, x, y); EGL_QUAT_SHUFFLE(2002, z, x, x, z); EGL_QUAT_SHUFFLE(2003, z, x, x, w); EGL_QUAT_SHUFFLE(2010, z, x, y, x); EGL_QUAT_SHUFFLE(2011, z, x, y, y); EGL_QUAT_SHUFFLE(2012, z, x, y, z); EGL_QUAT_SHUFFLE(2013, z, x, y, w); EGL_QUAT_SHUFFLE(2020, z, x, z, x); EGL_QUAT_SHUFFLE(2021, z, x, z, y); EGL_QUAT_SHUFFLE(2022, z, x, z, z); EGL_QUAT_SHUFFLE(2023, z, x, z, w); EGL_QUAT_SHUFFLE(2030, z, x, w, x); EGL_QUAT_SHUFFLE(2031, z, x, w, y); EGL_QUAT_SHUFFLE(2032, z, x, w, z); EGL_QUAT_SHUFFLE(2033, z, x, w, w);
  EGL_QUAT_SHUFFLE(2100, z, y, x, x); EGL_QUAT_SHUFFLE(2101, z, y, x, y); EGL_QUAT_SHUFFLE(2102, z, y, x, z); EGL_QUAT_SHUFFLE(2103, z, y, x, w); EGL_QUAT_SHUFFLE(2110, z, y, y, x); EGL_QUAT_SHUFFLE(2111, z, y, y, y); EGL_QUAT_SHUFFLE(2112, z, y, y, z); EGL_QUAT_SHUFFLE(2113, z, y, y, w); EGL_QUAT_SHUFFLE(2120, z, y, z, x); EGL_QUAT_SHUFFLE(2121, z, y, z, y); EGL_QUAT_SHUFFLE(2122, z, y, z, z); EGL_QUAT_SHUFFLE(2123, z, y, z, w); EGL_QUAT_SHUFFLE(2130, z, y, w, x); EGL_QUAT_SHUFFLE(2131, z, y, w, y); EGL_QUAT_SHUFFLE(2132, z, y, w, z); EGL_QUAT_SHUFFLE(2133, z, y, w, w);
  EGL_QUAT_SHUFFLE(2200, z, z, x, x); EGL_QUAT_SHUFFLE(2201, z, z, x, y); EGL_QUAT_SHUFFLE(2202, z, z, x, z); EGL_QUAT_SHUFFLE(2203, z, z, x, w); EGL_QUAT_SHUFFLE(2210, z, z, y, x); EGL_QUAT_SHUFFLE(2211, z, z, y, y); EGL_QUAT_SHUFFLE(2212, z, z, y, z); EGL_QUAT_SHUFFLE(2213, z, z, y, w); EGL_QUAT_SHUFFLE(2220, z, z, z, x); EGL_QUAT_SHUFFLE(2221, z, z, z, y); EGL_QUAT_SHUFFLE(2222, z, z, z, z); EGL_QUAT_SHUFFLE(2223, z, z, z, w); EGL_QUAT_SHUFFLE(2230, z, z, w, x); EGL_QUAT_SHUFFLE(2231, z, z, w, y); EGL_QUAT_SHUFFLE(2232, z, z, w, z); EGL_QUAT_SHUFFLE(2233, z, z, w, w);
  EGL_QUAT_SHUFFLE(2300, z, w, x, x); EGL_QUAT_SHUFFLE(2301, z, w, x, y); EGL_QUAT_SHUFFLE(2302, z, w, x, z); EGL_QUAT_SHUFFLE(2303, z, w, x, w); EGL_QUAT_SHUFFLE(2310, z, w, y, x); EGL_QUAT_SHUFFLE(2311, z, w, y, y); EGL_QUAT_SHUFFLE(2312, z, w, y, z); EGL_QUAT_SHUFFLE(2313, z, w, y, w); EGL_QUAT_SHUFFLE(2320, z, w, z, x); EGL_QUAT_SHUFFLE(2321, z, w, z, y); EGL_QUAT_SHUFFLE(2322, z, w, z, z); EGL_QUAT_SHUFFLE(2323, z, w, z, w); EGL_QUAT_SHUFFLE(2330, z, w, w, x); EGL_QUAT_SHUFFLE(2331, z, w, w, y); EGL_QUAT_SHUFFLE(2332, z, w, w, z); EGL_QUAT_SHUFFLE(2333, z, w, w, w);
  EGL_QUAT_SHUFFLE(3000, w, x, x, x); EGL_QUAT_SHUFFLE(3001, w, x, x, y); EGL_QUAT_SHUFFLE(3002, w, x, x, z); EGL_QUAT_SHUFFLE(3003, w, x, x, w); EGL_QUAT_SHUFFLE(3010, w, x, y, x); EGL_QUAT_SHUFFLE(3011, w, x, y, y); EGL_QUAT_SHUFFLE(3012, w, x, y, z); EGL_QUAT_SHUFFLE(3013, w, x, y, w); EGL_QUAT_SHUFFLE(3020, w, x, z, x); EGL_QUAT_SHUFFLE(3021, w, x, z, y); EGL_QUAT_SHUFFLE(3022, w, x, z, z); EGL_QUAT_SHUFFLE(3023, w, x, z, w); EGL_QUAT_SHUFFLE(3030, w, x, w, x); EGL_QUAT_SHUFFLE(3031, w, x, w, y); EGL_QUAT_SHUFFLE(3032, w, x, w, z); EGL_QUAT_SHUFFLE(3033, w, x, w, w);
  EGL_QUAT_SHUFFLE(3100, w, y, x, x); EGL_QUAT_SHUFFLE(3101, w, y, x, y); EGL_QUAT_SHUFFLE(3102, w, y, x, z); EGL_QUAT_SHUFFLE(3103, w, y, x, w); EGL_QUAT_SHUFFLE(3110, w, y, y, x); EGL_QUAT_SHUFFLE(3111, w, y, y, y); EGL_QUAT_SHUFFLE(3112, w, y, y, z); EGL_QUAT_SHUFFLE(3113, w, y, y, w); EGL_QUAT_SHUFFLE(3120, w, y, z, x); EGL_QUAT_SHUFFLE(3121, w, y, z, y); EGL_QUAT_SHUFFLE(3122, w, y, z, z); EGL_QUAT_SHUFFLE(3123, w, y, z, w); EGL_QUAT_SHUFFLE(3130, w, y, w, x); EGL_QUAT_SHUFFLE(3131, w, y, w, y); EGL_QUAT_SHUFFLE(3132, w, y, w, z); EGL_QUAT_SHUFFLE(3133, w, y, w, w);
  EGL_QUAT_SHUFFLE(3200, w, z, x, x); EGL_QUAT_SHUFFLE(3201, w, z, x, y); EGL_QUAT_SHUFFLE(3202, w, z, x, z); EGL_QUAT_SHUFFLE(3203, w, z, x, w); EGL_QUAT_SHUFFLE(3210, w, z, y, x); EGL_QUAT_SHUFFLE(3211, w, z, y, y); EGL_QUAT_SHUFFLE(3212, w, z, y, z); EGL_QUAT_SHUFFLE(3213, w, z, y, w); EGL_QUAT_SHUFFLE(3220, w, z, z, x); EGL_QUAT_SHUFFLE(3221, w, z, z, y); EGL_QUAT_SHUFFLE(3222, w, z, z, z); EGL_QUAT_SHUFFLE(3223, w, z, z, w); EGL_QUAT_SHUFFLE(3230, w, z, w, x); EGL_QUAT_SHUFFLE(3231, w, z, w, y); EGL_QUAT_SHUFFLE(3232, w, z, w, z); EGL_QUAT_SHUFFLE(3233, w, z, w, w);
  EGL_QUAT_SHUFFLE(3300, w, w, x, x); EGL_QUAT_SHUFFLE(3301, w, w, x, y); EGL_QUAT_SHUFFLE(3302, w, w, x, z); EGL_QUAT_SHUFFLE(3303, w, w, x, w); EGL_QUAT_SHUFFLE(3310, w, w, y, x); EGL_QUAT_SHUFFLE(3311, w, w, y, y); EGL_QUAT_SHUFFLE(3312, w, w, y, z); EGL_QUAT_SHUFFLE(3313, w, w, y, w); EGL_QUAT_SHUFFLE(3320, w, w, z, x); EGL_QUAT_SHUFFLE(3321, w, w, z, y); EGL_QUAT_SHUFFLE(3322, w, w, z, z); EGL_QUAT_SHUFFLE(3323, w, w, z, w); EGL_QUAT_SHUFFLE(3330, w, w, w, x); EGL_QUAT_SHUFFLE(3331, w, w, w, y); EGL_QUAT_SHUFFLE(3332, w, w, w, z); EGL_QUAT_SHUFFLE(3333, w, w, w, w);
  #undef EGL_QUAT_SHUFFLE
} // namespace priv
//----

template<unsigned shuffle_, typename T>
EGL_INLINE quat<T> shuffle(const quat<T> &q_)
{
  return priv::shuffle_quat_helper<shuffle_, T>::shuffle(q_);
}
//----

template<typename T>
EGL_INLINE void pack_s1(quat_32s &qr_, const quat<T> &q_)
{
  // pack quat (components in range [-1, 1]) to 32-bit quat
  EGL_ASSERT_PEDANTIC_MSG(is_ssat(q_), "All quaternion components [%f, %f, %f, %f] must be in range [-1, 1]\r\n", float(q_.x), float(q_.y), float(q_.z), float(q_.w));
  typedef typename math<T>::scalar_t scalar_t;
  qr_.x=int8_t(q_.x*scalar_t(127.999));
  qr_.y=int8_t(q_.y*scalar_t(127.999));
  qr_.z=int8_t(q_.z*scalar_t(127.999));
  qr_.w=int8_t(q_.w*scalar_t(127.999));
}
//----

template<typename T>
EGL_INLINE void pack_u1(quat_32u &qr_, const quat<T> &q_)
{
  // pack quat (components in range [0, 1]) to 32-bit quat
  EGL_ASSERT_PEDANTIC_MSG(is_sat(q_), "All quaternion components [%f, %f, %f, %f] must be in range [0, 1]\r\n", float(q_.x), float(q_.y), float(q_.z), float(q_.w));
  typedef typename math<T>::scalar_t scalar_t;
  qr_.x=uint8_t(q_.x*scalar_t(255.999));
  qr_.y=uint8_t(q_.y*scalar_t(255.999));
  qr_.z=uint8_t(q_.z*scalar_t(255.999));
  qr_.w=uint8_t(q_.w*scalar_t(255.999));
}
//----

template<typename T>
EGL_INLINE void unpack_s1(quat<T> &qr_, const quat_32s &q_)
{
  // unpack quat (components in range [-1, 1]) from 32-bit quat
  typedef typename math<T>::scalar_t scalar_t;
  qr_.x=q_.x*scalar_t(1.0/127.0);
  qr_.y=q_.y*scalar_t(1.0/127.0);
  qr_.z=q_.z*scalar_t(1.0/127.0);
  qr_.w=q_.w*scalar_t(1.0/127.0);
}
//----

template<typename T>
EGL_INLINE void unpack_u1(quat<T> &qr_, const quat_32u &q_)
{
  // unpack quat (components in range [0, 1]) from 32-bit quat
  typedef typename math<T>::scalar_t scalar_t;
  qr_.x=q_.x*scalar_t(1.0/255.0);
  qr_.y=q_.y*scalar_t(1.0/255.0);
  qr_.z=q_.z*scalar_t(1.0/255.0);
  qr_.w=q_.w*scalar_t(1.0/255.0);
}
//----

template<typename T>
EGL_INLINE const vec3<T> &vec(const quat<T> &q_)
{
  // return vector of quat
  return reinterpret_cast<const vec3<T>&>(q_);
}
//----

template<typename T>
EGL_INLINE vec3<T> &vec(quat<T> &q_)
{
  // return vector of quat
  return reinterpret_cast<vec3<T>&>(q_);
}
//----------------------------------------------------------------------------


//============================================================================
// complex
//============================================================================
template<typename T>
complex<T> complex<T>::zero()
{
  return complex<T>(scalar_t(0));
}
//----

template<typename T>
complex<T> complex<T>::one()
{
  return complex<T>(scalar_t(1));
}
//----

template<typename T>
complex<T> complex<T>::neg_one()
{
  return complex<T>(scalar_t(-1));
}
//----------------------------------------------------------------------------

template<typename T>
complex<T>::complex()
{
}
//----

template<typename T>
complex<T>::complex(T s_)
  :real(s_)
  ,imag(s_)
{
  // init complex with real scalar
}
//----

template<typename T>
complex<T>::complex(T real_, T imag_)
  :real(real_)
  ,imag(imag_)
{
  // init complex with real and imaginary scalars
}
//----

template<typename T>
complex<T>::complex(const T *a_)
{
  // init complex with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  real=a_[0];
  imag=a_[1];
}
//----------------------------------------------------------------------------

template<typename T>
const T &complex<T>::operator[](unsigned idx_) const
{
  // return reference to nth component (real=0, imag=1)
  EGL_ASSERT_PEDANTIC(idx_<2);
  return reinterpret_cast<const T*>(this)[idx_];
}
//----

template<typename T>
T &complex<T>::operator[](unsigned idx_)
{
  // return reference to nth component (real=0, imag=1)
  EGL_ASSERT_PEDANTIC(idx_<2);
  return reinterpret_cast<T*>(this)[idx_];
}
//----

template<typename T>
void complex<T>::set(T s_)
{
  // set complex with scalar
  real=s_;
  imag=s_;
}
//----

template<typename T>
void complex<T>::set(T real_, T imag_)
{
  // set complex with real and imaginary scalars
  real=real_;
  imag=imag_;
}
//----

template<typename T>
void complex<T>::set(const complex<T> &c_)
{
  // set complex with complex
  real=c_.real;
  imag=c_.imag;
}
//----

template<typename T>
void complex<T>::set(const T *a_)
{
  // init complex with an array of scalars
  EGL_ASSERT_PEDANTIC(a_);
  real=a_[0];
  imag=a_[1];
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const complex<T> &c_)
{
  // test for zero-complex number
  return c_.real==0 && c_.imag==0;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const complex<T> &c_)
{
  // test for saturated complex number
  typedef typename math<T>::scalar_t scalar_t;
  return    c_.real>=0 && c_.real<=scalar_t(1)
         && c_.imag>=0 && c_.imag<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const complex<T> &c_)
{
  // test for signed saturated complex number
  typedef typename math<T>::scalar_t scalar_t;
  return    c_.real>=scalar_t(-1) && c_.real<=scalar_t(1)
         && c_.imag>=scalar_t(-1) && c_.imag<=scalar_t(1);
}
//----

template<typename T>
EGL_INLINE bool operator==(const complex<T> &c0_, const complex<T> &c1_)
{
  // test for equality of complex numbers
  return c0_.real==c1_.real && c0_.imag==c1_.imag;
}
//----

template<typename T>
EGL_INLINE bool operator==(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // test for equality of complex numbers and scalar
  return c_.real==s_ && c_.imag==s_;
}
//----

template<typename T>
EGL_INLINE bool operator==(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // test for equality of complex numbers and scalar
  return s_==c_.real && s_==c_.imag;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const complex<T> &c0_, const complex<T> &c1_)
{
  // test for inequality of complex numbers
  return c0_.real!=c1_.real || c0_.imag!=c1_.imag;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // test for inequality of complex number and scalar
  return c_.real!=s_ || c_.imag!=s_;
}
//----

template<typename T>
EGL_INLINE bool operator!=(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // test for inequality of complex number and scalar
  return s_!=c_.real || s_!=c_.imag;
}
//----

template<typename T>
EGL_INLINE complex<T> operator<(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c0_.real<c1_.real?scalar_t(1):0, c0_.imag<c1_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator<(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<s_?scalar_t(1):0, c_.imag<s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator<(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // component-wise less-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(s_<c_.real?scalar_t(1):0, s_<c_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator>(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c0_.real>c1_.real?scalar_t(1):0, c0_.imag>c1_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator>(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real>s_?scalar_t(1):0, c_.imag>s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator>(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // component-wise greater-than (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(s_>c_.real?scalar_t(1):0, s_>c_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator<=(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c0_.real<=c1_.real?scalar_t(1):0, c0_.imag<=c1_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator<=(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<=s_?scalar_t(1):0, c_.imag<=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator<=(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // component-wise less-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(s_<=c_.real?scalar_t(1):0, s_<=c_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator>=(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c0_.real>=c1_.real?scalar_t(1):0, c0_.imag>=c1_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator>=(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real>=s_?scalar_t(1):0, c_.imag>=s_?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> operator>=(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // component-wise greater-or-equal (false=0, true=1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(s_>=c_.real?scalar_t(1):0, s_>=c_.imag?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> sel_eq(const complex<T> &t0_, const complex<T> &t1_, const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise equal complex number select (t0_==t1_?c0_:c1_)
  return complex<T>(t0_.real==t1_.real?c0_.real:c1_.real,
                    t0_.imag==t1_.imag?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sel_lt(const complex<T> &t0_, const complex<T> &t1_, const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise less-than complex number select (t0_<t1_?c0_:c1_)
  return complex<T>(t0_.real<t1_.real?c0_.real:c1_.real,
                    t0_.imag<t1_.imag?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sel_le(const complex<T> &t0_, const complex<T> &t1_, const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise less-or-equal complex number select (t0_<=t1_?c0_:c1_)
  return complex<T>(t0_.real<=t1_.real?c0_.real:c1_.real,
                    t0_.imag<=t1_.imag?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sel_eqz(const complex<T> &t_, const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise equal-to-zero complex number select (t_==0?c0_:c1_)
  return complex<T>(t_.real==0?c0_.real:c1_.real,
                    t_.imag==0?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sel_ltz(const complex<T> &t_, const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise less-than-zero complex number select (t_<0?c0_:c1_)
  return complex<T>(t_.real<0?c0_.real:c1_.real,
                    t_.imag<0?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sel_lez(const complex<T> &t_, const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise less-or-equal-to-zero complex number select (t_<=0?c0_:c1_)
  return complex<T>(t_.real<=0?c0_.real:c1_.real,
                    t_.imag<=0?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE void operator+=(complex<T> &cr_, const complex<T> &c_)
{
  // add complex to complex
  cr_.real+=c_.real;
  cr_.imag+=c_.imag;
}
//----

template<typename T>
EGL_INLINE void operator+=(complex<T> &cr_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the complex
  cr_.real+=s_;
  cr_.imag+=s_;
}
//----

template<typename T>
EGL_INLINE void operator-=(complex<T> &cr_, const complex<T> &c_)
{
  // subtract complex from complex
  cr_.real-=c_.real;
  cr_.imag-=c_.imag;
}
//----

template<typename T>
EGL_INLINE void operator-=(complex<T> &cr_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the complex
  cr_.real-=s_;
  cr_.imag-=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(complex<T> &cr_, const complex<T> &c_)
{
  // multiply complex by complex
  typename math<T>::scalar_t i=cr_.real*c_.imag+cr_.imag*c_.real;
  cr_.real=cr_.real*c_.real-cr_.imag*c_.imag;
  cr_.imag=i;
}
//----

template<typename T>
EGL_INLINE void operator*=(complex<T> &cr_, typename math<T>::scalar_t s_)
{
  // multiply complex by scalar
  cr_.real*=s_;
  cr_.imag*=s_;
}
//----

template<typename T>
EGL_INLINE void operator/=(complex<T> &cr_, typename math<T>::scalar_t s_)
{
  // divide complex by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  cr_.real*=rs;
  cr_.imag*=rs;
}
//----

template<typename T>
EGL_INLINE complex<T> operator+(const complex<T> &c0_, const complex<T> &c1_)
{
  // add complex to complex
  return complex<T>(c0_.real+c1_.real, c0_.imag+c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> operator+(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // add scalar to each component of the complex
  return complex<T>(c_.real+s_, c_.imag+s_);
}
//----

template<typename T>
EGL_INLINE complex<T> operator+(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // add each component of the complex to a scalar complex
  return complex<T>(s_+c_.real, s_+c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> operator-(const complex<T> &c0_, const complex<T> &c1_)
{
  // subtract complex from complex
  return complex<T>(c0_.real-c1_.real, c0_.imag-c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> operator-(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // subtract scalar from each component of the complex
  return complex<T>(c_.real-s_, c_.imag-s_);
}
//----

template<typename T>
EGL_INLINE complex<T> operator-(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // subtract each component of the complex from a scalar complex
  return complex<T>(s_-c_.real, s_-c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> operator-(const complex<T> &c_)
{
  // negate complex
  return complex<T>(-c_.real, -c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> operator*(const complex<T> &c0_, const complex<T> &c1_)
{
  // multiply complex by complex
  return complex<T>(c0_.real*c1_.real-c0_.imag*c1_.imag,
                    c0_.real*c1_.imag+c0_.imag*c1_.real);
}
//----

template<typename T>
EGL_INLINE complex<T> operator*(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // multiply complex by scalar
  return complex<T>(c_.real*s_, c_.imag*s_);
}
//----

template<typename T>
EGL_INLINE complex<T> operator*(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // multiply complex by scalar
  return complex<T>(c_.real*s_, c_.imag*s_);
}
//----

template<typename T>
EGL_INLINE complex<T> operator/(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // divide complex by scalar
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(s_);
  scalar_t rs=scalar_t(1)/s_;
  return complex<T>(c_.real*rs, c_.imag*rs);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t min(const complex<T> &c_)
{
  // minimum component value
  return min(c_.real, c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> min(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise min complex
  return complex<T>(c0_.real<c1_.real?c0_.real:c1_.real,
                    c0_.imag<c1_.imag?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> min(const complex<T> &c0_, const complex<T> &c1_, const complex<T> &c2_)
{
  // component-wise min complex
  return complex<T>(min(c0_.real, c1_.real, c2_.real),
                    min(c0_.imag, c1_.imag, c2_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> min(const complex<T> &c0_, const complex<T> &c1_, const complex<T> &c2_, const complex<T> &c3_)
{
  // component-wise min complex
  return complex<T>(min(c0_.real, c1_.real, c2_.real, c3_.real),
                    min(c0_.imag, c1_.imag, c2_.imag, c3_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> min(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // component-wise min complex
  return complex<T>(c_.real<s_?c_.real:s_,
                    c_.imag<s_?c_.imag:s_);
}
//----

template<typename T>
EGL_INLINE complex<T> min(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // component-wise min complex
  return complex<T>(c_.real<s_?c_.real:s_,
                    c_.imag<s_?c_.imag:s_);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t max(const complex<T> &c_)
{
  // maximum component value
  return max(c_.real, c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> max(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise max complex
  return complex<T>(c0_.real>c1_.real?c0_.real:c1_.real,
                    c0_.imag>c1_.imag?c0_.imag:c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> max(const complex<T> &c0_, const complex<T> &c1_, const complex<T> &c2_)
{
  // component-wise max complex
  return complex<T>(max(c0_.real, c1_.real, c2_.real),
                    max(c0_.imag, c1_.imag, c2_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> max(const complex<T> &c0_, const complex<T> &c1_, const complex<T> &c2_, const complex<T> &c3_)
{
  // component-wise max complex
  return complex<T>(max(c0_.real, c1_.real, c2_.real, c3_.real),
                    max(c0_.imag, c1_.imag, c2_.imag, c3_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> max(const complex<T> &c_, typename math<T>::scalar_t s_)
{
  // component-wise max complex
  return complex<T>(c_.real>s_?c_.real:s_,
                    c_.imag>s_?c_.imag:s_);
}
//----

template<typename T>
EGL_INLINE complex<T> max(typename math<T>::scalar_t s_, const complex<T> &c_)
{
  // component-wise max complex
  return complex<T>(c_.real>s_?c_.real:s_,
                    c_.imag>s_?c_.imag:s_);
}
//----

template<typename T>
EGL_INLINE complex<T> mul(const complex<T> &c0_, const complex<T> &c1_)
{
  // component-wise multiply: c0_*c1
  return complex<T>(c0_.real*c1_.real, c0_.imag*c1_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> madd(const complex<T> &cm0_, const complex<T> &cm1_, const complex<T> &ca_)
{
  // component-wise multiply-add: cm0_*cm1_+ca_
  return complex<T>(cm0_.real*cm1_.real+ca_.real, cm0_.imag*cm1_.imag+ca_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> madd(const complex<T> &cm_, typename math<T>::scalar_t sm_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: cm_*sm_+sa_
  return complex<T>(cm_.real*sm_+sa_, cm_.imag*sm_+sa_);
}
//----

template<typename T>
EGL_INLINE complex<T> madd(const complex<T> &cm0_, const complex<T> &cm1_, typename math<T>::scalar_t sa_)
{
  // component-wise multiply-add: cm0_*cm1_+sa_
  return complex<T>(cm0_.real*cm1_.real+sa_, cm0_.imag*cm1_.imag+sa_);
}
//----

template<typename T>
EGL_INLINE complex<T> madd(const complex<T> &cm_, typename math<T>::scalar_t sm_, const complex<T> &ca_)
{
  // component-wise multiply-add: cm_*sm_+ca_
  return complex<T>(cm_.real*sm_+ca_.real, cm_.imag*sm_+ca_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> rcp(const complex<T> &c_)
{
  // component-wise reciprocal
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(c_.real && c_.imag);
  return complex<T>(scalar_t(1)/c_.real, scalar_t(1)/c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> rcp_z(const complex<T> &c_)
{
  // component-wise reciprocal (set to 0 for 0 components)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real?scalar_t(1)/c_.real:0, c_.imag?scalar_t(1)/c_.imag:0);
}
//----

template<typename T>
EGL_INLINE complex<T> floor(const complex<T> &c_)
{
  // component-wise floor
  return complex<T>(MATH_NAMESPACE::floor(c_.real), MATH_NAMESPACE::floor(c_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> ceil(const complex<T> &c_)
{
  // component-wise ceil
  return complex<T>(MATH_NAMESPACE::ceil(c_.real), MATH_NAMESPACE::ceil(c_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> trunc(const complex<T> &c_)
{
  // component-wise truncate
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(scalar_t(int(c_.real)), scalar_t(int(c_.imag)));
}
//----

template<typename T>
EGL_INLINE complex<T> frc(const complex<T> &c_)
{
  // component-wise fractional
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real-scalar_t(int(c_.real)), c_.imag-scalar_t(int(c_.imag)));
}
//----

template<typename T>
EGL_INLINE complex<T> mod(const complex<T> &c_, typename math<T>::scalar_t div_)
{
  // component-wise modulo
  return complex<T>(MATH_NAMESPACE::fmod(c_.real, div_), MATH_NAMESPACE::fmod(c_.imag, div_));
}
//----

template<typename T>
EGL_INLINE complex<T> cycle(const complex<T> &c_, typename math<T>::scalar_t cycle_)
{
  // component-wise cycle
  return complex<T>(MATH_NAMESPACE::fmod(c_.real, cycle_)+(c_.real<0?cycle_:0),
                    MATH_NAMESPACE::fmod(c_.imag, cycle_)+(c_.imag<0?cycle_:0));
}
//----

template<typename T>
EGL_INLINE complex<T> cycle1(const complex<T> &c_)
{
  // cycle1
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real-scalar_t(int(c_.real))+(c_.real<0?scalar_t(1):0),
                    c_.imag-scalar_t(int(c_.imag))+(c_.imag<0?scalar_t(1):0));
}
//----

template<typename T>
EGL_INLINE complex<T> sat(const complex<T> &c_)
{
  // component-wise saturate
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<0?0:c_.real>scalar_t(1)?scalar_t(1):c_.real,
                    c_.imag<0?0:c_.imag>scalar_t(1)?scalar_t(1):c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> ssat(const complex<T> &c_)
{
  // component-wise signed saturate
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<scalar_t(-1)?scalar_t(-1):c_.real>scalar_t(1)?scalar_t(1):c_.real,
                    c_.imag<scalar_t(-1)?scalar_t(-1):c_.imag>scalar_t(1)?scalar_t(1):c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> clamp(const complex<T> &c_, const complex<T> &min_, const complex<T> &max_)
{
  // component-wise clamp (to complex)
  return complex<T>(c_.real<min_.real?min_.real:c_.real>max_.real?max_.real:c_.real,
                    c_.imag<min_.imag?min_.imag:c_.imag>max_.imag?max_.imag:c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> clamp(const complex<T> &c_, typename math<T>::scalar_t min_, typename math<T>::scalar_t max_)
{
  // component-wise clamp (to scalar)
  return complex<T>(c_.real<min_?min_:c_.real>max_?max_:c_.real,
                    c_.imag<min_?min_:c_.imag>max_?max_:c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> abs(const complex<T> &c_)
{
  // component-wise absolute
  return complex<T>(c_.real<0?-c_.real:c_.real, c_.imag<0?-c_.imag:c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sgn(const complex<T> &c_)
{
  // component-wise signum (neg=-1, pos=+1, zero=0)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<0?scalar_t(-1):c_.real>0?scalar_t(1):0, c_.imag<0?scalar_t(-1):c_.imag>0?scalar_t(1):0);
}
//----

template<typename T>
EGL_INLINE complex<T> sgn_zp(const complex<T> &c_)
{
  // component-wise signum (neg=-1, pos&zero=+1)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<0?scalar_t(-1):scalar_t(1), c_.imag<0?scalar_t(-1):scalar_t(1));
}
//----

template<typename T>
EGL_INLINE complex<T> sqr(const complex<T> &c_)
{
  // component-wise square
  return complex<T>(c_.real*c_.real, c_.imag*c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> cubic(const complex<T> &c_)
{
  // component-wise cubic
  return complex<T>(c_.real*c_.real*c_.real, c_.imag*c_.imag*c_.imag);
}
//----

template<typename T>
EGL_INLINE complex<T> sqrt(const complex<T> &c_)
{
  // component-wise square root
  EGL_ASSERT_PEDANTIC(c_.real>=0 && c_.imag>=0);
  return complex<T>(MATH_NAMESPACE::sqrt(c_.real), MATH_NAMESPACE::sqrt(c_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> sqrt_z(const complex<T> &c_)
{
  // component-wise square root (set to 0 for components less than zero)
  return complex<T>(sqrt_z(c_.real), sqrt_z(c_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> cbrt(const complex<T> &c_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<0?-MATH_NAMESPACE::pow(-c_.real, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(c_.real, scalar_t(1.0/3.0)),
                    c_.imag<0?-MATH_NAMESPACE::pow(-c_.imag, scalar_t(1.0/3.0)): MATH_NAMESPACE::pow(c_.imag, scalar_t(1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE complex<T> rsqrt(const complex<T> &c_)
{
  // component-wise reciprocal square root
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(c_.real>0 && c_.imag>0);
  return complex<T>(scalar_t(1)/MATH_NAMESPACE::sqrt(c_.real), scalar_t(1)/MATH_NAMESPACE::sqrt(c_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> rsqrt_z(const complex<T> &c_)
{
  // component-wise reciprocal square root (set to 0 for components less than or equal to zero)
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real>0?scalar_t(1)/MATH_NAMESPACE::sqrt(c_.real):0,
                    c_.imag>0?scalar_t(1)/MATH_NAMESPACE::sqrt(c_.imag):0);
}
//----

template<typename T>
EGL_INLINE complex<T> rcbrt(const complex<T> &c_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real<0?-MATH_NAMESPACE::pow(-c_.real, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(c_.real, scalar_t(-1.0/3.0)),
                    c_.imag<0?-MATH_NAMESPACE::pow(-c_.imag, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(c_.imag, scalar_t(-1.0/3.0)));
}
//----

template<typename T>
EGL_INLINE complex<T> rcbrt_z(const complex<T> &c_)
{
  // component-wise cubic root
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(c_.real?c_.real<0?-MATH_NAMESPACE::pow(-c_.real, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(c_.real, scalar_t(-1.0/3.0)):0,
                    c_.imag?c_.imag<0?-MATH_NAMESPACE::pow(-c_.imag, scalar_t(-1.0/3.0)): MATH_NAMESPACE::pow(c_.imag, scalar_t(-1.0/3.0)):0);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm(const complex<T> &c_)
{
  // norm of the complex (=complex length)
  return MATH_NAMESPACE::sqrt(c_.real*c_.real+c_.imag*c_.imag);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm(const complex<T> &c_)
{
  // reciprocal norm of the complex
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=c_.real*c_.real+c_.imag*c_.imag;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm_z(const complex<T> &c_)
{
  // reciprocal norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=c_.real*c_.real+c_.imag*c_.imag;
  return n2>0?scalar_t(1)/MATH_NAMESPACE::sqrt(n2):0;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t norm2(const complex<T> &c_)
{
  // squared norm of the complex
  return c_.real*c_.real+c_.imag*c_.imag;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2(const complex<T> &c_)
{
  // reciprocal squared norm of the complex
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=c_.real*c_.real+c_.imag*c_.imag;
  EGL_ASSERT_PEDANTIC(n2);
  return scalar_t(1)/n2;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t rnorm2_z(const complex<T> &c_)
{
  // reciprocal squared norm of the vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t v=c_.real*c_.real+c_.imag*c_.imag;
  return v>0?scalar_t(1)/v:0;
}
//----

template<typename T>
EGL_INLINE complex<T> unit(const complex<T> &c_)
{
  // unit complex of the complex
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t n2=c_.real*c_.real+c_.imag*c_.imag;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return complex<T>(c_.real*rs, c_.imag*rs);
}
//----

template<typename T>
EGL_INLINE complex<T> unit_z(const complex<T> &c_)
{
  // unit complex of the complex. if |c|=0, return c=[0, 0]
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t rs=c_.real*c_.real+c_.imag*c_.imag;
  if(!rs)
    return complex<T>(0, 0);
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return complex<T>(c_.real*rs, c_.imag*rs);
}
//----

template<typename T>
EGL_INLINE complex<T> conj(const complex<T> &c_)
{
  // conjugate of the complex
  return complex<T>(c_.real, -c_.imag);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot(const complex<T> &c0_, const complex<T> &c1_)
{
  // dot-product of complex numbers
  return c0_.real*c1_.real+c0_.imag*c1_.imag;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t dot1(const complex<T> &c_)
{
  // dot-product with [1, 1]
  return c_.real+c_.imag;
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t arg(const complex<T> &c_)
{
  // argument of the complex
  return atan2(c_.imag, c_.real);
}
//----

template<typename T>
EGL_INLINE void neg(complex<T> &c_)
{
  // negate the complex
  c_.real=-c_.real;
  c_.imag=-c_.imag;
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_u(complex<T> &cr_, Rng &rng_)
{
  // random unit complex number (even distribution on unit circle)
  sincos(cr_.real, cr_.imag, typename math<T>::scalar_t(rng_.rand_ureal1())*math<T>::two_pi);
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_real1(complex<T> &cr_, Rng &rng_)
{
  // random complex number where each component is in range [-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  cr_.real=scalar_t(rng_.rand_real1());
  cr_.imag=scalar_t(rng_.rand_real1());
}
//----

template<typename T, class Rng>
EGL_INLINE void rand_ureal1(complex<T> &cr_, Rng &rng_)
{
  // random complex number where each component is in range [0, 1]
  typedef typename math<T>::scalar_t scalar_t;
  cr_.real=scalar_t(rng_.rand_ureal1());
  cr_.imag=scalar_t(rng_.rand_ureal1());
}
//----

template<typename T>
EGL_INLINE complex<T> smoothstep(const complex<T> &c_)
{
  // component-wise smoothstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(c_.real>=0 && c_.real<=scalar_t(1) && c_.imag>=0 && c_.imag<=scalar_t(1));
  return complex<T>(c_.real*c_.real*(scalar_t(3)-scalar_t(2)*c_.real),
                    c_.imag*c_.imag*(scalar_t(3)-scalar_t(2)*c_.imag));
}
//----

template<typename T>
EGL_INLINE complex<T> smootherstep(const complex<T> &c_)
{
  // component-wise smootherstep function
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT_PEDANTIC(c_.real>=0 && c_.real<=scalar_t(1) && c_.imag>=0 && c_.imag<=scalar_t(1));
  return complex<T>(c_.real*c_.real*c_.real*(c_.real*(c_.real*scalar_t(6)-scalar_t(15))+scalar_t(10)),
                    c_.imag*c_.imag*c_.imag*(c_.imag*(c_.imag*scalar_t(6)-scalar_t(15))+scalar_t(10)));
}
//----

template<typename T>
EGL_INLINE complex<T> lerp(const complex<T> &c0_, const complex<T> &c1_, typename math<T>::scalar_t t_)
{
  // linear complex interpolation. f(t=0)=c0, f(t=1)=c1
  return complex<T>(c0_.real+(c1_.real-c0_.real)*t_, c0_.imag+(c1_.imag-c0_.imag)*t_);
}
//----

template<typename T>
EGL_INLINE complex<T> nlerp(const complex<T> &c0_, const complex<T> &c1_, typename math<T>::scalar_t t_)
{
  // normalized linear complex interpolation. f(t=0)=c0/|c0|, f(t=1)=c1/|c1|
  typedef typename math<T>::scalar_t scalar_t;
  complex<T> c(c0_.real+(c1_.real-c0_.real)*t_, c0_.imag+(c1_.imag-c0_.imag)*t_);
  scalar_t n2=c.real*c.real+c.imag*c.imag;
  EGL_ASSERT_PEDANTIC(n2);
  scalar_t rs=scalar_t(1)/MATH_NAMESPACE::sqrt(n2);
  return complex<T>(c.real*rs, c.imag*rs);
}
//----

template<typename T>
EGL_INLINE complex<T> nlerp_z(const complex<T> &c0_, const complex<T> &c1_, typename math<T>::scalar_t t_)
{
  // normalized linear complex interpolation. f(t=0)=c0/|c0|, f(t=1)=c1/|c1|. if interpolated |c|=0, return [0, 0]
  typedef typename math<T>::scalar_t scalar_t;
  complex<T> c(c0_.real+(c1_.real-c0_.real)*t_, c0_.imag+(c1_.imag-c0_.imag)*t_);
  scalar_t rs=c.real*c.real+c.imag*c.imag;
  if(!rs)
    return complex<T>(0, 0);
  rs=scalar_t(1)/MATH_NAMESPACE::sqrt(rs);
  return complex<T>(c.real*rs, c.imag*rs);
}
//----

template<typename T, typename U>
EGL_INLINE complex<T> to_complex(const complex<U> &c_)
{
  // convert between complex types
  typedef typename math<T>::scalar_t scalar_t;
  return complex<T>(scalar_t(c_.real), scalar_t(c_.imag));
}
//----

namespace priv
{
  template<unsigned shuffle_, typename T>
  struct shuffle_complex_helper
  {
    static complex<T> shuffle(const complex<T>&) {EGL_STATIC_ERROR(T, invalid_complex_component_shuffle);}
  };
  #define EGL_COMPLEX_SHUFFLE(shuffle__, real__, imag__)\
  template<typename T> struct shuffle_complex_helper<shuffle__, T>\
  {\
    static EGL_INLINE complex<T> shuffle(const complex<T> &c_) {return complex<T>(c_.real__, c_.imag__);}\
  };
  EGL_COMPLEX_SHUFFLE(00, real, real);
  EGL_COMPLEX_SHUFFLE(01, real, imag);
  EGL_COMPLEX_SHUFFLE(10, imag, real);
  EGL_COMPLEX_SHUFFLE(11, imag, imag);
  #undef EGL_COMPLEX_SHUFFLE
} // namespace priv
//----

template<unsigned shuffle_, typename T>
EGL_INLINE complex<T> shuffle(const complex<T> &c_)
{
  return priv::shuffle_complex_helper<shuffle_, T>::shuffle(c_);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
