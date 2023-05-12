
#include "egl_device_lib.h"
#include "egl_shaders.h"

#include "gpio.h"
#include "spi.h"

EGL_USING_NAMESPACE

typedef graphics_device_st7735 gfx_device_t;

static gfx_device_t s_gfx_device;

static const EGL_ALIGN(4) uint32_t s_ptx_gauge_bg[] = {
#include "ptx_gauge_bg.h"
};
static texture                                     s_tex_gauge_bg;
typedef sampler2d<pixfmt_r5g6b5, texfilter_linear> smp_gauge_bg_t;
smp_gauge_bg_t                                     s_smp_gauge_bg;

static const EGL_ALIGN(4) uint32_t s_ptx_gauge_needle[] = {
#include "ptx_gauge_needle.h"
};
static texture                              s_tex_gauge_needle;
sampler2d<pixfmt_r5g5b5a1, texfilter_point> s_smp_gauge_needle;

struct rasterizer_memory_cfg : rasterizer_memory_cfg_base {
    enum { rt0_fmt = pixfmt_r5g5b5a1 };
    enum { pso_store_size = 2048 };
    enum { vcache_size = 2048 };
    enum { max_dma_transfers = 0 };
    enum { dma_buffer_px = 0 };
};

struct vertex_needle {
    struct transform_state {
        template <class PSC>
        EGL_INLINE transform_state(const PSC&) {}
    };
    vec2f pos;
};

static vertex_needle s_needle_vertices[8];
static const uint8_t s_needle_indices[12] = {0, 1, 2, 1, 3, 2, 4, 5, 6, 5, 7, 6};

static rasterizer_memory<gfx_device_t, rasterizer_memory_cfg> s_gfx_device_mem;

struct vs_gauge_needle {
    template <class PSIn, class PSOState, class VIn>
    EGL_INLINE void exec(PSIn& psin_, const PSOState&, const typename VIn::transform_state&, const VIn& vin_, uint8_t vertex_idx_) const
    {
        psin_.pos = vin_.pos;
        psin_.uv.set(float(vertex_idx_ & 1), float(vertex_idx_ >> 1));
    }
};

struct ps_gauge_needle {
    struct psin {
        vec2f pos;
        vec2f uv;
    };

    template <class PSC>
    EGL_INLINE void exec(rasterizer_pixel_out<PSC>& psout_, const typename PSC::pso_state&, const psin& v0_, const psin& v1_, const psin& v2_, const vec3f& bc_, uint8_t prim_idx_) const
    {
        if (prim_idx_ < 2) {
            vec2f                  uv = v0_.uv * bc_.x + v1_.uv * bc_.y + v2_.uv * bc_.z;
            pixel<pixfmt_r5g5b5a1> smp;
            s_smp_gauge_needle.sample(smp, s_tex_gauge_needle, uv);
            if (smp.c.a)
                psout_.export_rt0(smp);
        }
    }
};

struct rstate_gauge_needle : rasterizer_psc_base {
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
    enum { tbfc_mode = rstate_tbfc_none };
};

struct tshader_gauge {
    template <e_pixel_format dst_fmt>
    EGL_INLINE void exec(pixel<dst_fmt>& res_, const void* const* rts_, const rasterizer_depth_target&, size_t src_px_offs_, uint16_t x_, uint16_t y_) const
    {
        typedef pixel<e_pixel_format(rasterizer_memory_cfg::rt0_fmt)> rt0_fmt_t;

        const rt0_fmt_t smp = ((const rt0_fmt_t*)rts_[0])[src_px_offs_];
        if (smp.c.a) {
            res_ = smp;
            return;
        }

#if ROTATE_90_DEGREES == 0
        s_smp_gauge_bg.sample(res_, s_tex_gauge_bg, vec2f((x_ + 0.5f) / gfx_device_t::fb_width, (y_ + 0.5f) / gfx_device_t::fb_height));
#else
        s_smp_gauge_bg.sample(res_, s_tex_gauge_bg, vec2f((y_ + 0.5f) / gfx_device_t::fb_height, -(x_ + 0.5f) / gfx_device_t::fb_width));
#endif
    }
};

