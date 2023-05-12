#include "egl_device_lib.h"
#include "egl_mesh.h"
#include "egl_vertex.h"
#include "egl_texture.h"

#include "gpio.h"
#include "spi.h"

EGL_USING_NAMESPACE

#define ENABLE_ROUGHNESS 0  // RCT6 开启这内存就不够了，RCT6 以上可以开启试试

typedef graphics_device_st7735 gfx_device_t;

static const EGL_ALIGN(4) uint32_t s_p3g_monkey_data[] = {
#include "p3g_monkey.h"
};
static p3g_mesh             s_p3g_monkey;
typedef vertex_p48tbn32uv32 vtx_monkey_t;

static const EGL_ALIGN(4) uint32_t s_ptx_color_metalness[] = {
#include "ptx_color_metalness.h"
};
static texture s_tex_color_metalness;

#if ENABLE_ROUGHNESS
static const EGL_ALIGN(4) uint32_t s_ptx_normal_roughness[] = {
#include "ptx_normal_roughness.h"
};
static texture s_tex_normal_roughness;
#endif

typedef sampler2d<pixfmt_r8g8b8a8, texfilter_linear, texaddr_wrap> smp_r8g8b8a8_linear_wrap_t;
static smp_r8g8b8a8_linear_wrap_t                                  smp_r8g8b8a8_linear_wrap;

struct rasterizer_memory_cfg : rasterizer_memory_cfg_base {
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
    enum { pso_store_size = 2048 };
    enum { vcache_size = 2048 };
    enum { max_dma_transfers = 0 };
    enum { dma_buffer_px = 0 };
    enum { max_ptv_buffer_size = 64 * 18 * 4 };  // 不能调小
};

static gfx_device_t                                           s_gfx_device;
static rasterizer_memory<gfx_device_t, rasterizer_memory_cfg> s_gfx_device_mem;
static cameraf                                                s_camera;

struct vs_static_brdf {
    struct vsout {
        vec3f  opos;
        mat33f tbn;
        vec2f  uv;
    };

    template <class PSIn, class PSOState, class VIn>
    EGL_INLINE void exec(PSIn& psin_, const PSOState& pso_state_, const typename VIn::transform_state& tstate_, const VIn& vin_, uint8_t) const
    {
        vec3f opos = get_pos(vin_, tstate_);
        psin_.pos  = vec4f(opos, 1.0f) * pso_state_.obj_to_proj;
        vsout vo;
        vo.opos = opos;
        vo.tbn  = get_tbn(vin_, tstate_);
        vo.uv   = get_uv(vin_, tstate_);
        psin_.set_attribs(vo);
    }
};

template <typename T>
EGL_INLINE T fresnel_schlick(const T& r0_, typename math<T>::scalar_t n_dot_wi_)
{
    typedef typename math<T>::scalar_t scalar_t;
    return r0_ + (scalar_t(1) - r0_) * pow(scalar_t(1) - n_dot_wi_, scalar_t(5));
}

template <typename T>
EGL_INLINE T rcp_smith_gterm(T n_dot_v_, T alpha2_)
{
    typedef typename math<T>::scalar_t scalar_t;
    return n_dot_v_ + sqrt(alpha2_ + n_dot_v_ * n_dot_v_ * (scalar_t(1) - alpha2_));
}

EGL_INLINE color_rgbf brdf_ggx(const vec3f& wo_, const vec3f& wi_, const vec3f& n_, const color_rgbf& r0_, float alpha2_, float rcp_smith_gwo_, float n_dot_wi_)
{
    float n_dot_h       = sat(dot(n_, unit(wo_ + wi_)));
    float ndf_denom     = n_dot_h * n_dot_h * (alpha2_ - 1.0f) + 1.0f;
    float rcp_smith_gwi = rcp_smith_gterm(n_dot_wi_, alpha2_);
    return fresnel_schlick(r0_, n_dot_wi_) * (n_dot_wi_ * alpha2_ * rcp_z(mathf::pi * ndf_denom * ndf_denom * rcp_smith_gwo_ * rcp_smith_gwi));
}

struct directional_light {
    vec3f      dir;
    color_rgbf illuminance;
};

struct ps_lambert_ggx {
    struct psin {
        vec4f  pos;
        vec3f  opos;
        mat33f tbn;
        vec2f  uv;

        template <class VSOut>
        EGL_INLINE void set_attribs(const VSOut& vo_)
        {
            opos = vo_.opos;
            tbn  = vo_.tbn;
            uv   = vo_.uv * 2.0f;
        }
    };

