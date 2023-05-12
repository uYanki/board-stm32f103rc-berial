//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_RASTERIZER_H
#define EGL_RASTERIZER_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_rasterizer_tiling.h"
#include "egl_rasterizer_target.h"
#include "egl_rasterizer_cache.h"
#include "egl_rasterizer_psc.h"
#include "egl_fast_math.h"
#include "egl_pixel.h"
#include "egl_meta.h"
EGL_NAMESPACE_BEGIN

// new
typedef vec2<uint16_t> vec2u16;
typedef vec2<int32_t> vec2i32;
typedef vec3<int32_t> vec3i32;
struct rasterizer_depth_target;
struct rasterizer_dispatch;
struct rasterizer_stats;
struct rasterizer_cfg;
class rasterizer_callback_base;
class rasterizer;
#define EGL_RT_BLENDFUNC(rt_idx__, blendfunc__) template<e_pixel_format rt_fmt, e_pixel_format src_fmt> static EGL_INLINE void blend_rt##rt_idx__(pixel<rt_fmt> &res_, const pixel<rt_fmt> &dst_, const pixel<src_fmt> &src_) {blendfunc__(res_, dst_, src_);}
template<e_pixel_format dst_fmt, e_pixel_format src_fmt> EGL_INLINE void blendfunc_opaque(pixel<dst_fmt> &res_, const pixel<dst_fmt> &dst_, const pixel<src_fmt> &src_);
template<e_pixel_format dst_fmt, e_pixel_format src_fmt> EGL_INLINE void blendfunc_add(pixel<dst_fmt> &res_, const pixel<dst_fmt> &dst_, const pixel<src_fmt> &src_);
template<e_pixel_format dst_fmt, e_pixel_format src_fmt> EGL_INLINE void blendfunc_alpha_blend(pixel<dst_fmt> &res_, const pixel<dst_fmt> &dst_, const pixel<src_fmt> &src_);
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_dispatch
//============================================================================
struct rasterizer_dispatch
{
  size_t psc_wrapper[2];
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_data_transfer
//============================================================================
struct rasterizer_data_transfer
{
  size_t data_offset;
  uint16_t x, y;
  uint16_t width, height;
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_memory_cfg_base
//============================================================================
// defines the rasterizer properties.
// struct my_rasterizer_mem_cfg: rasterizer_memory_cfg_base
// {
//   enum {depth_format=depthfmt_float32};
// };
struct rasterizer_memory_cfg_base
{
  // depth
  enum {depth_format=depthfmt_uint16};
  enum {depth_hiz=true};
  // render tile formats
  enum {rt0_fmt=pixfmt_r8g8b8a8};
  enum {rt1_fmt=pixfmt_none};
  enum {rt2_fmt=pixfmt_none};
  enum {rt3_fmt=pixfmt_none};
  // config
  enum {max_dispatches=64};                // max PSO dispatches per commit
  enum {pso_store_size=16384};             // total PSO store per commit
  enum {max_pso_state_size=256};           // max PSO state size
  enum {tile_width=64, tile_height=64};    // tile size in pixels
  enum {tile_order=tileorder_morton};      // tile render order
  enum {max_clusters=256};                 // maximum total rendered clusters per commit
  enum {max_cluster_strips=256};           // max total cluster strips (for cluster tile binning)
  enum {num_strip_clusters=32};            // number of clusters in a cluster strip
  enum {max_ptv_buffer_size=64*32};        // max cluster post-transform vertex buffer size in bytes
  enum {vcache_size=32768};                // vertex cache size in bytes
  enum {max_dma_transfers=2};              // maximum queued DMA transfers
  enum {dma_buffer_px=tile_width*          // number of pixel allocated for DMA transfer buffer
                      tile_height};
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_memory
//============================================================================
// container for rasterizer memory defined by the config
template<class HWDevice, class Cfg>
struct rasterizer_memory
{
  enum {tiled_rt_width=(HWDevice::fb_width+Cfg::tile_width-1)/Cfg::tile_width};
  enum {tiled_rt_height=(HWDevice::fb_height+Cfg::tile_height-1)/Cfg::tile_height};
  enum {num_rt_tiles=tiled_rt_width*tiled_rt_height};
  enum {max_concurrency=HWDevice::max_concurrency};
  //--------------------------------------------------------------------------

  // initialization
  rasterizer_memory();
  rasterizer_memory(HWDevice&);
  void init(HWDevice&);
  //--------------------------------------------------------------------------

