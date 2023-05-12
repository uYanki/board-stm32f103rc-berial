//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_RASTERIZER_CACHE_H
#define EGL_RASTERIZER_CACHE_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_rasterizer_config.h"
#include "egl_math.h"
EGL_NAMESPACE_BEGIN

// new
struct rasterizer_vertex_cache_cfg;
class rasterizer_vertex_cache;
template<class PTV> EGL_INLINE void ndc_transform(vec2f*, PTV*, uint8_t num_vertices_);
template<class PTV> EGL_INLINE void ndc_transform(vec3f*, PTV*, uint8_t num_vertices_);
template<class PTV> EGL_INLINE void ndc_transform(vec4f*, PTV*, uint8_t num_vertices_);
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_vertex_cache_cfg
//============================================================================
struct rasterizer_vertex_cache_cfg
{
  // construction and setup
  rasterizer_vertex_cache_cfg();
  void set_cache(void *cache_, size_t cache_size_);
  void set_cluster_ptv_buffer(void *ptv_buffer_, size_t ptv_buffer_size_);
  void set_cache_clusters(rasterizer_vertex_cache_offset_t*, size_t max_clusters_);
  //--------------------------------------------------------------------------

  void *cache;
  void *tmp_cluster_ptv_buffer;
  rasterizer_vertex_cache_offset_t *cluster_vcache_offs;
  size_t cache_size;
  size_t tmp_ptv_buffer_size;
  size_t max_clusters;
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_vertex_cache
//============================================================================
class rasterizer_vertex_cache
{
public:
  // construction
  rasterizer_vertex_cache();
  void init(const rasterizer_vertex_cache_cfg&);
  void clear();
  //--------------------------------------------------------------------------

  template<class PSC> const typename PSC::ptv_fmt *get_cluster_ptv_buffer(const PSC&, const typename PSC::pso_state&, const void *cluster_, rasterizer_local_cluster_index_t, rasterizer_global_cluster_index_t, uint16_t thread_idx_);
  //--------------------------------------------------------------------------

private:
  rasterizer_vertex_cache(const rasterizer_vertex_cache&); // not implemented
  void operator=(const rasterizer_vertex_cache&); // not implemented
  EGL_STATIC_ASSERT_MSG((rasterizer_tform_cache_block_size&(rasterizer_tform_cache_block_size-1))==0, block_size_must_be_power_of_two);
  //--------------------------------------------------------------------------

  // accessors & allocation
  EGL_INLINE void *cluster_cache(rasterizer_global_cluster_index_t cluster_idx_);
  void *alloc_cluster_cache(rasterizer_global_cluster_index_t, size_t num_bytes_);
  //--------------------------------------------------------------------------

  //==========================================================================
  // block_header
  //==========================================================================
  struct block_header
  {
    rasterizer_global_cluster_index_t cluster_idx;
    rasterizer_vertex_cache_offset_t num_items;
  };
  EGL_STATIC_ASSERT_MSG(sizeof(block_header)<=rasterizer_tform_cache_block_size, block_header_does_not_fit_in_block);
  //--------------------------------------------------------------------------

  rasterizer_vertex_cache_cfg m_cfg;
  size_t m_cache_alloc_pos;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// ndc_transform
//============================================================================
template<class PTV> EGL_INLINE void ndc_transform(vec2f*, PTV*, uint8_t)
{
}
template<class PTV> EGL_INLINE void ndc_transform(vec3f*, PTV*, uint8_t)
{
}
template<class PTV> EGL_INLINE void ndc_transform(vec4f*, PTV *ptvs_, uint8_t num_vertices_)
{
  for(uint8_t vi=0; vi<num_vertices_; ++vi)
  {
    float oow=1.0f/ptvs_[vi].pos.w;
    ptvs_[vi].pos.x*=oow;
    ptvs_[vi].pos.y*=oow;
    ptvs_[vi].pos.z*=oow;
    ptvs_[vi].pos.w=oow;
  }
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_vertex_cache
//============================================================================
template<class PSC>
const typename PSC::ptv_fmt *rasterizer_vertex_cache::get_cluster_ptv_buffer(const PSC &pso_, const typename PSC::pso_state &pso_state_, const void *cluster_, rasterizer_local_cluster_index_t cluster_idx_, rasterizer_global_cluster_index_t global_cluster_idx_, uint16_t)
{
  typedef typename PSC::ptv_fmt ptv_fmt_t;
  typedef decltype(ptv_fmt_t::pos) ptv_pos_t;
  uint8_t num_cluster_vertices=pso_.num_cluster_vertices(cluster_);
  ptv_fmt_t *ptv_buffer;
  size_t ptv_buffer_size=num_cluster_vertices*sizeof(ptv_fmt_t);
  if(!PSC::vertex_cache || cluster_idx_&rasterizer_max_local_clusters || !m_cfg.cache_size)
  {
    // use temporal store for post-transform vertices (single tile cluster lifetime or no caching)
    EGL_ASSERT_MSG(ptv_buffer_size<=m_cfg.tmp_ptv_buffer_size, "Temporal post-transform vertex buffer is too small (%i bytes) for the cluster (%i bytes).\r\n", (int)m_cfg.tmp_ptv_buffer_size, (int)ptv_buffer_size);
    ptv_buffer=(ptv_fmt_t*)m_cfg.tmp_cluster_ptv_buffer;
  }
  else
  {
    // allocate post-transform vertex buffer from the v-cache if the result is not in the cache yet
    ptv_buffer=(ptv_fmt_t*)cluster_cache(global_cluster_idx_);
    if(ptv_buffer)
      return ptv_buffer;
    ptv_buffer=(ptv_fmt_t*)alloc_cluster_cache(global_cluster_idx_, ptv_buffer_size);
  }

  // apply vertex transform and transform the vertex positions from clip space to NDC
  pso_.tform_cluster(ptv_buffer, pso_state_, cluster_);
  ndc_transform((ptv_pos_t*)0, ptv_buffer, num_cluster_vertices);
  return ptv_buffer;
}
//----------------------------------------------------------------------------

void *rasterizer_vertex_cache::cluster_cache(rasterizer_global_cluster_index_t cluster_idx_)
{
  EGL_ASSERT_PEDANTIC(m_cfg.cluster_vcache_offs);
  if(cluster_idx_>=m_cfg.max_clusters)
    return 0;
  rasterizer_vertex_cache_offset_t offs=m_cfg.cluster_vcache_offs[cluster_idx_];
  return offs==rasterizer_vertex_cache_offset_t(-1)?0:(uint8_t*)m_cfg.cache+offs*rasterizer_tform_cache_block_size;
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
