//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_DEVICE_H
#define EGL_DEVICE_H
//----------------------------------------------------------------------------

//============================================================================
// interface
//============================================================================
// external
#include "egl_rasterizer.h"
EGL_NAMESPACE_BEGIN

// new
template <class HWDevice>
struct graphics_device_traits;
template <class HWDevice>
class graphics_device;
template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
class graphics_device_mem;
//----------------------------------------------------------------------------

//============================================================================
// graphics_device_traits
//============================================================================
template <class HWDevice>
struct graphics_device_traits {
    typedef rasterizer_vertex_cache vcache_t;
};
//----------------------------------------------------------------------------

//============================================================================
// graphics_device
//============================================================================
// defines the common interface for graphics devices
template <class HWDevice>
class graphics_device : public rasterizer_callback_base {
public:
    // properties
    enum { supports_dma = false };
    enum { max_concurrency = 1 };
    typedef typename graphics_device_traits<HWDevice>::vcache_t vcache_t;
    //--------------------------------------------------------------------------

    // rasterization
    EGL_INLINE void clear_depth(e_clear_depth);
    template <class PSC>
    EGL_INLINE void dispatch_pso(const PSC&);
    EGL_INLINE void commit();
    template <class TShader>
    EGL_INLINE void commit(const TShader&);
    //--------------------------------------------------------------------------

    // accessors
    EGL_INLINE void* const*                   render_targets() const;
    EGL_INLINE const rasterizer_depth_target& depth_target() const;
    EGL_INLINE vcache_t&                      vcache();
    //--------------------------------------------------------------------------

    // immediate rendering interface
    template <class IPShader>
    void draw_hline(int16_t x_, int16_t y_, uint16_t width_, const IPShader&);
    template <class IPShader>
    void draw_vline(int16_t x_, int16_t y_, uint16_t height_, const IPShader&);
    template <class IPShader>
    void draw_rect(int16_t x_, int16_t y_, uint16_t width_, uint16_t height_, const IPShader&);
    template <class IPShader>
    void fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader&);
    template <class IPShader>
    void fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader&);
    template <class IPShader>
    void fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader&);
    //--------------------------------------------------------------------------

    // stats
    EGL_INLINE void             reset_rasterizer_stats();
    EGL_INLINE rasterizer_stats get_rasterizer_stats() const;
    EGL_INLINE void             log_rasterizer_stats() const;
    //--------------------------------------------------------------------------

protected:
    // construction
    graphics_device();
    void init(const rasterizer_cfg&, const rasterizer_tiling_cfg&, const rasterizer_vertex_cache_cfg&);
    template <e_pixel_format rt0_fmt>
    void init_default_tile_shader();
    //--------------------------------------------------------------------------

    tile_shader_wrapper_base* m_tile_shader;
    tile_shader_wrapper_base* m_tile_shader_default;
    //--------------------------------------------------------------------------

private:
    template <class HWDev, class Cfg>
    friend struct rasterizer_memory;
    graphics_device(const graphics_device&);  // not implemented
    void operator=(const graphics_device&);   // not implemented
    //--------------------------------------------------------------------------

    rasterizer m_rasterizer;
    vcache_t   m_vcache;
};
//----------------------------------------------------------------------------

//============================================================================
// graphics_device_mem
//============================================================================
template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
class graphics_device_mem : public graphics_device<graphics_device_mem<pixel_fmt, width, height>> {
public:
    // device properties
    enum { fb_width  = width,
           fb_height = height };
    enum { fb_format = pixel_fmt };
    typedef pixel<e_pixel_format(fb_format)> fb_format_t;
    //--------------------------------------------------------------------------

    // construction
    graphics_device_mem();
    graphics_device_mem(void* frame_buffer_);
    ~graphics_device_mem();
    void init(void* frame_buffer_);
    void init_rasterizer(const rasterizer_cfg&, const rasterizer_tiling_cfg&, const rasterizer_vertex_cache_cfg&);
    void init_dma(rasterizer_data_transfer*, uint8_t num_transfers_, void* dma_buffer_, size_t dma_buffer_size_);
    //--------------------------------------------------------------------------