  // tile pixel storage
  typename depth_format_traits<(e_depth_format)Cfg::depth_format>::depth_format_t depth_tile[Cfg::depth_format!=int(depthfmt_none)?max_concurrency*Cfg::tile_width*Cfg::tile_height:1];
  uint16_t depth_hiz_tile[Cfg::depth_format!=int(depthfmt_none) && Cfg::depth_hiz?max_concurrency*(Cfg::tile_width*Cfg::tile_height/rasterizer_hiz_tile_pixels):1];
  pixel<e_pixel_format(Cfg::rt0_fmt)> rtile0[Cfg::rt0_fmt?max_concurrency*Cfg::tile_width*Cfg::tile_height:1];
  pixel<e_pixel_format(Cfg::rt1_fmt)> rtile1[Cfg::rt1_fmt?max_concurrency*Cfg::tile_width*Cfg::tile_height:1];
  pixel<e_pixel_format(Cfg::rt2_fmt)> rtile2[Cfg::rt2_fmt?max_concurrency*Cfg::tile_width*Cfg::tile_height:1];
  pixel<e_pixel_format(Cfg::rt3_fmt)> rtile3[Cfg::rt3_fmt?max_concurrency*Cfg::tile_width*Cfg::tile_height:1];
  EGL_STATIC_ASSERT_MSG(rasterizer_max_rts==4, render_tiles_only_for_4_rts);
  // PSO dispatching
  rasterizer_dispatch dispatches[Cfg::max_dispatches];
  size_t pso_store[Cfg::pso_store_size/sizeof(size_t)];
  // frame buffer tiling
  uint16_t tile_map[Cfg::tile_order==int(tileorder_linear)?1:num_rt_tiles];
  rasterizer_tile tiles[num_rt_tiles];
  rasterizer_cluster clusters[Cfg::max_clusters];
  rasterizer_tile_cluster_strip<Cfg::num_strip_clusters> cluster_strips[Cfg::max_cluster_strips];
  void *rts[rasterizer_max_rts];
  // vertex cache
  size_t cluster_ptv_buf[max_concurrency*(Cfg::max_ptv_buffer_size/sizeof(size_t))];
  size_t vcache_buf[Cfg::vcache_size?Cfg::vcache_size/sizeof(size_t):1];
  rasterizer_vertex_cache_offset_t vcache_offs[Cfg::vcache_size?Cfg::max_clusters:1];
  // DMA
  pixel<e_pixel_format(HWDevice::fb_format)> dma_buf[Cfg::max_dma_transfers && HWDevice::supports_dma?Cfg::dma_buffer_px:1];
  rasterizer_data_transfer dma_transfers[Cfg::max_dma_transfers && HWDevice::supports_dma?Cfg::max_dma_transfers:1];
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_stats
//============================================================================
struct rasterizer_stats
{
  void reset();
  void log() const;
  //--------------------------------------------------------------------------

  size_t num_dispatches;
  size_t pso_store_size;
  size_t max_pso_state_size;
  size_t num_cluster_strips;
  size_t tmp_ptv_buffer_size;
  size_t num_clusters;
  rasterizer_global_cluster_index_t max_tile_clusters;
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_cfg
//============================================================================
struct rasterizer_cfg
{
  // construction & setup
  rasterizer_cfg();
  void set_dispatches(rasterizer_dispatch*, rasterizer_dispatch_index_t max_dispatches_);
  void set_pso_store(void *pso_store_, size_t pso_store_size_, uint16_t max_pso_state_size_);
  void set_tile_buffers(void *const*rts_, void *depth_buf_, uint16_t *hiz_buf_, e_depth_format, uint16_t num_tile_buffers_);
  //--------------------------------------------------------------------------

  rasterizer_dispatch *dispatches;
  rasterizer_dispatch_index_t max_dispatches;
  void *pso_store;
  size_t pso_store_size;
  rasterizer_depth_target depth;
  uint8_t rt_pixel_sizes[rasterizer_max_rts];
  void *const*rts;
  uint16_t num_tile_buffers;
  uint16_t max_pso_state_size;
};
//----------------------------------------------------------------------------


//============================================================================
// render_target_cfg
//============================================================================
struct render_target_cfg
{
  uint16_t rt_width, rt_height;
  vec2f rt_coord_scale;
  uint16_t tile_pitch;
  rasterizer_depth_target depth;
  void *const*rts;
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_callback_base
//============================================================================
class rasterizer_callback_base
{
public:
  // callback interface
  virtual void rasterize_tiles(rasterizer&, const render_target_cfg&, rasterizer_tile *tiles_, size_t num_tiles_);
  virtual void submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16 &reg_min_, const vec2u16 &reg_end_, uint16_t thread_idx_)=0;
  //--------------------------------------------------------------------------

protected:
  // construction
  rasterizer_callback_base() {}
  //--------------------------------------------------------------------------

  //==========================================================================
  // tile_pshader
  //==========================================================================
  template<class TShader>
  struct tile_pshader
  {
    template<e_pixel_format dst_fmt>
    EGL_INLINE void exec(pixel<dst_fmt> &dst_, uint16_t x_, uint16_t y_) const
    {
      shader->exec(dst_, rts, *dt, px_offs+x_+y_*pitch, x_, y_);
    }
    //------------------------------------------------------------------------

    const TShader *shader;
    const void *const*rts;
    const rasterizer_depth_target *dt;
    size_t px_offs;
    uint16_t pitch;
  };
  //--------------------------------------------------------------------------

  //==========================================================================
  // tshader_rt0_pixel_convert
  //==========================================================================
  template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
  struct tshader_rt0_pixel_convert
  {
    typedef pixel<dst_fmt> dst_fmt_t;
    EGL_INLINE void exec(dst_fmt_t &dst_, const void *const*rts_, const rasterizer_depth_target&, size_t src_px_offs_, uint16_t, uint16_t) const
    {
      dst_=((const pixel<src_fmt>*)rts_[0])[src_px_offs_];
    }
  };
  //--------------------------------------------------------------------------

  //==========================================================================
  // tile_shader_wrapper_base
  //==========================================================================
  struct tile_shader_wrapper_base
  {
    virtual void copy_region(void *dst_, const void *const*rts_, const rasterizer_depth_target&, size_t src_px_offs_, uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, uint16_t pitch_)=0;
    virtual void transfer_region(const void *const*rts_, const rasterizer_depth_target &dt_, size_t src_px_offs_, uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, uint16_t pitch_)=0;
  };
  //--------------------------------------------------------------------------

