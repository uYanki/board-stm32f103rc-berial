//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#include "egl_rasterizer.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------

//============================================================================
// rasterizer_cfg
//============================================================================
rasterizer_cfg::rasterizer_cfg()
{
    dispatches         = 0;
    max_dispatches     = 0;
    pso_store          = 0;
    pso_store_size     = 0;
    max_pso_state_size = 0;
    depth.data         = 0;
    depth.hiz_data     = 0;
    depth.format       = depthfmt_none;
    rts                = 0;
    num_tile_buffers   = 0;
}
//----

void rasterizer_cfg::set_dispatches(rasterizer_dispatch* dispatches_, rasterizer_dispatch_index_t max_dispatches_)
{
    dispatches     = dispatches_;
    max_dispatches = max_dispatches_;
}
//----

void rasterizer_cfg::set_pso_store(void* pso_store_, size_t pso_store_size_, uint16_t max_pso_state_size_)
{
    pso_store          = pso_store_;
    pso_store_size     = pso_store_size_;
    max_pso_state_size = max_pso_state_size_;
}
//----

void rasterizer_cfg::set_tile_buffers(void* const* rts_, void* depth_buf_, uint16_t* hiz_buf_, e_depth_format depth_fmt_, uint16_t num_tile_buffers_)
{
    rts              = rts_;
    depth.data       = depth_buf_;
    depth.hiz_data   = hiz_buf_;
    depth.format     = depth_fmt_;
    num_tile_buffers = num_tile_buffers_;
}
//----------------------------------------------------------------------------

//============================================================================
// rasterizer_stats
//============================================================================
void rasterizer_stats::reset()
{
    // reset stats
    num_dispatches      = 0;
    pso_store_size      = 0;
    max_pso_state_size  = 0;
    num_cluster_strips  = 0;
    tmp_ptv_buffer_size = 0;
    num_clusters        = 0;
    max_tile_clusters   = 0;
}
//----

void rasterizer_stats::log() const
{
    // print log stats
    EGL_LOGF("> Max dispatches: %i\r\n", (int)num_dispatches);
    EGL_LOGF("> Max PSO store: %i\r\n", (int)pso_store_size);
    EGL_LOGF("> Max PSO state: %i\r\n", (int)max_pso_state_size);
    EGL_LOGF("> Max cluster strips: %i\r\n", (int)num_cluster_strips);
    EGL_LOGF("> Max PTV buffer size: %i\r\n", (int)tmp_ptv_buffer_size);
    EGL_LOGF("> Max clusters: %i\r\n", (int)num_clusters);
    EGL_LOGF("> Max tile clusters: %i\r\n", (int)max_tile_clusters);
}
//----------------------------------------------------------------------------

//============================================================================
// rasterizer_callback_base
//============================================================================
void rasterizer_callback_base::rasterize_tiles(rasterizer& rasterizer_, const render_target_cfg& rt_cfg_, rasterizer_tile* tiles_, size_t num_tiles_)
{
    const rasterizer_tile* tile_end = tiles_ + num_tiles_;
    do {
#if EGL_BUILDOP_RASTERIZER_STATS == 1
        rasterizer_.m_stats.max_tile_clusters = max<rasterizer_global_cluster_index_t>(rasterizer_.m_stats.max_tile_clusters, tiles_->num_clusters);
#endif
        rasterizer_.rasterize_tile(rt_cfg_, *tiles_, 0);
    } while (++tiles_ < tile_end);
}
//----------------------------------------------------------------------------

//============================================================================
// rasterizer
//============================================================================
rasterizer::rasterizer()
{
    m_callback = 0;
    mem_zero(&m_cfg, sizeof(m_cfg));
    m_num_dispatches = 0;
    m_clear_depth    = cleardepth_max;
#if EGL_BUILDOP_RASTERIZER_STATS == 1
    m_stats.reset();
#endif
}
//----