    template <class PSC>
    EGL_INLINE void exec(rasterizer_pixel_out<PSC>& psout_, const typename PSC::pso_state&, const psin& v0_, const psin& v1_, const psin& v2_, const vec3f& bc_, uint8_t) const
    {
        vec3f  opos = v0_.opos * bc_.x + v1_.opos * bc_.y + v2_.opos * bc_.z;
        mat33f tbn  = v0_.tbn * bc_.x + v1_.tbn * bc_.y + v2_.tbn * bc_.z;
        vec2f  uv   = v0_.uv * bc_.x + v1_.uv * bc_.y + v2_.uv * bc_.z;

        color_rgbaf color_metalness, normal_roughness;
        smp_r8g8b8a8_linear_wrap.sample(color_metalness, s_tex_color_metalness, uv);
#if ENABLE_ROUGHNESS
        smp_r8g8b8a8_linear_wrap.sample(normal_roughness, s_tex_normal_roughness, uv);
#else
        normal_roughness = (0.1f, 0.2f, 0.3f);
#endif
        color_rgbf color     = color_metalness;
        float      metalness = color_metalness.a;
        color_rgbf r0        = lerp(color_rgbf(0.04f, 0.04f, 0.04f), color, metalness);
        color_rgbf albedo    = color * (1.0f - metalness);
        vec3f      normal    = madd(vec3f(normal_roughness.r, normal_roughness.g, normal_roughness.b), 2.0f, -1.0f) * tbn;
        float      roughness = normal_roughness.a;
        float      alpha     = roughness * roughness;
        float      alpha2    = alpha * alpha;

        vec3f      v             = unit(view_opos - opos);
        float      rcp_smith_gwo = rcp_smith_gterm(sat(dot(normal, v)), alpha2);
        color_rgbf res           = 0.0f;
        for (unsigned li = 0; li < num_lights; ++li) {
            float      n_dot_l = sat(dot(lights[li].dir, normal));
            color_rgbf brdf    = albedo * n_dot_l;
            brdf += brdf_ggx(v, lights[li].dir, normal, r0, alpha2, rcp_smith_gwo, n_dot_l);
            res += lights[li].illuminance * brdf;
        }
        psout_.export_rt0(sat(res));
    }

    enum { num_lights = 2 };
    vec3f             view_opos;
    directional_light lights[num_lights];
};

struct rstate_opaque : rasterizer_psc_base {
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
    enum { depth_format = rasterizer_memory_cfg::depth_format };
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

    s_p3g_monkey.init(s_p3g_monkey_data);
    s_tex_color_metalness.init(s_ptx_color_metalness);

#if ENABLE_ROUGHNESS
    s_tex_normal_roughness.init(s_ptx_normal_roughness);
#endif

    float  cam_fov          = 60.0f;
    float  cam_aspect_ratio = float(gfx_device_t::fb_width) / float(gfx_device_t::fb_height);
    float  cam_near_plane   = 0.1f;
    float  cam_far_plane    = 100.0f;
    mat44f view2proj        = perspective_matrix<float>(cam_fov * mathf::deg_to_rad, cam_aspect_ratio, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_proj(view2proj, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_world(tform3f::identity());

    float s_rot_x = -1.7f, s_rot_y = -0.5f;
    while (1) {
        vec3f  obj_pos(0.0f, 0.0f, 3.0f);
        mat33f obj_rot;
        set_rotation_xyz(obj_rot, s_rot_x, s_rot_y, 0.0f);
        s_rot_x += 0.0123f;
        s_rot_y += 0.0321f;

        psc_p3g_mesh<rstate_opaque, vtx_monkey_t, vs_static_brdf, ps_lambert_ggx> pso;
        pso.set_geometry(s_p3g_monkey, 0);
        tform3f object_to_world(obj_rot, obj_pos);
        pso.set_transform(s_camera, object_to_world);
        tform3f world_to_object = inv(object_to_world);

        ps_lambert_ggx& ps = pso.pshader();
        ps.view_opos       = s_camera.world_pos() * world_to_object;
        ps.lights[0].dir   = unit(obj_rot * vec3f(-1.0f, -1.0f, -1.0f));
        ps.lights[0].illuminance.set(10.0f, 5.0f, 2.0f);
        ps.lights[1].dir = obj_rot * vec3f(1.0f, 1.0f, -2.0f);
        ps.lights[1].illuminance.set(2.0f, 2.0f, 2.0f);
        s_gfx_device.dispatch_pso(pso);

        s_gfx_device.commit();
    }
}

#ifdef __cplusplus
}
#endif