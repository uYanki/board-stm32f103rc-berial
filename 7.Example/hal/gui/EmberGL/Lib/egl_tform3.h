//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_TFORM3_H
#define EGL_TFORM3_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_math.h"
EGL_NAMESPACE_BEGIN

// new
template<typename> struct tform3;
typedef tform3<float> tform3f;
typedef tform3<double> tform3d;
template<typename> struct tform_rt3;
typedef tform_rt3<float> tform_rt3f;
typedef tform_rt3<double> tform_rt3d;
template<typename> class camera;
typedef camera<float> cameraf;
typedef camera<double> camerad;
// tform3 operations
template<typename T> EGL_INLINE bool is_zero(const tform3<T>&);                                      // test for zero-transform, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_orthogonal(const tform3<T>&);                                // test for orthogonal transform
template<typename T> EGL_INLINE bool is_sat(const tform3<T>&);                                       // test for saturated transform, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const tform3<T>&);                                      // test for signed saturated transform, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const tform3<T>&, const tform3<T>&);                 // test for equality of transforms, i.e. all components of transforms are equal (exact)
template<typename T> EGL_INLINE bool operator!=(const tform3<T>&, const tform3<T>&);                 // test for inequality of transforms, i.e. any of the components of the transforms are unequal (exact)
template<typename T> EGL_INLINE void operator+=(tform3<T>&, const tform3<T>&);                       // add transform to transform
template<typename T> EGL_INLINE void operator-=(tform3<T>&, const tform3<T>&);                       // subtract transform from transform
template<typename T> EGL_INLINE void operator*=(tform3<T>&, const tform3<T>&);                       // multiply transform by transform
template<typename T> EGL_INLINE void operator*=(tform3<T>&, const quat<T>&);                         // multiply transform by quaternion
template<typename T> EGL_INLINE void operator*=(tform3<T>&, const mat33<T>&);                        // multiply transform by matrix
template<typename T> EGL_INLINE void operator*=(tform3<T>&, typename math<T>::scalar_t);             // multiply transform by scalar
template<typename T> EGL_INLINE void operator*=(vec3<T>&, const tform3<T>&);                         // multiply 3d vector by transform (assume w=1)
template<typename T> EGL_INLINE void operator*=(vec4<T>&, const tform3<T>&);                         // multiply 4d vector by transform
template<typename T> EGL_INLINE void operator*=(mat44<T>&, const tform3<T>&);                        // multiply 4x4 matrix by transform
template<typename T> EGL_INLINE void operator/=(tform3<T>&, typename math<T>::scalar_t);             // divide transform by scalar
template<typename T> EGL_INLINE tform3<T> operator+(const tform3<T>&, const tform3<T>&);             // add transform to transform
template<typename T> EGL_INLINE tform3<T> operator-(const tform3<T>&, const tform3<T>&);             // subtract transform from transform
template<typename T> EGL_INLINE tform3<T> operator-(const tform3<T>&);                               // negate the transform
template<typename T> EGL_INLINE tform3<T> operator*(const tform3<T>&, const tform3<T>&);             // multiply transform by transform
template<typename T> EGL_INLINE tform3<T> operator*(const tform3<T>&, const quat<T>&);               // multiply transform by quaternion
template<typename T> EGL_INLINE tform3<T> operator*(const tform3<T>&, const mat33<T>&);              // multiply transform by matrix
template<typename T> EGL_INLINE tform3<T> operator*(const tform3<T>&, typename math<T>::scalar_t);   // multiply transform by scalar
template<typename T> EGL_INLINE tform3<T> operator*(typename math<T>::scalar_t, const tform3<T>&);   // multiply transform by scalar
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, const tform3<T>&);                 // multiply 3d vector by transform (assume w=1)
template<typename T> EGL_INLINE vec4<T> operator*(const vec4<T>&, const tform3<T>&);                 // multiply 4d vector by transform
template<typename T> EGL_INLINE vec3<T> operator*(const tform3<T>&, const vec3<T>&);                 // multiply transform by transposed 3d vector (assume w=1)
template<typename T> EGL_INLINE vec4<T> operator*(const tform3<T>&, const vec4<T>&);                 // multiply transform by transposed 4d vector
template<typename T> EGL_INLINE mat44<T> operator*(const mat44<T>&, const tform3<T>&);               // multiply 4x4 matrix by transform
template<typename T> EGL_INLINE mat44<T> operator*(const tform3<T>&, const mat44<T>&);               // multiply transform by 4x4 matrix
template<typename T> EGL_INLINE tform3<T> operator/(const tform3<T>&, typename math<T>::scalar_t);   // divide transform by scalar
template<typename T> EGL_INLINE tform3<T> madd(const tform3<T> &t0_,                                 // component-wise multiply-add: t0_*t1+t2_
                                               const tform3<T> &t1_,
                                               const tform3<T> &t2_);
