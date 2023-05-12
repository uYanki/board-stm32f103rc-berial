#include "arm_math.h"
#include "arm_const_structs.h"
#include "board.h"

#define M_PI         3.1415926f

#define SAMPLES_SIZE 1024  // 采样点数
#define BLOCK_SIZE   1024  // 每次处理点数
#define SAMPLE_RATE  1000

#define IIR_STAGE    2  // 滤波器个数

// 滤波器系数(src from fadtool)

// 巴特沃斯低通 80Hz (2个4阶级联->8阶)
const float32_t iir_coffes_lp[5 * IIR_STAGE] = {
    1.0f, 2.0f, 1.0f, 1.479798894397216679763573665695730596781f, -0.688676953053861784503908438637154176831f,
    1.0f, 2.0f, 1.0f, 1.212812092620218384908525877108331769705f, -0.384004162286553540894828984164632856846f};

// 巴特沃斯高通 140Hz
const float32_t iir_coffes_hp[5 * IIR_STAGE] = {
    1.0f, -2.0f, 1.0f, 0.98454301474115180070612041163258254528f, -0.544565360850816415627662081533344462514f,
    1.0f, -2.0f, 1.0f, 0.744714477864321211519893495278665795922f, -0.168318873843973093595849377379636280239f};

// 巴特沃斯带通 140Hz 400Hz
const float32_t iir_coffes_bp[5 * IIR_STAGE] = {
    1.0f, 0.0f, -1.0f, -1.127651872054164616798743736580945551395f, -0.470013145087532668853214090631809085608f,
    1.0f, 0.0f, -1.0f, 0.774953058046049081397654845204669982195f, -0.367077500556684199750634434167295694351f};

// 巴特沃斯带阻 100Hz 325Hz
const float32_t iir_coffes_bs[5 * IIR_STAGE] = {
    1.0f, -0.614001926383350049576392848393879830837f, 1.0f, 1.145142787949775309286337687808554619551f, -0.502980071467214684410862446384271606803f,
    1.0f, -0.614001926383350049576392848393879830837f, 1.0f, -0.474587046588418992598690238082781434059f, -0.35305199748708809837083322236139792949f};

static float32_t signal[2][SAMPLES_SIZE];
static float32_t input[SAMPLES_SIZE];

static float32_t output_lp[SAMPLES_SIZE];  // lowpass
static float32_t output_hp[SAMPLES_SIZE];  // highpass
static float32_t output_bp[SAMPLES_SIZE];  // bandpass
static float32_t output_bs[SAMPLES_SIZE];  // bandstop

// 状态缓存
static float32_t state_iir_lp[BLOCK_SIZE + ARRAY_SIZE(iir_coffes_lp) - 1];
static float32_t state_iir_hp[BLOCK_SIZE + ARRAY_SIZE(iir_coffes_hp) - 1];
static float32_t state_iir_bp[BLOCK_SIZE + ARRAY_SIZE(iir_coffes_bp) - 1];
static float32_t state_iir_bs[BLOCK_SIZE + ARRAY_SIZE(iir_coffes_bs) - 1];

// 放缩系数
static float32_t scale_iir_lp = 0.052219514664161220673932461977528873831f * 0.042798017416583809813257488485760404728f;
static float32_t scale_iir_hp = 0.632277093897992026327870007662568241358f * 0.478258337927073562401147910350118763745f;
static float32_t scale_iir_bp = 0.558156585760773649163013487850548699498f * 0.558156585760773649163013487850548699498f;
static float32_t scale_iir_bs = 0.583479203143786984142593610158655792475f * 0.583479203143786984142593610158655792475f;

int main()
{
    uint32_t i;

    arm_biquad_casd_df1_inst_f32 S1, S2, S3, S4;

    board_init();

    /* generate signal */

    // signal = 50Hz sin + 200Hz sin, sample rate = 1KHz
    for (i = 0; i < SAMPLES_SIZE; ++i) {
        signal[0][i] = arm_sin_f32(2 * M_PI * 50 * i / SAMPLE_RATE);
        signal[1][i] = arm_sin_f32(2 * M_PI * 200 * i / SAMPLE_RATE);
        input[i]     = signal[0][i] + signal[1][i];
    }

    /* do iir filter */

    arm_biquad_cascade_df1_init_f32(&S1, IIR_STAGE, iir_coffes_lp, state_iir_lp);
    arm_biquad_cascade_df1_init_f32(&S2, IIR_STAGE, iir_coffes_hp, state_iir_hp);
    arm_biquad_cascade_df1_init_f32(&S3, IIR_STAGE, iir_coffes_bp, state_iir_bp);
    arm_biquad_cascade_df1_init_f32(&S4, IIR_STAGE, iir_coffes_bs, state_iir_bs);

    for (i = 0; i < SAMPLES_SIZE; i += BLOCK_SIZE) {
        arm_biquad_cascade_df1_f32(&S1, input + i, output_lp + i, BLOCK_SIZE);
        arm_biquad_cascade_df1_f32(&S2, input + i, output_hp + i, BLOCK_SIZE);
        arm_biquad_cascade_df1_f32(&S1, input + i, output_bp + i, BLOCK_SIZE);
        arm_biquad_cascade_df1_f32(&S2, input + i, output_bs + i, BLOCK_SIZE);
    }

    /* print input & output */

    for (i = 0; i < SAMPLES_SIZE; ++i)
        println("%f,%f,%f,%f,%f,%f,%f", signal[0][i], signal[1][i], input[i], output_lp[i] * scale_iir_lp, output_hp[i] * scale_iir_hp, output_bp[i] * scale_iir_bp, output_bs[i] * scale_iir_bs);
    // signal[0] -> output_lp & output_bp
    // signal[1] -> output_hp & output_bs

    while (1) {}

    return 0;
}
