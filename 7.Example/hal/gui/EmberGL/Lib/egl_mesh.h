//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_MESH_H
#define EGL_MESH_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_rasterizer.h"
#include "egl_tform3.h"
EGL_NAMESPACE_BEGIN

// new
enum {p3g_file_ver=0x1000};
struct p3g_sphere3f;
struct p3g_cone3f;
class p3g_mesh_segment;
class p3g_meshlet;
class p3g_mesh;
template<class PSCBase, class VIn, class VShader, class PShader> class psc_p3g_mesh;
//----------------------------------------------------------------------------


//============================================================================
// p3g_sphere3f
//============================================================================
struct p3g_sphere3f
{
  vec3f pos;
  float rad;
};
//----------------------------------------------------------------------------


//============================================================================
// p3g_cone3f
//============================================================================
struct p3g_cone3f
{
  vec3f dir;
  float dot;
};
//----------------------------------------------------------------------------


//============================================================================
// p3g_mesh_segment
//============================================================================
class p3g_mesh_segment
{
public:
  // accessors
  EGL_INLINE uint32_t material_id() const   {return m_material_id;}
  EGL_INLINE uint16_t num_meshlets() const  {return m_num_mlets;}
  //--------------------------------------------------------------------------

private:
  friend class p3g_mesh;
  uint32_t m_material_id;
  uint16_t m_mlet_start_idx;
  uint16_t m_num_mlets;
  int16_t m_qbvol_pos[3];
  uint16_t m_qbvol_rad;
};
//----------------------------------------------------------------------------


//============================================================================
// p3g_meshlet
//============================================================================
class p3g_meshlet
{
public:
  // accessors
  EGL_INLINE uint8_t num_vertices() const   {return m_num_vtx;}
  EGL_INLINE uint8_t num_triangles() const  {return m_num_tris;}
  //--------------------------------------------------------------------------

private:
  friend class p3g_mesh;
  uint32_t m_vidx_offs:24;
  uint32_t m_num_vtx:8;
  uint32_t m_tidx_offs:24;
  uint32_t m_num_tris:8;
  int8_t m_qbvol_pos[3];
  uint8_t m_qbvol_rad;
  int8_t m_qvcone_dir[3];
  int8_t m_qvcone_dot;
};
//----------------------------------------------------------------------------


//============================================================================
// p3g_mesh
//============================================================================
class p3g_mesh
{
public:
  // construction
  p3g_mesh();
  explicit p3g_mesh(const void *p3g_file_);
  void init(const void *p3g_file_);
  EGL_INLINE bool is_valid() const;
  //--------------------------------------------------------------------------

  // mesh accessors
  EGL_INLINE p3g_sphere3f mesh_bvol() const;
  EGL_INLINE bool use_32bit_vertex_indices() const;
  EGL_INLINE bool use_tri_strips() const;
  EGL_INLINE bool use_meshlet_bvols() const;
  EGL_INLINE bool use_meshlet_vcones() const;
  //--------------------------------------------------------------------------

  // vertex buffer accessors
  EGL_INLINE uint8_t vertex_format_id() const;
  EGL_INLINE const void *vertex_buffer() const;
  //--------------------------------------------------------------------------

  // segment accessors
  EGL_INLINE uint8_t num_segments() const;
  EGL_INLINE const p3g_mesh_segment &segment(uint16_t seg_idx_) const;
  EGL_INLINE p3g_sphere3f segment_bvol(const p3g_mesh_segment&) const;
  //--------------------------------------------------------------------------

  // meshlet accessors
  EGL_INLINE const p3g_meshlet &meshlet(const p3g_mesh_segment&, uint16_t mlet_idx_) const;
  EGL_INLINE p3g_sphere3f meshlet_bvol(const p3g_meshlet&, const p3g_sphere3f &seg_bsphere_) const;
  EGL_INLINE p3g_cone3f meshlet_vcone(const p3g_meshlet&) const;
  EGL_INLINE const uint16_t *meshlet_vertex_ibuf16(const p3g_meshlet&) const;
  EGL_INLINE const uint32_t *meshlet_vertex_ibuf32(const p3g_meshlet&) const;
  EGL_INLINE const uint8_t *meshlet_triangle_ibuf(const p3g_meshlet&) const;
  //--------------------------------------------------------------------------

private:
  p3g_mesh(const p3g_mesh&); // not implemented
  void operator=(const p3g_mesh&); // not implemented
  //--------------------------------------------------------------------------

