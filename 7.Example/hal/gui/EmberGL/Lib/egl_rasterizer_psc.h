//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_RASTERIZER_PSC_H
#define EGL_RASTERIZER_PSC_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_rasterizer_config.h"
#include "egl_rasterizer_target.h"
#include "egl_tform3.h"
EGL_NAMESPACE_BEGIN
class rasterizer_tiling;

// new
struct rasterizer_psc_base;
template<class PSCBase, class VIn, class VShader, class PShader> class psc_geometry_cluster;
//----------------------------------------------------------------------------


//============================================================================
// e_rstate_tbfc
//============================================================================
// Triangle back-face culling mode
enum e_rstate_tbfc_mode
{
  rstate_tbfc_none,  // no triangle culling
  rstate_tbfc_cw,    // cull clockwise winding triangles
  rstate_tbfc_ccw,   // cull counter-clockwise winding triangles
};
//----------------------------------------------------------------------------


//============================================================================
// e_rstate_depth_func
//============================================================================
// Depth testing function
enum e_rstate_depth_func
{
  rstate_depthfunc_always,
  rstate_depthfunc_never,
  rstate_depthfunc_equal,
  rstate_depthfunc_not_equal,
  rstate_depthfunc_less,
  rstate_depthfunc_less_equal,
  rstate_depthfunc_greater,
  rstate_depthfunc_greater_equal,
};
//----------------------------------------------------------------------------


//============================================================================
// e_rstate_bci_mode
//============================================================================
// Triangle barycentric coordinate interpolation mode
enum e_rstate_bci_mode
{
  rstate_bcimode_noperspective,     // no perspective-correct interpolation (fastest)
  rstate_bcimode_perspective,       // perspective correct interpolation
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_psc_base
//============================================================================
struct rasterizer_psc_base
{
  // default render state
  enum {rt0_fmt=pixfmt_none};                    // render target formats
  enum {rt1_fmt=pixfmt_none};
  enum {rt2_fmt=pixfmt_none};
  enum {rt3_fmt=pixfmt_none};
  enum {depth_format=depthfmt_none};             // depth buffer format. set this to the same format as used in device config
  enum {depth_func=rstate_depthfunc_less_equal}; // depth test function
  enum {depth_write=true};                       // enable depth buffer writes
  enum {hiz_test=true};                          // enable hi-z test
  enum {hiz_update=true};                        // enable hi-z update
  enum {tbfc_mode=rstate_tbfc_ccw};              // triangle back-face culling mode
  enum {bci_mode=rstate_bcimode_perspective};    // barycentric coordinate interpolation mode
  enum {vertex_cache=true};                      // enable post-transform vertex caching
  // debug state
  enum {debug_disable_pixel_depth_test=false};   // force-disable per-pixel depth test (always passes)
  //--------------------------------------------------------------------------

  // post-transform vertex format output by the cluster vertex transform and
  // input by pixel shading function. only requirement is that the format contains
  // "vec4f pos", "vec3f pos" or "vec2f pos" member to define clip-space vertex
  // coordinates required by the rasterizer. otherwise the format can be freely
  // defined.
  struct ptv_fmt {vec4f pos;};
  //--------------------------------------------------------------------------

  struct pso_state {};
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE rasterizer_local_cluster_index_t num_clusters() const;
  EGL_INLINE const void *cluster(rasterizer_local_cluster_index_t cluster_idx_) const; // returns pointer to the cluster at given index
  EGL_INLINE uint8_t num_cluster_vertices(const void *cluster_) const; // returns the number of vertices in the cluster
  EGL_INLINE uint8_t num_cluster_triangles(const void *cluster_) const; // returns the number of triangles in the cluster
  //--------------------------------------------------------------------------

