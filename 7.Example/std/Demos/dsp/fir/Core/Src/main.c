#include "arm_math.h"
#include "arm_const_structs.h"
#include "board.h"

#define M_PI         3.1415926f

#define SAMPLES_SIZE 1024  // 采样点数
#define BLOCK_SIZE   1024  // arm_fir_f32 单次处理点数
#define SAMPLE_RATE  1000

// 滤波器系数(src from fadtool)

// N 阶 fir filter 会造成延时, 延时时间 t=(N-2)/(2*fs)

const float32_t fir_coffes_lp[] = {
    -0.001822523074f, -0.001587929321f, 1.226008847e-18f, 0.003697750857f, 0.008075430058f,
    0.008530221879f, -4.273456581e-18f, -0.01739769801f, -0.03414586186f, -0.03335915506f,
    8.073562366e-18f, 0.06763084233f, 0.1522061825f, 0.2229246944f, 0.2504960895f,
    0.2229246944f, 0.1522061825f, 0.06763084233f, 8.073562366e-18f, -0.03335915506f,
    -0.03414586186f, -0.01739769801f, -4.273456581e-18f, 0.008530221879f, 0.008075430058f,
    0.003697750857f, 1.226008847e-18f, -0.001587929321f, -0.001822523074f};

const float32_t fir_coffes_hp[] = {
    0.0018157335f, 0.001582013792f, -6.107207639e-18f, -0.003683975432f, -0.008045346476f,
    -0.008498443291f, -1.277260999e-17f, 0.01733288541f, 0.03401865438f, 0.0332348831f,
    -4.021742543e-17f, -0.06737889349f, -0.1516391635f, -0.2220942229f, 0.7486887574f,
    -0.2220942229f, -0.1516391635f, -0.06737889349f, -4.021742543e-17f, 0.0332348831f,
    0.03401865438f, 0.01733288541f, -1.277260999e-17f, -0.008498443291f, -0.008045346476f,
    -0.003683975432f, -6.107207639e-18f, 0.001582013792f, 0.0018157335f};

const float32_t fir_coffes_bp[] = {
    0.003531039227f, 0.0002660876198f, -0.001947779674f, 0.001266813371f, -0.008019094355f,
    -0.01986379735f, 0.01018396299f, 0.03163734451f, 0.00165955862f, 0.03312643617f,
    0.0622616075f, -0.1229852438f, -0.2399847955f, 0.07637182623f, 0.3482480049f,
    0.07637182623f, -0.2399847955f, -0.1229852438f, 0.0622616075f, 0.03312643617f,
    0.00165955862f, 0.03163734451f, 0.01018396299f, -0.01986379735f, -0.008019094355f,
    0.001266813371f, -0.001947779674f, 0.0002660876198f, 0.003531039227f};

const float32_t fir_coffes_bs[] = {
    -0.003560454352f, -0.0002683042258f, 0.001964005642f, -0.001277366537f, 0.008085897192f,
    0.02002927102f, -0.01026879996f, -0.03190089762f, -0.001673383522f, -0.0334023945f,
    -0.06278027594f, 0.1240097657f, 0.2419839799f, -0.07700803876f, 0.6521340013f,
    -0.07700803876f, 0.2419839799f, 0.1240097657f, -0.06278027594f, -0.0334023945f,
    -0.001673383522f, -0.03190089762f, -0.01026879996f, 0.02002927102f, 0.008085897192f,
    -0.001277366537f, 0.001964005642f, -0.0002683042258f, -0.003560454352f};

static float32_t signal[2][SAMPLES_SIZE];
static float32_t input[SAMPLES_SIZE];

static float32_t output_lp[SAMPLES_SIZE];  // lowpass
static float32_t output_hp[SAMPLES_SIZE];  // highpass
static float32_t output_bp[SAMPLES_SIZE];  // bandpass
static float32_t output_bs[SAMPLES_SIZE];  // bandstop

static float32_t state_fir_lp[BLOCK_SIZE + ARRAY_SIZE(fir_coffes_lp) - 1];
static float32_t state_fir_hp[BLOCK_SIZE + ARRAY_SIZE(fir_coffes_hp) - 1];
static float32_t state_fir_bp[BLOCK_SIZE + ARRAY_SIZE(fir_coffes_bp) - 1];
static float32_t state_fir_bs[BLOCK_SIZE + ARRAY_SIZE(fir_coffes_bs) - 1];

int main()
{
    uint32_t i;

    arm_fir_instance_f32 S1, S2, S3, S4;

    board_init();

    /* generate signal */

    // signal = 50Hz sin + 200Hz sin, sample rate = 1KHz
    for (i = 0; i < SAMPLES_SIZE; ++i) {
        signal[0][i] = arm_sin_f32(2 * M_PI * 50 * i / SAMPLE_RATE);
        signal[1][i] = arm_sin_f32(2 * M_PI * 200 * i / SAMPLE_RATE);
        input[i]     = signal[0][i] + signal[1][i];
    }

    /* do fir filter */

    arm_fir_init_f32(&S1, ARRAY_SIZE(fir_coffes_lp), fir_coffes_lp, state_fir_lp, BLOCK_SIZE);
    arm_fir_init_f32(&S2, ARRAY_SIZE(fir_coffes_hp), fir_coffes_hp, state_fir_hp, BLOCK_SIZE);
    arm_fir_init_f32(&S3, ARRAY_SIZE(fir_coffes_bp), fir_coffes_bp, state_fir_bp, BLOCK_SIZE);
    arm_fir_init_f32(&S4, ARRAY_SIZE(fir_coffes_bs), fir_coffes_bs, state_fir_bs, BLOCK_SIZE);

    for (i = 0; i < SAMPLES_SIZE; i += BLOCK_SIZE) {
        arm_fir_f32(&S1, input + i, output_lp + i, BLOCK_SIZE);
        arm_fir_f32(&S2, input + i, output_hp + i, BLOCK_SIZE);
        arm_fir_f32(&S1, input + i, output_bp + i, BLOCK_SIZE);
        arm_fir_f32(&S2, input + i, output_bs + i, BLOCK_SIZE);
    }

    /* print input & output */

    for (i = 0; i < SAMPLES_SIZE; ++i) println("%f,%f,%f,%f,%f,%f,%f", signal[0][i], signal[1][i], input[i], output_lp[i], output_hp[i], output_bp[i], output_bs[i]);
    // signal[0] -> output_lp & output_bp
    // signal[1] -> output_hp & output_bs

    while (1) {}

    return 0;
}
