//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#include "egl_rasterizer_tiling.h"
#include "egl_tform3.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------


//============================================================================
// helpers
//============================================================================
namespace
{
  uint8_t morton_coord2d(uint16_t z_)
  {
    z_&=0x5555;
    z_=(z_^(z_>>1))&0x3333;
    z_=(z_^(z_>>2))&0x0f0f;
    z_=z_^(z_>>4);
    return uint8_t(z_);
  }
} // namespace <anonymous>
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_tiling_cfg
//============================================================================
rasterizer_tiling_cfg::rasterizer_tiling_cfg()
{
  tile_order=tileorder_morton;
  tiles=0;
  tile_map=0;
  clusters=0;
  max_clusters=0;
  cluster_strips=0;
  max_cluster_strips=0;
  rt_width=0;
  rt_height=0;
  tile_width=0;
  tile_height=0;
}
//----

void rasterizer_tiling_cfg::set_render_target_size(uint16_t width_, uint16_t height_)
{
  rt_width=width_;
  rt_height=height_;
}
//----

void rasterizer_tiling_cfg::set_tiles(rasterizer_tile *tiles_, uint16_t *tile_map_, rasterizer_tile_size_t tile_width_, rasterizer_tile_size_t tile_height_)
{
  tiles=tiles_;
  tile_map=tile_map_;
  tile_width=tile_width_;
  tile_height=tile_height_;
}
//----

void rasterizer_tiling_cfg::set_tile_order(e_tile_order order_)
{
  tile_order=order_;
}
//----

void rasterizer_tiling_cfg::set_clusters(rasterizer_cluster *clusters_, rasterizer_global_cluster_index_t max_clusters_)
{
  clusters=clusters_;
  max_clusters=max_clusters_;
}
//----

void rasterizer_tiling_cfg::set_cluster_strips(void *cluster_strips_, rasterizer_cluster_strip_index_t max_cluster_strips_, uint16_t num_strip_clusters_)
{
  EGL_ASSERT_MSG((num_strip_clusters_&(num_strip_clusters_-1))==0, "Number of clusters in cluster strip must be power-of-2\r\n");
  cluster_strips=(uint8_t*)cluster_strips_;
  max_cluster_strips=max_cluster_strips_;
  cluster_strip_cluster_mask=num_strip_clusters_-1;
  cluster_strip_size=sizeof(rasterizer_tile_cluster_strip<1>)+sizeof(rasterizer_global_cluster_index_t)*cluster_strip_cluster_mask;
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_tiling
//============================================================================
rasterizer_tiling::rasterizer_tiling()
{
  mem_zero(&m_cfg, sizeof(m_cfg));
  m_num_tiles=0;
  m_tile_width_shift=0;
  m_tile_height_shift=0;
  m_tiled_width=0;
  m_tiled_height=0;
  m_num_clusters=0;
  m_num_cluster_strips=0;
}
//----

rasterizer_tiling::~rasterizer_tiling()
{
}
//----

void rasterizer_tiling::init(const rasterizer_tiling_cfg &cfg_)
{
  // check and setup config
  EGL_ASSERT(cfg_.tiles && cfg_.clusters && cfg_.cluster_strips);
  EGL_ASSERT_MSG((cfg_.tile_width&(cfg_.tile_width-1))==0 && (cfg_.tile_height&(cfg_.tile_height-1))==0, "Tile dimensions (%i x %i) must be power-of-2.\r\n", cfg_.tile_width, cfg_.tile_height);
  m_cfg=cfg_;

  // calculate tiling values
  m_tile_width_shift=0;
  for(uint16_t v=cfg_.tile_width>>1; v; v>>=1)
    ++m_tile_width_shift;
  m_tile_height_shift=0;
  for(uint16_t v=cfg_.tile_height>>1; v; v>>=1)
    ++m_tile_height_shift;
  uint16_t tiled_width=(cfg_.rt_width+cfg_.tile_width-1)>>m_tile_width_shift;
  uint16_t tiled_height=(cfg_.rt_height+cfg_.tile_height-1)>>m_tile_height_shift;
  EGL_ASSERT_MSG(tiled_width<256 && tiled_height<256, "Too small tile size (%i x %i) for resolution %i x %i.\r\n", cfg_.tile_width, cfg_.tile_height, cfg_.rt_width, cfg_.rt_height);
  m_tiled_width=uint8_t(tiled_width);
  m_tiled_height=uint8_t(tiled_height);
  m_num_tiles=uint16_t(m_tiled_width)*m_tiled_height;

  // setup tiles
  if(cfg_.tile_map)
    mem_zero(cfg_.tile_map, m_num_tiles*sizeof(*cfg_.tile_map));
  rasterizer_tile *tile=cfg_.tiles, *tile_end=tile+m_num_tiles;
  uint16_t tile_idx=0;
  do
  {
    tile->tile_min.set(cfg_.tile_width, cfg_.tile_height);
    tile->tile_end.set(0, 0);
    switch(cfg_.tile_order)
    {
      // type-write order (start top-left) of tiles
      case tileorder_linear:
      {
        tile->x=uint8_t(tile_idx%m_tiled_width);
        tile->y=uint8_t(tile_idx/m_tiled_width);
        ++tile_idx;
      } break;

      // Morton Z-curve tile ordering
      case tileorder_morton:
      {
        uint8_t x, y;
        do
        {
          x=morton_coord2d(tile_idx);
          y=morton_coord2d(tile_idx>>1);
          ++tile_idx;
        } while(x>=m_tiled_width || y>=m_tiled_height);
        tile->x=x;
        tile->y=y;
      } break;

      // unknown
      default: EGL_ERROR("Unsupported tiling order.\r\n");
    }
    if(cfg_.tile_map)
      cfg_.tile_map[tile->x+tile->y*m_tiled_width]=uint16_t(tile-cfg_.tiles);
  } while(++tile<tile_end);
  clear();
}
//----

void rasterizer_tiling::clear()
{
  if(!m_cfg.tiles)
    return;
  uint16_t num_tiles=uint16_t(m_tiled_width)*m_tiled_height;
  rasterizer_tile *tile=m_cfg.tiles, *tile_end=tile+num_tiles;
  do
  {
    tile->num_clusters=0;
  } while(++tile<tile_end);
  m_num_clusters=0;
  m_num_cluster_strips=0;
}
//----------------------------------------------------------------------------

void rasterizer_tiling::add_cluster(rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t local_cluster_idx_)
{
  add_cluster(vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f), dispatch_idx_, local_cluster_idx_);
}
//----

