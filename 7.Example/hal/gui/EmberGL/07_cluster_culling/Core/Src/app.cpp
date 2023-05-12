
#include "egl_device_lib.h"
#include "egl_mesh.h"
#include "egl_vertex.h"
#include "egl_shaders.h"

#include "main.h"
#include "spi.h"
#include "gpio.h"

EGL_USING_NAMESPACE

typedef graphics_device_st7735 gfx_device_t;

// 3D geometry
static const EGL_ALIGN(4) uint32_t s_p3g_cup_data[] = {
#include "p3g_cup.h"
};
static p3g_mesh       s_p3g_cup;
typedef vertex_p48n32 vtx_cup_t;

static const EGL_ALIGN(4) uint32_t s_p3g_dragon_data[] = {
#include "p3g_dragon.h"
};
static p3g_mesh       s_p3g_dragon;
typedef vertex_p48n32 vtx_dragon_t;

// rasterizer_memory_cfg
struct rasterizer_memory_cfg : rasterizer_memory_cfg_base {
    enum { rt0_fmt = gfx_device_t::fb_format };
    enum { pso_store_size = 2048 };
    enum { vcache_size = 2048 };
    enum { max_dma_transfers = 0 };
    enum { dma_buffer_px = 0 };
};

// setup
static gfx_device_t                                           s_gfx_device;
static rasterizer_memory<gfx_device_t, rasterizer_memory_cfg> s_gfx_device_mem;
static cameraf                                                s_camera;

//

struct rstate_cluster_overdraw : rasterizer_psc_base {
    enum { depth_format = rasterizer_memory_cfg::depth_format };
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
    enum { tbfc_mode = rstate_tbfc_none };
    enum { debug_disable_pixel_depth_test = true };
    EGL_RT_BLENDFUNC(0, blendfunc_add);
};

struct rstate_opaque : rasterizer_psc_base {
    enum { depth_format = rasterizer_memory_cfg::depth_format };
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
};

struct ps_simple_lighting {
    struct psin {
        vec4f pos;
        vec3f normal;
        template <class VSOut>
        EGL_INLINE void set_attribs(const VSOut& vo_) { normal = vo_.normal; }
    };

    template <class PSC>
    EGL_INLINE void exec(rasterizer_pixel_out<PSC>& psout_, const typename PSC::pso_state&, const psin& v0_, const psin& v1_, const psin& v2_, const vec3f& bc_, uint8_t) const
    {
        // Perform simple double sided n.l lighting with fixed light direction
        vec3f normal = v0_.normal * bc_.x + v1_.normal * bc_.y + v2_.normal * bc_.z;
        psout_.export_rt0(abs(dot(normal, unit(vec3f(1.0f, 1.0f, -1.0f)))));
    }
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

    // Init the assets
    s_p3g_cup.init(s_p3g_cup_data);
    s_p3g_dragon.init(s_p3g_dragon_data);

    // Setup camera projection.
    float  cam_fov          = 60.0f;
    float  cam_aspect_ratio = float(gfx_device_t::fb_width) / float(gfx_device_t::fb_height);
    float  cam_near_plane   = 0.1f;
    float  cam_far_plane    = 100.0f;
    mat44f view2proj        = perspective_matrix<float>(cam_fov * mathf::deg_to_rad, cam_aspect_ratio, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_proj(view2proj, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_world(tform3f::identity());

    while (1) {
        float time = HAL_GetTick() / 1000.0f;

        // transforms
        mat33f cup_rot;
        set_rotation_xyz(cup_rot, -mathf::pi * 0.5f, -time * 0.5f, 0.0f);
        vec3f  cup_pos = vec3f(0.0f, -7.0f, 25.0f);
        mat33f dragon_rot;
        set_rotation_xyz(dragon_rot, time * 0.123f, time * 0.321f, 0.0f);
        vec3f dragon_pos = vec3f(0.0f, sin(time) * 4.0f + 2.0f, 25.0f);

        bool use_debug_rendering = time > 5.0f;
        if (use_debug_rendering) {
            // Render the cup
            psc_p3g_mesh<rstate_cluster_overdraw, vtx_cup_t, vs_static, ps_color<pixfmt_r32g32b32a32f>> pso;
            pso.pshader().color = color_rgbaf(0.1f, 0.3f, 0.1f);
            pso.set_geometry(s_p3g_cup, 0);
            pso.set_transform(s_camera, tform3f(cup_rot, cup_pos));
            s_gfx_device.dispatch_pso(pso);

            pso.pshader().color = color_rgbaf(0.3f, 0.1f, 0.1f);
            pso.set_geometry(s_p3g_dragon, 0);
            pso.set_transform(s_camera, tform3f(dragon_rot, dragon_pos));
            s_gfx_device.dispatch_pso(pso);
        } else {
            // Render cup
            psc_p3g_mesh<rstate_opaque, vtx_cup_t, vs_static, ps_simple_lighting> pso;
            pso.set_geometry(s_p3g_cup, 0);
            pso.set_transform(s_camera, tform3f(cup_rot, cup_pos));
            s_gfx_device.dispatch_pso(pso);

            // Render dragon
            pso.set_geometry(s_p3g_dragon, 0);
            pso.set_transform(s_camera, tform3f(dragon_rot, dragon_pos));
            s_gfx_device.dispatch_pso(pso);
        }

        // Commit the dispatches to the device.
        s_gfx_device.commit();
    }
}

#ifdef __cplusplus
}
#endif