  //==========================================================================
  // e_p3g_flags
  //==========================================================================
  enum e_p3g_flags
  {
    p3gflag_32bit_vidx  =0x0001,
    p3gflag_tristrips   =0x0002,
    p3gflag_bvols       =0x0004,
    p3gflag_vcones      =0x0008
  };
  //--------------------------------------------------------------------------

  //==========================================================================
  // p3g_header
  //==========================================================================
  struct p3g_header
  {
    char file_sig[4];
    uint16_t file_ver;
    uint16_t flags;
    uint32_t fsize;
    uint16_t num_mlets;
    uint8_t num_segs;
    uint8_t vfmt_id;
    uint32_t vbuf_offs;
    uint32_t vbuf_size;
    p3g_sphere3f mesh_bvol;
    p3g_mesh_segment segs[1];
  };
  //--------------------------------------------------------------------------

  const p3g_header *m_header;
  const uint8_t *m_meshlets;
  uint8_t m_meshlet_size;
};
//----------------------------------------------------------------------------


//============================================================================
// psc_p3g_mesh
//============================================================================
// Basic PSC for rendering P3G meshes.
template<class PSCBase, class VIn, class VShader, class PShader>
class psc_p3g_mesh: public PSCBase
{
public:
  // vertex and pixel formats
  typedef VIn vin;
  typedef typename PShader::psin ptv_fmt;
  //--------------------------------------------------------------------------

  //==========================================================================
  // pso_state
  //==========================================================================
  struct pso_state
  {
    tform3f obj_to_view;
    mat44f obj_to_proj;
    vec3f cam_opos;
    float meshlet_bvol_scale;
  };
  //--------------------------------------------------------------------------

  // construction & setup
  EGL_INLINE psc_p3g_mesh();
  EGL_INLINE void set_geometry(const p3g_mesh &m_, uint16_t seg_idx_);
  EGL_INLINE void set_transform(const cameraf&, const tform3f &object_to_world_);
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE vec3f mesh_bvol_pos() const;
  EGL_INLINE float mesh_bvol_rad() const;
  EGL_INLINE VShader &vshader();
  EGL_INLINE PShader &pshader();
  //--------------------------------------------------------------------------