void update_gauge_needle(float normalized_speed_)
{
    static const float s_rcp_aspect_ratio = float(s_tex_gauge_bg.height()) / float(s_tex_gauge_bg.width());
    static const vec2f s_needle_center_pos(0.0f, -0.3f);
    static const float s_needle_base_offset = -0.233f;
    static const float s_angle_speed_0      = 3.6f;
    static const float s_angle_speed_max    = -0.46f;

    const float needle_half_width = s_tex_gauge_needle.width() / float(s_tex_gauge_bg.width());
    const float needle_length     = 2.0f * s_tex_gauge_needle.height() / float(s_tex_gauge_bg.height());
    float       angle             = lerp(s_angle_speed_0, s_angle_speed_max, normalized_speed_);
    float       cos_angle = cos(angle), sin_angle = sin(angle);
    vec2f       needle_base(s_needle_center_pos.x + cos_angle * s_needle_base_offset, s_needle_center_pos.y + sin_angle * s_needle_base_offset);
    vec2f       needle_tip(needle_base.x + cos_angle * needle_length, needle_base.y + sin_angle * needle_length);
    vec2f       needle_right(sin_angle * needle_half_width, -cos_angle * needle_half_width);

    mem_copy(s_needle_vertices + 4, s_needle_vertices, sizeof(s_needle_vertices) / 2);
#if ROTATE_90_DEGREES == 0
    s_needle_vertices[0].pos.set((needle_tip.x - needle_right.x) * s_rcp_aspect_ratio, needle_tip.y - needle_right.y);
    s_needle_vertices[1].pos.set((needle_tip.x + needle_right.x) * s_rcp_aspect_ratio, needle_tip.y + needle_right.y);
    s_needle_vertices[2].pos.set((needle_base.x - needle_right.x) * s_rcp_aspect_ratio, needle_base.y - needle_right.y);
    s_needle_vertices[3].pos.set((needle_base.x + needle_right.x) * s_rcp_aspect_ratio, needle_base.y + needle_right.y);
#else
    s_needle_vertices[0].pos.set(needle_tip.y - needle_right.y, -(needle_tip.x - needle_right.x) * s_rcp_aspect_ratio);
    s_needle_vertices[1].pos.set(needle_tip.y + needle_right.y, -(needle_tip.x + needle_right.x) * s_rcp_aspect_ratio);
    s_needle_vertices[2].pos.set(needle_base.y - needle_right.y, -(needle_base.x - needle_right.x) * s_rcp_aspect_ratio);
    s_needle_vertices[3].pos.set(needle_base.y + needle_right.y, -(needle_base.x + needle_right.x) * s_rcp_aspect_ratio);
#endif

    psc_geometry_cluster<rstate_gauge_needle, vertex_needle, vs_gauge_needle, ps_gauge_needle> needle_pso;
    needle_pso.set_geometry(s_needle_vertices, 8, s_needle_indices, 4);
    s_gfx_device.dispatch_pso(needle_pso);

    tshader_gauge tsh;
    s_gfx_device.commit(tsh);
}

#ifdef __cplusplus
extern "C" {
#endif
void app_entry(void* arg)
{
    s_gfx_device.init(
        &hspi1,
        LCD_CS_GPIO_Port,
        LCD_RES_GPIO_Port,
        LCD_DC_GPIO_Port,
        LCD_BL_GPIO_Port,
        LCD_CS_Pin,
        LCD_RES_Pin,
        LCD_DC_Pin,
        LCD_BL_Pin);

    s_gfx_device_mem.init(s_gfx_device);
    s_gfx_device.clear_depth(cleardepth_max); /*todo: should be able to remove since we don't enable depth testing*/

    s_tex_gauge_bg.init(s_ptx_gauge_bg);
    s_tex_gauge_needle.init(s_ptx_gauge_needle);
    mem_zero(s_needle_vertices, sizeof(s_needle_vertices));

#if ROTATE_90_DEGREES == 0
    s_gfx_device.fast_draw_rect(0, 0, gfx_device_t::fb_width, gfx_device_t::fb_height, ips_texture<smp_gauge_bg_t>(s_tex_gauge_bg, 0, 0, 1.0f / gfx_device_t::fb_width, 1.0f / gfx_device_t::fb_height));
#else
    s_gfx_device.fast_draw_rect(0, 0, gfx_device_t::fb_width, gfx_device_t::fb_height, ips_texture<smp_gauge_bg_t, true>(s_tex_gauge_bg, 0, 0, -1.0f / gfx_device_t::fb_width, 1.0f / gfx_device_t::fb_height));
#endif
    while (1) {
        // 速度表动画
        float t     = 0.3f * HAL_GetTick() / 1000.0f + 9.7f;
        float speed = clamp(0.2f + 0.15f * sin(t) + 0.075f * sin(t * 2.27f), 0.0f, 1.0f);
        update_gauge_needle(speed);
        psc_geometry_cluster<rstate_gauge_needle, vertex_needle, vs_gauge_needle, ps_gauge_needle> needle_pso;
        needle_pso.set_geometry(s_needle_vertices, 8, s_needle_indices, 4);
        s_gfx_device.dispatch_pso(needle_pso);
        tshader_gauge tsh;
        s_gfx_device.commit(tsh);
    }
}

#ifdef __cplusplus
}
#endif