void rasterizer::init(const rasterizer_cfg& cfg_, const rasterizer_tiling_cfg& tcfg_)
{
    EGL_ASSERT(m_cfg.depth.format == depthfmt_none || m_cfg.depth.data);
    m_tiling.init(tcfg_);
    m_cfg            = cfg_;
    m_num_dispatches = 0;
    m_num_clusters   = 0;
    m_pso_write_pos  = 0;
    EGL_ASSERT_MSG(((size_t)cfg_.pso_store & (psc_mem_align - 1)) == 0, "PSO store not properly aligned.\r\n");

    // clear render targets
    unsigned num_tile_buffer_pixels = m_tiling.tile_width() * m_tiling.tile_height() * m_cfg.num_tile_buffers;
    for (uint8_t rt_idx = 0; rt_idx < rasterizer_max_rts; ++rt_idx) {
        size_t rt_tile_size = m_cfg.rt_pixel_sizes[rt_idx] * num_tile_buffer_pixels;
        mem_zero(m_cfg.rts[rt_idx], rt_tile_size);
    }
}
//----------------------------------------------------------------------------

void rasterizer::clear_depth(e_clear_depth clear_depth_)
{
    // clear depth
    m_clear_depth                   = clear_depth_;
    unsigned num_tile_buffer_pixels = m_tiling.tile_width() * m_tiling.tile_height() * m_cfg.num_tile_buffers;
    switch (m_cfg.depth.format) {
        // no depth
        case depthfmt_none: {
            m_cfg.depth.data     = 0;
            m_cfg.depth.hiz_data = 0;
        } break;

        // 8bpp unorm depth buffer
        case depthfmt_uint8: {
            size_t depth_tile_size = sizeof(uint8_t) * num_tile_buffer_pixels;
            mem_set(m_cfg.depth.data, clear_depth_ == cleardepth_max ? 0xff : 0x00, depth_tile_size);
        } break;

        // 16bpp unorm depth buffer
        case depthfmt_uint16: {
            size_t depth_tile_size = sizeof(uint16_t) * num_tile_buffer_pixels;
            mem_set(m_cfg.depth.data, clear_depth_ == cleardepth_max ? 0xff : 0x00, depth_tile_size);
        } break;

        // float32 depth buffer
        case depthfmt_float32: {
            float *p = (float*)m_cfg.depth.data, *p_end = p + num_tile_buffer_pixels;
            if (clear_depth_ == cleardepth_max) {
                do {
                    *p = 1.0f;
                } while (++p < p_end);
            } else
                mem_set(p, 0x00, sizeof(float32_t) * num_tile_buffer_pixels);
        } break;

        // unsupported format
        default: EGL_ERROR("Unsupported depth format.\r\n");
    }

    // clear hi-z
    if (m_cfg.depth.hiz_data)
        mem_set(m_cfg.depth.hiz_data, clear_depth_ == cleardepth_max ? 0xff : 0x00, num_tile_buffer_pixels / rasterizer_hiz_tile_pixels);
}
//----

void rasterizer::commit()
{
    // setup render target config
    EGL_ASSERT(m_callback);
    render_target_cfg rt_cfg;
    rt_cfg.rt_width  = m_tiling.rt_width();
    rt_cfg.rt_height = m_tiling.rt_height();
    rt_cfg.rt_coord_scale.set(float((m_tiling.rt_width() / 2) << rasterizer_subpixel_bits), float((m_tiling.rt_height() / 2) << rasterizer_subpixel_bits));
    rt_cfg.depth      = m_cfg.depth;
    rt_cfg.rts        = m_cfg.rts;
    rt_cfg.tile_pitch = m_tiling.tile_width();

#if EGL_BUILDOP_RASTERIZER_STATS == 1
    // update stats
    m_stats.num_dispatches     = max<size_t>(m_num_dispatches, m_stats.num_dispatches);
    m_stats.pso_store_size     = max(m_pso_write_pos, m_stats.pso_store_size);
    m_stats.num_cluster_strips = max<size_t>(m_tiling.num_cluster_strips(), m_stats.num_cluster_strips);
    m_stats.num_clusters       = max<size_t>(m_num_clusters, m_stats.num_clusters);
#endif

    // rasterize tiles and clear rasterizer state
    m_callback->rasterize_tiles(*this, rt_cfg, m_tiling.tiles(), m_tiling.num_tiles());
    m_tiling.clear();
    m_num_dispatches = 0;
    m_num_clusters   = 0;
    m_pso_write_pos  = 0;
}
//----

