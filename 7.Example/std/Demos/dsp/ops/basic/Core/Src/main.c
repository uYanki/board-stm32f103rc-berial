#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

// 绝对值
void do_dsp_abs(void)
{
    float32_t pSrc = 0.0f;
    float32_t pDst;

    q31_t pSrc1 = 0;
    q31_t pDst1;

    q15_t pSrc2;
    q15_t pDst2;

    q7_t pSrc3;
    q7_t pDst3;

    pSrc -= 1.23f;
    arm_abs_f32(&pSrc, &pDst, 1);
    println("arm_abs_f32 = %f", pDst);

    pSrc1 -= 1;
    arm_abs_q31(&pSrc1, &pDst1, 1);
    println("arm_abs_q31 = %d", pDst1);

    pSrc2 = -32768;
    arm_abs_q15(&pSrc2, &pDst2, 1);
    println("arm_abs_q15 = %d", pDst2);

    pSrc3 = 127;
    arm_abs_q7(&pSrc3, &pDst3, 1);
    println("arm_abs_q7 = %d", pDst3);
}

// 加法
void do_dsp_add(void)
{
    float32_t pSrcA;
    float32_t pSrcB;
    float32_t pDst;

    q31_t pSrcA1;
    q31_t pSrcB1;
    q31_t pDst1;

    q15_t pSrcA2;
    q15_t pSrcB2;
    q15_t pDst2;

    q7_t pSrcA3;
    q7_t pSrcB3;
    q7_t pDst3;

    pSrcA = 0.1f;
    pSrcB = 0.2f;
    arm_add_f32(&pSrcA, &pSrcB, &pDst, 1);
    println("arm_add_f32 = %f", pDst);

    pSrcA1 = 1;
    pSrcB1 = 1;
    arm_add_q31(&pSrcA1, &pSrcB1, &pDst1, 1);
    println("arm_add_q31 = %d", pDst1);

    pSrcA2 = 2;
    pSrcB2 = 2;
    arm_add_q15(&pSrcA2, &pSrcB2, &pDst2, 1);
    println("arm_add_q15 = %d", pDst2);

    pSrcA3 = 30;
    pSrcB3 = 120;
    arm_add_q7(&pSrcA3, &pSrcB3, &pDst3, 1);
    println("arm_add_q7 = %d", pDst3);
}