  // called when the cluster should be binned to tiles. the function can implement
  // cluster culling (e.g. with visibility codes) and call tiling_.add_cluster()
  // to bin the cluster if it passes the culling.
  EGL_INLINE void bin_cluster(rasterizer_tiling &tiling_, const pso_state&, rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t cluster_idx_) const;
  // called for each tile before the PSO is being rasterized. This can be used
  // to store temporal data used for rasterization in different PSO stages.
  EGL_INLINE void get_pso_state(pso_state&) const;
  // called to transform cluster_ vertices to ptvs_.
  EGL_INLINE void tform_cluster(ptv_fmt *ptvs_, const pso_state&, const void *cluster_) const;
  // called to prepare cluster_ primitive prim_idx_ for rasterization and output
  // 3 vertex indices (referring to ptvs_ array of tform_cluster) to vidx_
  EGL_INLINE void setup_primitive(const void *cluster_, uint8_t prim_idx_, uint8_t vidx_[3]) const;
  // called for shading a triangle pixel, which export the result to psout_.
  // v0_, v1_ and v2_ are the post-transform triangle vertices whose attributes
  // can be interpolated with the passed barycentric coordinates bc_. For
  // example if the vertices have an attribute "color", then the interpolated
  // value is: v0_.color*bc_.x+v1_.color*bc_.y+v2_.color*bc.z
  // prim_idx_ is the primitive index of the currently rendered cluster.
  template<class PSC> EGL_INLINE void shade_pixel(rasterizer_pixel_out<PSC> &psout_, const pso_state&, const ptv_fmt &v0_, const ptv_fmt &v1_, const ptv_fmt &v2_, const vec3f &bc_, uint8_t prim_idx_) const;
  // programmable render target blending
  template<e_pixel_format rt_fmt, e_pixel_format src_fmt> static EGL_INLINE void blend_rt0(pixel<rt_fmt> &res_, const pixel<rt_fmt> &dst_, const pixel<src_fmt> &src_);
  template<e_pixel_format rt_fmt, e_pixel_format src_fmt> static EGL_INLINE void blend_rt1(pixel<rt_fmt> &res_, const pixel<rt_fmt> &dst_, const pixel<src_fmt> &src_);
  template<e_pixel_format rt_fmt, e_pixel_format src_fmt> static EGL_INLINE void blend_rt2(pixel<rt_fmt> &res_, const pixel<rt_fmt> &dst_, const pixel<src_fmt> &src_);
  template<e_pixel_format rt_fmt, e_pixel_format src_fmt> static EGL_INLINE void blend_rt3(pixel<rt_fmt> &res_, const pixel<rt_fmt> &dst_, const pixel<src_fmt> &src_);
  EGL_STATIC_ASSERT_MSG(rasterizer_max_rts==4, blend_rt_supports_4_rts);
};
//----------------------------------------------------------------------------


//============================================================================
// psc_geometry_cluster
//============================================================================
template<class PSCBase, class VIn, class VShader, class PShader>
class psc_geometry_cluster: public PSCBase
{
public:
  // vertex and pixel formats
  typedef VIn vin;
  typedef typename PShader::psin ptv_fmt;
  typedef typename PSCBase::pso_state pso_state;
  //--------------------------------------------------------------------------

  // construction & setup
  EGL_INLINE psc_geometry_cluster();
  EGL_INLINE psc_geometry_cluster(const VIn *vertices_, uint8_t num_vertices_, const uint8_t *tri_indices_, uint8_t num_triangles_);
  EGL_INLINE void set_geometry(const VIn *vertices_, uint8_t num_vertices_, const uint8_t *tri_indices_, uint8_t num_triangles_);
  EGL_INLINE void set_transform(const cameraf&, const tform3f &object_to_world_);
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE vec3f mesh_bvol_pos() const;
  EGL_INLINE float mesh_bvol_rad() const;
  EGL_INLINE VShader &vshader();
  EGL_INLINE PShader &pshader();
  EGL_INLINE const mat44f &object_to_proj() const;
  //--------------------------------------------------------------------------

