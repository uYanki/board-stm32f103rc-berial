
#include "gpio.h"
#include "spi.h"
#include "egl_device_lib.h"
#include "egl_mesh.h"
#include "egl_vertex.h"
#include "egl_rasterizer.h"
EGL_USING_NAMESPACE
typedef graphics_device_st7735 gfx_device_t;
static gfx_device_t            s_gfx_device;
static const EGL_ALIGN(4) uint32_t s_p3g_monkey_data[] = {
#include "p3g_monkey.h"
    // #include "p3g_cube.h"
    // #include "p3g_torus.h"
};
static p3g_mesh       s_p3g_monkey;
typedef vertex_p48n32 vtx_monkey_t;
struct rasterizer_memory_cfg : rasterizer_memory_cfg_base {
    enum { depth_format = depthfmt_uint16 };  // 深度缓冲区格式
    enum { depth_hiz = true };
    enum { rt0_fmt = gfx_device_t::fb_format };
    enum { max_dispatches = 1 };
    enum { pso_store_size = 128 };
    enum { max_pso_state_size = 128 };  // 粒子群状态
    enum { tile_width  = 64,
           tile_height = 64 };
    enum { tile_order = tileorder_morton };  // 平铺渲染顺序
    enum { max_clusters = 256 };
    enum { max_cluster_strips = 256 };
    enum { max_vout_size = 64 * 20 };
    enum { vcache_size = 0 };  // 顶点缓存
    enum { max_dma_transfers = 0 };
};
static rasterizer_memory<gfx_device_t, rasterizer_memory_cfg> s_gfx_device_mem;
static cameraf                                                s_camera;
static vec3f                                                  s_light_dir;

// 顶点着色器
struct vs_simple_ndotl {
    struct vsout {
        float ndotl;
    };
    template <class PSIn, class PSOState, class VIn>
    EGL_INLINE void exec(PSIn& psin_, const PSOState& pso_state_, const typename VIn::transform_state& tstate_, const VIn& vin_, uint8_t) const
    {
        vec3f pos    = get_pos(vin_, tstate_);
        psin_.pos    = vec4f(pos, 1.0f) * pso_state_.obj_to_proj;
        vec3f normal = get_normal(vin_, tstate_);
        vsout vo;
        vo.ndotl = dot(normal, light_obj_dir);
        psin_.set_attribs(vo);
    }
    vec3f light_obj_dir;
};

// 光栅化器
struct ps_simple_ndotl {
    struct psin {
        vec4f pos;
        float ndotl;
        template <class VSOut>
        EGL_INLINE void set_attribs(const VSOut& vo_)
        {
            ndotl = vo_.ndotl;
        }
    };
    template <class PSC>
    EGL_INLINE void exec(rasterizer_pixel_out<PSC>& psout_, const typename PSC::pso_state&, const psin& v0_, const psin& v1_, const psin& v2_, const vec3f& bc_, uint8_t) const
    {
        float ndotl = v0_.ndotl * bc_.x + v1_.ndotl * bc_.y + v2_.ndotl * bc_.z;
        psout_.export_rt0(color * abs(ndotl));  // abs-> 双面照明
    }
    color_rgbaf color;
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

    // 初始化3d模型的网格
    s_p3g_monkey.init(s_p3g_monkey_data);

    float  cam_fov          = 60.0f;  // 视角
    float  cam_aspect_ratio = float(gfx_device_t::fb_width) / float(gfx_device_t::fb_height);
    float  cam_near_plane   = 0.1f;
    float  cam_far_plane    = 100.0f;
    mat44f view2proj        = perspective_matrix<float>(cam_fov * mathf::deg_to_rad, cam_aspect_ratio, cam_near_plane, cam_far_plane);
    s_camera.set_view_to_proj(view2proj, cam_near_plane, cam_far_plane);

    // 摄像机位置和旋转矩阵
    // +x=右,+y=上,+z=前,-x=左,-y=下,-z=后
    vec3f  cam_pos(0.0f, 0.0f, -1.5f);
    mat33f cam_rot(1.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 1.0f);

    // 灯光方向和长度
    s_camera.set_view_to_world(tform3f(cam_rot, cam_pos));
    s_light_dir = unit(vec3f(1.0f, 1.0f, -1.0f));

    while (1) {
        // 物体位置和旋转矩阵
        static float s_rot_x = -1.2f, s_rot_y = -0.5f;  // 旋转角度（弧度制）
        vec3f        obj_pos(0.0f, 0.0f, 3.0f);
        mat33f       obj_rot;  // 旋转矩阵
        set_rotation_xyz(obj_rot, s_rot_x, s_rot_y, 0.0f);
        s_rot_x += 0.0123f;
        s_rot_y += 0.0321f;

        // 输入顶点格式,顶点像素着色器
        psc_p3g_mesh<rstate_opaque, vtx_monkey_t, vs_simple_ndotl, ps_simple_ndotl> pso;

        // 位置变换
        pso.set_transform(s_camera, tform3f(obj_rot, obj_pos));

        pso.vshader().light_obj_dir = unit(obj_rot * s_light_dir);

        // 设置物体颜色
        pso.pshader().color.set(0.5f, 1.0f, 1.0f);  // 浅清

        pso.set_geometry(s_p3g_monkey, 0);

        s_gfx_device.dispatch_pso(pso);

        s_gfx_device.commit();
        static unsigned s_log_counter = 0;
        if (s_log_counter == 0) {
            s_gfx_device.log_rasterizer_stats();
            s_log_counter = 100;
        }
    }
}
#ifdef __cplusplus
}
#endif