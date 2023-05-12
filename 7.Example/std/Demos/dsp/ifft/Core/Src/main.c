#include "arm_math.h"
#include "arm_const_structs.h"
#include "board.h"

// !!! 左移1表示×2,右移1表示÷2

#define M_PI         3.1415926f

#define SAMPLES_SIZE 256   // 采样数
#define SAMPLE_RATE  1024  // 采样率

static float32_t f32_i[SAMPLES_SIZE];
static float32_t f32_fft_o[SAMPLES_SIZE << 1];
static float32_t f32_ifft_o[SAMPLES_SIZE << 1];

static float64_t f64_i[SAMPLES_SIZE];
static float64_t f64_fft_o[SAMPLES_SIZE << 1];
static float64_t f64_ifft_o[SAMPLES_SIZE << 1];

void do_rfft_f32(void)
{
    uint16_t i;

    arm_rfft_fast_instance_f32 S;

    arm_rfft_fast_init_f32(&S, SAMPLES_SIZE);  // init

    for (i = 0; i < SAMPLES_SIZE; ++i)
        f32_ifft_o[i] = f32_i[i] = 1 + cos(2 * M_PI * 50 * i / SAMPLE_RATE + M_PI / 3);

    arm_rfft_fast_f32(&S, f32_ifft_o, f32_fft_o, 0);  // fft

#if 0

    // 变换过程中，原输入信号会发被改变
    for (i = 0; i < SAMPLES_SIZE; ++i)
        println("%f, %f", f32_ifft_o[i], f32_fft_o[i]);  // log

#endif

    arm_rfft_fast_f32(&S, f32_fft_o, f32_ifft_o, 1);  // ifft

    for (i = 0; i < SAMPLES_SIZE; ++i)
        println("%f, %f", f32_i[i], f32_ifft_o[i]);  // log
}

void do_rfft_f64(void)
{
    uint16_t i;

    arm_rfft_fast_instance_f64 S;

    arm_rfft_fast_init_f64(&S, SAMPLES_SIZE);  // init

    for (i = 0; i < SAMPLES_SIZE; ++i)
        f64_ifft_o[i] = f64_i[i] = 1 + cos(2 * M_PI * 50 * i / SAMPLE_RATE + M_PI / 3);

    arm_rfft_fast_f64(&S, f64_ifft_o, f64_fft_o, 0);  // fft

    arm_rfft_fast_f64(&S, f64_fft_o, f64_ifft_o, 1);  // ifft

    for (i = 0; i < SAMPLES_SIZE; ++i)
        println("%.11f, %.11f", f64_i[i], f64_ifft_o[i]);  // log
}

int main()
{
    board_init();

    // println("[f32]");
    do_rfft_f32();
    // println("[f64]");
    do_rfft_f64();

    while (1) {}

    return 0;
}