    // accessors
    EGL_INLINE fb_format_t* frame_buffer();
    //--------------------------------------------------------------------------

    // immediate rendering interface
    template <class IPShader>
    EGL_INLINE void fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader&);
    template <class IPShader>
    EGL_INLINE void fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader&);
    template <class IPShader>
    EGL_INLINE void fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader&);
    //--------------------------------------------------------------------------

private:
    typedef graphics_device<graphics_device_mem<pixel_fmt, width, height>> base;
    graphics_device_mem(const graphics_device_mem&);     // not implemented
    void         operator=(const graphics_device_mem&);  // not implemented
    virtual void submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16& reg_min_, const vec2u16& reg_end_, uint16_t thread_idx_);
    //--------------------------------------------------------------------------

    fb_format_t* m_frame_buffer;
    // tile data
    fb_format_t*           m_tile_rt0;
    uint8_t                m_rt_pixel_sizes[rasterizer_max_rts];
    uint8_t                m_depth_pixel_size;
    rasterizer_tile_size_t m_tile_width;
    rasterizer_tile_size_t m_tile_height;
};
//----------------------------------------------------------------------------

//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================

//============================================================================
// graphics_device
//============================================================================
template <class HWDevice>
EGL_INLINE void graphics_device<HWDevice>::clear_depth(e_clear_depth clear_depth_)
{
    m_rasterizer.clear_depth(clear_depth_);
}
//----

template <class HWDevice>
template <class PSC>
void graphics_device<HWDevice>::dispatch_pso(const PSC& pso_)
{
    m_rasterizer.dispatch_pso(pso_, static_cast<HWDevice&>(*this));
}
//----

template <class HWDevice>
void graphics_device<HWDevice>::commit()
{
    m_rasterizer.commit();
    m_vcache.clear();
}
//----

template <class HWDevice>
template <class TShader>
void graphics_device<HWDevice>::commit(const TShader& sh_)
{
    // setup tile shader and commit the tile to the device
    tile_shader_wrapper<HWDevice, TShader> tshader;
    tshader.m_dev    = static_cast<HWDevice*>(this);
    tshader.m_shader = &sh_;
    m_tile_shader    = &tshader;
    m_rasterizer.commit();
    m_tile_shader = m_tile_shader_default;
    m_vcache.clear();
}
//----------------------------------------------------------------------------

template <class HWDevice>
void* const* graphics_device<HWDevice>::render_targets() const
{
    return m_rasterizer.render_targets();
}
//----

template <class HWDevice>
const rasterizer_depth_target& graphics_device<HWDevice>::depth_target() const
{
    return m_rasterizer.depth_target();
}
//----

template <class HWDevice>
typename graphics_device<HWDevice>::vcache_t& graphics_device<HWDevice>::vcache()
{
    return m_vcache;
}
//----------------------------------------------------------------------------

template <class HWDevice>
template <class IPShader>
void graphics_device<HWDevice>::draw_hline(int16_t x_, int16_t y_, uint16_t width_, const IPShader& ips_)
{
    // clip horizontal line to display bounds and draw the line
    if (x_ < 0) {
        width_ += x_;
        x_ = 0;
    }
    if (int16_t(width_) <= 0 || x_ >= HWDevice::fb_width || y_ < 0 || y_ >= HWDevice::fb_height)
        return;
    width_ = min<uint16_t>(width_, HWDevice::fb_width - x_);
    ((HWDevice*)this)->fast_draw_hline(x_, y_, width_, ips_);
}
//----

template <class HWDevice>
template <class IPShader>
void graphics_device<HWDevice>::draw_vline(int16_t x_, int16_t y_, uint16_t height_, const IPShader& ips_)
{
    // clip vertical line to display bounds and draw the line
    if (y_ < 0) {
        height_ += y_;
        y_ = 0;
    }
    if (int16_t(height_) <= 0 || x_ >= HWDevice::fb_width || x_ < 0 || y_ >= HWDevice::fb_height)
        return;
    height_ = min<uint16_t>(height_, HWDevice::fb_height - y_);
    ((HWDevice*)this)->fast_draw_vline(x_, y_, height_, ips_);
}
//----

