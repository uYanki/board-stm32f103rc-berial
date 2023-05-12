#include "arm_math.h"
#include "arm_const_structs.h"
#include "board.h"

// !!! 左移1表示×2,右移1表示÷2

#define M_PI         3.1415926f

// 固定点数
#define SAMPLES_SIZE 128

// 采样率
#define SAMPLE_RATE  256

static float32_t f32_i[SAMPLES_SIZE << 1];       // input 输入
static float32_t f32_o[SAMPLES_SIZE << 1];  // output 输出
static float32_t f32_p[SAMPLES_SIZE];       // phase 相位
static float32_t f32_m[SAMPLES_SIZE];       // magnitude 幅度

static float64_t f64_i[SAMPLES_SIZE << 1];
static float64_t f64_o[SAMPLES_SIZE << 1];
static float64_t f64_p[SAMPLES_SIZE];
static float64_t f64_m[SAMPLES_SIZE];

void do_rfft_f32(void)
{
    uint16_t i;

    float32_t a, b;

    arm_rfft_fast_instance_f32 S;

    // init

    for (i = 0; i < SAMPLES_SIZE; i++) {
        // 采样率需大于两倍的正弦波的最大频率
        f32_i[i] = 3 + 2 * cos(2 * M_PI * 50 * i / SAMPLE_RATE + M_PI / 6) +
                   cos(2 * M_PI * 120 * i / SAMPLE_RATE + M_PI / 4);
    }

    arm_rfft_fast_init_f32(&S, SAMPLES_SIZE);

    // fft

    arm_rfft_fast_f32(&S, f32_i, f32_o, 0);

    // mag

    arm_cmplx_mag_f32(f32_o, f32_m, SAMPLES_SIZE);
    f32_m[0] /= SAMPLES_SIZE;  // DC
    for (i = 1; i < SAMPLES_SIZE; ++i)
        f32_m[i] /= SAMPLES_SIZE >> 1;  // AC

    // phase

    for (i = 0; i < SAMPLES_SIZE; i++) {
        if (0.5 > f32_m[i]) {
            // use threshold to filter noise
            f32_p[i] = 0;
        } else {
            a        = f32_o[(i << 1) + 0];
            b        = f32_o[(i << 1) + 1];
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

void do_rfft_f64(void)
{
    uint16_t i;

    float64_t a, b;

    arm_rfft_fast_instance_f64 S;

    // init

    arm_rfft_fast_init_f64(&S, SAMPLES_SIZE);

    for (i = 0; i < SAMPLES_SIZE; i++) {
        f64_i[i] = 3 + 2 * cos(2 * M_PI * 50 * i / SAMPLE_RATE + M_PI / 6) +
                   cos(2 * M_PI * 120 * i / SAMPLE_RATE + M_PI / 4);
    }

    // fft

    arm_rfft_fast_f64(&S, f64_i, f64_o, 0);

    // mag
    for (i = 0; i < SAMPLES_SIZE; i++) {
        a        = f64_o[(i << 1) + 0];
        b        = f64_o[(i << 1) + 1];
        f64_m[i] = sqrt(a * a + b * b);
    }

    f64_m[0] /= SAMPLES_SIZE;  // DC
    for (i = 1; i < SAMPLES_SIZE; ++i)
        f64_m[i] /= SAMPLES_SIZE >> 1;  // AC

    // phase

    for (i = 0; i < SAMPLES_SIZE; i++) {
        if (0.5 > f64_m[i]) {
            // use threshold to filter noise
            f64_p[i] = 0;
        } else {
            // radian -> angle
            a        = f64_o[(i << 1) + 0];
            b        = f64_o[(i << 1) + 1];
            f64_p[i] = 180.0f / M_PI * atan2(b, a);
        }
    }

    // log

    for (i = 0; i < SAMPLES_SIZE; i++) {
        // i -> signal frequency
        if (f64_m[i] > 1e-4) println("%d,%.11f,%.11f", i, f64_p[i], f64_m[i]);
    }
}

int main()
{
    board_init();

    println("[f32]");
    do_rfft_f32();
    println("[f64]");
    do_rfft_f64();

    /** output
     * [f32]
     * 0,0.000002,3.000000
     * 25,29.999628,1.999999
     * 60,44.999249,1.000000
     * [f64]
     * 0,0.00000371889,2.99999961656
     * 25,29.99963011427,1.99999899398
     * 60,44.99925088558,1.00000013508
     */

    while (1) {}

    return 0;
}
