
#include "egl_device_lib.h"
#include "egl_mesh.h"
#include "egl_vertex.h"
#include "egl_texture.h"

#include "gpio.h"
#include "spi.h"

EGL_USING_NAMESPACE

typedef graphics_device_st7735 gfx_device_t;

static gfx_device_t s_gfx_device;

static const EGL_ALIGN(4) uint32_t s_p3g_cube_data[] = {
#include "p3g_cube.h"
};
static p3g_mesh           s_p3g_cube;
typedef vertex_p48n32uv32 vtx_cube_t;

static const EGL_ALIGN(4) uint32_t s_ptx_smiley_data[] = {
#include "ptx_smiley.h"
};
static texture                                                   s_tex_smiley;
typedef sampler2d<pixfmt_r5g6b5, texfilter_linear, texaddr_wrap> smp_r5g6b5_linear_wrap_t;

struct rasterizer_memory_cfg : rasterizer_memory_cfg_base {
    enum { rt0_fmt = gfx_device_t::fb_format };
    enum { pso_store_size = 4096 };
    enum { vcache_size = 4096 };
    enum { max_dma_transfers = 0 };
    enum { dma_buffer_px = 0 };
};

static rasterizer_memory<gfx_device_t, rasterizer_memory_cfg> s_gfx_device_mem;
static cameraf                                                s_camera;

struct vs_uv {
    struct vsout {
        vec2f uv;
    };
    template <class PSIn, class PSOState, class VIn>
    EGL_INLINE void exec(PSIn& psin_, const PSOState& pso_state_, const typename VIn::transform_state& tstate_, const VIn& vin_, uint8_t) const
    {
        psin_.pos = vec4f(get_pos(vin_, tstate_), 1.0f) * pso_state_.obj_to_proj;
        vsout vo;
        vo.uv = get_uv(vin_, tstate_);
        psin_.set_attribs(vo);
    }
};

template <class Sampler>
struct ps_tex {
    struct psin {
        vec4f pos;
        vec2f uv;
        template <class VSOut>
        EGL_INLINE void set_attribs(const VSOut& vo_)
        {
            uv = vo_.uv;
        }
    };

    template <class PSC>
    EGL_INLINE void exec(rasterizer_pixel_out<PSC>& psout_, const typename PSC::pso_state&, const psin& v0_, const psin& v1_, const psin& v2_, const vec3f& bc_, uint8_t) const
    {
        vec2f uv = v0_.uv * bc_.x + v1_.uv * bc_.y + v2_.uv * bc_.z;

        color_rgbaf res;
        sampler.sample(res, *tex, uv);
        psout_.export_rt0(res);
    }

    const texture* tex;
    Sampler        sampler;
};

struct rstate_opaque : rasterizer_psc_base {
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
    enum { depth_format = rasterizer_memory_cfg::depth_format };
    enum { bci_mode = rstate_bcimode_perspective };
};

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
    s_gfx_device.clear_depth(cleardepth_max);

    s_p3g_cube.init(s_p3g_cube_data);
    s_tex_smiley.init(s_ptx_smiley_data);

    float  cam_fov          = 60.0f;
    float  cam_aspect_ratio = float(gfx_device_t::fb_width) / float(gfx_device_t::fb_height);
    float  cam_near_plane   = 0.1f;
    float  cam_far_plane    = 100.0f;
    mat44f view2proj        = perspective_matrix<float>(cam_fov * mathf::deg_to_rad, cam_aspect_ratio, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_proj(view2proj, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_world(tform3f::identity());

    while (1) {
        static float s_rot_x = -0.8f, s_rot_y = -0.7f;
        vec3f        obj_pos(0.0f, 0.0f, 8.0f);
        mat33f       obj_rot;
        set_rotation_xyz(obj_rot, s_rot_x, s_rot_y, 0.0f);
        s_rot_x += 0.0123f;
        s_rot_y += 0.0321f;

        psc_p3g_mesh<rstate_opaque, vtx_cube_t, vs_uv, ps_tex<smp_r5g6b5_linear_wrap_t>> pso;
        pso.set_geometry(s_p3g_cube, 0);
        pso.set_transform(s_camera, tform3f(obj_rot, obj_pos));
        pso.pshader().tex = &s_tex_smiley;
        s_gfx_device.dispatch_pso(pso);

        s_gfx_device.commit();
    }
}

#ifdef __cplusplus
}
#endif