// 点乘
void do_dsp_dot(void)
{
    float32_t pSrcA[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t pSrcB[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t result;

    q31_t pSrcA1[5] = {0x7ffffff0, 1, 1, 1, 1};
    q31_t pSrcB1[5] = {1, 1, 1, 1, 1};
    q63_t result1;

    q15_t pSrcA2[5] = {1, 1, 1, 1, 1};
    q15_t pSrcB2[5] = {1, 1, 1, 1, 1};
    q63_t result2;

    q7_t  pSrcA3[5] = {1, 1, 1, 1, 1};
    q7_t  pSrcB3[5] = {1, 1, 1, 1, 1};
    q31_t result3;

    // dot_product
    arm_dot_prod_f32(pSrcA, pSrcB, 5, &result);
    println("arm_dot_prod_f32 = %f", result);

    arm_dot_prod_q31(pSrcA1, pSrcB1, 5, &result1);
    println("arm_dot_prod_q31 = %lld", result1);

    arm_dot_prod_q15(pSrcA2, pSrcB2, 5, &result2);
    println("arm_dot_prod_q15 = %lld", result2);

    arm_dot_prod_q7(pSrcA3, pSrcB3, 5, &result3);
    println("arm_dot_prod_q7 = %d", result3);
}

// 乘积
void do_dsp_mul(void)
{
    float32_t pSrcA[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t pSrcB[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t pDst[5];

    q31_t pSrcA1[5] = {1, 1, 1, 1, 1};
    q31_t pSrcB1[5] = {1, 1, 1, 1, 1};
    q31_t pDst1[5];

    q15_t pSrcA2[5] = {1, 1, 1, 1, 1};
    q15_t pSrcB2[5] = {1, 1, 1, 1, 1};
    q15_t pDst2[5];

    q7_t pSrcA3[5] = {0x70, 1, 1, 1, 1};
    q7_t pSrcB3[5] = {0x7f, 1, 1, 1, 1};
    q7_t pDst3[5];

    // multiplication

    pSrcA[0] += 1.1f;
    arm_mult_f32(pSrcA, pSrcB, pDst, 5);
    println("arm_mult_f32 = %f", pDst[0]);

    pSrcA1[0] += 1;
    arm_mult_q31(pSrcA1, pSrcB1, pDst1, 5);
    println("arm_mult_q31 = %d", pDst1[0]);

    pSrcA2[0] += 1;
    arm_mult_q15(pSrcA2, pSrcB2, pDst2, 5);
    println("arm_mult_q15 = %d", pDst2[0]);

    pSrcA3[0] += 1;
    arm_mult_q7(pSrcA3, pSrcB3, pDst3, 5);
    println("arm_mult_q7 = %d", pDst3[0]);
}

// 相反数
void do_dsp_negate(void)
{
    float32_t pSrc = 0.0f;
    float32_t pDst;

    q31_t pSrc1 = 0;
    q31_t pDst1;

    q15_t pSrc2 = 0;
    q15_t pDst2;

    q7_t pSrc3 = 0;
    q7_t pDst3;

    pSrc -= 1.23f;
    arm_negate_f32(&pSrc, &pDst, 1);
    println("arm_negate_f32 = %f", pDst);

    pSrc1 -= 1;
    arm_negate_q31(&pSrc1, &pDst1, 1);
    println("arm_negate_q31 = %d", pDst1);

    pSrc2 -= 1;
    arm_negate_q15(&pSrc2, &pDst2, 1);
    println("arm_negate_q15 = %d", pDst2);

    pSrc3 += 1;
    arm_negate_q7(&pSrc3, &pDst3, 1);
    println("arm_negate_q7 = %d", pDst3);
}

// 偏移
void do_dsp_offset(void)
{
    float32_t pSrcA  = 0.0f;
    float32_t Offset = 0.0f;
    float32_t pDst;

    q31_t pSrcA1  = 0;
    q31_t Offset1 = 0;
    q31_t pDst1;

    q15_t pSrcA2  = 0;
    q15_t Offset2 = 0;
    q15_t pDst2;

    q7_t pSrcA3  = 0;
    q7_t Offset3 = 0;
    q7_t pDst3;

    Offset--;
    arm_offset_f32(&pSrcA, Offset, &pDst, 1);
    println("arm_offset_f32 = %f", pDst);

    Offset1--;
    arm_offset_q31(&pSrcA1, Offset1, &pDst1, 1);
    println("arm_offset_q31 = %d", pDst1);

    Offset2--;
    arm_offset_q15(&pSrcA2, Offset2, &pDst2, 1);
    println("arm_offset_q15 = %d", pDst2);

    Offset3--;
    arm_offset_q7(&pSrcA3, Offset3, &pDst3, 1);
    println("arm_offset_q7 = %d", pDst3);
}

// 移位
void do_dsp_shift(void)
{
    q31_t pSrcA1 = 0x88886666;
    q31_t pDst1;

    q15_t pSrcA2 = 0x8866;
    q15_t pDst2;

    q7_t pSrcA3 = 0x86;
    q7_t pDst3;

    arm_shift_q31(&pSrcA1, 3, &pDst1, 1);
    println("arm_shift_q31 = %8x", pDst1);

    arm_shift_q15(&pSrcA2, -3, &pDst2, 1);
    println("arm_shift_q15 = %4x", pDst2);

    arm_shift_q7(&pSrcA3, 3, &pDst3, 1);
    println("arm_shift_q7 = %2x", pDst3);
}

// 减法
void do_dsp_sub(void)
{
    float32_t pSrcA[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t pSrcB[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t pDst[5];

    q31_t pSrcA1[5] = {1, 1, 1, 1, 1};
    q31_t pSrcB1[5] = {1, 1, 1, 1, 1};
    q31_t pDst1[5];

    q15_t pSrcA2[5] = {1, 1, 1, 1, 1};
    q15_t pSrcB2[5] = {1, 1, 1, 1, 1};
    q15_t pDst2[5];

    q7_t pSrcA3[5] = {0x70, 1, 1, 1, 1};
    q7_t pSrcB3[5] = {0x7f, 1, 1, 1, 1};
    q7_t pDst3[5];

    pSrcA[0] += 1.1f;
    arm_sub_f32(pSrcA, pSrcB, pDst, 5);
    println("arm_sub_f32 = %f", pDst[0]);

    pSrcA1[0] += 1;
    arm_sub_q31(pSrcA1, pSrcB1, pDst1, 5);
    println("arm_sub_q31 = %d", pDst1[0]);

    pSrcA2[0] += 1;
    arm_sub_q15(pSrcA2, pSrcB2, pDst2, 5);
    println("arm_sub_q15 = %d", pDst2[0]);

    pSrcA3[0] += 1;
    arm_sub_q7(pSrcA3, pSrcB3, pDst3, 5);
    println("arm_sub_q7 = %d", pDst3[0]);
}

// 比例因子
void do_dsp_scale(void)
{
    float32_t pSrcA[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float32_t scale    = 0.0f;
    float32_t pDst[5];

    q31_t pSrcA1[5] = {0x6fffffff, 1, 1, 1, 1};
    q31_t scale1    = 0x6fffffff;
    q31_t pDst1[5];

    q15_t pSrcA2[5] = {0x6fff, 1, 1, 1, 1};
    q15_t scale2    = 0x6fff;
    q15_t pDst2[5];

    q7_t pSrcA3[5] = {0x70, 1, 1, 1, 1};
    q7_t scale3    = 0x6f;
    q7_t pDst3[5];

    scale += 0.1f;
    arm_scale_f32(pSrcA, scale, pDst, 5);
    println("arm_scale_f32 = %f", pDst[0]);

    scale1 += 1;
    arm_scale_q31(pSrcA1, scale1, 0, pDst1, 5);
    println("arm_scale_q31 = %x", pDst1[0]);

    scale2 += 1;
    arm_scale_q15(pSrcA2, scale2, 0, pDst2, 5);
    println("arm_scale_q15 = %x", pDst2[0]);

    scale3 += 1;
    arm_scale_q7(pSrcA3, scale3, 0, pDst3, 5);
    println("arm_scale_q7 = %x", pDst3[0]);
}

// 平方根
void do_dsp_sqrt(void)
{
    q15_t     pOut;
    q31_t     pOut1;
    float32_t pOut2;

    /* 求平方根 */
    arm_sqrt_q15(1000, &pOut);
    println("arm_sqrt_q15 = %d", pOut);

    arm_sqrt_q31(1000, &pOut1);
    println("arm_sqrt_q31 = %d", pOut1);

    arm_sqrt_f32(1000, &pOut2);
    println("arm_sqrt_f32 = %f", pOut2);
}

int main()
{
    board_init();

    do_dsp_abs();
    do_dsp_add();
    do_dsp_dot();
    do_dsp_mul();

    do_dsp_negate();
    do_dsp_offset();
    do_dsp_shift();
    do_dsp_sub();
    do_dsp_scale();

    do_dsp_sqrt();

    while (1) {}

    return 0;
}