template<typename T> EGL_INLINE typename math<T>::scalar_t det(const tform3<T>&);                    // determinant of the transform
template<typename T> EGL_INLINE tform3<T> inv(const tform3<T>&, typename math<T>::scalar_t *det_=0); // inverse of the transform
template<typename T> EGL_INLINE typename math<T>::scalar_t tr(const tform3<T>&);                     // trace of the transform
template<typename T> EGL_INLINE void identity(tform3<T>&);                                           // set transform to identity
template<typename T> EGL_INLINE void transpose_matrix33(tform3<T>&);                                 // transpose the 3x3 upper-left matrix of the transform
template<typename T> EGL_INLINE void transpose_matrix33(tform3<T>&, const tform3<T>&);               // transpose the 3x3 upper-left matrix of the transform
template<typename T> EGL_INLINE void neg(tform3<T>&);                                                // negate the transform
template<typename T> EGL_INLINE tform3<T> lerp(const tform3<T> &t0_,                                 // linear interpolation of transforms: f(t=0)=t0, f(t=1)=t1
                                               const tform3<T> &t1_,
                                               typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE void lerp(tform3<T> &tr_,                                            // linear interpolation of transforms: f(t=0)=t0, f(t=1)=t1
                                          const tform3<T> &t0_,
                                          const tform3<T> &t1_,
                                          typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE void zrot_u(tform3<T>&,                                              // setup transform from position and z-axis direction
                                            const vec3<T> &pos_,
                                            const vec3<T> &zdir_);
template<typename T> EGL_INLINE void zrot_u(tform3<T>&,                                              // setup transform from position, z-axis direction and up-vector
                                            const vec3<T> &pos_,
                                            const vec3<T> &zdir_,
                                            const vec3<T> &up_);
template<typename T> EGL_INLINE void axis_rot(tform3<T>&,                                            // setup transform from position and rotation about given axis
                                              const vec3<T> &pos_,
                                              const vec3<T> &axis_,
                                              typename math<T>::scalar_t angle_);
template<typename T> EGL_INLINE void axis_rot_u(tform3<T>&,                                          // setup transform from position and rotation about given unit axis
                                                const vec3<T> &pos_,
                                                const vec3<T> &axis_,
                                                typename math<T>::scalar_t angle_);
template<typename T, typename U> EGL_INLINE tform3<T> to_tform3(const tform3<U>&);                   // convert between tform3 types
template<typename T> EGL_INLINE void orthonormalize(tform3<T>&);                                     // ortho-normalize the transform
// tform_rt3 operations
template<typename T> EGL_INLINE bool is_zero(const tform_rt3<T>&);                                       // test for zero-transform, i.e. all components equal zero (exact)
template<typename T> EGL_INLINE bool is_sat(const tform3<T>&);                                           // test for saturated transform, i.e. all components are in range [0, 1]
template<typename T> EGL_INLINE bool is_ssat(const tform3<T>&);                                          // test for signed saturated transform, i.e. all components are in range [-1, 1]
template<typename T> EGL_INLINE bool operator==(const tform_rt3<T>&, const tform_rt3<T>&);               // test for equality of transforms, i.e. all components of transforms are equal (exact)
template<typename T> EGL_INLINE bool operator!=(const tform_rt3<T>&, const tform_rt3<T>&);               // test for inequality of transforms, i.e. any of the components of the transforms are unequal (exact)
template<typename T> EGL_INLINE void operator+=(tform_rt3<T>&, const tform_rt3<T>&);                     // add transform to transform
template<typename T> EGL_INLINE void operator-=(tform_rt3<T>&, const tform_rt3<T>&);                     // subtract transform from transform
template<typename T> EGL_INLINE void operator*=(tform_rt3<T>&, const tform_rt3<T>&);                     // multiply transform by transform
template<typename T> EGL_INLINE void operator*=(tform_rt3<T>&, const quat<T>&);                          // multiply transform by quaternion
template<typename T> EGL_INLINE void operator*=(tform_rt3<T>&, typename math<T>::scalar_t);              // multiply transform by scalar
template<typename T> EGL_INLINE void operator*=(vec3<T>&, const tform_rt3<T>&);                          // multiply 3d vector by transform (assume w=1)
template<typename T> EGL_INLINE void operator*=(vec4<T>&, const tform_rt3<T>&);                          // multiply 4d vector by transform
template<typename T> EGL_INLINE void operator*=(mat44<T>&, const tform_rt3<T>&);                         // multiply 4x4 matrix by transform
template<typename T> EGL_INLINE void operator/=(tform_rt3<T>&, typename math<T>::scalar_t);              // divide transform by scalar
template<typename T> EGL_INLINE tform_rt3<T> operator+(const tform_rt3<T>&, const tform_rt3<T>&);        // add transform to transform
template<typename T> EGL_INLINE tform_rt3<T> operator-(const tform_rt3<T>&, const tform_rt3<T>&);        // subtract transform from transform
template<typename T> EGL_INLINE tform_rt3<T> operator-(const tform_rt3<T>&);                             // negate the transform
template<typename T> EGL_INLINE tform_rt3<T> operator*(const tform_rt3<T>&, const tform_rt3<T>&);        // multiply transform by transform
template<typename T> EGL_INLINE tform_rt3<T> operator*(const tform_rt3<T>&, const quat<T>&);             // multiply transform by quaternion
template<typename T> EGL_INLINE tform_rt3<T> operator*(const tform_rt3<T>&, typename math<T>::scalar_t); // multiply transform by scalar
template<typename T> EGL_INLINE tform_rt3<T> operator*(typename math<T>::scalar_t, const tform_rt3<T>&); // multiply transform by scalar
template<typename T> EGL_INLINE vec3<T> operator*(const vec3<T>&, const tform_rt3<T>&);                  // multiply 3d vector by transform (assume w=1)
template<typename T> EGL_INLINE vec4<T> operator*(const vec4<T>&, const tform_rt3<T>&);                  // multiply 4d vector by transform
template<typename T> EGL_INLINE vec3<T> operator*(const tform_rt3<T>&, const vec3<T>&);                  // multiply transform by transposed 3d vector (assume w=1)
template<typename T> EGL_INLINE vec4<T> operator*(const tform_rt3<T>&, const vec4<T>&);                  // multiply transform by transposed 4d vector
template<typename T> EGL_INLINE mat44<T> operator*(const mat44<T>&, const tform_rt3<T>&);                // multiply 4x4 matrix by transform
template<typename T> EGL_INLINE mat44<T> operator*(const tform_rt3<T>&, const mat44<T>&);                // multiply transform by 4x4 matrix
template<typename T> EGL_INLINE tform_rt3<T> operator/(const tform_rt3<T>&, typename math<T>::scalar_t); // divide transform by scalar
template<typename T> EGL_INLINE tform_rt3<T> madd(const tform_rt3<T> &t0_,                               // component-wise multiply-add: t0_*t1+t2_
                                                  const tform_rt3<T> &t1_,
                                                  const tform_rt3<T> &t2_);
template<typename T> EGL_INLINE tform_rt3<T> inv(const tform_rt3<T>&);                                   // inverse of the transform
template<typename T> EGL_INLINE void identity(tform_rt3<T>&);                                            // set transform to identity
template<typename T> EGL_INLINE void neg(tform_rt3<T>&);                                                 // negate the transform
template<typename T> EGL_INLINE tform_rt3<T> lerp(const tform_rt3<T> &t0_,                               // linear interpolation of transforms: f(t=0)=t0, f(t=1)=t1
                                                  const tform_rt3<T> &t1_,
                                                  typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE tform_rt3<T> nlerp(const tform_rt3<T> &t0_,                              // linear interpolation of transforms with normalized rotation
                                                   const tform_rt3<T> &t1_,
                                                   typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE tform_rt3<T> nlerp_z(const tform_rt3<T> &t0_,                            // linear interpolation of transforms with normalized rotation, if |rotation|=0, rotation=[0, 0, 0, 1]
                                                     const tform_rt3<T> &t1_,
                                                     typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE tform_rt3<T> slerp(const tform_rt3<T> &t0_,                              // linear interpolation of translation with spherical rotation interpolation
                                                   const tform_rt3<T> &t1_,
                                                   typename math<T>::scalar_t t_);
template<typename T> EGL_INLINE void zrot_u(tform_rt3<T>&,                                               // setup transform from position and z-axis direction
                                            const vec3<T> &pos_,
                                            const vec3<T> &zdir_);
template<typename T> EGL_INLINE void axis_rot(tform_rt3<T>&,                                             // setup transform from position and rotation about given axis
                                              const vec3<T> &pos_,
                                              const vec3<T> &axis_,
                                              typename math<T>::scalar_t angle_);
template<typename T> EGL_INLINE void axis_rot_u(tform_rt3<T>&,                                           // setup transform from position and rotation about given unit axis
                                                const vec3<T> &pos_,
                                                const vec3<T> &axis_,
                                                typename math<T>::scalar_t angle_);
template<typename T, typename U> EGL_INLINE tform_rt3<T> to_tform_rt3(const tform_rt3<U>&);              // convert between tform_rt3 types
// 3d unit vector <-> octahedron coordinate transforms
template<typename T> EGL_INLINE vec2<T> vec3_to_oct(const vec3<T>&);                                     // convert 3d unit vector to 2d octahedron coordinates [-1, 1]
template<typename T> EGL_INLINE vec3<T> oct_to_vec3(const vec2<T>&);                                     // convert 2d octahedron coordinates [-1, 1] to 3d unit vector
template<typename T> EGL_INLINE vec2<T> vec3_to_oct2x1(const vec3<T>&);                                  // convert 3d unit vector to 2d octahedron coordinates [-2, 2]x[-1, 1]
template<typename T> EGL_INLINE vec3<T> oct2x1_to_vec3(const vec2<T>&);                                  // convert 2d octahedron coordinates [-2, 2]x[-1, 1] to 3d unit vector
// 3d frame quantization
template<typename T> uint32_t quantize_mat33_32(const mat33<T>&);                                        // quantize left/right handed 3d rotation matrix to 32 bits
template<typename T> mat33<T> dequantize_mat33_32(uint32_t);                                             // dequantize left/right handed 3d rotation matrix from 32 bits
template<typename T> vec3<T> dequantize_mat33z_32(uint32_t);                                             // dequantize z-axis from 32bit quantized 3d rotation matrix
// projection matrix setup
template<typename T> mat44<T> perspective_matrix(typename math<T>::scalar_t fov_width_, typename math<T>::scalar_t aspect_ratio_, typename math<T>::scalar_t near_, typename math<T>::scalar_t far_, bool reversed_=false);
template<typename T> mat44<T> orthogonal_matrix(typename math<T>::scalar_t width_, typename math<T>::scalar_t aspect_ratio_, typename math<T>::scalar_t near_, typename math<T>::scalar_t far_, bool reversed_=false);
template<typename T> EGL_INLINE typename math<T>::scalar_t fov_width(const mat44<T> &v2p_);  // return fov width angle from view->projection matrix
template<typename T> EGL_INLINE typename math<T>::scalar_t fov_height(const mat44<T> &v2p_); // return fov height angle from view->projection matrix
// miscellaneous
template<typename T> vec4<T> sphere_screen_extents(const vec3<T> &pos_, typename math<T>::scalar_t rad_, const mat44<T> &v2p_);
//----------------------------------------------------------------------------


//============================================================================
// tform3
//============================================================================
template<typename T>
struct tform3
{
  typedef typename math<T>::scalar_t scalar_t;
  // construction
  EGL_INLINE tform3();
  EGL_INLINE tform3(scalar_t s_);
  EGL_INLINE tform3(scalar_t sx_, scalar_t sy_, scalar_t sz_);
  EGL_INLINE tform3(const vec3<T> &translation_);
  EGL_INLINE tform3(const vec3<T> &x_,
                    const vec3<T> &y_,
                    const vec3<T> &z_);
  EGL_INLINE tform3(const vec3<T> &x_,
                    const vec3<T> &y_,
                    const vec3<T> &z_,
                    const vec3<T> &translation_);
  EGL_INLINE tform3(const vec4<T> &x_,
                    const vec4<T> &y_,
                    const vec4<T> &z_);
  EGL_INLINE tform3(scalar_t xx_, scalar_t xy_, scalar_t xz_, scalar_t xtrans_,
                    scalar_t yx_, scalar_t yy_, scalar_t yz_, scalar_t ytrans_,
                    scalar_t zx_, scalar_t zy_, scalar_t zz_, scalar_t ztrans_);
  EGL_INLINE tform3(const mat33<T>&);
  EGL_INLINE tform3(const mat33<T>&, const vec3<T> &translation_);
  EGL_INLINE tform3(const quat<T>&);
  EGL_INLINE tform3(const quat<T>&, const vec3<T> &translation_);
  EGL_INLINE tform3(const tform_rt3<T>&);
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE const vec4<T> &operator[](unsigned vidx_) const;
  EGL_INLINE vec4<T> &operator[](unsigned vidx_);
  EGL_INLINE mat33<T> matrix33() const;
  EGL_INLINE mat44<T> matrix44() const;
  EGL_INLINE const vec3<T> &x_axis() const;
  EGL_INLINE const vec3<T> &y_axis() const;
  EGL_INLINE const vec3<T> &z_axis() const;
  EGL_INLINE vec3<T> translation() const;
  //--------------------------------------------------------------------------

  // mutators
  EGL_INLINE void set(scalar_t s_);
  EGL_INLINE void set(scalar_t sx_, scalar_t sy_, scalar_t sz_);
  EGL_INLINE void set(const vec3<T> &translation_);
  EGL_INLINE void set(const vec3<T> &x_,
                      const vec3<T> &y_,
                      const vec3<T> &z_);
  EGL_INLINE void set(const vec3<T> &x_,
                      const vec3<T> &y_,
                      const vec3<T> &z_,
                      const vec3<T> &translation_);
  EGL_INLINE void set(const vec4<T> &x_,
                      const vec4<T> &y_,
                      const vec4<T> &z_);
  EGL_INLINE void set(scalar_t xx_, scalar_t xy_, scalar_t xz_, scalar_t xtrans_,
                      scalar_t yx_, scalar_t yy_, scalar_t yz_, scalar_t ytrans_,
                      scalar_t zx_, scalar_t zy_, scalar_t zz_, scalar_t ztrans_);
  EGL_INLINE void set(const mat33<T>&);
  EGL_INLINE void set(const mat33<T>&, const vec3<T> &translation_);
  EGL_INLINE void set(const quat<T>&);
  EGL_INLINE void set(const quat<T>&, const vec3<T> &translation_);
  EGL_INLINE void set(const tform_rt3<T>&);
  EGL_INLINE void set_rotation(const mat33<T>&);
  EGL_INLINE void set_translation(const vec3<T>&);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE tform3<T> zero();
  static EGL_INLINE tform3<T> identity();
  //--------------------------------------------------------------------------

  vec4<T> x;
  vec4<T> y;
  vec4<T> z;
};
//----------------------------------------------------------------------------


//============================================================================
// tform_rt3
//============================================================================
template<typename T>
struct tform_rt3
{
  typedef typename math<T>::scalar_t scalar_t;
  // construction
  EGL_INLINE tform_rt3();
  EGL_INLINE tform_rt3(const quat<T> &rotation_);
  EGL_INLINE tform_rt3(const vec3<T> &translation_);
  EGL_INLINE tform_rt3(const quat<T> &rotation_, const vec3<T> &translation_);
  EGL_INLINE tform_rt3(const tform3<T>&);
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE mat33<T> matrix33() const;
  EGL_INLINE mat44<T> matrix44() const;
  //--------------------------------------------------------------------------

  // mutators
  EGL_INLINE void set(const quat<T> &rotation_);
  EGL_INLINE void set(const vec3<T> &translation_);
  EGL_INLINE void set(const quat<T> &rotation_, const vec3<T> &translation_);
  //--------------------------------------------------------------------------

  // constants
  static EGL_INLINE tform_rt3<T> zero();
  static EGL_INLINE tform_rt3<T> identity();
  //--------------------------------------------------------------------------

  quat<T> rotation;
  vec3<T> translation;
};
//----------------------------------------------------------------------------


//============================================================================
// camera
//============================================================================
template<typename T>
class camera
{
public:
  typedef typename math<T>::scalar_t scalar_t;
  // construction
  camera();
  camera(const mat44<T> &view_to_proj_, scalar_t near_, scalar_t far_);
  camera(const tform3<T> &view_to_world_, const mat44<T> &view_to_proj_, scalar_t near_, scalar_t far_);
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE const mat44<T> &view_to_proj() const;
  EGL_INLINE const mat44<T> &proj_to_view() const;
  EGL_INLINE const tform3<T> &world_to_view() const;
  EGL_INLINE const tform3<T> &view_to_world() const;
  EGL_INLINE const mat44<T> &world_to_proj() const;
  EGL_INLINE const mat44<T> &proj_to_world() const;
  EGL_INLINE scalar_t near_plane() const;
  EGL_INLINE scalar_t far_plane() const;
  EGL_INLINE vec3<T> world_x() const;
  EGL_INLINE vec3<T> world_y() const;
  EGL_INLINE vec3<T> world_z() const;
  EGL_INLINE vec3<T> world_pos() const;
  //--------------------------------------------------------------------------

  // mutators
  EGL_INLINE void set_transform(const mat44<T> &view_to_proj_,
                                const tform3<T> &view_to_world_,
                                scalar_t near_,
                                scalar_t far_);
  EGL_INLINE void set_view_to_proj(const mat44<T>&, scalar_t near_, scalar_t far_);
  EGL_INLINE void set_view_to_world(const tform3<T>&);
  //--------------------------------------------------------------------------

private:
  void update_transforms();
  //--------------------------------------------------------------------------

  mat44<T> m_view_to_proj;
  mat44<T> m_proj_to_view;
  tform3<T> m_world_to_view;
  tform3<T> m_view_to_world;
  mat44<T> m_world_to_proj;
  mat44<T> m_proj_to_world;
  scalar_t m_near, m_far;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// tform3
//============================================================================
template<typename T>
tform3<T> tform3<T>::zero()
{
  return tform3<T>(0, 0, 0, 0,
                   0, 0, 0, 0,
                   0, 0, 0, 0);
}
//----

template<typename T>
tform3<T> tform3<T>::identity()
{
  return tform3<T>(scalar_t(1),           0,           0, 0,
                             0, scalar_t(1),           0, 0,
                             0,           0, scalar_t(1), 0);
}
//----------------------------------------------------------------------------

template<typename T>
tform3<T>::tform3()
  :x(scalar_t(1), 0, 0, 0)
  ,y(0, scalar_t(1), 0, 0)
  ,z(0, 0, scalar_t(1), 0)
{
}
//----

template<typename T>
tform3<T>::tform3(scalar_t s_)
  :x(s_, 0, 0, 0)
  ,y(0, s_, 0, 0)
  ,z(0, 0, s_, 0)
{
}
//----

template<typename T>
tform3<T>::tform3(scalar_t sx_, scalar_t sy_, scalar_t sz_)
  :x(sx_, 0, 0, 0)
  ,y(0, sy_, 0, 0)
  ,z(0, 0, sz_, 0)
{
}
//----

template<typename T>
tform3<T>::tform3(const vec3<T> &translation_)
  :x(scalar_t(1), 0, 0, translation_.x)
  ,y(0, scalar_t(1), 0, translation_.y)
  ,z(0, 0, scalar_t(1), translation_.z)
{
}
//----

template<typename T>
tform3<T>::tform3(const vec3<T> &x_, const vec3<T> &y_, const vec3<T> &z_)
  :x(x_, 0)
  ,y(y_, 0)
  ,z(z_, 0)
{
}
//----

template<typename T>
tform3<T>::tform3(const vec3<T> &x_, const vec3<T> &y_, const vec3<T> &z_, const vec3<T> &translation_)
  :x(x_, translation_.x)
  ,y(y_, translation_.y)
  ,z(z_, translation_.z)
{
}
//----

template<typename T>
tform3<T>::tform3(const vec4<T> &x_, const vec4<T> &y_, const vec4<T> &z_)
  :x(x_)
  ,y(y_)
  ,z(z_)
{
}
//----

template<typename T>
tform3<T>::tform3(scalar_t xx_, scalar_t xy_, scalar_t xz_, scalar_t xtrans_,
                  scalar_t yx_, scalar_t yy_, scalar_t yz_, scalar_t ytrans_,
                  scalar_t zx_, scalar_t zy_, scalar_t zz_, scalar_t ztrans_)
  :x(xx_, xy_, xz_, xtrans_)
  ,y(yx_, yy_, yz_, ytrans_)
  ,z(zx_, zy_, zz_, ztrans_)
{
}
//----

template<typename T>
tform3<T>::tform3(const mat33<T> &m_)
  :x(m_.x, 0)
  ,y(m_.y, 0)
  ,z(m_.z, 0)
{
}
//----

template<typename T>
tform3<T>::tform3(const mat33<T> &m_, const vec3<T> &translation_)
  :x(m_.x, translation_.x)
  ,y(m_.y, translation_.y)
  ,z(m_.z, translation_.z)
{
}
//----

template<typename T>
tform3<T>::tform3(const quat<T> &q_)
{
  // convert quaternion to matrix and setup transform
  mat33<T> m;
  convert(m, q_);
  x.set(m.x, 0);
  y.set(m.y, 0);
  z.set(m.z, 0);
}
//----

template<typename T>
tform3<T>::tform3(const quat<T> &q_, const vec3<T> &translation_)
{
  // convert quaternion to matrix and setup transform
  mat33<T> m;
  convert(m, q_);
  x.set(m.x, translation_.x);
  y.set(m.y, translation_.y);
  z.set(m.z, translation_.z);
}
//----

template<typename T>
tform3<T>::tform3(const tform_rt3<T> &t_)
{
  // convert tform_rt3 rotation to matrix and setup transform
  mat33<T> m;
  convert(m, t_.rotation);
  x.set(m.x, t_.translation.x);
  y.set(m.y, t_.translation.y);
  z.set(m.z, t_.translation.z);
}
//----------------------------------------------------------------------------

template<typename T>
const vec4<T> &tform3<T>::operator[](unsigned vidx_) const
{
  // return reference to nth transformation vector (x=0, y=1, z=2)
  EGL_STATIC_ASSERT_MSG(sizeof(tform3)==sizeof(vec4<T>)*3, transform3_size_is_incorrect);
  EGL_ASSERT_PEDANTIC(vidx_<3);
  return reinterpret_cast<const vec4<T>*>(this)[vidx_];
}
//----

template<typename T>
vec4<T> &tform3<T>::operator[](unsigned vidx_)
{
  // return reference to nth transformation vector (x=0, y=1, z=2)
  EGL_STATIC_ASSERT_MSG(sizeof(tform3)==sizeof(vec4<T>)*3, transform3_size_is_incorrect);
  EGL_ASSERT_PEDANTIC(vidx_<3);
  return reinterpret_cast<vec4<T>*>(this)[vidx_];
}
//----

template<typename T>
mat33<T> tform3<T>::matrix33() const
{
  // return affine matrix of the transform
  return mat33<T>(x.x, x.y, x.z,
                  y.x, y.y, y.z,
                  z.x, z.y, z.z);
}
//----

template<typename T>
mat44<T> tform3<T>::matrix44() const
{
  // return 4x4 matrix of the transform
  return mat44<T>(x.x, x.y, x.z, 0,
                  y.x, y.y, y.z, 0,
                  z.x, z.y, z.z, 0,
                  x.w, y.w, z.w, scalar_t(1));
}
//----

template<typename T>
const vec3<T> &tform3<T>::x_axis() const
{
  // return x-axis vector of the transform
  const void *xv=&x;
  return *(const vec3<T>*)xv;
}
//----

template<typename T>
const vec3<T> &tform3<T>::y_axis() const
{
  // return y-axis vector of the transform
  const void *yv=&y;
  return *(const vec3<T>*)yv;
}
//----

template<typename T>
const vec3<T> &tform3<T>::z_axis() const
{
  // return z-axis vector of the transform
  const void *zv=&z;
  return *(const vec3<T>*)zv;
}
//----

template<typename T>
vec3<T> tform3<T>::translation() const
{
  // return translation vector of the transform
  return vec3<T>(x.w, y.w, z.w);
}
//----------------------------------------------------------------------------

template<typename T>
void tform3<T>::set(scalar_t s_)
{
  // setup uniform scaling transform
  x.set(s_, 0, 0, 0);
  y.set(0, s_, 0, 0);
  z.set(0, 0, s_, 0);
}
//----

template<typename T>
void tform3<T>::set(scalar_t sx_, scalar_t sy_, scalar_t sz_)
{
  // setup non-uniform scaling transform
  x.set(sx_, 0, 0, 0);
  y.set(0, sy_, 0, 0);
  z.set(0, 0, sz_, 0);
}
//----

template<typename T>
void tform3<T>::set(const vec3<T> &translation_)
{
  // setup translation transform
  x.set(scalar_t(1), 0, 0, translation_.x);
  y.set(0, scalar_t(1), 0, translation_.y);
  z.set(0, 0, scalar_t(1), translation_.z);
}
//----

template<typename T>
void tform3<T>::set(const vec3<T> &x_, const vec3<T> &y_, const vec3<T> &z_)
{
  // setup transform from 3 axis and translation
  x.set(x_, 0);
  y.set(y_, 0);
  z.set(z_, 0);
}
//----

template<typename T>
void tform3<T>::set(const vec3<T> &x_, const vec3<T> &y_, const vec3<T> &z_, const vec3<T> &translation_)
{
  // setup transform from 3 axis and translation
  x.set(x_, translation_.x);
  y.set(y_, translation_.y);
  z.set(z_, translation_.z);
}
//----

template<typename T>
void tform3<T>::set(const vec4<T> &x_, const vec4<T> &y_, const vec4<T> &z_)
{
  // setup transform from 3 transform vectors
  x=x_;
  y=y_;
  z=z_;
}
//----

template<typename T>
void tform3<T>::set(scalar_t xx_, scalar_t xy_, scalar_t xz_, scalar_t xtrans_,
                    scalar_t yx_, scalar_t yy_, scalar_t yz_, scalar_t ytrans_,
                    scalar_t zx_, scalar_t zy_, scalar_t zz_, scalar_t ztrans_)
{
  // setup transform from scalars
  x.set(xx_, xy_, xz_, xtrans_);
  y.set(yx_, yy_, yz_, ytrans_);
  z.set(zx_, zy_, zz_, ztrans_);
}
//----

template<typename T>
void tform3<T>::set(const mat33<T> &m_)
{
  // setup transform from affine matrix
  x.set(m_.x, 0);
  y.set(m_.y, 0);
  z.set(m_.z, 0);
}
//----

template<typename T>
void tform3<T>::set(const mat33<T> &m_, const vec3<T> &translation_)
{
  // setup transform from affine matrix and translation
  x.set(m_.x, translation_.x);
  y.set(m_.y, translation_.y);
  z.set(m_.z, translation_.z);
}
//----

template<typename T>
void tform3<T>::set(const quat<T> &q_)
{
  // setup transform from quaternion rotation
  mat33<T> m;
  convert(m, q_);
  x.set(m.x, 0);
  y.set(m.y, 0);
  z.set(m.z, 0);
}
//----

template<typename T>
void tform3<T>::set(const quat<T> &q_, const vec3<T> &translation_)
{
  // setup transform from quaternion rotation and translation
  mat33<T> m;
  convert(m, q_);
  x.set(m.x, translation_.x);
  y.set(m.y, translation_.y);
  z.set(m.z, translation_.z);
}
//----

template<typename T>
void tform3<T>::set(const tform_rt3<T> &t_)
{
  // setup transform from tform_rt3
  mat33<T> m;
  convert(m, t_.rotation);
  x.set(m.x, t_.translation.x);
  y.set(m.y, t_.translation.y);
  z.set(m.z, t_.translation.z);
}
//----

template<typename T>
void tform3<T>::set_rotation(const mat33<T> &r_)
{
  // set rotation part of the transform
  x.x=r_.x.x; x.y=r_.x.y; x.z=r_.x.z;
  y.x=r_.y.x; y.y=r_.y.y; y.z=r_.y.z;
  z.x=r_.z.x; z.y=r_.z.y; z.z=r_.z.z;
}
//----

template<typename T>
void tform3<T>::set_translation(const vec3<T> &t_)
{
  // set translation part of the transform
  x.w=t_.x;
  y.w=t_.y;
  z.w=t_.z;
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const tform3<T> &t_)
{
  // test for zero-transform
  return is_zero(t_.x) && is_zero(t_.y) && is_zero(t_.z);
}
//----

template<typename T>
EGL_INLINE bool is_orthogonal(const tform3<T> &t_)
{
  // test for orthogonal transform
  return    t_.x.x*t_.y.x+t_.x.y*t_.y.y+t_.x.z*t_.y.z<0.0001f
         && t_.x.x*t_.z.x+t_.x.y*t_.z.y+t_.x.z*t_.z.z<0.0001f
         && t_.y.x*t_.z.x+t_.y.y*t_.z.y+t_.y.z*t_.z.z<0.0001f;
}
//----

template<typename T>
EGL_INLINE bool is_sat(const tform3<T> &t_)
{
  // test for saturated transform
  return is_sat(t_.x) && is_sat(t_.y) && is_sat(t_.z);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const tform3<T> &t_)
{
  // test for signed saturated transform
  return is_ssat(t_.x) && is_ssat(t_.y) && is_ssat(t_.z);
}
//----

template<typename T>
EGL_INLINE bool operator==(const tform3<T> &t0_, const tform3<T> &t1_)
{
  // test for equality of transforms
  return    t0_.x==t1_.x
         && t0_.y==t1_.y
         && t0_.z==t1_.z;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const tform3<T> &t0_, const tform3<T> &t1_)
{
  // test for inequality of transforms
  return    t0_.x!=t1_.x
         || t0_.y!=t1_.y
         || t0_.z!=t1_.z;
}
//----

template<typename T>
EGL_INLINE void operator+=(tform3<T> &tr_, const tform3<T> &t_)
{
  // add transform to transform
  tr_.x+=t_.x;
  tr_.y+=t_.y;
  tr_.z+=t_.z;
}
//----

template<typename T>
EGL_INLINE void operator-=(tform3<T> &tr_, const tform3<T> &t_)
{
  // subtract transform from transform
  tr_.x-=t_.x;
  tr_.y-=t_.y;
  tr_.z-=t_.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform3<T> &tr_, const tform3<T> &t_)
{
  // multiply transform by transform
  const vec3<T> p(tr_.x.w, tr_.y.w, tr_.z.w);
  vec4<T> v=tr_.x;
  tr_.x.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x; tr_.x.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y; tr_.x.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z; tr_.x.w=p.x*t_.x.x+p.y*t_.y.x+p.z*t_.z.x+t_.x.w;
  v=tr_.y;
  tr_.y.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x; tr_.y.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y; tr_.y.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z; tr_.y.w=p.x*t_.x.y+p.y*t_.y.y+p.z*t_.z.y+t_.y.w;
  v=tr_.z;
  tr_.z.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x; tr_.z.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y; tr_.z.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z; tr_.z.w=p.x*t_.x.z+p.y*t_.y.z+p.z*t_.z.z+t_.z.w;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform3<T> &tr_, const quat<T> &q_)
{
  // multiply transform by quaternion
  vec3<T> p(tr_.x.w, tr_.y.w, tr_.z.w);
  p*=q_;
  reinterpret_cast<vec3<T>&>(tr_.x)*=q_; tr_.x.w=p.x;
  reinterpret_cast<vec3<T>&>(tr_.y)*=q_; tr_.y.w=p.y;
  reinterpret_cast<vec3<T>&>(tr_.z)*=q_; tr_.z.w=p.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform3<T> &tr_, const mat33<T> &m_)
{
  // multiply transform by matrix
  vec3<T> p(tr_.x.w, tr_.y.w, tr_.z.w);
  p*=m_;
  reinterpret_cast<vec3<T>&>(tr_.x)*=m_; tr_.x.w=p.x;
  reinterpret_cast<vec3<T>&>(tr_.y)*=m_; tr_.y.w=p.y;
  reinterpret_cast<vec3<T>&>(tr_.z)*=m_; tr_.z.w=p.z;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform3<T> &tr_, typename math<T>::scalar_t s_)
{
  // multiply transform by scalar
  tr_.x*=s_;
  tr_.y*=s_;
  tr_.z*=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, const tform3<T> &t_)
{
  // multiply vector by transform (assume w=1)
  vec3<T> v(vr_);
  vr_.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x+t_.x.w;
  vr_.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y+t_.y.w;
  vr_.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z+t_.z.w;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec4<T> &vr_, const tform3<T> &t_)
{
  // multiply vector by transform
  vec4<T> v(vr_);
  vr_.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x+v.w*t_.x.w;
  vr_.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y+v.w*t_.y.w;
  vr_.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z+v.w*t_.z.w;
}
//----

template<typename T>
EGL_INLINE void operator*=(mat44<T> &mr_, const tform3<T> &t_)
{
  // multiply 4x4 matrix by transform
  vec4<T> v=mr_.x;
  mr_.x.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x+v.w*t_.x.w; mr_.x.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y+v.w*t_.y.w; mr_.x.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z+v.w*t_.z.w; mr_.x.w=v.w;
  v=mr_.y;
  mr_.y.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x+v.w*t_.x.w; mr_.y.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y+v.w*t_.y.w; mr_.y.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z+v.w*t_.z.w; mr_.y.w=v.w;
  v=mr_.z;
  mr_.z.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x+v.w*t_.x.w; mr_.z.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y+v.w*t_.y.w; mr_.z.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z+v.w*t_.z.w; mr_.z.w=v.w;
  v=mr_.w;
  mr_.w.x=v.x*t_.x.x+v.y*t_.y.x+v.z*t_.z.x+v.w*t_.x.w; mr_.w.y=v.x*t_.x.y+v.y*t_.y.y+v.z*t_.z.y+v.w*t_.y.w; mr_.w.z=v.x*t_.x.z+v.y*t_.y.z+v.z*t_.z.z+v.w*t_.z.w; mr_.w.w=v.w;
}
//----

template<typename T>
EGL_INLINE void operator/=(tform3<T> &tr_, typename math<T>::scalar_t s_)
{
  // divide transform by scalar
  typename math<T>::scalar_t rs=rcp(s_);
  tr_.x*=rs;
  tr_.y*=rs;
  tr_.z*=rs;
}
//----

template<typename T>
EGL_INLINE tform3<T> operator+(const tform3<T> &t0_, const tform3<T> &t1_)
{
  // add transform to transform
  return tform3<T>(t0_.x+t1_.x,
                   t0_.y+t1_.y,
                   t0_.z+t1_.z);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator-(const tform3<T> &t0_, const tform3<T> &t1_)
{
  // subtract transform from transform
  return tform3<T>(t0_.x-t1_.x,
                   t0_.y-t1_.y,
                   t0_.z-t1_.z);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator-(const tform3<T> &t_)
{
  // negate transform
  return tform3<T>(-t_.x, -t_.y, -t_.z);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator*(const tform3<T> &t0_, const tform3<T> &t1_)
{
  // multiply transform by transform
  return tform3<T>(t0_.x.x*t1_.x.x+t0_.x.y*t1_.y.x+t0_.x.z*t1_.z.x, t0_.x.x*t1_.x.y+t0_.x.y*t1_.y.y+t0_.x.z*t1_.z.y, t0_.x.x*t1_.x.z+t0_.x.y*t1_.y.z+t0_.x.z*t1_.z.z, t0_.x.w*t1_.x.x+t0_.y.w*t1_.y.x+t0_.z.w*t1_.z.x+t1_.x.w,
                   t0_.y.x*t1_.x.x+t0_.y.y*t1_.y.x+t0_.y.z*t1_.z.x, t0_.y.x*t1_.x.y+t0_.y.y*t1_.y.y+t0_.y.z*t1_.z.y, t0_.y.x*t1_.x.z+t0_.y.y*t1_.y.z+t0_.y.z*t1_.z.z, t0_.x.w*t1_.x.y+t0_.y.w*t1_.y.y+t0_.z.w*t1_.z.y+t1_.y.w,
                   t0_.z.x*t1_.x.x+t0_.z.y*t1_.y.x+t0_.z.z*t1_.z.x, t0_.z.x*t1_.x.y+t0_.z.y*t1_.y.y+t0_.z.z*t1_.z.y, t0_.z.x*t1_.x.z+t0_.z.y*t1_.y.z+t0_.z.z*t1_.z.z, t0_.x.w*t1_.x.z+t0_.y.w*t1_.y.z+t0_.z.w*t1_.z.z+t1_.z.w);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator*(const tform3<T> &t_, const quat<T> &q_)
{
  // multiply transform by quaternion
  return tform3<T>(reinterpret_cast<const vec3<T>&>(t_.x)*q_,
                   reinterpret_cast<const vec3<T>&>(t_.y)*q_,
                   reinterpret_cast<const vec3<T>&>(t_.z)*q_,
                   vec3<T>(t_.x.w, t_.y.w, t_.z.w)*q_);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator*(const tform3<T> &t_, const mat33<T> &m_)
{
  // multiply transform by matrix
  return tform3<T>(reinterpret_cast<const vec3<T>&>(t_.x)*m_,
                   reinterpret_cast<const vec3<T>&>(t_.y)*m_,
                   reinterpret_cast<const vec3<T>&>(t_.z)*m_,
                   vec3<T>(t_.x.w, t_.y.w, t_.z.w)*m_);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator*(const tform3<T> &t_, typename math<T>::scalar_t s_)
{
  // multiply transform by scalar
  return tform3<T>(t_.x*s_, t_.y*s_, t_.z*s_);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator*(typename math<T>::scalar_t s_, const tform3<T> &t_)
{
  // multiply transform by scalar
  return tform3<T>(t_.x*s_, t_.y*s_, t_.z*s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v_, const tform3<T> &t_)
{
  // multiply 3d vector by transform (assume vector w=1)
  return vec3<T>(v_.x*t_.x.x+v_.y*t_.y.x+v_.z*t_.z.x+t_.x.w,
                 v_.x*t_.x.y+v_.y*t_.y.y+v_.z*t_.z.y+t_.y.w,
                 v_.x*t_.x.z+v_.y*t_.y.z+v_.z*t_.z.z+t_.z.w);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const vec4<T> &v_, const tform3<T> &t_)
{
  // multiply 4d vector by transform
  return vec4<T>(v_.x*t_.x.x+v_.y*t_.y.x+v_.z*t_.z.x+v_.w*t_.x.w,
                 v_.x*t_.x.y+v_.y*t_.y.y+v_.z*t_.z.y+v_.w*t_.y.w,
                 v_.x*t_.x.z+v_.y*t_.y.z+v_.z*t_.z.z+v_.w*t_.z.w,
                 v_.w);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const tform3<T> &t_, const vec3<T> &v_)
{
  // multiply transform by transposed 3d vector
  return vec3<T>(v_.x*t_.x.x+v_.y*t_.x.y+v_.z*t_.x.z,
                 v_.x*t_.y.x+v_.y*t_.y.y+v_.z*t_.y.z,
                 v_.x*t_.z.x+v_.y*t_.z.y+v_.z*t_.z.z);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const tform3<T> &t_, const vec4<T> &v_)
{
  // multiply transform by transposed 4d vector
  return vec4<T>(v_.x*t_.x.x+v_.y*t_.x.y+v_.z*t_.x.z,
                 v_.x*t_.y.x+v_.y*t_.y.y+v_.z*t_.y.z,
                 v_.x*t_.z.x+v_.y*t_.z.y+v_.z*t_.z.z,
                 v_.x*t_.x.w+v_.y*t_.y.w+v_.z*t_.z.w+v_.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(const mat44<T> &m_, const tform3<T> &t_)
{
  // multiply 4x4 matrix by transform
  return mat44<T>(m_.x.x*t_.x.x+m_.x.y*t_.y.x+m_.x.z*t_.z.x+m_.x.w*t_.x.w, m_.x.x*t_.x.y+m_.x.y*t_.y.y+m_.x.z*t_.z.y+m_.x.w*t_.y.w, m_.x.x*t_.x.z+m_.x.y*t_.y.z+m_.x.z*t_.z.z+m_.x.w*t_.z.w, m_.x.w,
                  m_.y.x*t_.x.x+m_.y.y*t_.y.x+m_.y.z*t_.z.x+m_.y.w*t_.x.w, m_.y.x*t_.x.y+m_.y.y*t_.y.y+m_.y.z*t_.z.y+m_.y.w*t_.y.w, m_.y.x*t_.x.z+m_.y.y*t_.y.z+m_.y.z*t_.z.z+m_.y.w*t_.z.w, m_.y.w,
                  m_.z.x*t_.x.x+m_.z.y*t_.y.x+m_.z.z*t_.z.x+m_.z.w*t_.x.w, m_.z.x*t_.x.y+m_.z.y*t_.y.y+m_.z.z*t_.z.y+m_.z.w*t_.y.w, m_.z.x*t_.x.z+m_.z.y*t_.y.z+m_.z.z*t_.z.z+m_.z.w*t_.z.w, m_.z.w,
                  m_.w.x*t_.x.x+m_.w.y*t_.y.x+m_.w.z*t_.z.x+m_.w.w*t_.x.w, m_.w.x*t_.x.y+m_.w.y*t_.y.y+m_.w.z*t_.z.y+m_.w.w*t_.y.w, m_.w.x*t_.x.z+m_.w.y*t_.y.z+m_.w.z*t_.z.z+m_.w.w*t_.z.w, m_.w.w);
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(const tform3<T> &t_, const mat44<T> &m_)
{
  // multiply transform by 4x4 matrix
  return mat44<T>(t_.x.x*m_.x.x+t_.x.y*m_.y.x+t_.x.z*m_.z.x,        t_.x.x*m_.x.y+t_.x.y*m_.y.y+t_.x.z*m_.z.y,        t_.x.x*m_.x.z+t_.x.y*m_.y.z+t_.x.z*m_.z.z,        t_.x.x*m_.x.w+t_.x.y*m_.y.w+t_.x.z*m_.z.w,
                  t_.y.x*m_.x.x+t_.y.y*m_.y.x+t_.y.z*m_.z.x,        t_.y.x*m_.x.y+t_.y.y*m_.y.y+t_.y.z*m_.z.y,        t_.y.x*m_.x.z+t_.y.y*m_.y.z+t_.y.z*m_.z.z,        t_.y.x*m_.x.w+t_.y.y*m_.y.w+t_.y.z*m_.z.w,
                  t_.z.x*m_.x.x+t_.z.y*m_.y.x+t_.z.z*m_.z.x,        t_.z.x*m_.x.y+t_.z.y*m_.y.y+t_.z.z*m_.z.y,        t_.z.x*m_.x.z+t_.z.y*m_.y.z+t_.z.z*m_.z.z,        t_.z.x*m_.x.w+t_.z.y*m_.y.w+t_.z.z*m_.z.w,
                  t_.x.w*m_.x.x+t_.y.w*m_.y.x+t_.z.w*m_.z.x+m_.w.x, t_.x.w*m_.x.y+t_.y.w*m_.y.y+t_.z.w*m_.z.y+m_.w.y, t_.x.w*m_.x.z+t_.y.w*m_.y.z+t_.z.w*m_.z.z+m_.w.z, t_.x.w*m_.x.w+t_.y.w*m_.y.w+t_.z.w*m_.z.w+m_.w.w);
}
//----

template<typename T>
EGL_INLINE tform3<T> operator/(const tform3<T> &t_, typename math<T>::scalar_t s_)
{
  // divide transform by scalar
  typename math<T>::scalar_t rs=rcp(s_);
  return tform3<T>(t_.x*rs, t_.y*rs, t_.z*rs);
}
//----

template<typename T>
EGL_INLINE tform3<T> madd(const tform3<T> &t0_, const tform3<T> &t1_, const tform3<T> &t2_)
{
  // component-wise multiply-add: t0_*t1+t2_
  return tform3<T>(madd(t0_.x, t1_.x, t2_.x), madd(t0_.y, t1_.y, t2_.z), madd(t0_.z, t1_.z, t2_.z));
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t det(const tform3<T> &t_)
{
  // calculate determinant
  return  t_.x.x*(t_.y.y*t_.z.z-t_.y.z*t_.z.y)
         +t_.y.x*(t_.z.y*t_.x.z-t_.z.z*t_.x.y)
         +t_.z.x*(t_.x.y*t_.y.z-t_.x.z*t_.y.y);
}
//----

template<typename T>
EGL_INLINE tform3<T> inv(const tform3<T> &t_, typename math<T>::scalar_t *det_)
{
  // calculate determinant
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d= t_.x.x*(t_.y.y*t_.z.z-t_.y.z*t_.z.y)
             +t_.y.x*(t_.z.y*t_.x.z-t_.z.z*t_.x.y)
             +t_.z.x*(t_.x.y*t_.y.z-t_.x.z*t_.y.y);
  if(det_)
    *det_=d;
  if(!d)
    return tform3<T>(scalar_t(0));

  // calculate inverse of the transform
  mat33<T> m((t_.y.y*t_.z.z-t_.y.z*t_.z.y), (t_.x.z*t_.z.y-t_.x.y*t_.z.z), (t_.x.y*t_.y.z-t_.x.z*t_.y.y),
             (t_.y.z*t_.z.x-t_.y.x*t_.z.z), (t_.x.x*t_.z.z-t_.x.z*t_.z.x), (t_.x.z*t_.y.x-t_.x.x*t_.y.z),
             (t_.y.x*t_.z.y-t_.y.y*t_.z.x), (t_.x.y*t_.z.x-t_.x.x*t_.z.y), (t_.x.x*t_.y.y-t_.x.y*t_.y.x));
  m/=d;
  return tform3<T>(m, -t_.translation()*m);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t tr(const tform3<T> &t_)
{
  // calculate trace of the transform
  return t_.x.x+t_.y.y+t_.z.z;
}
//----

template<typename T>
EGL_INLINE void identity(tform3<T> &tr_)
{
  // set identity transform
  typedef typename math<T>::scalar_t scalar_t;
  tr_.x.set(scalar_t(1), 0, 0, 0);
  tr_.y.set(0, scalar_t(1), 0, 0);
  tr_.z.set(0, 0, scalar_t(1), 0);
}
//----

template<typename T>
EGL_INLINE void transpose_matrix33(tform3<T> &tr_)
{
  // transpose the 3x3 upper-left matrix of the transform
  typename math<T>::scalar_t t;
  t=tr_.x.y;
  tr_.x.y=tr_.y.x;
  tr_.y.x=t;
  t=tr_.x.z;
  tr_.x.z=tr_.z.x;
  tr_.z.x=t;
  t=tr_.y.z;
  tr_.y.z=tr_.z.y;
  tr_.z.y=t;
}
//----

template<typename T>
EGL_INLINE void transpose_matrix33(tform3<T> &tr_, const tform3<T> &t_)
{
  // transpose the 3x3 upper-left matrix of the transform
  tr_.x.x=t_.x.x; tr_.x.y=t_.y.x; tr_.x.z=t_.z.x; tr_.x.w=t_.x.w;
  tr_.y.x=t_.x.y; tr_.y.y=t_.y.y; tr_.y.z=t_.z.y; tr_.y.w=t_.y.w;
  tr_.z.x=t_.x.z; tr_.z.y=t_.y.z; tr_.z.z=t_.z.z; tr_.z.w=t_.z.w;
}
//----

template<typename T>
EGL_INLINE void neg(tform3<T> &tr_)
{
  // negate transform
  neg(tr_.x);
  neg(tr_.y);
  neg(tr_.z);
}
//----

template<typename T>
EGL_INLINE tform3<T> lerp(const tform3<T> &t0_, const tform3<T> &t1_, typename math<T>::scalar_t t_)
{
  // linear transform interpolation. f(t=0)=t0, f(t=1)=t1
  return tform3<T>(lerp(t0_.x, t1_.x, t_),
                   lerp(t0_.y, t1_.y, t_),
                   lerp(t0_.z, t1_.z, t_));
}
//----

template<typename T>
EGL_INLINE void lerp(tform3<T> &tr_, const tform3<T> &t0_, const tform3<T> &t1_, typename math<T>::scalar_t t_)
{
  // linear transform interpolation. f(t=0)=t0, f(t=1)=t1
  tr_.x=lerp(t0_.x, t1_.x, t_);
  tr_.y=lerp(t0_.y, t1_.y, t_);
  tr_.z=lerp(t0_.z, t1_.z, t_);
}
//----

template<typename T>
EGL_INLINE void zrot_u(tform3<T> &tr_, const vec3<T> &pos_, const vec3<T> &zdir_)
{
  // set z-axis to point to given direction by using quaternion rotation
  mat33<T> m;
  convert_u(m, zrot_u(zdir_));
  tr_.set(m, pos_);
}
//----

template<typename T>
EGL_INLINE void zrot_u(tform3<T> &tr_, const vec3<T> &pos_, const vec3<T> &zdir_, const vec3<T> &up_)
{
  // set z-axis to point to given direction by using up-vector
  mat33<T> m;
  vec3<T> x=cross(up_, zdir_);
  if(is_zero(x))
    x.set(typename math<T>::scalar_t(1), 0, 0);
  m.x=unit(x);
  m.y=cross(zdir_, m.x);
  m.z=zdir_;
  tr_.set(m, pos_);
}
//----

template<typename T>
EGL_INLINE void axis_rot(tform3<T> &tr_, const vec3<T> &pos_, const vec3<T> &axis_, typename math<T>::scalar_t angle_)
{
  // generate rotation from axis and angle (radians)
  mat33<T> m;
  convert(m, axis_rot(axis_, angle_));
  tr_.x.x=m.x.x; tr_.x.y=m.x.y; tr_.x.z=m.x.z; tr_.x.w=pos_.x;
  tr_.y.x=m.y.x; tr_.y.y=m.y.y; tr_.y.z=m.y.z; tr_.y.w=pos_.y;
  tr_.z.x=m.z.x; tr_.z.y=m.z.y; tr_.z.z=m.z.z; tr_.z.w=pos_.z;
}
//----

template<typename T>
EGL_INLINE void axis_rot_u(tform3<T> &tr_, const vec3<T> &pos_, const vec3<T> &axis_, typename math<T>::scalar_t angle_)
{
  // generate rotation from unit axis and angle (radians)
  mat33<T> m;
  convert(m, axis_rot_u(axis_, angle_));
  tr_.x.x=m.x.x; tr_.x.y=m.x.y; tr_.x.z=m.x.z; tr_.x.w=pos_.x;
  tr_.y.x=m.y.x; tr_.y.y=m.y.y; tr_.y.z=m.y.z; tr_.y.w=pos_.y;
  tr_.z.x=m.z.x; tr_.z.y=m.z.y; tr_.z.z=m.z.z; tr_.z.w=pos_.z;
}
//----

template<typename T, typename U>
EGL_INLINE tform3<T> to_tform3(const tform3<U> &t_)
{
  // convert between tform3 types
  typedef typename math<T>::scalar_t scalar_t;
  tform3<T> t(scalar_t(t_.x.x), scalar_t(t_.x.y), scalar_t(t_.x.z), scalar_t(t_.x.w),
              scalar_t(t_.y.x), scalar_t(t_.y.y), scalar_t(t_.y.z), scalar_t(t_.y.w),
              scalar_t(t_.z.x), scalar_t(t_.z.y), scalar_t(t_.z.z), scalar_t(t_.z.w));
  return t;
}
//----

template<typename T>
EGL_INLINE void orthonormalize(tform3<T> &tr_)
{
  // ortho-normalize the transform
  tr_.x.set(unit((vec3<T>&)tr_.x), tr_.x.w);
  tr_.y.set(unit(cross((vec3<T>&)tr_.z, (vec3<T>&)tr_.x)), tr_.y.w);
  tr_.z.set(cross((vec3<T>&)tr_.x, (vec3<T>&)tr_.y), tr_.z.w);
}
//----------------------------------------------------------------------------


//============================================================================
// tform_rt3
//============================================================================
template<typename T>
tform_rt3<T> tform_rt3<T>::zero()
{
  return tform_rt3<T>(quat<T>(0, 0, 0, 0), vec3<T>(0));
}
//----

template<typename T>
tform_rt3<T> tform_rt3<T>::identity()
{
  return tform_rt3<T>(quat<T>(0, 0, 0, scalar_t(1)), vec3<T>(0));
}
//----------------------------------------------------------------------------

template<typename T>
tform_rt3<T>::tform_rt3()
  :rotation(0, 0, 0, scalar_t(1))
  ,translation(0, 0, 0)
{
}
//----

template<typename T>
tform_rt3<T>::tform_rt3(const quat<T> &rotation_)
  :rotation(rotation_)
  ,translation(0, 0, 0)
{
}
//----

template<typename T>
tform_rt3<T>::tform_rt3(const vec3<T> &translation_)
  :rotation(0, 0, 0, scalar_t(1))
  ,translation(translation_)
{
}
//----

template<typename T>
tform_rt3<T>::tform_rt3(const quat<T> &rotation_, const vec3<T> &translation_)
  :rotation(rotation_)
  ,translation(translation_)
{
}
//----

template<typename T>
tform_rt3<T>::tform_rt3(const tform3<T> &tform_)
{
  // convert tform3 to tform_rt3
  mat33<T> rot=tform_.matrix33();
  orthonormalize(rot);
  convert(rotation, rot);
  translation=tform_.translation();
}
//----------------------------------------------------------------------------

template<typename T>
mat33<T> tform_rt3<T>::matrix33() const
{
  // return 3x3 rotation matrix
  mat33<T> mr;
  convert(mr, rotation);
  return mr;
}
//----

template<typename T>
mat44<T> tform_rt3<T>::matrix44() const
{
  // return 4x4 matrix
  mat33<T> mr;
  convert(mr, rotation);
  mat44<T> m(mr);
  m.w.set(translation, scalar_t(1));
  return m;
}
//----------------------------------------------------------------------------

template<typename T>
void tform_rt3<T>::set(const quat<T> &rotation_)
{
  rotation=rotation_;
  translation.set(scalar_t(0));
}
//----

template<typename T>
void tform_rt3<T>::set(const vec3<T> &translation_)
{
  rotation=quat<T>::identity();
  translation=translation_;
}
//---

template<typename T>
void tform_rt3<T>::set(const quat<T> &rotation_, const vec3<T> &translation_)
{
  rotation=rotation_;
  translation=translation_;
}
//----------------------------------------------------------------------------

template<typename T>
EGL_INLINE bool is_zero(const tform_rt3<T> &t_)
{
  // test for zero-transform
  return is_zero(t_.rotation) && is_zero(t_.translation);
}
//----

template<typename T>
EGL_INLINE bool is_sat(const tform_rt3<T> &t_)
{
  // test for saturated transform
  return is_sat(t_.rotation) && is_sat(t_.translation);
}
//----

template<typename T>
EGL_INLINE bool is_ssat(const tform_rt3<T> &t_)
{
  // test for signed saturated transform
  return is_ssat(t_.rotation) && is_ssat(t_.translation);
}
//----

template<typename T>
EGL_INLINE bool operator==(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_)
{
  // test for equality of transforms
  return    t0_.rotation==t1_.rotation
         && t0_.translation==t1_.translation;
}
//----

template<typename T>
EGL_INLINE bool operator!=(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_)
{
  // test for inequality of transforms
  return    t0_.rotation!=t1_.rotation
         || t0_.translation!=t1_.translation;
}
//----

template<typename T>
EGL_INLINE void operator+=(tform_rt3<T> &tr_, const tform_rt3<T> &t_)
{
  // add transform to transform
  tr_.rotation+=t_.rotation;
  tr_.translation+=t_.translation;
}
//----

template<typename T>
EGL_INLINE void operator-=(tform_rt3<T> &tr_, const tform_rt3<T> &t_)
{
  // subtract transform from transform
  tr_.rotation-=t_.rotation;
  tr_.translation-=t_.translation;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform_rt3<T> &tr_, const tform_rt3<T> &t_)
{
  // multiply transform by transform
  tr_.rotation=t_.rotation*tr_.rotation;
  tr_.translation*=t_.rotation;
  tr_.translation+=t_.translation;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform_rt3<T> &tr_, const quat<T> &q_)
{
  // multiply transform by quaternion
  tr_.rotation=q_*tr_.rotation;
  tr_.translation*=q_;
}
//----

template<typename T>
EGL_INLINE void operator*=(tform_rt3<T> &tr_, typename math<T>::scalar_t s_)
{
  // multiply transform by scalar
  tr_.rotation*=s_;
  tr_.translation*=s_;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec3<T> &vr_, const tform_rt3<T> &t_)
{
  // multiply 3d vector by transform (assume w=1)
  vr_*=t_.rotation;
  vr_+=t_.translation;
}
//----

template<typename T>
EGL_INLINE void operator*=(vec4<T> &vr_, const tform_rt3<T> &t_)
{
  // multiply 4d vector by transform
  vr_.set(vec3<T>(vr_.x, vr_.y, vr_.z)*t_.rotation+vr_.w*t_.translation, vr_.w);
}
//----

template<typename T>
EGL_INLINE void operator*=(mat44<T> &mr_, const tform_rt3<T> &t_)
{
  mr_*=t_.matrix();
}
//----

template<typename T>
EGL_INLINE void operator/=(tform_rt3<T> &tr_, typename math<T>::scalar_t s_)
{
  // divide transform by scalar
  typename math<T>::scalar_t rs=rcp(s_);
  tr_.rotation*=rs;
  tr_.translation*=rs;
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator+(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_)
{
  // add transform to transform
  return tform_rt3<T>(t0_.rotation+t1_.rotation, t0_.translation+t1_.translation);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator-(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_)
{
  // subtract transform from transform
  return tform_rt3<T>(t0_.rotation-t1_.rotation, t0_.translation-t1_.translation);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator-(const tform_rt3<T> &t_)
{
  // negate transform
  return tform_rt3<T>(-t_.rotation, -t_.translation);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator*(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_)
{
  // multiply transform by transform
  return tform_rt3<T>(t1_.rotation*t0_.rotation, t0_.translation*t1_.rotation+t1_.translation);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator*(const tform_rt3<T> &t_, const quat<T> &q_)
{
  // multiply transform by quaternion
  return tform_rt3<T>(q_*t_.rotation, t_.translation*q_);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator*(const tform_rt3<T> &t_, typename math<T>::scalar_t s_)
{
  // multiply transform by scalar
  return tform_rt3<T>(t_.rotation*s_, t_.translation*s_);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator*(typename math<T>::scalar_t s_, const tform_rt3<T> &t_)
{
  // multiply transform by scalar
  return tform_rt3<T>(t_.rotation*s_, t_.translation*s_);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const vec3<T> &v_, const tform_rt3<T> &t_)
{
  // multiply 3d vector by transform (assume w=1)
  return v_*t_.rotation+t_.translation;
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const vec4<T> &v_, const tform_rt3<T> &t_)
{
  // multiply 4d vector by transform
  return vec4<T>(vec3<T>(v_.x, v_.y, v_.z)*t_.rotation+v_.w*t_.translation, v_.w);
}
//----

template<typename T>
EGL_INLINE vec3<T> operator*(const tform_rt3<T> &t_, const vec3<T> &v_)
{
  // multiply transform by transposed 3d vector
  return v_*inv_u(t_.rotation);
}
//----

template<typename T>
EGL_INLINE vec4<T> operator*(const tform_rt3<T> &t_, const vec4<T> &v_)
{
  // multiply transform by transposed 4d vector
  return vec4<T>(((const vec3<T>&)v_)*inv_u(t_.rotation), dot(v_, vec4<T>(t_.translation, typename math<T>::scalar_t(1))));
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(const mat44<T> &m_, const tform_rt3<T> &t_)
{
  return m_*t_.matrix();
}
//----

template<typename T>
EGL_INLINE mat44<T> operator*(const tform_rt3<T> &t_, const mat44<T> &m_)
{
  return t_.matrix()*m_;
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> operator/(const tform_rt3<T> &t_, typename math<T>::scalar_t s_)
{
  // divide transform by scalar
  typename math<T>::scalar_t rs=rcp(s_);
  return tform_rt3<T>(t_.rotation*rs, t_.translation*rs);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> madd(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_, const tform_rt3<T> &t2_)
{
  // component-wise multiply-add: t0_*t1+t2_
  return tform_rt3<T>(madd(t0_.rotation, t1_.rotation, t2_.rotation), madd(t0_.translation, t1_.translation, t2_.translation));
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> inv(const tform_rt3<T> &t_)
{
  // inverse of the transform
  quat<T> ir=inv_u(t_.rotation);
  return tform_rt3<T>(ir, -t_.translation*ir);
}
//----

template<typename T>
EGL_INLINE void identity(tform_rt3<T> &tr_)
{
  // set transform to identity
  tr_.rotation=quat<T>::identity();
  tr_.translation.set(math<T>::scalar_t(0));
}
//----

template<typename T>
EGL_INLINE void neg(tform_rt3<T> &tr_)
{
  // negate the transform
  neg(tr_.rotation);
  neg(tr_.translation);
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> lerp(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_, typename math<T>::scalar_t t_)
{
  // linear interpolation of transforms: f(t=0)=t0, f(t=1)=t1
  return t0_*(typename math<T>::scalar_t(1)-t_)+t1_*t_;
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> nlerp(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_, typename math<T>::scalar_t t_)
{
  // linear interpolation of transforms with normalized rotation
  tform_rt3<T> t=t0_*(typename math<T>::scalar_t(1)-t_)+t1_*t_;
  t.rotation=unit(t.rotation);
  return t;
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> nlerp_z(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_, typename math<T>::scalar_t t_)
{
  // linear interpolation of transforms with normalized rotation, if |rotation|=0, rotation=[0, 0, 0, 1]
  tform_rt3<T> t=t0_*(typename math<T>::scalar_t(1)-t_)+t1_*t_;
  t.rotation=unit_z(t.rotation);
  return t;
}
//----

template<typename T>
EGL_INLINE tform_rt3<T> slerp(const tform_rt3<T> &t0_, const tform_rt3<T> &t1_, typename math<T>::scalar_t t_)
{
  // linear interpolation of translation with spherical rotation interpolation
  return tform_rt3<T>(slerp(t0_.rotation, t1_.rotation, t_), lerp(t0_.translation, t1_.translation, t_));
}
//----

template<typename T>
EGL_INLINE void zrot_u(tform_rt3<T> &tr_, const vec3<T> &pos_, const vec3<T> &zdir_)
{
  // set z-axis to point to given direction by using quaternion rotation
  tr_.rotation=zrot_u(zdir_);
  tr_.translation=pos_;
}
//----

template<typename T>
EGL_INLINE void axis_rot(tform_rt3<T> &tr_, const vec3<T> &pos_, const vec3<T> &axis_, typename math<T>::scalar_t angle_)
{
  // setup transform from position and rotation about given axis
  tr_.rotation=axis_rot(axis_, angle_);
  tr_.translation=pos_;
}
//----

template<typename T>
EGL_INLINE void axis_rot_u(tform_rt3<T> &tr_, const vec3<T> &pos_, const vec3<T> &axis_, typename math<T>::scalar_t angle_)
{
  // setup transform from position and rotation about unit given axis
  tr_.rotation=axis_rot_u(axis_, angle_);
  tr_.translation=pos_;
}
//----

template<typename T, typename U>
EGL_INLINE tform_rt3<T> to_tform_rt3(const tform_rt3<U> &t_)
{
  // convert between tform_rt3 types
  tform_rt3<T> t(to_quat<T>(t_.rotation), to_vec3<T>(t_.translation));
  return t;
}
//----------------------------------------------------------------------------


//============================================================================
// camera
//============================================================================
template<typename T>
camera<T>::camera()
  :m_view_to_proj(scalar_t(1),           0,           0,          0,
                            0, scalar_t(1),           0,          0,
                            0,           0, scalar_t(1),          0,
                            0,           0,          0, scalar_t(1))
  ,m_view_to_world(scalar_t(1),           0,           0,           0,
                             0, scalar_t(1),           0,           0,
                             0,           0, scalar_t(1),           0)
  ,m_near(scalar_t(0))
  ,m_far(scalar_t(1))
{
  update_transforms();
}
//----

template<typename T>
camera<T>::camera(const mat44<T> &view_to_proj_, scalar_t near_, scalar_t far_)
  :m_view_to_proj(view_to_proj_)
  ,m_view_to_world(scalar_t(1),           0,           0, 0,
                             0, scalar_t(1),           0, 0,
                             0,           0, scalar_t(1), 0)
  ,m_near(near_)
  ,m_far(far_)
{
  update_transforms();
}
//----

template<typename T>
camera<T>::camera(const tform3<T> &view_to_world_, const mat44<T> &view_to_proj_, scalar_t near_, scalar_t far_)
  :m_view_to_proj(view_to_proj_)
  ,m_view_to_world(view_to_world_)
  ,m_near(near_)
  ,m_far(far_)
{
  update_transforms();
}
//----------------------------------------------------------------------------

template<typename T>
const mat44<T> &camera<T>::view_to_proj() const
{
  return m_view_to_proj;
}
//----

template<typename T>
const mat44<T> &camera<T>::proj_to_view() const
{
  return m_proj_to_view;
}
//----

template<typename T>
const tform3<T> &camera<T>::world_to_view() const
{
  return m_world_to_view;
}
//----

template<typename T>
const tform3<T> &camera<T>::view_to_world() const
{
  return m_view_to_world;
}
//----

template<typename T>
const mat44<T> &camera<T>::world_to_proj() const
{
  return m_world_to_proj;
}
//----

template<typename T>
const mat44<T> &camera<T>::proj_to_world() const
{
  return m_proj_to_world;
}
//----

template<typename T>
typename math<T>::scalar_t camera<T>::near_plane() const
{
  return m_near;
}
//----

template<typename T>
typename math<T>::scalar_t camera<T>::far_plane() const
{
  return m_far;
}
//----

template<typename T>
vec3<T> camera<T>::world_x() const
{
  return vec3<T>(m_view_to_world.x.x, m_view_to_world.x.y, m_view_to_world.x.z);
}
//----

template<typename T>
vec3<T> camera<T>::world_y() const
{
  return vec3<T>(m_view_to_world.y.x, m_view_to_world.y.y, m_view_to_world.y.z);
}
//----

template<typename T>
vec3<T> camera<T>::world_z() const
{
  return vec3<T>(m_view_to_world.z.x, m_view_to_world.z.y, m_view_to_world.z.z);
}
//----

template<typename T>
vec3<T> camera<T>::world_pos() const
{
  return vec3<T>(m_view_to_world.x.w, m_view_to_world.y.w, m_view_to_world.z.w);
}
//----------------------------------------------------------------------------

template<typename T>
void camera<T>::set_transform(const mat44<T> &view_to_proj_,
                              const tform3<T> &view_to_world_,
                              scalar_t near_,
                              scalar_t far_)
{
  m_view_to_proj=view_to_proj_;
  m_view_to_world=view_to_world_;
  m_near=near_;
  m_far=far_;
  update_transforms();
}
//----

template<typename T>
void camera<T>::set_view_to_proj(const mat44<T> &m_, scalar_t near_, scalar_t far_)
{
  m_view_to_proj=m_;
  m_near=near_;
  m_far=far_;
  update_transforms();
}
//----

template<typename T>
void camera<T>::set_view_to_world(const tform3<T> &t_)
{
  m_view_to_world=t_;
  update_transforms();
}
//----------------------------------------------------------------------------

template<typename T>
void camera<T>::update_transforms()
{
  // update transformation matrices
  m_proj_to_view=inv(m_view_to_proj);
  m_world_to_view=inv(m_view_to_world);
  m_world_to_proj=m_world_to_view*m_view_to_proj;
  m_proj_to_world=m_proj_to_view*m_view_to_world;
}
//----------------------------------------------------------------------------

template<typename T, typename U>
EGL_INLINE camera<T> to_camera(const camera<U> &c_)
{
  // convert between camera types
  typedef typename math<T>::scalar_t scalar_t;
  camera<T> c(to_tform3<T>(c_.view_to_world()),
              to_mat44<T>(c_.view_to_proj()),
              scalar_t(c_.near_plane()),
              scalar_t(c_.far_plane()));
  return c;
}
//----------------------------------------------------------------------------


//============================================================================
// 3d unit vector <-> octahedron coordinate transforms
//============================================================================
template<typename T>
EGL_INLINE vec2<T> vec3_to_oct(const vec3<T> &v_)
{
  // map 3D unit vector to 2D octahedron coordinates [-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vec3<T> v=v_*rnorm_l1(v_);
  scalar_t t=sat(-v.z);
  return vec2<T>(v.x+(v.x<scalar_t(0)?-t:t), v.y+(v.y<scalar_t(0)?-t:t));
}
//----

template<typename T>
EGL_INLINE vec3<T> oct_to_vec3(const vec2<T> &oct_)
{
  // map 2D octahedron coordinates [-1, 1] to 3D unit vector
  typedef typename math<T>::scalar_t scalar_t;
  vec3<T> v(oct_, scalar_t(1)-norm_l1(oct_));
  scalar_t t=max(-v.z, scalar_t(0));
  return unit(vec3<T>(v.x+(v.x<scalar_t(0)?t:-t), v.y+(v.y<scalar_t(0)?t:-t), v.z));
}
//----

template<typename T>
EGL_INLINE vec2<T> vec3_to_oct2x1(const vec3<T> &v_)
{
  // map 3D unit vector to 2D octahedron coordinates [-2, 2]x[-1, 1]
  typedef typename math<T>::scalar_t scalar_t;
  vec3<T> v=v_*rnorm_l1(v_);
  T t=v.x-v.y-scalar_t(1);
  return vec2<T>(v_.z<0?-t:t, v.x+v.y);
}
//----

template<typename T>
EGL_INLINE vec3<T> oct2x1_to_vec3(const vec2<T> &oct_)
{
  // map 2D octahedron coordinates [-2, 2]x[-1, 1] to 3D unit vector
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t x=oct_.y+scalar_t(1)-abs(oct_.x);
  scalar_t y=oct_.y-scalar_t(1)+abs(oct_.x);
  scalar_t t=abs(x)+abs(y)-scalar_t(2);
  return unit(vec3<T>(x, y, oct_.x<0?-t:t));
}
//----------------------------------------------------------------------------


//============================================================================
// 3d frame quantization
//============================================================================
template<typename T>
uint32_t quantize_mat33_32(const mat33<T> &rot_)
{
  // check rotation handedness
  typedef typename math<T>::scalar_t scalar_t;
  scalar_t d=det(rot_);
  EGL_ASSERT_MSG(abs(abs(d)-scalar_t(1))<scalar_t(0.001), "The 3x3 rotation matrix isn't orthonormal\r\n");
  bool is_right_handed=d<0;

  // map frame z-vector to quantized 2:1 octahedron space
  vec2<T> oct=vec3_to_oct2x1(rot_.z);
  uint32_t qx=uint32_t((oct.x+scalar_t(2))*scalar_t(0.25*2047.0)+scalar_t(0.5)); // 11 bits
  uint32_t qy=uint32_t((oct.y+scalar_t(1))*scalar_t(0.5*1023.0)+scalar_t(0.5));  // 10 bits

  // calculate rotation angle about z-vector from quaternion reference rotation.
  // dequantize the quantized z-vector for the exact angle calculation.
  vec3<T> z=oct2x1_to_vec3(vec2<T>(qx*scalar_t(4.0/2047.0)-scalar_t(2),
                                   qy*scalar_t(2.0/1023.0)-scalar_t(1)));
  vec3<T> h=z+vec3<T>(0, 0, scalar_t(1)); // half-vector of the reference rotation
  scalar_t a=scalar_t(2)*rnorm2(h);
  vec3<T> ref_x(scalar_t(1)-a*h.x*h.x, -a*h.y*h.x, -a*h.x*h.z); // reference x-vector
  scalar_t ry=dot(rot_.y, ref_x);
  scalar_t rx=dot(rot_.x, ref_x);
  scalar_t angle=atan2(is_right_handed?ry:-ry, rx);
  uint32_t qa=uint32_t((angle/math<T>::pi+scalar_t(1))*scalar_t(0.5*1023.0)+scalar_t(0.5)); // 10 bits

  // pack bits: aaaaaaaaaahyyyyyyyyyyxxxxxxxxxxx
  // a=angle, h=handedness (0=left, 1=right), xy=octahedron coords
  return (qa<<22)|(is_right_handed?0x200000:0)|(qy<<11)|qx;
}
//----

template<typename T>
mat33<T> dequantize_mat33_32(uint32_t qrot_)
{
  // dequantize frame z-vector
  typedef typename math<T>::scalar_t scalar_t;
  mat33<T> res;
  vec2<T> oct((qrot_&2047)*scalar_t(4.0/2047.0)-scalar_t(2),
              ((qrot_>>11)&1023)*scalar_t(2.0/1023.0)-scalar_t(1));
  res.z=oct2x1_to_vec3(oct);

  // dequantize frame tangent vectors
  scalar_t sa, ca;
  sincos(sa, ca, ((qrot_>>22)*scalar_t(2.0/1023.0)-scalar_t(1))*math<T>::pi);
  vec3<T> h=res.z+vec3<T>(0, 0, scalar_t(1));
  scalar_t a=scalar_t(2)*rnorm2(h)*(-h.x*ca-h.y*sa);
  res.x=h*a+vec3<T>(ca, sa, 0);
  res.y=cross(res.z, qrot_&0x200000?-res.x:res.x);
  return res;
}
//----

template<typename T>
vec3<T> dequantize_mat33z_32(uint32_t qrot_)
{
  // dequantize frame z-vector
  typedef typename math<T>::scalar_t scalar_t;
  vec2<T> oct((qrot_&2047)*scalar_t(4.0/2047.0)-scalar_t(2),
              ((qrot_>>11)&1023)*scalar_t(2.0/1023.0)-scalar_t(1));
  return oct2x1_to_vec3(oct);
}
//----------------------------------------------------------------------------


//============================================================================
// projection matrix setup
//============================================================================
template<typename T>
mat44<T> perspective_matrix(typename math<T>::scalar_t fov_width_, typename math<T>::scalar_t aspect_ratio_, typename math<T>::scalar_t near_, typename math<T>::scalar_t far_, bool reversed_)
{
  // setup perspective matrix with FOV angles (radians) and near/far planes
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT(near_>=0);
  EGL_ASSERT(far_>near_);
  EGL_ASSERT(fov_width_<math<T>::pi);
  scalar_t q=far_*rcp(far_-near_);
  scalar_t b=0;
  if(reversed_)
  {
    q=-q;
    b=scalar_t(1);
  }
  scalar_t w=cot(fov_width_*scalar_t(0.5));
  scalar_t h=w*aspect_ratio_;
  return mat44f(w, 0,        0,           0,
                0, h,        0,           0,
                0, 0,      b+q, scalar_t(1),
                0, 0, -q*near_,           0);
}
//----

template<typename T>
mat44<T> orthogonal_matrix(typename math<T>::scalar_t width_, typename math<T>::scalar_t aspect_ratio_, typename math<T>::scalar_t near_, typename math<T>::scalar_t far_, bool reversed_)
{
  // setup orthogonal projection matrix
  typedef typename math<T>::scalar_t scalar_t;
  EGL_ASSERT(width_>0);
  EGL_ASSERT(far_>near_);
  scalar_t q=rcp(far_-near_);
  scalar_t b=0;
  if(reversed_)
  {
    q=-q;
    b=scalar_t(1);
  }
  scalar_t w=scalar_t(2)*rcp(width_);
  scalar_t h=w*aspect_ratio_;
  return mat44<T>(w, 0,         0,           0,
                  0, h,         0,           0,
                  0, 0,         q,           0,
                  0, 0, b-q*near_, scalar_t(1));
}
//----------------------------------------------------------------------------


//============================================================================
// fov_width/height
//============================================================================
template<typename T>
EGL_INLINE typename math<T>::scalar_t fov_width(const mat44<T> &v2p_)
{
  return 2*atan(1/v2p_.x.x);
}
//----

template<typename T>
EGL_INLINE typename math<T>::scalar_t fov_height(const mat44<T> &v2p_)
{
  return 2*atan(1/v2p_.y.y);
}
//----------------------------------------------------------------------------


//============================================================================
// sphere_screen_extents
//============================================================================
// Calculates the exact screen extents xyzw=[left, bottom, right, top] in
// normalized screen coordinates [-1, 1] for a sphere in view space. For
// performance, the projection matrix (v2p) is assumed to be setup so that
// z.w=1 and w.w=0. The sphere is also assumed to be completely in front
// of the camera.
// This is an optimized implementation of paper "2D Polyhedral Bounds of a
// Clipped Perspective-Projected 3D Sphere": http://jcgt.org/published/0002/02/05/paper.pdf
template<typename T>
vec4<T> sphere_screen_extents(const vec3<T> &pos_, typename math<T>::scalar_t rad_, const mat44<T> &v2p_)
{
  // calculate horizontal extents
  EGL_ASSERT_PEDANTIC(v2p_.z.w==1 && v2p_.w.w==0);
  typedef typename math<T>::scalar_t scalar_t;
  vec4<T> res;
  scalar_t r2=rad_*rad_, d=pos_.z*rad_;
  scalar_t hv=sqrt(pos_.x*pos_.x+pos_.z*pos_.z-r2);
  scalar_t ha=pos_.x*hv, hb=pos_.x*rad_, hc=pos_.z*hv;
  res.x=(ha-d)*v2p_.x.x/(hc+hb); // left
  res.z=(ha+d)*v2p_.x.x/(hc-hb); // right

  // calculate vertical extents
  scalar_t vv=sqrt(pos_.y*pos_.y+pos_.z*pos_.z-r2);
  scalar_t va=pos_.y*vv, vb=pos_.y*rad_, vc=pos_.z*vv;
  res.y=(va-d)*v2p_.y.y/(vc+vb); // bottom
  res.w=(va+d)*v2p_.y.y/(vc-vb); // top
  return res;
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