void rasterizer::rasterize_tile(const render_target_cfg& rt_cfg_, rasterizer_tile& tile_, uint16_t thread_idx_)
{
    // setup tile coords and size
    uint8_t  tx = tile_.x, ty = tile_.y;
    uint16_t tile_x          = tx << m_tiling.tile_width_shift();
    uint16_t tile_y          = ty << m_tiling.tile_height_shift();
    uint16_t tile_hiz_x      = tile_x / rasterizer_hiz_tile_width;
    uint16_t tile_hiz_y      = tile_y / rasterizer_hiz_tile_width;
    uint16_t tile_width      = min<uint16_t>(m_tiling.rt_width() - tile_x, m_tiling.tile_width());
    uint16_t tile_height     = min<uint16_t>(m_tiling.rt_height() - tile_y, m_tiling.tile_height());
    unsigned num_tile_pixels = m_tiling.tile_width() * m_tiling.tile_height();

    // rasterize tile clusters
    rasterizer_global_cluster_index_t num_tile_clusters = tile_.num_clusters;
    rasterizer_cluster_strip_index_t  cstrip_idx        = tile_.cluster_strip_first;
    rasterizer_dispatch_index_t       cur_dispatch_idx  = rasterizer_dispatch_index_t(-1);
    result                            res;
    res.tile_min.set(tile_width, tile_height);
    res.tile_end.set(0, 0);
    res.hiz_update = false;
#if EGL_BUILDOP_RASTERIZER_STATS == 1
    res.max_pso_state_size  = 0;
    res.max_ptv_buffer_size = 0;
#endif
    vec2u16                 tile_min         = {tile_width, tile_height};
    vec2u16                 tile_end         = {0, 0};
    bool                    hiz_test         = false;
    e_hiz_mode              current_hiz_mode = m_clear_depth == cleardepth_max ? hizmode_max : hizmode_min;
    const psc_wrapper_base* psc_wrapper      = 0;
    void*                   pso_state_buffer = EGL_STACK_MALLOC(m_cfg.max_pso_state_size);
    while (num_tile_clusters) {
        // rasterize cluster strip
        const rasterizer_tile_cluster_strip<1>& cstrip             = m_tiling.cluster_strip(cstrip_idx);
        uint8_t                                 num_strip_clusters = (uint8_t)min<rasterizer_global_cluster_index_t>(num_tile_clusters, m_tiling.m_cfg.cluster_strip_cluster_mask + 1);
        num_tile_clusters -= num_strip_clusters;
        const rasterizer_global_cluster_index_t *global_cluster_idx = cstrip.global_cluster_idx, *global_cluster_idx_end = global_cluster_idx + num_strip_clusters;
        do {
            // check for dispatch change
            const rasterizer_cluster& cluster = m_tiling.cluster(*global_cluster_idx);
            if (cur_dispatch_idx != cluster.dispatch_idx) {
                // pick PSO for the dispatch
                psc_wrapper = (const psc_wrapper_base*)m_cfg.dispatches[cluster.dispatch_idx].psc_wrapper;
                psc_wrapper->get_pso_state(pso_state_buffer, m_cfg.max_pso_state_size);

#if EGL_BUILDOP_RASTERIZER_HIZ == 1
                if (m_cfg.depth.hiz_data) {
                    // update tile bounds
                    tile_min = min(tile_min, res.tile_min);
                    tile_end = max(tile_end, res.tile_end);

                    // check for hi-z mode switch
                    e_hiz_mode dispatch_hiz_mode = psc_wrapper->hiz_mode();
                    hiz_test                     = cur_dispatch_idx != rasterizer_dispatch_index_t(-1) && dispatch_hiz_mode != hizmode_none;
                    if (dispatch_hiz_mode != current_hiz_mode && dispatch_hiz_mode != hizmode_none && dispatch_hiz_mode != hizmode_any) {
                        // switch hi-z mode and force full tile hi-z update
                        current_hiz_mode = dispatch_hiz_mode;
                        res.hiz_update   = true;
                        res.tile_min.set(0, 0);
                        res.tile_end.set(tile_width, tile_height);
                    }

                    // check for hi-z update
                    if (dispatch_hiz_mode != hizmode_none && res.hiz_update && res.tile_end.x && res.tile_end.y) {
                        // update hi-z for the changed tile region
                        switch (m_cfg.depth.format) {
                            // 8bpp unorm depth buffer
                            case depthfmt_uint8: update_hiz<uint8_t>(res, current_hiz_mode, thread_idx_); break;
                            // 16bpp unorm depth buffer
                            case depthfmt_uint16: update_hiz<uint16_t>(res, current_hiz_mode, thread_idx_); break;
                            // float32 depth buffer
                            case depthfmt_float32: update_hiz<float32_t>(res, current_hiz_mode, thread_idx_); break;
                            // unsupported format
                            default: EGL_ERROR("Unsupported depth format.\r\n");
                        }

                        // reset bounds for dispatch bounds update
                        res.tile_min.set(tile_width, tile_height);
                        res.tile_end.set(0, 0);
                    }
                }
#endif
                cur_dispatch_idx = cluster.dispatch_idx;
            }

#if EGL_BUILDOP_RASTERIZER_HIZ == 1
            // check cluster occlusion against hi-z
            if (hiz_test) {
                // get min/max hi-z within cluster region bounds of the tile
                const uint16_t  hiz_test_x      = (uint16_t)max<int16_t>(0, cluster.hiz_x - tile_hiz_x);
                const uint16_t  hiz_test_y      = (uint16_t)max<int16_t>(0, cluster.hiz_y - tile_hiz_y);
                const uint16_t  hiz_test_width  = min<int16_t>(m_tiling.tile_width() / rasterizer_hiz_tile_width - hiz_test_x, cluster.hiz_x + cluster.hiz_width - tile_hiz_x - hiz_test_x);
                const uint16_t  hiz_test_height = min<int16_t>(m_tiling.tile_height() / rasterizer_hiz_tile_width - hiz_test_y, cluster.hiz_y + cluster.hiz_height - tile_hiz_y - hiz_test_y);
                const uint16_t  hiz_pitch       = m_tiling.tile_width() / rasterizer_hiz_tile_width;
                const uint16_t  hiz_scan_pitch  = hiz_pitch - hiz_test_width;
                unsigned        hiz_tile_offset = (num_tile_pixels / rasterizer_hiz_tile_pixels) * thread_idx_;
                const uint16_t* hiz_buf         = m_cfg.depth.hiz_data + hiz_tile_offset + hiz_test_x + hiz_test_y * hiz_pitch;
                const uint16_t* hiz_buf_end     = hiz_buf + hiz_test_height * hiz_pitch;
                if (!hiz_test_width || !hiz_test_height)
                    continue;
                if (current_hiz_mode == hizmode_max) {
                    // get max hi-z for cluster region
                    uint16_t max_hiz = 0;
                    do {
                        const uint16_t* hiz_scan_end = hiz_buf + hiz_test_width;
                        do {
                            max_hiz = max(max_hiz, *hiz_buf);
                        } while (++hiz_buf < hiz_scan_end);
                        hiz_buf += hiz_scan_pitch;
                    } while (hiz_buf < hiz_buf_end);

                    // skip cluster if the region max hi-z is less than cluster min hi-z
                    if (max_hiz < cluster.hiz_minz)
                        continue;
                } else {
                    // get min hi-z for cluster region
                    uint16_t min_hiz = 0;
                    do {
                        const uint16_t* hiz_scan_end = hiz_buf + hiz_test_width;
                        do {
                            min_hiz = min(min_hiz, *hiz_buf);
                        } while (++hiz_buf < hiz_scan_end);
                        hiz_buf += hiz_scan_pitch;
                    } while (hiz_buf < hiz_buf_end);

                    // skip cluster if the region min hi-z is greater than cluster max hi-z
                    if (min_hiz > cluster.hiz_maxz)
                        continue;
                }
            }
#endif

            // rasterize the cluster for the tile
            psc_wrapper->rasterize_cluster(*m_callback, res, rt_cfg_, tile_x, tile_y, tile_width, tile_height, num_tile_pixels * thread_idx_, pso_state_buffer, thread_idx_, cluster.local_cluster_idx, *global_cluster_idx);
        } while (++global_cluster_idx < global_cluster_idx_end);
        cstrip_idx = cstrip.next;
    }
    res.tile_min = min(res.tile_min, tile_min);
    res.tile_end = max(res.tile_end, tile_end);
#if EGL_BUILDOP_RASTERIZER_STATS == 1
    m_stats.max_pso_state_size  = max(res.max_pso_state_size, m_stats.max_pso_state_size);
    m_stats.tmp_ptv_buffer_size = max(res.max_ptv_buffer_size, m_stats.tmp_ptv_buffer_size);
#endif

    // calculate tile update region and submit tile
    vec2u16 update_region_min = min(res.tile_min, vec2u16(tile_.tile_min.x, tile_.tile_min.y));
    vec2u16 update_region_end = max(res.tile_end, vec2u16(tile_.tile_end.x, tile_.tile_end.y));
    tile_.tile_min.x          = rasterizer_tile_size_t(res.tile_min.x);
    tile_.tile_min.y          = rasterizer_tile_size_t(res.tile_min.y);
    tile_.tile_end.x          = rasterizer_tile_size_t(res.tile_end.x);
    tile_.tile_end.y          = rasterizer_tile_size_t(res.tile_end.y);
    if (update_region_end.x && update_region_end.y) {
        // submit tile
        m_callback->submit_tile(tx, ty, update_region_min, update_region_end, thread_idx_);
        bool clear_subtile = res.tile_min.x || res.tile_end.x < m_tiling.tile_width() || res.tile_min.y || res.tile_end.y < m_tiling.tile_height();
        if (!res.tile_end.x || !res.tile_end.y)
            return;

        // clear depth
        unsigned tile_buffer_pixel_offset = num_tile_pixels * thread_idx_;
        switch (m_cfg.depth.format) {
            // no depth
            case depthfmt_none: break;

            // 8bpp unorm depth buffer
            case depthfmt_uint8: {
                uint8_t  clear_value = m_clear_depth == cleardepth_max ? 0xff : 0x00;
                uint8_t* depth_buf   = (uint8_t*)m_cfg.depth.data + tile_buffer_pixel_offset;
                if (clear_subtile) {
                    size_t   depth_subtile_sline_size = sizeof(uint8_t) * (res.tile_end.x - res.tile_min.x);
                    size_t   depth_tile_width         = m_tiling.tile_width();
                    uint8_t* p                        = depth_buf + res.tile_min.x + res.tile_min.y * m_tiling.tile_width();
                    uint8_t* p_end                    = p + depth_tile_width * (res.tile_end.y - res.tile_min.y);
                    do {
                        mem_set(p, clear_value, depth_subtile_sline_size);
                        p += depth_tile_width;
                    } while (p < p_end);
                } else {
                    size_t depth_tile_size = sizeof(uint8_t) * num_tile_pixels;
                    mem_set(depth_buf, clear_value, depth_tile_size);
                }
            } break;

            // 16bpp unorm depth buffer
            case depthfmt_uint16: {
                uint8_t   clear_value = m_clear_depth == cleardepth_max ? 0xff : 0x00;
                uint16_t* depth_buf   = (uint16_t*)m_cfg.depth.data + tile_buffer_pixel_offset;
                if (clear_subtile) {
                    size_t    depth_subtile_sline_size = sizeof(uint16_t) * (res.tile_end.x - res.tile_min.x);
                    size_t    depth_tile_width         = m_tiling.tile_width();
                    uint16_t* p                        = depth_buf + res.tile_min.x + res.tile_min.y * m_tiling.tile_width();
                    uint16_t* p_end                    = p + depth_tile_width * (res.tile_end.y - res.tile_min.y);
                    do {
                        mem_set(p, clear_value, depth_subtile_sline_size);
                        p += depth_tile_width;
                    } while (p < p_end);
                } else {
                    size_t depth_tile_size = sizeof(uint16_t) * num_tile_pixels;
                    mem_set(depth_buf, clear_value, depth_tile_size);
                }
            } break;

            // 32bpp float depth buffer
            case depthfmt_float32: {
                float32_t* depth_buf = (float32_t*)m_cfg.depth.data + tile_buffer_pixel_offset;
                if (clear_subtile) {
                    size_t     subtile_width = res.tile_end.x - res.tile_min.x;
                    float32_t* p             = depth_buf + res.tile_min.x + res.tile_min.y * m_tiling.tile_width();
                    float32_t* p_tile_end    = p + m_tiling.tile_width() * (res.tile_end.y - res.tile_min.y);
                    if (m_clear_depth == cleardepth_max) {
                        size_t depth_tile_pitch_delta = m_tiling.tile_width() - subtile_width;
                        do {
                            float32_t* scan_p_end = p + subtile_width;
                            do {
                                *p = 1.0f;
                            } while (++p < scan_p_end);
                            p += depth_tile_pitch_delta;
                        } while (p < p_tile_end);
                    } else {
                        size_t tile_width = m_tiling.tile_width();
                        do {
                            mem_zero(p, subtile_width * sizeof(float32_t));
                            p += tile_width;
                        } while (p < p_tile_end);
                    }
                } else {
                    if (m_clear_depth == cleardepth_max) {
                        float32_t *p = depth_buf, *p_end = p + num_tile_pixels;
                        do {
                            *p = 1.0f;
                        } while (++p < p_end);
                    } else
                        mem_zero(depth_buf, num_tile_pixels * sizeof(float32_t));
                }
            } break;

            // unsupported format
            default: EGL_ERROR("Unsupported depth format.\r\n");
        }

        // clear hi-z
        if (m_cfg.depth.hiz_data)
            mem_set(m_cfg.depth.hiz_data + tile_buffer_pixel_offset / rasterizer_hiz_tile_pixels, m_clear_depth == cleardepth_max ? 0xff : 0x00, sizeof(uint16_t) * num_tile_pixels / (rasterizer_hiz_tile_width * rasterizer_hiz_tile_width));

        // clear render targets
        for (uint8_t rt_idx = 0; rt_idx < rasterizer_max_rts; ++rt_idx) {
            uint8_t px_size = m_cfg.rt_pixel_sizes[rt_idx];
            if (!px_size)
                continue;
            size_t rt_tile_pitch = px_size * m_tiling.tile_width();
            if (clear_subtile) {
                size_t   rt_subtile_sline_size = px_size * (res.tile_end.x - res.tile_min.x);
                uint8_t* p                     = (uint8_t*)m_cfg.rts[rt_idx] + px_size * (tile_buffer_pixel_offset + res.tile_min.x + res.tile_min.y * m_tiling.tile_width());
                uint8_t* p_end                 = p + rt_tile_pitch * (res.tile_end.y - res.tile_min.y);
                do {
                    mem_zero(p, rt_subtile_sline_size);
                    p += rt_tile_pitch;
                } while (p < p_end);
            } else {
                size_t rt_tile_size = rt_tile_pitch * m_tiling.tile_height();
                mem_zero((uint8_t*)m_cfg.rts[rt_idx] + px_size * tile_buffer_pixel_offset, rt_tile_size);
            }
        }
    }
}
//----------------------------------------------------------------------------

