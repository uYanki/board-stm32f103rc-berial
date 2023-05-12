#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

// 共轭 conjugate
void dsp_do_conj(void)
{
    uint8_t   i;
    float32_t pSrc[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
    float32_t pDst[10];

    q31_t pSrc1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    q31_t pDst1[10];

    q15_t pSrc2[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    q15_t pDst2[10];

    println("共轭f32");
    arm_cmplx_conj_f32(pSrc, pDst, 5);
    for (i = 0; i < 5; i++) println("pSrc[%d] = %f %fj    pDst[%d] = %f %fj", i, pSrc[2 * i], pSrc[2 * i + 1], i, pDst[2 * i], pDst[2 * i + 1]);

    println("共轭Q31");
    arm_cmplx_conj_q31(pSrc1, pDst1, 5);
    for (i = 0; i < 5; i++) println("pSrc1[%d] = %d %dj    pDst1[%d] = %d %dj", i, pSrc1[2 * i], pSrc1[2 * i + 1], i, pDst1[2 * i], pDst1[2 * i + 1]);

    println("共轭Q15");
    arm_cmplx_conj_q15(pSrc2, pDst2, 5);
    for (i = 0; i < 5; i++) println("pSrc2[%d] = %d %dj    pDst2[%d] = %d %dj", i, pSrc2[2 * i], pSrc2[2 * i + 1], i, pDst2[2 * i], pDst2[2 * i + 1]);
}

// 点乘 dot product
void dsp_do_dot(void)
{
    float32_t pSrcA[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
    float32_t pSrcB[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
    float32_t realResult;
    float32_t imagResult;

    q31_t pSrcA1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                        4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};
    q31_t pSrcB1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                        4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};
    q63_t realResult1;
    q63_t imagResult1;

    q15_t pSrcA2[10] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
    q15_t pSrcB2[10] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
    q31_t realResult2;
    q31_t imagResult2;

    arm_cmplx_dot_prod_f32(pSrcA, pSrcB, 5, &realResult, &imagResult);
    println("arm_cmplx_dot_prod_f32:realResult = %f    imagResult = %f", realResult, imagResult);

    arm_cmplx_dot_prod_q31(pSrcA1, pSrcB1, 5, &realResult1, &imagResult1);
    println("arm_cmplx_dot_prod_q31:realResult1 = %lld    imagResult1 = %lld", realResult1, imagResult1);

    arm_cmplx_dot_prod_q15(pSrcA2, pSrcB2, 5, &realResult2, &imagResult2);
    println("arm_cmplx_dot_prod_q15:realResult2 = %d    imagResult2 = %d", realResult2, imagResult2);
}

// 模
void dsp_do_mag(void)
{
    uint8_t   i;
    float32_t pSrc[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
    float32_t pDst[10];

    q31_t pSrc1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                       4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};
    q31_t pDst1[10];

    q15_t pSrc2[10] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
    q15_t pDst2[10];

    println("模f32:");
    arm_cmplx_mag_f32(pSrc, pDst, 5);
    for (i = 0; i < 5; i++) println("pDst[%d] = %f", i, pDst[i]);

    println("模Q31:");
    arm_cmplx_mag_q31(pSrc1, pDst1, 5);
    for (i = 0; i < 5; i++) println("pDst1[%d] = %d", i, pDst1[i]);

    println("模Q15:");
    arm_cmplx_mag_q15(pSrc2, pDst2, 5);
    for (i = 0; i < 5; i++) println("pDst2[%d] = %d", i, pDst2[i]);
}

// 平方
void dsp_do_square(void)
{
    uint8_t   i;
    float32_t pSrc[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
    float32_t pDst[10];

    q31_t pSrc1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                       4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};
    q31_t pDst1[10];

    q15_t pSrc2[10] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
    q15_t pDst2[10];

    println("平方f32:");
    arm_cmplx_mag_squared_f32(pSrc, pDst, 5);
    for (i = 0; i < 5; i++) println("pDst[%d] = %f", i, pDst[i]);

    println("平方q31:");
    arm_cmplx_mag_squared_q31(pSrc1, pDst1, 5);
    for (i = 0; i < 5; i++) println("pDst1[%d] = %d", i, pDst1[i]);

    println("平方q15:");
    arm_cmplx_mag_squared_q15(pSrc2, pDst2, 5);
    for (i = 0; i < 5; i++) println("pDst2[%d] = %d", i, pDst2[i]);
}

// 乘法
void dsp_do_mul_cmplx(void)
{
    uint8_t   i;
    float32_t pSrcA[10] = {1.1f, 1.2f, 2.1f, 2.2f, 3.1f, 3.2f, 4.1f, 4.2f, 5.1f, 5.2f};
    float32_t pSrcB[10] = {1.2f, 1.2f, 2.2f, 2.2f, 3.2f, 3.2f, 4.2f, 4.2f, 5.2f, 5.2f};
    float32_t pDst[10];

    q31_t pSrcA1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                        4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};
    q31_t pSrcB1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                        4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};
    q31_t pDst1[10];

    q15_t pSrcA2[10] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
    q15_t pSrcB2[10] = {6000, 11000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
    q15_t pDst2[10];

    println("乘复数f32:");
    arm_cmplx_mult_cmplx_f32(pSrcA, pSrcB, pDst, 5);
    for (i = 0; i < 5; i++) println("pDst[%d] = %f %fj", i, pDst[2 * i], pDst[2 * i + 1]);

    println("乘复数q31:");
    arm_cmplx_mult_cmplx_q31(pSrcA1, pSrcB1, pDst1, 5);
    for (i = 0; i < 5; i++) println("pDst1[%d] = %d %dj", i, pDst1[2 * i], pDst1[2 * i + 1]);

    println("乘复数q15:");
    arm_cmplx_mult_cmplx_q15(pSrcA2, pSrcB2, pDst2, 5);
    for (i = 0; i < 5; i++) println("pDst2[%d] = %d %dj", i, pDst2[2 * i], pDst2[2 * i + 1]);
}

// 复数乘实数
void dsp_do_mul_real(void)
{
    uint8_t   i;
    float32_t pSrcCmplx[10] = {1.1f, 1.2f, 2.1f, 2.2f, 3.1f, 3.2f, 4.1f, 4.2f, 5.1f, 5.2f};
    float32_t pSrcReal[5]   = {1.2f, 1.2f, 2.2f, 2.2f, 3.2f};
    float32_t pCmplxDst[10];

    q31_t pSrcCmplx1[10] = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456, 3 * 268435456,
                            4 * 268435456, 4 * 268435456, 5 * 268435456, 5 * 268435456};

    q31_t pSrcReal1[10]  = {1 * 268435456, 1 * 268435456, 2 * 268435456, 2 * 268435456, 3 * 268435456};
    q31_t pCmplxDst1[10];

    q15_t pSrcCmplx2[10] = {14000, 16000, 20000, 20000, 30000, 31000, 12000, 13000, 14000, 25000};
    q15_t pSrcReal2[10]  = {15000, 17000, 20000, 20000, 30000};
    q15_t pCmplxDst2[10];

    println("乘实数f32:");
    arm_cmplx_mult_cmplx_f32(pSrcCmplx, pSrcReal, pCmplxDst, 5);
    for (i = 0; i < 5; i++) println("pDst[%d] = %f %fj", i, pCmplxDst[2 * i], pCmplxDst[2 * i + 1]);

    println("乘实数q31:");
    arm_cmplx_mult_cmplx_q31(pSrcCmplx1, pSrcReal1, pCmplxDst1, 5);
    for (i = 0; i < 5; i++) println("pDst1[%d] = %d %dj", i, pCmplxDst1[2 * i], pCmplxDst1[2 * i + 1]);

    println("乘实数q15:");
    arm_cmplx_mult_cmplx_q15(pSrcCmplx2, pSrcReal2, pCmplxDst2, 5);
    for (i = 0; i < 5; i++) println("pDst2[%d] = %d %dj", i, pCmplxDst2[2 * i], pCmplxDst2[2 * i + 1]);
}

int main()
{
    board_init();

    dsp_do_conj();
    dsp_do_dot();
    dsp_do_mag();
    dsp_do_square();
    dsp_do_mul_cmplx();
    dsp_do_mul_real();

    while (1) {}

    return 0;
}
