#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

// 正弦
void do_dsp_sin(void)
{
    uint16_t i;

    for (i = 0; i < 256; i++) {
        println("%f,%d,%d",
                arm_sin_f32(i * PI / 128),  // [0 2*pi)
                arm_sin_q15(i * 128),       // [0 2*pi) map to [0,0xFFFF)
                arm_sin_q31(i * 0x800000)   // [0 2*pi) map to [0,0xFFFFFFFF)
        );
    }
}

// 余弦
void do_dsp_cos(void)
{
    uint16_t i;

    for (i = 0; i < 256; i++) {
        println("%f,%d,%d",
                arm_cos_f32(i * PI / 128),  // [0 2*pi)
                arm_cos_q15(i * 128),       // [0 2*pi) map to [0,0xFFFF)
                arm_cos_q31(i * 0x800000)   // [0 2*pi) map to [0,0xFFFFFFFF)
        );
    }
}

// sin & cos

void do_dsp_sin_cos_q31(void)
{
    int16_t i;

    q31_t sin_q31, cos_q31;

    for (i = -256; i < 256; i++) {
        arm_sin_cos_q31(i * 8388608, &sin_q31, &cos_q31);
        println("%d,%d,%d", i, sin_q31, cos_q31);
    }
}

void do_dsp_sin_cos_f32(void)
{
    int16_t i;

    float32_t sin_f32, cos_f32;

    for (i = -180; i < 180; i++) {
        arm_sin_cos_f32(i, &sin_f32, &cos_f32);
        println("%d,%f,%f", i, sin_f32, cos_f32);
    }
}

#define LVL 2

int main()
{
    board_init();

#if LVL == 0

    do_dsp_sin();
    do_dsp_cos();

#elif LVL == 1

    do_dsp_sin_cos_q31();

#elif LVL == 2

    do_dsp_sin_cos_f32();

#else

#error "invalid level"

#endif

    while (1) {}

    return 0;
}