template <typename T>
void rasterizer::update_hiz(const rasterizer::result& res_, e_hiz_mode hiz_mode_, uint16_t thread_idx_)
{
    // prepare hi-z update
    vec2i32  hiz_tile_min         = {res_.tile_min.x / rasterizer_hiz_tile_width, res_.tile_min.y / rasterizer_hiz_tile_width};
    vec2i32  hiz_tile_end         = {(res_.tile_end.x + rasterizer_hiz_tile_width - 1) / rasterizer_hiz_tile_width, (res_.tile_end.y + rasterizer_hiz_tile_width - 1) / rasterizer_hiz_tile_width};
    uint16_t hiz_update_width     = uint16_t(hiz_tile_end.x - hiz_tile_min.x);
    uint16_t hiz_update_height    = uint16_t(hiz_tile_end.y - hiz_tile_min.y);
    uint16_t hiz_tile_pitch       = m_tiling.tile_width() / rasterizer_hiz_tile_width;
    uint16_t zbuf_tile_scan_pitch = m_tiling.tile_width() - hiz_update_width * rasterizer_hiz_tile_width;

    // iterate over all hi-z tiles
    unsigned  tile_buffer_pixel_offset = thread_idx_ * m_tiling.tile_width() * m_tiling.tile_height();
    const T*  zbuf_base                = (T*)m_cfg.depth.data + tile_buffer_pixel_offset;
    uint16_t* hiz_buf_base             = m_cfg.depth.hiz_data + tile_buffer_pixel_offset / rasterizer_hiz_tile_pixels;
    uint16_t* hiz_buf                  = hiz_buf_base + hiz_tile_min.x + hiz_tile_min.y * hiz_tile_pitch;
    uint16_t* hiz_buf_end              = hiz_buf + hiz_update_height * hiz_tile_pitch;
    uint16_t  hiz_y                    = uint16_t(hiz_tile_min.y * rasterizer_hiz_tile_width);
    if (hiz_mode_ == hizmode_max) {
        do {
            uint16_t* hiz_buf_scan_end = hiz_buf + hiz_update_width;
            uint16_t  hiz_x            = uint16_t(hiz_tile_min.x * rasterizer_hiz_tile_width);
            do {
                // update hi-z tile
                const T* zbuf      = zbuf_base + hiz_x + hiz_y * m_tiling.tile_width();
                const T* zbuf_end  = zbuf + m_tiling.tile_width() * rasterizer_hiz_tile_width;
                T        tile_maxz = 0;
                do {
                    // update hi-z tile scanline
                    EGL_STATIC_ASSERT(rasterizer_hiz_tile_width == 16);
                    T maxz[4];
                    maxz[0]   = max(zbuf[0], zbuf[1], zbuf[2], zbuf[3]);
                    maxz[1]   = max(zbuf[4], zbuf[5], zbuf[6], zbuf[7]);
                    maxz[2]   = max(zbuf[8], zbuf[9], zbuf[10], zbuf[11]);
                    maxz[3]   = max(zbuf[12], zbuf[13], zbuf[14], zbuf[15]);
                    tile_maxz = max(tile_maxz, max(maxz[0], maxz[1], maxz[2], maxz[3]));
                    zbuf += m_tiling.tile_width();
                } while (zbuf < zbuf_end);
                *hiz_buf = hiz_value(tile_maxz);
                hiz_x += rasterizer_hiz_tile_width;
            } while (++hiz_buf < hiz_buf_scan_end);

            // proceed to the next row of hi-z tiles
            hiz_y += rasterizer_hiz_tile_width;
            hiz_buf += zbuf_tile_scan_pitch / rasterizer_hiz_tile_width;
        } while (hiz_buf < hiz_buf_end);
    } else {
        do {
            uint16_t* hiz_buf_scan_end = hiz_buf + hiz_update_width;
            uint16_t  hiz_x            = uint16_t(hiz_tile_min.x * rasterizer_hiz_tile_width);
            do {
                // update hi-z tile
                const T* zbuf      = zbuf_base + hiz_x + hiz_y * m_tiling.tile_width();
                const T* zbuf_end  = zbuf + m_tiling.tile_width() * rasterizer_hiz_tile_width;
                T        tile_minz = 0;
                do {
                    // update hi-z tile scanline
                    EGL_STATIC_ASSERT(rasterizer_hiz_tile_width == 16);
                    T minz[4];
                    minz[0]   = min(zbuf[0], zbuf[1], zbuf[2], zbuf[3]);
                    minz[1]   = min(zbuf[4], zbuf[5], zbuf[6], zbuf[7]);
                    minz[2]   = min(zbuf[8], zbuf[9], zbuf[10], zbuf[11]);
                    minz[3]   = min(zbuf[12], zbuf[13], zbuf[14], zbuf[15]);
                    tile_minz = min(tile_minz, min(minz[0], minz[1], minz[2], minz[3]));
                    zbuf += m_tiling.tile_width();
                } while (zbuf < zbuf_end);
                *hiz_buf = hiz_value(tile_minz);
                hiz_x += rasterizer_hiz_tile_width;
            } while (++hiz_buf < hiz_buf_scan_end);

            // proceed to the next row of hi-z tiles
            hiz_y += rasterizer_hiz_tile_width;
            hiz_buf += zbuf_tile_scan_pitch / rasterizer_hiz_tile_width;
        } while (hiz_buf < hiz_buf_end);
    }
}
//----------------------------------------------------------------------------
