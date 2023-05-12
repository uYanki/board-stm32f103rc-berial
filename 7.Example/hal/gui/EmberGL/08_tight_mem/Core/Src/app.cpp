#include "egl_device_lib.h"
#include "egl_mesh.h"
#include "egl_vertex.h"
#include "egl_shaders.h"

#include "gpio.h"
#include "spi.h"

EGL_USING_NAMESPACE

typedef graphics_device_st7735 gfx_device_t;

// 3D geometry
static const EGL_ALIGN(4) uint32_t s_p3g_torus_data[] = {
#include "p3g_torus.h"
};
static p3g_mesh       s_p3g_torus;
typedef vertex_p48n32 vtx_torus_t;

// rasterizer_memory_cfg
struct rasterizer_memory_cfg : rasterizer_memory_cfg_base {
    // For this example we'll use only 8bpp depth buffer format to save memory.
    // Later we'll tune the camera near- and far-planes so that they are fairly
    // close to the object so that the 8-bit precision is enough.
    enum { depth_format = depthfmt_uint8 };
    // We'll disable Hi-Z to save memory
    enum { depth_hiz = false };
    // We'll use only 8bpp for the intermediate color to save memory. Because we
    // are rendering a monochromatic image, single channel format is enough. This
    // format is using red-channel, and we'll later use tile shader for custom
    // conversion of the red image to grayscale.
    enum { rt0_fmt = pixfmt_r8 };
    // To optimize the tile memory usage we'll use 32x32px tile size (default is
    // 64x64px). However, decreasing the tile size increases the number of tiles
    // which adds memory usage, and 32x32px seems to be a good balance for this
    // example
    enum { tile_width  = 32,
           tile_height = 32 };
    // For rendering order of tiles we'll just use the linear order to save a bit
    // of memory.
    enum { tile_order = tileorder_linear };
    // We have only single object so we allocate memory only for single dispatch.
    enum { max_dispatches = 1 };
    // The PSO we use consumes 216 bytes, so we allocate exactly that
    enum { pso_store_size = 216 };
    // There are 8 clusters in the object we render so we allocate memory for it.
    enum { max_clusters = 8 };
    // It seems that 12 cluster strips is enough when rendering the object at
    // the specified distance. If we would move the object closer, it would
    // cover more tiles and require more cluster strips to be allocated.
    enum { max_cluster_strips = 12 };
    // Stats report there is max 8 clusters per tile so we set the cluster strip
    // to hold max 8 clusters. If there are more clusters per tile, the rasterizer
    // allocates more strips and might require increasing max_cluster_strips.
    enum { num_strip_clusters = 8 };
    // We used only 32 vertices per cluster (default = 64) for this object to
    // reduce PTV buffer size. Each vertex consumes 20 bytes (16 bytes for NDC
    // position and 4 bytes for n.l result)
    enum { max_ptv_buffer_size = 32 * 20 };
    // We disable vertex cache
    enum { vcache_size = 0 };
    // And also disable DMA transfers
    enum { max_dma_transfers = 0 };
};

// setup
static gfx_device_t                                           s_gfx_device;
static rasterizer_memory<gfx_device_t, rasterizer_memory_cfg> s_gfx_device_mem;

struct rstate_opaque : rasterizer_psc_base {
    enum { depth_format = rasterizer_memory_cfg::depth_format };
    enum { rt0_fmt = rasterizer_memory_cfg::rt0_fmt };
    // Perspective correct interpolation has very little impact on quality in this
    // example so we'll disable it to optimize performance.
    enum { bci_mode = rstate_bcimode_noperspective };
};

struct vs_simple_ndotl {
    template <class PSIn, class PSOState, class VIn>
    EGL_INLINE void exec(PSIn& psin_, const PSOState& pso_state_, const typename VIn::transform_state& tstate_, const VIn& vin_, uint8_t) const
    {
        // Perform regular vertex position object->projection space transform and
        // simple n.l lighting with fixed light source direction in object space.
        vec3f pos    = get_pos(vin_, tstate_);
        psin_.pos    = vec4f(pos, 1.0f) * pso_state_.obj_to_proj;
        vec3f normal = get_normal(vin_, tstate_);
        psin_.ndotl  = dot(normal, vec3f(0.577f, 0.577f, -0.577f));
    }
};

struct ps_simple_ndotl {
    struct psin {
        vec4f pos;
        float ndotl;
    };

    template <class PSC>
    EGL_INLINE void exec(rasterizer_pixel_out<PSC>& psout_, const typename PSC::pso_state&, const psin& v0_, const psin& v1_, const psin& v2_, const vec3f& bc_, uint8_t) const
    {
        // Perform simple double sided n.l lighting
        float ndotl = v0_.ndotl * bc_.x + v1_.ndotl * bc_.y + v2_.ndotl * bc_.z;
        psout_.export_rt0(abs(ndotl));
    }
};

struct ts_red_as_grayscale {
    template <e_pixel_format dst_fmt>
    void exec(pixel<dst_fmt>& dst_, const void* const* rts_, const rasterizer_depth_target&, size_t src_px_offs_, uint16_t, uint16_t) const
    {
        // Convert a pixel in the intermediate red-channel 8bpp pixel format to
        // grayscale pixel. We simply copy the 8-bit value to all RGB channels.
        uint8_t v = ((uint8_t*)rts_[0])[src_px_offs_];
        dst_.set_rgba(v, v, v, 0);
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
    s_p3g_torus.init(s_p3g_torus_data);

    cameraf     cam;
    const float cam_fov          = 60.0f;
    const float cam_aspect_ratio = float(gfx_device_t::fb_width) / float(gfx_device_t::fb_height);
    const float cam_near_plane   = 5.0f;
    const float cam_far_plane    = 20.0f;
    mat44f      view2proj        = perspective_matrix<float>(cam_fov * mathf::deg_to_rad, cam_aspect_ratio, cam_near_plane, cam_far_plane);
    cam.set_view_to_proj(view2proj, cam_near_plane, cam_far_plane);
    cam.set_view_to_world(tform3f::identity());
    while (1) {
        // Setup rotation
        static float s_rot_x = -1.2f, s_rot_y = -0.5f;
        vec3f        obj_pos(0.0f, 0.0f, 12.0f);
        mat33f       obj_rot;
        set_rotation_xyz(obj_rot, s_rot_x, s_rot_y, 0.0f);
        s_rot_x += 0.0246f;
        s_rot_y += 0.0642f;

        // Render the object
        psc_p3g_mesh<rstate_opaque, vtx_torus_t, vs_simple_ndotl, ps_simple_ndotl> pso;
        pso.set_transform(cam, tform3f(obj_rot, obj_pos));
        pso.set_geometry(s_p3g_torus, 0);
        s_gfx_device.dispatch_pso(pso);

        ts_red_as_grayscale ts;
        s_gfx_device.commit(ts);
    }
}

#ifdef __cplusplus
}
#endif