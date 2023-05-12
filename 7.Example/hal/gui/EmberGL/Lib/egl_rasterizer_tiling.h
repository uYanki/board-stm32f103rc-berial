//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_RASTERIZER_TILING_H
#define EGL_RASTERIZER_TILING_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_rasterizer_config.h"
#include "egl_math.h"
EGL_NAMESPACE_BEGIN

// new
struct rasterizer_cluster;
template<unsigned num_clusters> struct rasterizer_tile_cluster_strip;
struct rasterizer_tile;
struct rasterizer_tiling_cfg;
class rasterizer_tiling;
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_cluster
//============================================================================
struct rasterizer_cluster
{
  rasterizer_dispatch_index_t dispatch_idx;
  rasterizer_local_cluster_index_t local_cluster_idx;
#if EGL_BUILDOP_RASTERIZER_HIZ==1
  uint8_t hiz_x, hiz_y;
  uint8_t hiz_width, hiz_height;
  uint16_t hiz_minz, hiz_maxz;
#endif
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_tile_cluster_strip
//============================================================================
template<unsigned num_clusters>
struct rasterizer_tile_cluster_strip
{
  rasterizer_cluster_strip_index_t next;
  rasterizer_global_cluster_index_t global_cluster_idx[num_clusters];
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_tile
//============================================================================
struct rasterizer_tile
{
  rasterizer_cluster_strip_index_t cluster_strip_first, cluster_strip_last;
  rasterizer_global_cluster_index_t num_clusters;
  uint8_t x, y;
  vec2<rasterizer_tile_size_t> tile_min, tile_end;
};
//----------------------------------------------------------------------------


//============================================================================
// e_tile_order
//============================================================================
enum e_tile_order
{
  tileorder_linear,
  tileorder_morton,
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_tiling_cfg
//============================================================================
struct rasterizer_tiling_cfg
{
  // construction and setup
  rasterizer_tiling_cfg();
  void set_render_target_size(uint16_t width_, uint16_t height_);
  void set_tiles(rasterizer_tile*, uint16_t *tile_map_, rasterizer_tile_size_t tile_width_, rasterizer_tile_size_t tile_height_);
  void set_tile_order(e_tile_order);
  void set_clusters(rasterizer_cluster*, rasterizer_global_cluster_index_t max_clusters_);
  void set_cluster_strips(void *cluter_strips_, rasterizer_cluster_strip_index_t max_cluster_strips_, uint16_t num_strip_clusters_);
  //--------------------------------------------------------------------------

  e_tile_order tile_order;
  rasterizer_tile *tiles;
  uint16_t *tile_map;
  rasterizer_cluster *clusters;
  rasterizer_global_cluster_index_t max_clusters;
  uint8_t *cluster_strips;
  uint16_t cluster_strip_size;
  uint16_t cluster_strip_cluster_mask;
  rasterizer_cluster_strip_index_t max_cluster_strips;
  uint16_t rt_width, rt_height;
  rasterizer_tile_size_t tile_width, tile_height;
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_tiling
//============================================================================
class rasterizer_tiling
{
public:
  // construction
  rasterizer_tiling();
  ~rasterizer_tiling();
  void init(const rasterizer_tiling_cfg&);
  void clear();
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE uint16_t rt_width() const;
  EGL_INLINE uint16_t rt_height() const;
  EGL_INLINE rasterizer_tile_size_t tile_width() const;
  EGL_INLINE rasterizer_tile_size_t tile_height() const;
  EGL_INLINE uint16_t num_tiles() const;
  EGL_INLINE uint8_t tile_width_shift() const;
  EGL_INLINE uint8_t tile_height_shift() const;
  EGL_INLINE uint8_t tiled_width() const;
  EGL_INLINE uint8_t tiled_height() const;
  EGL_INLINE rasterizer_tile *tiles() const;
  EGL_INLINE rasterizer_global_cluster_index_t num_clusters() const;
  EGL_INLINE rasterizer_cluster_strip_index_t num_cluster_strips() const;
  EGL_INLINE const rasterizer_cluster &cluster(rasterizer_global_cluster_index_t) const;
  EGL_INLINE const rasterizer_tile_cluster_strip<1> &cluster_strip(rasterizer_cluster_strip_index_t strip_idx_) const;
  //--------------------------------------------------------------------------

  // tiling
  void add_cluster(rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t local_cluster_idx_);
  void add_cluster(const mat44f &v2p_, const vec3f &pos_, ufloat_t rad_, rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t local_cluster_idx_);
  void add_cluster(const vec3f &bbox_min_, const vec3f &bbox_max_, rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t local_cluster_idx_);
  //--------------------------------------------------------------------------

private:
  friend class rasterizer;
  rasterizer_tiling_cfg m_cfg;
  uint16_t m_num_tiles;
  uint8_t m_tile_width_shift, m_tile_height_shift;
  uint8_t m_tiled_width, m_tiled_height;
  rasterizer_global_cluster_index_t m_num_clusters;
  rasterizer_cluster_strip_index_t m_num_cluster_strips;
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// rasterizer_tiling
//============================================================================
rasterizer_tile_size_t rasterizer_tiling::tile_width() const
{
  return m_cfg.tile_width;
}
//----

rasterizer_tile_size_t rasterizer_tiling::tile_height() const
{
  return m_cfg.tile_height;
}
//----

uint16_t rasterizer_tiling::rt_width() const
{
  return m_cfg.rt_width;
}
//----

uint16_t rasterizer_tiling::rt_height() const
{
  return m_cfg.rt_height;
}
//----

uint16_t rasterizer_tiling::num_tiles() const
{
  return m_num_tiles;
}
//----

uint8_t rasterizer_tiling::tile_width_shift() const
{
  return m_tile_width_shift;
}
//----

uint8_t rasterizer_tiling::tile_height_shift() const
{
  return m_tile_height_shift;
}
//----

uint8_t rasterizer_tiling::tiled_width() const
{
  return m_tiled_width;
}
//----

uint8_t rasterizer_tiling::tiled_height() const
{
  return m_tiled_height;
}
//----

rasterizer_tile *rasterizer_tiling::tiles() const
{
  return m_cfg.tiles;
}
//----

rasterizer_global_cluster_index_t rasterizer_tiling::num_clusters() const
{
  return m_num_clusters;
}
//----

rasterizer_cluster_strip_index_t rasterizer_tiling::num_cluster_strips() const
{
  return m_num_cluster_strips;
}
//----

const rasterizer_cluster &rasterizer_tiling::cluster(rasterizer_global_cluster_index_t idx_) const
{
  EGL_ASSERT_PEDANTIC(idx_<m_num_clusters);
  return m_cfg.clusters[idx_];
}
//----

const rasterizer_tile_cluster_strip<1> &rasterizer_tiling::cluster_strip(rasterizer_cluster_strip_index_t strip_idx_) const
{
  EGL_ASSERT_PEDANTIC(strip_idx_<m_num_cluster_strips);
  return *(rasterizer_tile_cluster_strip<1>*)(m_cfg.cluster_strips+m_cfg.cluster_strip_size*strip_idx_);
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