  // PSC interface
  EGL_INLINE rasterizer_local_cluster_index_t num_clusters() const;
  EGL_INLINE const void *cluster(rasterizer_local_cluster_index_t) const;
  EGL_INLINE uint8_t num_cluster_vertices(const void *cluster_) const;
  EGL_INLINE uint8_t num_cluster_triangles(const void *cluster_) const;
  void get_pso_state(pso_state&) const;
  void bin_cluster(rasterizer_tiling &tiling_, const pso_state&, rasterizer_dispatch_index_t, rasterizer_local_cluster_index_t) const;
  void tform_cluster(ptv_fmt *ptvs_, const pso_state&, const void *cluster_) const;
  EGL_INLINE void setup_primitive(const void *cluster_, uint8_t prim_idx_, uint8_t vidx_[3]) const;
  template<class PSC> EGL_INLINE void shade_pixel(rasterizer_pixel_out<PSC>&, const pso_state&, const ptv_fmt &v0_, const ptv_fmt &v1_, const ptv_fmt &v2_, const vec3f &bc_, uint8_t prim_idx_) const;
  //--------------------------------------------------------------------------

private:
  // state
  const p3g_mesh *m_mesh;
  const p3g_mesh_segment *m_seg;
  const cameraf *m_camera;
  tform3f m_obj_to_world;
  VShader m_vshader;
  PShader m_pshader;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// p3g_mesh
//============================================================================
bool p3g_mesh::is_valid() const
{
  return m_header!=0;
}
//----------------------------------------------------------------------------

p3g_sphere3f p3g_mesh::mesh_bvol() const
{
  return m_header->mesh_bvol;
}
//----

bool p3g_mesh::use_32bit_vertex_indices() const
{
  return m_header->flags&p3gflag_32bit_vidx;
}
//----

bool p3g_mesh::use_tri_strips() const
{
  return m_header->flags&p3gflag_tristrips;
}
//----

bool p3g_mesh::use_meshlet_bvols() const
{
  return m_header->flags&p3gflag_bvols;
}
//----

bool p3g_mesh::use_meshlet_vcones() const
{
  return m_header->flags&p3gflag_vcones;
}
//----------------------------------------------------------------------------

uint8_t p3g_mesh::vertex_format_id() const
{
  return m_header->vfmt_id;
}
//----

const void *p3g_mesh::vertex_buffer() const
{
  return ((uint8_t*)m_header)+m_header->vbuf_offs;
}
//----------------------------------------------------------------------------

uint8_t p3g_mesh::num_segments() const
{
  return m_header->num_segs;
}
//----

const p3g_mesh_segment &p3g_mesh::segment(uint16_t seg_idx_) const
{
  EGL_ASSERT_PEDANTIC(seg_idx_<m_header->num_segs);
  return m_header->segs[seg_idx_];
}
//----

p3g_sphere3f p3g_mesh::segment_bvol(const p3g_mesh_segment &seg_) const
{
  // dequantize segment bounding volume
  float mesh_bvol_rad=m_header->mesh_bvol.rad;
  float rad_scale=mesh_bvol_rad/32767.0f;
  p3g_sphere3f bvol=
  {
    vec3f(seg_.m_qbvol_pos[0], seg_.m_qbvol_pos[1], seg_.m_qbvol_pos[2])*rad_scale+m_header->mesh_bvol.pos,
    seg_.m_qbvol_rad*mesh_bvol_rad/65535.0f
  };
  return bvol;
}
//----------------------------------------------------------------------------

const p3g_meshlet &p3g_mesh::meshlet(const p3g_mesh_segment &seg_, uint16_t mlet_idx_) const
{
  EGL_ASSERT_PEDANTIC(mlet_idx_<m_header->num_mlets);
  uint16_t mlet_idx=seg_.m_mlet_start_idx+mlet_idx_;
  return *(const p3g_meshlet*)(m_meshlets+mlet_idx*m_meshlet_size);
}
//----

p3g_sphere3f p3g_mesh::meshlet_bvol(const p3g_meshlet &mlet_, const p3g_sphere3f &seg_bsphere_) const
{
  // dequantize meshlet bounding volume
  float seg_bvol_rad=seg_bsphere_.rad;
  float rad_scale=seg_bvol_rad/127.0f;
  p3g_sphere3f bvol=
  {
    vec3f(mlet_.m_qbvol_pos[0], mlet_.m_qbvol_pos[1], mlet_.m_qbvol_pos[2])*rad_scale+seg_bsphere_.pos,
    mlet_.m_qbvol_rad*seg_bvol_rad/255.0f
  };
  return bvol;
}
//----

p3g_cone3f p3g_mesh::meshlet_vcone(const p3g_meshlet &mlet_) const
{
  // dequantize meshlet visibility cone
  p3g_cone3f cone=
  {
    vec3f(mlet_.m_qvcone_dir[0], mlet_.m_qvcone_dir[1], mlet_.m_qvcone_dir[2])*(1.0f/127.0f),
    mlet_.m_qvcone_dot/127.0f
  };
  return cone;
}
//----

const uint16_t *p3g_mesh::meshlet_vertex_ibuf16(const p3g_meshlet &mlet_) const
{
  return (const uint16_t*)(((const uint8_t*)m_header)+mlet_.m_vidx_offs);
}
//----

const uint32_t *p3g_mesh::meshlet_vertex_ibuf32(const p3g_meshlet &mlet_) const
{
  return (const uint32_t*)(((const uint8_t*)m_header)+mlet_.m_vidx_offs);
}
//----

const uint8_t *p3g_mesh::meshlet_triangle_ibuf(const p3g_meshlet &mlet_) const
{
  return ((const uint8_t*)m_header)+mlet_.m_tidx_offs;
}
//----------------------------------------------------------------------------


//============================================================================
// psc_p3g_mesh
//============================================================================
template<class PSCBase, class VIn, class VShader, class PShader>
psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::psc_p3g_mesh()
{
  m_mesh=0;
  m_seg=0;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::set_geometry(const p3g_mesh &m_, uint16_t seg_idx_)
{
  m_mesh=&m_;
  m_seg=&m_.segment(seg_idx_);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::set_transform(const cameraf &cam_, const tform3f &object_to_world_)
{
  m_camera=&cam_;
  m_obj_to_world=object_to_world_;
}
//----------------------------------------------------------------------------

template<class PSCBase, class VIn, class VShader, class PShader>
vec3f psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::mesh_bvol_pos() const
{
  return m_mesh->mesh_bvol().pos;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
float psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::mesh_bvol_rad() const
{
  return m_mesh->mesh_bvol().rad;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
VShader &psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::vshader()
{
  return m_vshader;
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
PShader &psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::pshader()
{
  return m_pshader;
}
//----------------------------------------------------------------------------

template<class PSCBase, class VIn, class VShader, class PShader>
rasterizer_local_cluster_index_t psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::num_clusters() const
{
  return m_seg->num_meshlets();
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
const void *psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::cluster(rasterizer_local_cluster_index_t cluster_idx_) const
{
  return &m_mesh->meshlet(*m_seg, cluster_idx_);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
uint8_t psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::num_cluster_vertices(const void *cluster_) const
{
  const p3g_meshlet *mlet=(const p3g_meshlet*)cluster_;
  uint8_t num_cluster_vtx=mlet->num_vertices();
  return num_cluster_vtx;
}
//---

template<class PSCBase, class VIn, class VShader, class PShader>
uint8_t psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::num_cluster_triangles(const void *cluster_) const
{
  const p3g_meshlet *mlet=(const p3g_meshlet*)cluster_;
  uint8_t num_cluster_tris=mlet->num_triangles();
  return num_cluster_tris;
}
//----
                       
template<class PSCBase, class VIn, class VShader, class PShader>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::get_pso_state(pso_state &pso_state_) const
{
  pso_state_.obj_to_view=m_obj_to_world*m_camera->world_to_view();
  pso_state_.obj_to_proj=m_obj_to_world*m_camera->world_to_proj();
  pso_state_.cam_opos=m_camera->world_pos()*inv(m_obj_to_world);
  pso_state_.meshlet_bvol_scale=sqrt(max(norm2(m_obj_to_world.x_axis()),
                                         norm2(m_obj_to_world.y_axis()),
                                         norm2(m_obj_to_world.z_axis())));
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::bin_cluster(rasterizer_tiling &tiling_, const pso_state &pso_state_, rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t cluster_idx_) const 
{
  // check for visibility cone culling and bin the cluster
  const p3g_meshlet &mlet=m_mesh->meshlet(*m_seg, cluster_idx_);
  const p3g_sphere3f &mlet_bvol=m_mesh->meshlet_bvol(mlet, m_mesh->segment_bvol(*m_seg));
  const p3g_cone3f &mlet_vcone=m_mesh->meshlet_vcone(mlet);
  if(m_mesh->use_meshlet_vcones() && dot(mlet_vcone.dir, unit(pso_state_.cam_opos-(mlet_bvol.pos-mlet_vcone.dir*mlet_bvol.rad)))<mlet_vcone.dot)
    return;
  tiling_.add_cluster(m_camera->view_to_proj(), mlet_bvol.pos*pso_state_.obj_to_view, mlet_bvol.rad*pso_state_.meshlet_bvol_scale, dispatch_idx_, cluster_idx_);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::tform_cluster(ptv_fmt *ptvs_, const pso_state &pso_state_, const void *cluster_) const
{
  // transform cluster vertices
  const p3g_meshlet *mlet=(const p3g_meshlet*)cluster_;
  uint8_t num_cluster_vtx=mlet->num_vertices();
  const uint16_t *vibuf=m_mesh->meshlet_vertex_ibuf16(*mlet);
  const vin *vbuf=(const vin*)m_mesh->vertex_buffer();
  typename vin::transform_state tstate(*this);
  for(uint8_t i=0; i<num_cluster_vtx; ++i)
    m_vshader.exec(*ptvs_++, pso_state_, tstate, vbuf[vibuf[i]], i);
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::setup_primitive(const void *cluster_, uint8_t prim_idx_, uint8_t vidx_[3]) const
{
  // setup primitive vertex indices
  const p3g_meshlet *mlet=(const p3g_meshlet*)cluster_;
  const uint8_t *tibuf=m_mesh->meshlet_triangle_ibuf(*mlet);
  const uint8_t *pidx=tibuf+prim_idx_*3;
  vidx_[0]=pidx[0];
  vidx_[1]=pidx[1];
  vidx_[2]=pidx[2];
}
//----

template<class PSCBase, class VIn, class VShader, class PShader>
template<class PSC>
void psc_p3g_mesh<PSCBase, VIn, VShader, PShader>::shade_pixel(rasterizer_pixel_out<PSC> &psout_, const pso_state &pso_state_, const ptv_fmt &v0_, const ptv_fmt &v1_, const ptv_fmt &v2_, const vec3f &bc_, uint8_t prim_idx_) const
{
  m_pshader.exec(psout_, pso_state_, v0_, v1_, v2_, bc_, prim_idx_);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
