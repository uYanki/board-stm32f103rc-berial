
#include "egl_device_lib.h"
#include "gpio.h"
#include "spi.h"

EGL_USING_NAMESPACE

typedef graphics_device_st7735 gfx_device_t;

static gfx_device_t s_gfx_device;

// 自定义着色器

struct test_shader {
    test_shader(int16_t x_, int16_t y_, uint8_t frame_) : x(x_), y(y_), frame(frame_) {}

    // 像素点着色（内联函数没有函数调用开销）
    template <e_pixel_format dst_fmt>
    EGL_INLINE void exec(pixel<dst_fmt>& res_, uint16_t x_, uint16_t y_) const
    {
        res_.set_rgba8(/*R*/ (uint8_t)(x_ - x) * 2, /*G*/ (uint8_t)(y_ - y) * 2, /*B*/ frame, 0);
    }

    int16_t x, y;   // 坐标
    uint8_t frame;  // 帧号
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

    // 渐变矩形

    uint8_t s_frame = 0;  // 帧索引: [1,255] 色彩渐变, [0] 变换主色调
    enum { rect_width  = 70,
           rect_height = 70 };

    int16_t x = gfx_device_t::fb_width / 2 - rect_width / 2;  // 居中位置
    int16_t y = gfx_device_t::fb_height / 2 - rect_height / 2;

    int16_t x1 = gfx_device_t::fb_width / 2;
    int16_t x2 = gfx_device_t::fb_width / 2 - rect_width;

    while (1) {
        s_gfx_device.draw_rect(x1, y, rect_width, rect_height, test_shader(x1, y, s_frame));
        s_gfx_device.draw_rect(x2, y, rect_width, rect_height, test_shader(x2, y, 255 - s_frame));
        s_frame += 5;
        printf("frame:%d\r\n", s_frame);
    }
}

#ifdef __cplusplus
}
#endif