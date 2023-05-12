#include "stdio.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "board.h"

// !!! 左移1表示×2,右移1表示÷2

#define M_PI         3.1415926f

// 固定点数
#define SAMPLES_SIZE 512  // 需同步更改 arm_cfft_fxx(&arm_cfft_sR_fxx_len)

// 采样率
#define SAMPLE_RATE  512

static float32_t f32_i[SAMPLES_SIZE << 1];  // signal
static float32_t f32_m[SAMPLES_SIZE << 1];  // magnitude 幅度
static float32_t f32_p[SAMPLES_SIZE << 1];  // phase 相位

static float64_t f64_i[SAMPLES_SIZE << 1];
static float64_t f64_m[SAMPLES_SIZE << 1];
static float64_t f64_p[SAMPLES_SIZE << 1];

void do_cfft_f32(void)
{
    /** 输出(频率,相位,幅值)
     *
     * 直流分量:
     *      0   ,  0.000000,   3.000001
     * 交流分量(双边谱):
     *      50  ,  29.999352,  2.000001
     *      100 ,  44.998684,  1.000000
     *      412 ,  -44.998684, 1.000000
     *      462 ,  -29.999353, 2.000001
     *
     */

    uint16_t i;

    float32_t a, b;

    // init

    // 信号 = 直流分量 (1) + 交流分量 (初始相位角为 30° 的 50Hz 正弦波, 初始相位角为 45° 的 100Hz 正弦波), 采样率 512

    for (i = 0; i < SAMPLES_SIZE; i++) {
        // 采样率需大于正弦波的最大频率的两倍!!!
        f32_i[(i << 1) + 0] = 3 + 2 * cos(2 * M_PI * 50 * i / SAMPLE_RATE + M_PI / 6) + cos(2 * M_PI * 100 * i / SAMPLE_RATE + M_PI / 4);  // 实部
        f32_i[(i << 1) + 1] = 0;                                                                                                           // 虚部
    }

    // fft

    arm_cfft_f32(&arm_cfft_sR_f32_len512 /* 点数 */, f32_i, 0 /* ifft */, 1 /* bit reverse */);

    // mag

    arm_cmplx_mag_f32(f32_i, f32_m, SAMPLES_SIZE);

    f32_m[0] /= SAMPLES_SIZE;  // DC
    for (i = 1; i < SAMPLES_SIZE; ++i)
        f32_m[i] /= SAMPLES_SIZE >> 1;  // AC

    // phase

    for (i = 0; i < SAMPLES_SIZE; i++) {
        a = f32_i[(i << 1) + 0];  // 虚部
        b = f32_i[(i << 1) + 1];  // 实部
        if (0.5f > f32_m[0]) {
            // use threshold to filter noise
            f32_p[i] = 0;
        } else {
            // radian -> angle
            f32_p[i] = 180.0f / M_PI * atan2f(b, a);
        }
    }

    // log

    for (i = 0; i < SAMPLES_SIZE; i++) {
        // i -> signal frequency
        if (f32_m[i] > 1e-4) println("%d,%f,%f", i, f32_p[i], f32_m[i]);
    }
}

void do_cfft_f64(void)
{
    uint16_t i;

    float64_t a, b;

    float64_t phase;

    for (i = 0; i < SAMPLES_SIZE; i++) {
        f64_i[(i << 1) + 0] = 3 + 2 * cos(2 * M_PI * 30 * i / SAMPLE_RATE + M_PI / 6) + cos(2 * M_PI * 100 * i / SAMPLE_RATE + M_PI / 4);  // 实部
        f64_i[(i << 1) + 1] = 0;
    }

    arm_cfft_f64(&arm_cfft_sR_f64_len512, f64_i, 0, 1);

    // mag

    for (i = 0; i < SAMPLES_SIZE; i++) {
        a        = f64_i[(i << 1) + 0];
        b        = f64_i[(i << 1) + 1];
        f64_m[i] = sqrt(a * a + b * b);
    }

    f64_m[0] /= SAMPLES_SIZE;
    for (i = 1; i < SAMPLES_SIZE; ++i)
        f64_m[i] /= SAMPLES_SIZE >> 1;

    // phase

    for (i = 0; i < SAMPLES_SIZE; i++) {
        a = f64_i[(i << 1) + 0];
        b = f64_i[(i << 1) + 1];
        if (0.5f > f64_m[0]) {
            f64_p[i] = 0;
        } else {
            f64_p[i] = 180.0f / M_PI * atan2f(b, a);
        }
    }

    // log

    for (i = 0; i < SAMPLES_SIZE; i++) {
        if (f64_m[i] > 1e-3) println("%d,%.11f, %.11f", i, f64_p[i], f64_m[i]);
    }
}

int main()
{
    board_init();

    println("[f32]");
    do_cfft_f32();
    println("[f64]");
    do_cfft_f64();

    while (1) {}

    return 0;
}