template <class HWDevice>
template <class IPShader>
void graphics_device<HWDevice>::draw_rect(int16_t x_, int16_t y_, uint16_t width_, uint16_t height_, const IPShader& ips_)
{
    // clip the rect to display bounds and draw the box
    if (x_ < 0) {
        width_ += x_;
        x_ = 0;
    }
    if (y_ < 0) {
        height_ += y_;
        y_ = 0;
    }
    if (int16_t(width_) <= 0 || int16_t(height_) <= 0 || x_ >= HWDevice::fb_width || y_ >= HWDevice::fb_height)
        return;
    width_  = min<uint16_t>(width_, HWDevice::fb_width - x_);
    height_ = min<uint16_t>(height_, HWDevice::fb_height - y_);
    ((HWDevice*)this)->fast_draw_rect(x_, y_, width_, height_, ips_);
}
//----------------------------------------------------------------------------

template <class HWDevice>
void graphics_device<HWDevice>::reset_rasterizer_stats()
{
    m_rasterizer.reset_stats();
}
//----

template <class HWDevice>
rasterizer_stats graphics_device<HWDevice>::get_rasterizer_stats() const
{
    return m_rasterizer.stats();
}
//----

template <class HWDevice>
void graphics_device<HWDevice>::log_rasterizer_stats() const
{
    m_rasterizer.log_stats();
}
//----------------------------------------------------------------------------

template <class HWDevice>
graphics_device<HWDevice>::graphics_device()
{
    m_tile_shader         = 0;
    m_tile_shader_default = 0;
}
//----

template <class HWDevice>
void graphics_device<HWDevice>::init(const rasterizer_cfg& rcfg_, const rasterizer_tiling_cfg& tcfg_, const rasterizer_vertex_cache_cfg& vccfg_)
{
    m_rasterizer.init(rcfg_, tcfg_);
    m_rasterizer.set_callback(this);
    m_vcache.init(vccfg_);
}
//----

template <class HWDevice>
template <e_pixel_format rt0_fmt>
void graphics_device<HWDevice>::init_default_tile_shader()
{
    // set rtile0 => frame buffer pixel conversion as default tile shader if the formats doesn't match
    typedef tshader_rt0_pixel_convert<(e_pixel_format)HWDevice::fb_format, rt0_fmt> default_tshader_t;
    static default_tshader_t                                                        tshader;
    static tile_shader_wrapper<HWDevice, default_tshader_t>                         wrapper;
    wrapper.m_dev         = (HWDevice*)this;
    wrapper.m_shader      = &tshader;
    m_tile_shader_default = (int)HWDevice::fb_format != (int)rt0_fmt ? &wrapper : 0;
    m_tile_shader         = m_tile_shader_default;
}
//----------------------------------------------------------------------------