void rasterizer_tiling::add_cluster(const mat44f &v2p_, const vec3f &pos_, ufloat_t rad_, rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t local_cluster_idx_)
{
  // calculate cluster tile bounds
  if(pos_.z<-rad_)
    return;
  if(pos_.z<rad_+0.1f)
    return add_cluster(dispatch_idx_, local_cluster_idx_);
  vec4f bbox=sphere_screen_extents(pos_, rad_, v2p_);
  float min_z=pos_.z-rad_, min_q=min_z*v2p_.z.z+v2p_.w.z;
  float max_z=pos_.z+rad_, max_q=max_z*v2p_.z.z+v2p_.w.z;
  add_cluster(vec3f(0.5f+0.5f*bbox.x, 0.5f-0.5f*bbox.w, min_q>0.0f?min_q/min_z:0.0f), vec3f(0.5f+0.5f*bbox.z, 0.5f-0.5f*bbox.y, max_q>0.0f?max_q/max_z:0.0f), dispatch_idx_, local_cluster_idx_);
}
//----

void rasterizer_tiling::add_cluster(const vec3f &bbox_min_, const vec3f &bbox_max_, rasterizer_dispatch_index_t dispatch_idx_, rasterizer_local_cluster_index_t local_cluster_idx_)
{
  // calculate cluster tile bounds
  EGL_ASSERT(m_cfg.tiles);
  EGL_ASSERT_MSG(local_cluster_idx_<rasterizer_max_local_clusters, "Local cluster index (%i) must be smaller than %i.\r\n", local_cluster_idx_, rasterizer_max_local_clusters);
  uint16_t bbox_min_x=(uint16_t)max<int16_t>(0, int16_t(bbox_min_.x*m_cfg.rt_width));
  uint16_t bbox_end_x=(uint16_t)clamp<int16_t>(int16_t(bbox_max_.x*m_cfg.rt_width)+1, 0, m_cfg.rt_width);
  uint16_t bbox_min_y=(uint16_t)max<int16_t>(0, int16_t(bbox_min_.y*m_cfg.rt_height));
  uint16_t bbox_end_y=(uint16_t)clamp<int16_t>(int16_t(bbox_max_.y*m_cfg.rt_height)+1, 0, m_cfg.rt_height);
  uint16_t bbox_tile_min_x=bbox_min_x>>m_tile_width_shift;
  uint16_t bbox_tile_end_x=(bbox_end_x+m_cfg.tile_width-1)>>m_tile_width_shift;
  uint16_t bbox_tile_min_y=bbox_min_y>>m_tile_height_shift;
  uint16_t bbox_tile_end_y=(bbox_end_y+m_cfg.tile_height-1)>>m_tile_height_shift;

  // check cluster bounds
  uint8_t twidth=(uint8_t)max<int16_t>(0, bbox_tile_end_x-bbox_tile_min_x);
  uint8_t theight=(uint8_t)max<int16_t>(0, bbox_tile_end_y-bbox_tile_min_y);
  if(!twidth || !theight)
    return;

  // setup cluster
  rasterizer_global_cluster_index_t global_cluster_idx=m_num_clusters++;
  rasterizer_cluster &cluster=m_cfg.clusters[global_cluster_idx];
  cluster.dispatch_idx=dispatch_idx_;
  if(twidth==1 && theight==1)
    local_cluster_idx_|=rasterizer_max_local_clusters;
  cluster.local_cluster_idx=local_cluster_idx_;
#if EGL_BUILDOP_RASTERIZER_HIZ==1
  cluster.hiz_x=uint8_t(bbox_min_x/rasterizer_hiz_tile_width);
  cluster.hiz_width=uint8_t((bbox_end_x+rasterizer_hiz_tile_width-1)/rasterizer_hiz_tile_width-cluster.hiz_x);
  cluster.hiz_y=uint8_t(bbox_min_y/rasterizer_hiz_tile_width);
  cluster.hiz_height=uint8_t((bbox_end_y+rasterizer_hiz_tile_width-1)/rasterizer_hiz_tile_width-cluster.hiz_y);
  cluster.hiz_minz=uint16_t(65535.0f*bbox_min_.z);
  cluster.hiz_maxz=uint16_t(65535.0f*bbox_max_.z);
#endif

  // bin the cluster to tiles
  const uint16_t *tmap=m_cfg.tile_map+bbox_tile_min_x+bbox_tile_min_y*m_tiled_width;
  do
  {
    const uint16_t *tmap_end=tmap+twidth;
    do
    {
      rasterizer_tile &tile=m_cfg.tiles[m_cfg.tile_map?*tmap:uint16_t((size_t(tmap)/sizeof(*tmap)))];
      rasterizer_tile_cluster_strip<1> *cstrip;
      unsigned cis_offs=(tile.num_clusters++)&m_cfg.cluster_strip_cluster_mask;
      if(!cis_offs)
      {
        // allocate cluster strip for the tile
        EGL_ASSERT_MSG(m_num_cluster_strips<m_cfg.max_cluster_strips, "Out of cluster strips (%i).\r\n", m_cfg.max_cluster_strips);
        rasterizer_cluster_strip_index_t cstrip_idx=m_num_cluster_strips++;
        cstrip=(rasterizer_tile_cluster_strip<1>*)(m_cfg.cluster_strips+cstrip_idx*m_cfg.cluster_strip_size);
        cstrip->next=rasterizer_cluster_strip_index_t(-1);
        if(tile.num_clusters==1)
        {
          tile.cluster_strip_first=cstrip_idx;
          tile.cluster_strip_last=cstrip_idx;
        }
        else
        {
          ((rasterizer_tile_cluster_strip<1>*)(m_cfg.cluster_strips+tile.cluster_strip_last*m_cfg.cluster_strip_size))->next=cstrip_idx;
          tile.cluster_strip_last=cstrip_idx;
        }
      }
      else
        cstrip=(rasterizer_tile_cluster_strip<1>*)(m_cfg.cluster_strips+tile.cluster_strip_last*m_cfg.cluster_strip_size);

      // assign cluster to the tile
      cstrip->global_cluster_idx[cis_offs]=global_cluster_idx;
      ++tmap;
    } while(tmap<tmap_end);
    tmap+=m_tiled_width-twidth;
  } while(--theight);
}
//----------------------------------------------------------------------------