  // PSC interface
  EGL_INLINE rasterizer_local_cluster_index_t num_clusters() const;
  EGL_INLINE const void *cluster(rasterizer_local_cluster_index_t) const;
  EGL_INLINE uint8_t num_cluster_vertices(const void *cluster_) const;
  EGL_INLINE uint8_t num_cluster_triangles(const void *cluster_) const;
  void bin_cluster(rasterizer_tiling &tiling_, const pso_state&, rasterizer_dispatch_index_t, rasterizer_local_cluster_index_t) const;
  void tform_cluster(ptv_fmt *ptvs_, const pso_state&, const void *cluster_) const;
  EGL_INLINE void setup_primitive(const void *cluster_, uint8_t prim_idx_, uint8_t vidx_[3]) const;
  template<class PSC> EGL_INLINE void shade_pixel(rasterizer_pixel_out<PSC> &psout_, const pso_state&, const ptv_fmt &v0_, const ptv_fmt &v1_, const ptv_fmt &v2_, const vec3f &bc_, uint8_t prim_idx_) const;
  //--------------------------------------------------------------------------

private:
  VShader m_vshader;
  PShader m_pshader;
  const VIn *m_vertices;
  const uint8_t *m_tri_indices;
  uint8_t m_num_vertices;
  uint8_t m_num_triangles;
  // transforms
  mat44f m_object_to_proj;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// rasterizer_psc_base
//============================================================================
void rasterizer_psc_base::get_pso_state(pso_state&) const
{
}
//----

template<e_pixel_format rt_fmt, e_pixel_format src_fmt>
void rasterizer_psc_base::blend_rt0(pixel<rt_fmt> &res_, const pixel<rt_fmt>&, const pixel<src_fmt> &src_)
{
  res_=src_;
}
//----

template<e_pixel_format rt_fmt, e_pixel_format src_fmt>
void rasterizer_psc_base::blend_rt1(pixel<rt_fmt> &res_, const pixel<rt_fmt>&, const pixel<src_fmt> &src_)
{
  res_=src_;
}
//----

template<e_pixel_format rt_fmt, e_pixel_format src_fmt>
void rasterizer_psc_base::blend_rt2(pixel<rt_fmt> &res_, const pixel<rt_fmt>&, const pixel<src_fmt> &src_)
{
  res_=src_;
}
//----

template<e_pixel_format rt_fmt, e_pixel_format src_fmt>
void rasterizer_psc_base::blend_rt3(pixel<rt_fmt> &res_, const pixel<rt_fmt>&, const pixel<src_fmt> &src_)
{
  res_=src_;
}
//----------------------------------------------------------------------------


//============================================================================
// psc_geometry_cluster
//============================================================================
template<class PSCBase, class VIn, class VShader, class PShader>
psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::psc_geometry_cluster()
  :m_vertices(0)
  ,m_tri_indices(0)
  ,m_num_vertices(0)
  ,m_num_triangles(0)
{
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::psc_geometry_cluster(const VIn *vertices_, uint8_t num_vertices_, const uint8_t *tri_indices_, uint8_t num_triangles_)
{
  m_vertices=vertices_;
  m_tri_indices=tri_indices_;
  m_num_vertices=num_vertices_;
  m_num_triangles=num_triangles_;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::set_geometry(const VIn *vertices_, uint8_t num_vertices_, const uint8_t *tri_indices_, uint8_t num_triangles_)
{
  m_vertices=vertices_;
  m_tri_indices=tri_indices_;
  m_num_vertices=num_vertices_;
  m_num_triangles=num_triangles_;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::set_transform(const cameraf &cam_, const tform3f &object_to_world_)
{
  m_object_to_proj=object_to_world_*cam_.world_to_proj();
}
//----------------------------------------------------------------------------

template<class PSCBase, class VIn, class VShader, class PShader>
vec3f psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::mesh_bvol_pos() const
{
  EGL_ERROR_NOT_IMPL();
  /*todo*/
  return vec3f(0.0f);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
float psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::mesh_bvol_rad() const
{
  EGL_ERROR_NOT_IMPL();
  /*todo*/
  return 0.0f;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
VShader &psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::vshader()
{
  return m_vshader;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
PShader &psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::pshader()
{
  return m_pshader;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
const mat44f &psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::object_to_proj() const
{
  return m_object_to_proj;
}
//----------------------------------------------------------------------------

template<class PSCBase, class VIn, class VShader, class PShader>
rasterizer_local_cluster_index_t psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::num_clusters() const
{
  return 1;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
const void *psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::cluster(rasterizer_local_cluster_index_t) const
{
  return 0;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
uint8_t psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::num_cluster_vertices(const void*) const
{
  return m_num_vertices;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
uint8_t psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::num_cluster_triangles(const void*) const
{
  return m_num_triangles;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::bin_cluster(rasterizer_tiling &tiling_, const pso_state&, rasterizer_dispatch_index_t dispatch_index_, rasterizer_local_cluster_index_t) const
{
  tiling_.add_cluster(dispatch_index_, 0);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::tform_cluster(ptv_fmt *ptvs_, const pso_state &pso_state_, const void*) const
{
  // transform cluster vertices
  typename vin::transform_state tstate(*this);
  for(uint8_t i=0; i<m_num_vertices; ++i)
    m_vshader.exec(*ptvs_++, pso_state_, tstate, m_vertices[i], i);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::setup_primitive(const void*, uint8_t prim_idx_, uint8_t vidx_[3]) const
{
  // setup primitive vertex indices
  const uint8_t *pidx=m_tri_indices+prim_idx_*3;
  vidx_[0]=pidx[0];
  vidx_[1]=pidx[1];
  vidx_[2]=pidx[2];
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
template<class PSC>
void psc_geometry_cluster<PSCBase, VIn, VShader, PShader>::shade_pixel(rasterizer_pixel_out<PSC> &psout_, const pso_state &pso_state_, const ptv_fmt &v0_, const ptv_fmt &v1_, const ptv_fmt &v2_, const vec3f &bc_, uint8_t prim_idx_) const
{
  m_pshader.exec(psout_, pso_state_, v0_, v1_, v2_, bc_, prim_idx_);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