  //==========================================================================
  // tile_shader_wrapper
  //==========================================================================
  template<class HWDevice, class TShader>
  struct tile_shader_wrapper: tile_shader_wrapper_base
  {
    virtual void copy_region(void *dst_, const void *const*rts_, const rasterizer_depth_target&, size_t src_px_offs_, uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, uint16_t pitch_);
    virtual void transfer_region(const void *const*rts_, const rasterizer_depth_target &dt_, size_t src_px_offs_, uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, uint16_t pitch_);
    HWDevice *m_dev;
    const TShader *m_shader;
  };
  //--------------------------------------------------------------------------

private:
  rasterizer_callback_base(const rasterizer_callback_base&); // not implemented
  void operator=(const rasterizer_callback_base&); // not implemented
};
//----------------------------------------------------------------------------


//============================================================================
// e_clear_depth
//============================================================================
enum e_clear_depth
{
  cleardepth_max, // clear to maximum depth value
  cleardepth_min, // clear to minimum depth value
};
//----------------------------------------------------------------------------


//============================================================================
// rasterizer
//============================================================================
class rasterizer
{
public:
  // construction
  rasterizer();
  void init(const rasterizer_cfg&, const rasterizer_tiling_cfg&);
  EGL_INLINE void set_callback(rasterizer_callback_base*);
  //--------------------------------------------------------------------------

  // accessors
  EGL_INLINE void *const*render_targets() const;
  EGL_INLINE const rasterizer_depth_target &depth_target() const;
  //--------------------------------------------------------------------------

  // rasterization
  void clear_depth(e_clear_depth);
  template<class PSC, class HWDevice> void dispatch_pso(const PSC&, HWDevice&);
  void commit();
  void rasterize_tile(const render_target_cfg&, rasterizer_tile&, uint16_t thread_idx_);
  //--------------------------------------------------------------------------

  // stats (query after commit)
  EGL_INLINE void reset_stats();
  EGL_INLINE void log_stats() const;
  EGL_INLINE rasterizer_stats stats() const;
  //--------------------------------------------------------------------------
  
private:
  //==========================================================================
  // e_hiz_mode
  //==========================================================================
  enum e_hiz_mode
  {
    hizmode_none,
    hizmode_max,
    hizmode_min,
    hizmode_any
  };
  //--------------------------------------------------------------------------

  struct result;
  friend class rasterizer_callback_base;
  rasterizer(const rasterizer&); // not implemented
  void operator=(const rasterizer&); // not implemented
  template<typename> void update_hiz(const result&, e_hiz_mode, uint16_t thread_idx_);
  static EGL_INLINE int32_t cross2d(const vec2i32& a_, const vec2i32& b_, const vec2i32& c_)  {return (b_.x-a_.x)*(c_.y-a_.y)-(b_.y-a_.y)*(c_.x-a_.x);}
  template<int> static EGL_INLINE float read_depth(const void *dbuf_, uint32_t offs_);
  template<int> static EGL_INLINE void write_depth(const void *dbuf_, uint32_t offs_, float z_);
  template<int> static EGL_INLINE bool test_depth(float z_, float zbuf_);
  template<typename T> static EGL_INLINE uint16_t hiz_value(T);
  //--------------------------------------------------------------------------

  // vec2f/vec3f/vec4f position attribute support
  static EGL_INLINE void bc_scale_depth(vec2f&, vec2f&, vec2f&, float);
  static EGL_INLINE void bc_scale_depth(vec3f &v0_, vec3f &v1_, vec3f &v2_, float bc_scale_);
  static EGL_INLINE void bc_scale_depth(vec4f &v0_, vec4f &v1_, vec4f &v2_, float bc_scale_);
  static EGL_INLINE float interpolate_depth(const vec2f&, const vec2f&, const vec2f&, const vec3f&);
  static EGL_INLINE float interpolate_depth(const vec3f &v0_, const vec3f &v1_, const vec3f &v2_, const vec3f &bc_);
  static EGL_INLINE float interpolate_depth(const vec4f &v0_, const vec4f &v1_, const vec4f &v2_, const vec3f &bc_);
  static EGL_INLINE void perspective_correct_bc(vec3f &bc_, const vec2f &v0_, const vec2f &v1_, const vec2f &v2_, float bc_scale_);
  static EGL_INLINE void perspective_correct_bc(vec3f &bc_, const vec3f &v0_, const vec3f &v1_, const vec3f &v2_, float bc_scale_);
  static EGL_INLINE void perspective_correct_bc(vec3f &bc_, const vec4f &v0_, const vec4f &v1_, const vec4f &v2_, float bc_scale_);
  //--------------------------------------------------------------------------

  //==========================================================================
  // result
  //==========================================================================
  struct result
  {
    vec2u16 tile_min, tile_end;
    bool hiz_update;
#if EGL_BUILDOP_RASTERIZER_STATS==1
    size_t max_pso_state_size;
    size_t max_ptv_buffer_size;
#endif
  };
  //--------------------------------------------------------------------------

  //==========================================================================
  // psc_wrapper_base
  //==========================================================================
  struct psc_wrapper_base
  {
    virtual void get_pso_state(void*, uint16_t max_pso_state_size_) const=0;
    virtual e_hiz_mode hiz_mode() const=0;
    virtual void rasterize_cluster(rasterizer_callback_base &dev_, result&, const render_target_cfg&, uint16_t tile_x_, uint16_t tile_y_, uint16_t tile_width_, uint16_t tile_height_, size_t tile_buffer_pixel_offset_, const void *pso_state_, uint16_t thread_idx_, rasterizer_local_cluster_index_t, rasterizer_global_cluster_index_t) const=0;
    const void *m_pso;
  };
  //--------------------------------------------------------------------------