//============================================================================
// graphics_device_mem
//============================================================================
template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
graphics_device_mem<pixel_fmt, width, height>::graphics_device_mem()
{
    m_frame_buffer = 0;
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
graphics_device_mem<pixel_fmt, width, height>::graphics_device_mem(void* frame_buffer_)
{
    m_frame_buffer = (fb_format_t*)frame_buffer_;
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
graphics_device_mem<pixel_fmt, width, height>::~graphics_device_mem()
{
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
void graphics_device_mem<pixel_fmt, width, height>::init(void* frame_buffer_)
{
    m_frame_buffer = (fb_format_t*)frame_buffer_;
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
void graphics_device_mem<pixel_fmt, width, height>::init_rasterizer(const rasterizer_cfg& rcfg_, const rasterizer_tiling_cfg& tcfg_, const rasterizer_vertex_cache_cfg& vccfg_)
{
    graphics_device<graphics_device_mem>::init(rcfg_, tcfg_, vccfg_);
    m_tile_rt0 = (fb_format_t*)rcfg_.rts[0];
    mem_copy(m_rt_pixel_sizes, rcfg_.rt_pixel_sizes, sizeof(m_rt_pixel_sizes));
    m_depth_pixel_size = depth_format_pixel_size(rcfg_.depth.format);
    m_tile_width       = tcfg_.tile_width;
    m_tile_height      = tcfg_.tile_height;
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
void graphics_device_mem<pixel_fmt, width, height>::init_dma(rasterizer_data_transfer*, uint8_t, void*, size_t)
{
    EGL_ERROR_NOT_IMPL();
}
//----------------------------------------------------------------------------

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
typename graphics_device_mem<pixel_fmt, width, height>::fb_format_t* graphics_device_mem<pixel_fmt, width, height>::frame_buffer()
{
    return m_frame_buffer;
}
//----------------------------------------------------------------------------

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
template <class IPShader>
void graphics_device_mem<pixel_fmt, width, height>::fast_draw_hline(uint16_t x_, uint16_t y_, uint16_t width_, const IPShader& ips_)
{
    // draw horizontal line
    fb_format_t* dst   = m_frame_buffer + y_ * fb_width;
    uint16_t     x_end = x_ + width_;
    do {
        ips_.exec(dst[x_], x_, y_);
    } while (++x_ < x_end);
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
template <class IPShader>
void graphics_device_mem<pixel_fmt, width, height>::fast_draw_vline(uint16_t x_, uint16_t y_, uint16_t height_, const IPShader& ips_)
{
    // draw vertical line
    fb_format_t* dst   = m_frame_buffer + x_ + y_ * fb_width;
    uint16_t     y_end = y_ + height_;
    do {
        ips_.exec(*dst, x_, y_);
        dst += fb_width;
    } while (++y_ < y_end);
}
//----

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
template <class IPShader>
void graphics_device_mem<pixel_fmt, width, height>::fast_draw_rect(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, const IPShader& ips_)
{
    // draw rectangle
    fb_format_t* dst   = m_frame_buffer + y_ * fb_width;
    uint16_t     x_end = x_ + width_, y_end = y_ + height_;
    do {
        uint16_t x = x_;
        do {
            ips_.exec(dst[x], x, y_);
        } while (++x < x_end);
        dst += fb_width;
    } while (++y_ < y_end);
}
//----------------------------------------------------------------------------

template <e_pixel_format pixel_fmt, unsigned width, unsigned height>
void graphics_device_mem<pixel_fmt, width, height>::submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16& reg_min_, const vec2u16& reg_end_, uint16_t thread_idx_)
{
    // access update pos, size and data
    uint16_t x                        = tx_ * m_tile_width + reg_min_.x;
    uint16_t y                        = ty_ * m_tile_height + reg_min_.y;
    uint16_t update_width             = reg_end_.x - reg_min_.x;
    uint16_t update_height            = reg_end_.y - reg_min_.y;
    unsigned tile_buffer_pixel_offset = m_tile_width * m_tile_height * thread_idx_;
    if (base::m_tile_shader) {
        // setup tile rts and depth target
        const void* const* rts = (const void* const*)base::render_targets();
        const void*        tile_buffer_rts[rasterizer_max_rts];
        for (unsigned i = 0; i < rasterizer_max_rts; ++i)
            tile_buffer_rts[i] = (uint8_t*)rts[i] + tile_buffer_pixel_offset * m_rt_pixel_sizes[i];
        rasterizer_depth_target tile_buffer_dt = base::depth_target();
        if (tile_buffer_dt.data)
            (uint8_t*&)tile_buffer_dt.data += tile_buffer_pixel_offset * m_depth_pixel_size;
        if (tile_buffer_dt.hiz_data)
            tile_buffer_dt.hiz_data += tile_buffer_pixel_offset / rasterizer_hiz_tile_pixels;
        base::m_tile_shader->transfer_region(tile_buffer_rts, tile_buffer_dt, size_t(reg_min_.x + reg_min_.y * m_tile_width), x, y, update_width, update_height, m_tile_width);
    } else {
        fb_format_t* data     = m_tile_rt0 + tile_buffer_pixel_offset + reg_min_.x + reg_min_.y * m_tile_width;
        fb_format_t* dst      = m_frame_buffer + x + y * fb_width;
        fb_format_t* data_end = data + m_tile_width * update_height;
        while (data < data_end) {
            mem_copy(dst, data, update_width * sizeof(fb_format_t));
            dst += fb_width;
            data += m_tile_width;
        }
    }
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