  //==========================================================================
  // psc_wrapper
  //==========================================================================
  template<class PSC, class HWDevice>
  struct psc_wrapper: psc_wrapper_base
  {
    psc_wrapper(const PSC&);
    virtual void get_pso_state(void*, uint16_t max_pso_state_size_) const;
    virtual e_hiz_mode hiz_mode() const;
    virtual void rasterize_cluster(rasterizer_callback_base &dev_, result&, const render_target_cfg&, uint16_t tile_x_, uint16_t tile_y_, uint16_t tile_width_, uint16_t tile_height_, size_t tile_buffer_pixel_offset_, const void *pso_state_, uint16_t thread_idx_, rasterizer_local_cluster_index_t, rasterizer_global_cluster_index_t) const;
  };
  //--------------------------------------------------------------------------

  enum {psc_mem_align=sizeof(size_t)};
  rasterizer_callback_base *m_callback;
  rasterizer_cfg m_cfg;
  rasterizer_tiling m_tiling;
  rasterizer_dispatch_index_t m_num_dispatches;
  rasterizer_global_cluster_index_t m_num_clusters;
  size_t m_pso_write_pos;
  // state
  e_clear_depth m_clear_depth;
  // stats
#if EGL_BUILDOP_RASTERIZER_STATS==1
  rasterizer_stats m_stats;
#endif
};
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// Basic blending functions
//============================================================================
template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
void blendfunc_opaque(pixel<dst_fmt> &res_, const pixel<dst_fmt> &dst_, const pixel<src_fmt> &src_)
{
  res_=src_;
}
//----

template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
void blendfunc_add(pixel<dst_fmt> &res_, const pixel<dst_fmt> &dst_, const pixel<src_fmt> &src_)
{
  color_rgbaf dst=dst_.get_colorf();
  color_rgbaf src=src_.get_colorf();
  res_=min(1.0f, dst+src);
}
//----

template<e_pixel_format dst_fmt, e_pixel_format src_fmt>
void blendfunc_alpha(pixel<dst_fmt> &res_, const pixel<dst_fmt> &dst_, const pixel<src_fmt> &src_)
{
  color_rgbaf dst=dst_.get_colorf();
  color_rgbaf src=src_.get_colorf();
  res_=lerp(dst, src, src.a);
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_memory
//============================================================================
template<class HWDevice, class Cfg>
rasterizer_memory<HWDevice, Cfg>::rasterizer_memory()
{
}
//----

template<class HWDevice, class Cfg>
rasterizer_memory<HWDevice, Cfg>::rasterizer_memory(HWDevice &dev_)
{
  init(dev_);
}
//----

template<class HWDevice, class Cfg>
void rasterizer_memory<HWDevice, Cfg>::init(HWDevice &dev_)
{
  // setup rasterizer config
  rasterizer_cfg rcfg;
  rcfg.set_dispatches(dispatches, Cfg::max_dispatches);
  rcfg.set_pso_store(pso_store, sizeof(pso_store), Cfg::max_pso_state_size);
  rts[0]=Cfg::rt0_fmt?rtile0:0;
  rcfg.rt_pixel_sizes[0]=Cfg::rt0_fmt?sizeof(pixel<e_pixel_format(Cfg::rt0_fmt)>):0;
  rts[1]=Cfg::rt1_fmt?rtile1:0;
  rcfg.rt_pixel_sizes[1]=Cfg::rt1_fmt?sizeof(pixel<e_pixel_format(Cfg::rt1_fmt)>):0;
  rts[2]=Cfg::rt2_fmt?rtile2:0;
  rcfg.rt_pixel_sizes[2]=Cfg::rt2_fmt?sizeof(pixel<e_pixel_format(Cfg::rt2_fmt)>):0;
  rts[3]=Cfg::rt3_fmt?rtile3:0;
  rcfg.rt_pixel_sizes[3]=Cfg::rt3_fmt?sizeof(pixel<e_pixel_format(Cfg::rt3_fmt)>):0;
  rcfg.set_tile_buffers(rts, depth_tile, Cfg::depth_hiz?depth_hiz_tile:0, (e_depth_format)Cfg::depth_format, HWDevice::max_concurrency);

  // setup tiling config
  rasterizer_tiling_cfg tcfg;
  tcfg.set_render_target_size(HWDevice::fb_width, HWDevice::fb_height);
  tcfg.set_tiles(tiles, Cfg::tile_order==int(tileorder_linear)?0:tile_map, Cfg::tile_width, Cfg::tile_height);
  tcfg.set_tile_order((e_tile_order)Cfg::tile_order);
  tcfg.set_clusters(clusters, Cfg::max_clusters);
  tcfg.set_cluster_strips(cluster_strips, Cfg::max_cluster_strips, Cfg::num_strip_clusters);

  // setup vertex cache config
  rasterizer_vertex_cache_cfg vccfg;
  vccfg.set_cluster_ptv_buffer(cluster_ptv_buf, sizeof(cluster_ptv_buf)/HWDevice::max_concurrency);
  vccfg.set_cache(vcache_buf, Cfg::vcache_size?sizeof(vcache_buf):0);
  vccfg.set_cache_clusters(Cfg::vcache_size?vcache_offs:0, Cfg::vcache_size?Cfg::max_clusters:0);

  // initialize rasterizer
  dev_.init_rasterizer(rcfg, tcfg, vccfg);
  dev_.template init_default_tile_shader<e_pixel_format(Cfg::rt0_fmt)>();
  if(Cfg::max_dma_transfers && HWDevice::supports_dma)
    dev_.init_dma(dma_transfers, Cfg::max_dma_transfers, dma_buf, Cfg::max_dma_transfers?sizeof(dma_buf):0);
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer_callback_base::tile_shader_wrapper
//============================================================================
template<class HWDevice, class TShader>
void rasterizer_callback_base::tile_shader_wrapper<HWDevice, TShader>::copy_region(void *dst_, const void *const*rts_, const rasterizer_depth_target &dt_, size_t src_px_offs_, uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, uint16_t pitch_)
{
  // apply tile shader to the given region of pixels
  typedef typename HWDevice::fb_format_t fb_format_t;
  fb_format_t *dst=(fb_format_t*)dst_, *dst_end=dst+width_*height_;
  const TShader *sh=m_shader;
  uint16_t row_pitch=pitch_-width_;
  do
  {
    // process scanline
    const fb_format_t *dst_scan_end=dst+width_;
    uint16_t x=x_;
    do
    {
      sh->exec(*dst++, rts_, dt_, src_px_offs_++, x++, y_);
    } while(dst<dst_scan_end);

    // proceed to the scanline
    src_px_offs_+=row_pitch;
    ++y_;
  } while(dst<dst_end);
}
//----

template<class HWDevice, class TShader>
void rasterizer_callback_base::tile_shader_wrapper<HWDevice, TShader>::transfer_region(const void *const*rts_, const rasterizer_depth_target &dt_, size_t src_px_offs_, uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, uint16_t pitch_)
{
  // transform tile region to the display
  tile_pshader<TShader> ps={m_shader, rts_, &dt_, src_px_offs_-x_-y_*pitch_, pitch_};
  m_dev->fast_draw_rect(x_, y_, width_, height_, ps);
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer::psc_wrapper
//============================================================================
template<class PSC, class HWDevice>
rasterizer::psc_wrapper<PSC, HWDevice>::psc_wrapper(const PSC &pso_)
{
  m_pso=&pso_;
}
//----------------------------------------------------------------------------

template<class PSC, class HWDevice>
void rasterizer::psc_wrapper<PSC, HWDevice>::get_pso_state(void *buffer_, uint16_t max_pso_state_size_) const
{
  (void)max_pso_state_size_;
  typedef typename PSC::pso_state pso_state_t;
  EGL_ASSERT_MSG(sizeof(pso_state_t)<=max_pso_state_size_, "Max PSO state size (%i bytes) is too small to fit the PSO state (%i bytes)\r\n", (int)max_pso_state_size_, (int)sizeof(pso_state_t));
  ((const PSC*)m_pso)->get_pso_state(*(pso_state_t*)buffer_);
}
//----

template<class PSC, class HWDevice>
rasterizer::e_hiz_mode rasterizer::psc_wrapper<PSC, HWDevice>::hiz_mode() const
{
#if EGL_BUILDOP_RASTERIZER_HIZ==1
  if(!PSC::hiz_test)
    return hizmode_none;
  switch((e_rstate_depth_func)PSC::depth_func)
  {
    case rstate_depthfunc_equal:
    case rstate_depthfunc_not_equal: return hizmode_any;
    case rstate_depthfunc_less:
    case rstate_depthfunc_less_equal: return hizmode_max;
    case rstate_depthfunc_greater:
    case rstate_depthfunc_greater_equal: return hizmode_min;
  }
#endif
  return hizmode_none;
}
//----

template<class PSC, class HWDevice>
void rasterizer::psc_wrapper<PSC, HWDevice>::rasterize_cluster(rasterizer_callback_base &dev_, result &res_, const render_target_cfg &rt_cfg_, uint16_t tile_x_, uint16_t tile_y_, uint16_t tile_width_, uint16_t tile_height_, size_t tile_buffer_pixel_offset_, const void *pso_state_, uint16_t thread_idx_, rasterizer_local_cluster_index_t cluster_idx_, rasterizer_global_cluster_index_t global_cluster_idx_) const
{
  // get cluster and cluster post transform vertices
  EGL_ASSERT(PSC::depth_format==int(depthfmt_none) || int(PSC::depth_format)==int(rt_cfg_.depth.format));
  HWDevice &dev=static_cast<HWDevice&>(dev_);
  typedef typename PSC::pso_state pso_state_t;
  typedef typename PSC::ptv_fmt ptv_fmt_t;
  typedef decltype(ptv_fmt_t::pos) ptv_pos_t;
  const PSC &pso=*(const PSC*)m_pso;
  const pso_state_t &pso_state=*(const pso_state_t*)pso_state_;
  const void *cluster=pso.cluster(cluster_idx_&~rasterizer_max_local_clusters);
  const ptv_fmt_t *ptv_buffer=dev.vcache().get_cluster_ptv_buffer(pso, pso_state, cluster, cluster_idx_, global_cluster_idx_, thread_idx_);
#if EGL_BUILDOP_RASTERIZER_STATS==1
  res_.max_pso_state_size=max(res_.max_pso_state_size, sizeof(pso_state_t));
  res_.max_ptv_buffer_size=max(res_.max_ptv_buffer_size, sizeof(ptv_fmt_t)*pso.num_cluster_vertices(cluster));
#endif

  // rasterize the cluster
  uint16_t tile_xe=tile_x_+tile_width_, tile_ye=tile_y_+tile_height_;
  uint8_t num_prims=pso.num_cluster_triangles(cluster);
  rasterizer_pixel_out<PSC> psout;
  psout.m_rts[0]=PSC::rt0_fmt?(uint8_t*)rt_cfg_.rts[0]+tile_buffer_pixel_offset_*sizeof(pixel<(e_pixel_format)PSC::rt0_fmt>):0;
  psout.m_rts[1]=PSC::rt1_fmt?(uint8_t*)rt_cfg_.rts[1]+tile_buffer_pixel_offset_*sizeof(pixel<(e_pixel_format)PSC::rt1_fmt>):0;
  psout.m_rts[2]=PSC::rt2_fmt?(uint8_t*)rt_cfg_.rts[2]+tile_buffer_pixel_offset_*sizeof(pixel<(e_pixel_format)PSC::rt2_fmt>):0;
  psout.m_rts[3]=PSC::rt3_fmt?(uint8_t*)rt_cfg_.rts[3]+tile_buffer_pixel_offset_*sizeof(pixel<(e_pixel_format)PSC::rt3_fmt>):0;
  EGL_STATIC_ASSERT_MSG(rasterizer_max_rts==4, psout_supports_4_rts);
  res_.hiz_update=PSC::depth_write && PSC::hiz_update && PSC::depth_func!=int(rstate_depthfunc_never);
  void *EGL_RESTRICT depth_buf=(uint8_t*)rt_cfg_.depth.data+tile_buffer_pixel_offset_*depth_format_traits<(e_depth_format)PSC::depth_format>::type_size;
  for(uint8_t prim_idx=0; prim_idx<num_prims; ++prim_idx)
  {
    // setup triangle
    uint8_t vidx[3];
    pso.setup_primitive(cluster, prim_idx, vidx);
    ptv_pos_t vpos[3]={ptv_buffer[vidx[0]].pos, ptv_buffer[vidx[1]].pos, ptv_buffer[vidx[2]].pos};
    vec2i32 v0={int32_t((1.0f+vpos[0].x)*rt_cfg_.rt_coord_scale.x)-rasterizer_half_subpixel_offsx,
                int32_t((1.0f-vpos[0].y)*rt_cfg_.rt_coord_scale.y)-rasterizer_half_subpixel_offsy};
    vec2i32 v1={int32_t((1.0f+vpos[1].x)*rt_cfg_.rt_coord_scale.x)-rasterizer_half_subpixel_offsx,
                int32_t((1.0f-vpos[1].y)*rt_cfg_.rt_coord_scale.y)-rasterizer_half_subpixel_offsy};
    vec2i32 v2={int32_t((1.0f+vpos[2].x)*rt_cfg_.rt_coord_scale.x)-rasterizer_half_subpixel_offsx,
                int32_t((1.0f-vpos[2].y)*rt_cfg_.rt_coord_scale.y)-rasterizer_half_subpixel_offsy};

    // check backface culling
    int32_t signed_area=cross2d(v0, v1, v2);
    enum {tbfc_mode=PSC::tbfc_mode};
    if(int(tbfc_mode)!=rstate_tbfc_none)
    {
      if(int(tbfc_mode)==rstate_tbfc_cw)
        signed_area=-signed_area;
      if(signed_area<=0)
        continue;
    }
    const bool is_front_facing=tbfc_mode==int(rstate_tbfc_ccw) || (tbfc_mode==int(rstate_tbfc_none) && signed_area>0);

    // calculate triangle bounding rectangle (clamp to tile boundaries)
    minmax_res<int32_t> brect_minmax_x=minmax(v0.x, v1.x, v2.x);
    minmax_res<int32_t> brect_minmax_y=minmax(v0.y, v1.y, v2.y);
    vec2u16 brect_min={(uint16_t)max<int16_t>(tile_x_, int16_t(brect_minmax_x.min+rasterizer_subpixel_bitmask)>>rasterizer_subpixel_bits),
                       (uint16_t)max<int16_t>(tile_y_, int16_t(brect_minmax_y.min+rasterizer_subpixel_bitmask)>>rasterizer_subpixel_bits)};
    vec2u16 brect_end={(uint16_t)min<int16_t>(tile_xe, int16_t(brect_minmax_x.max>>rasterizer_subpixel_bits)+1),
                       (uint16_t)min<int16_t>(tile_ye, int16_t(brect_minmax_y.max>>rasterizer_subpixel_bits)+1)};
    uint16_t brect_width=brect_end.x-brect_min.x;
    uint16_t brect_height=brect_end.y-brect_min.y;
    if(int16_t(brect_width)<=0 || int16_t(brect_height)<=0)
      continue;

    // setup triangle edge functions
    vec2i32 p={brect_min.x<<rasterizer_subpixel_bits, brect_min.y<<rasterizer_subpixel_bits};
    vec3i32 w_row={cross2d(v1, v2, p), cross2d(v2, v0, p), cross2d(v0, v1, p)};
    vec3i32 dwdx={v1.y-v2.y, v2.y-v0.y, v0.y-v1.y};
    vec3i32 dwdy={v2.x-v1.x, v0.x-v2.x, v1.x-v0.x};
    if(!is_front_facing)
    {
      w_row.x=-w_row.x; w_row.y=-w_row.y; w_row.z=-w_row.z;
      dwdx.x=-dwdx.x; dwdx.y=-dwdx.y; dwdx.z=-dwdx.z;
      dwdy.x=-dwdy.x; dwdy.y=-dwdy.y; dwdy.z=-dwdy.z;
    }
    if(dwdx.x<0 || (!dwdx.x && dwdy.x>0))
      --w_row.x;
    if(dwdx.y<0 || (!dwdx.y && dwdy.y>0))
      --w_row.y;
    if(dwdx.z<0 || (!dwdx.z && dwdy.z>0))
      --w_row.z;
    w_row.x>>=rasterizer_subpixel_bits;
    w_row.y>>=rasterizer_subpixel_bits;
    w_row.z>>=rasterizer_subpixel_bits;

    // setup triangle bounds
    brect_min.x-=tile_x_;
    brect_min.y-=tile_y_;
    brect_end.x-=tile_x_;
    brect_end.y-=tile_y_;
    res_.tile_min=min(res_.tile_min, brect_min);
    res_.tile_end=max(res_.tile_end, brect_end);

    // rasterize the triangle
    float bc_scale=1.0f/(w_row.x+w_row.y+w_row.z);
    bc_scale_depth(vpos[0], vpos[1], vpos[2], bc_scale);
    uint32_t px_offs_max=brect_min.x+brect_min.y*rt_cfg_.tile_pitch+brect_width-1;
    uint32_t px_offs_end=px_offs_max+brect_height*rt_cfg_.tile_pitch;
    do
    {
      // skip pixels until hit the triangle /*todo: can we efficiently skip to the triangle edge without iteration?*/
      vec3i32 w=w_row;
      uint32_t x_offs=uint32_t(-brect_width);
      while(int32_t((w.x|w.y|w.z)&x_offs)<=0)
      {
        w.x+=dwdx.x;
        w.y+=dwdx.y;
        w.z+=dwdx.z;
        ++x_offs;
      }

      // rasterize triangle scanline span
      x_offs=~x_offs;
      while(int32_t(w.x|w.y|w.z|x_offs)>0)
      {
        vec3f bc(float(w.x), float(w.y), float(w.z));
        float z=interpolate_depth(vpos[0], vpos[1], vpos[2], bc);
        uint32_t px_offs=px_offs_max-x_offs;
        bool depth_test=test_depth<PSC::depth_format!=int(depthfmt_none)?PSC::depth_func:int(rstate_depthfunc_always)>(z, read_depth<PSC::depth_format>(depth_buf, px_offs));
        if(PSC::debug_disable_pixel_depth_test || depth_test)
        {
          if(depth_test)
            write_depth<!is_type_equal<ptv_pos_t, vec2f>::res && PSC::depth_write?int(PSC::depth_format):int(depthfmt_none)>(depth_buf, px_offs, z);
          if(PSC::bci_mode==int(rstate_bcimode_perspective))
            perspective_correct_bc(bc, vpos[0], vpos[1], vpos[2], bc_scale);
          else
            bc*=bc_scale;
          psout.m_px_offs=px_offs;
          pso.shade_pixel(psout, pso_state, ptv_buffer[vidx[0]], ptv_buffer[vidx[1]], ptv_buffer[vidx[2]], bc, prim_idx);
        }
        w.x+=dwdx.x;
        w.y+=dwdx.y;
        w.z+=dwdx.z;
        --x_offs;
      }

      // proceed to the next scanline
      w_row.x+=dwdy.x;
      w_row.y+=dwdy.y;
      w_row.z+=dwdy.z;
      px_offs_max+=rt_cfg_.tile_pitch;
    } while(px_offs_max<px_offs_end);
  }
}
//----------------------------------------------------------------------------


//============================================================================
// rasterizer
//============================================================================
template<> EGL_INLINE float rasterizer::read_depth<depthfmt_none>(const void*, uint32_t)                {return 1.0f;}
template<> EGL_INLINE float rasterizer::read_depth<depthfmt_uint8>(const void *dbuf_, uint32_t offs_)   {return ((const uint8_t*)dbuf_)[offs_]/255.0f;}
template<> EGL_INLINE float rasterizer::read_depth<depthfmt_uint16>(const void *dbuf_, uint32_t offs_)  {return ((const uint16_t*)dbuf_)[offs_]/65535.0f;}
template<> EGL_INLINE float rasterizer::read_depth<depthfmt_float32>(const void *dbuf_, uint32_t offs_) {return ((const float*)dbuf_)[offs_];}
//----

template<> EGL_INLINE void rasterizer::write_depth<depthfmt_none>(const void*, uint32_t, float)                   {}
template<> EGL_INLINE void rasterizer::write_depth<depthfmt_uint8>(const void *dbuf_, uint32_t offs_, float z_)   {((uint8_t*)dbuf_)[offs_]=uint8_t(z_*255.0f);}
template<> EGL_INLINE void rasterizer::write_depth<depthfmt_uint16>(const void *dbuf_, uint32_t offs_, float z_)  {((uint16_t*)dbuf_)[offs_]=uint16_t(z_*65535.0f);}
template<> EGL_INLINE void rasterizer::write_depth<depthfmt_float32>(const void *dbuf_, uint32_t offs_, float z_) {((float*)dbuf_)[offs_]=z_;}
//----

template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_always>(float, float)                 {return true;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_never>(float, float)                  {return false;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_equal>(float z_, float zbuf_)         {return z_==zbuf_;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_not_equal>(float z_, float zbuf_)     {return z_!=zbuf_;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_less>(float z_, float zbuf_)          {return z_<zbuf_;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_less_equal>(float z_, float zbuf_)    {return z_<=zbuf_;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_greater>(float z_, float zbuf_)       {return z_>zbuf_;}
template<> EGL_INLINE bool rasterizer::test_depth<rstate_depthfunc_greater_equal>(float z_, float zbuf_) {return z_>=zbuf_;}
//----

template<> EGL_INLINE uint16_t rasterizer::hiz_value<uint8_t>(uint8_t z_)     {return uint16_t(z_)<<8;}
template<> EGL_INLINE uint16_t rasterizer::hiz_value<uint16_t>(uint16_t z_)   {return z_;}
template<> EGL_INLINE uint16_t rasterizer::hiz_value<float32_t>(float32_t z_) {return uint16_t(z_*65535.0f);}
//----------------------------------------------------------------------------

EGL_INLINE void rasterizer::bc_scale_depth(vec2f&, vec2f&, vec2f&, float)
{
}
EGL_INLINE void rasterizer::bc_scale_depth(vec3f &v0_, vec3f &v1_, vec3f &v2_, float bc_scale_)
{
  v0_.z*=bc_scale_;
  v1_.z*=bc_scale_;
  v2_.z*=bc_scale_;
}
EGL_INLINE void rasterizer::bc_scale_depth(vec4f &v0_, vec4f &v1_, vec4f &v2_, float bc_scale_)
{
  v0_.z*=bc_scale_;
  v1_.z*=bc_scale_;
  v2_.z*=bc_scale_;
}
//----

EGL_INLINE float rasterizer::interpolate_depth(const vec2f&, const vec2f&, const vec2f&, const vec3f&)
{
  return 0.0f;
}
EGL_INLINE float rasterizer::interpolate_depth(const vec3f &v0_, const vec3f &v1_, const vec3f &v2_, const vec3f &bc_)
{
  return v0_.z*bc_.x+v1_.z*bc_.y+v2_.z*bc_.z;
}
EGL_INLINE float rasterizer::interpolate_depth(const vec4f &v0_, const vec4f &v1_, const vec4f &v2_, const vec3f &bc_)
{
  return v0_.z*bc_.x+v1_.z*bc_.y+v2_.z*bc_.z;
}
//----

EGL_INLINE void rasterizer::perspective_correct_bc(vec3f &bc_, const vec2f&, const vec2f&, const vec2f&, float bc_scale_)
{
  bc_*=bc_scale_;
}
EGL_INLINE void rasterizer::perspective_correct_bc(vec3f &bc_, const vec3f&, const vec3f&, const vec3f&, float bc_scale_)
{
  bc_*=bc_scale_;
}
EGL_INLINE void rasterizer::perspective_correct_bc(vec3f &bc_, const vec4f &v0_, const vec4f &v1_, const vec4f &v2_, float)
{
  // perspective correct barycentric coordinates
  float bc_x=bc_.x*v0_.w;
  float bc_y=bc_.y*v1_.w;
  float bc_z=bc_.z*v2_.w;
  float bc_sum=bc_x+bc_y+bc_z;
  float oobc=1.0f/bc_sum;
  bc_.x=bc_x*oobc;
  bc_.y=bc_y*oobc;
  bc_.z=bc_z*oobc;
}
//----------------------------------------------------------------------------

void rasterizer::set_callback(rasterizer_callback_base *cb_)
{
  m_callback=cb_;
}
//----------------------------------------------------------------------------

void *const*rasterizer::render_targets() const
{
  return m_cfg.rts;
}
//----

const rasterizer_depth_target &rasterizer::depth_target() const
{
  return m_cfg.depth;
}
//----------------------------------------------------------------------------

void rasterizer::reset_stats()
{
#if EGL_BUILDOP_RASTERIZER_STATS==1
  m_stats.reset();
#endif
}
//----

void rasterizer::log_stats() const
{
#if EGL_BUILDOP_RASTERIZER_STATS==1
  m_stats.log();
#endif
}
//----

rasterizer_stats rasterizer::stats() const
{
#if EGL_BUILDOP_RASTERIZER_STATS==1
  return m_stats;
#else
  rasterizer_stats s;
  s.reset();
  return s;
#endif
}
//----------------------------------------------------------------------------

template<class PSC, class HWDevice>
void rasterizer::dispatch_pso(const PSC &pso_, HWDevice&)
{
  // prepare dispatch
  EGL_ASSERT(m_cfg.dispatches);
  EGL_ASSERT_MSG(m_num_dispatches<m_cfg.max_dispatches, "Exceeding max PSO dispatch count (%i).\r\n", m_cfg.max_dispatches);
  EGL_ASSERT_MSG(m_pso_write_pos+sizeof(PSC)<=m_cfg.pso_store_size, "Out of PSO store memory (%u bytes).\r\n", (unsigned)m_cfg.pso_store_size);
  rasterizer_dispatch &dispatch=m_cfg.dispatches[m_num_dispatches];
  EGL_STATIC_ASSERT(sizeof(psc_wrapper<PSC, HWDevice>)<=sizeof(dispatch.psc_wrapper));

  // setup PSO
  PSC *pso=(PSC*)((uint8_t*)m_cfg.pso_store+m_pso_write_pos);
  mem_copy(pso, &pso_, sizeof(pso_));
  EGL_PNEW(dispatch.psc_wrapper)psc_wrapper<PSC, HWDevice>(*pso);
  m_pso_write_pos+=(sizeof(pso_)+sizeof(size_t)-1)&-psc_mem_align;

  // setup clusters
  rasterizer_local_cluster_index_t num_clusters=pso->num_clusters();
  typename PSC::pso_state pso_state;
  pso->get_pso_state(pso_state);
  for(rasterizer_local_cluster_index_t cidx=0; cidx<num_clusters; ++cidx)
    pso->bin_cluster(m_tiling, pso_state, m_num_dispatches, cidx);
  m_num_clusters+=num_clusters;
  ++m_num_dispatches;
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
