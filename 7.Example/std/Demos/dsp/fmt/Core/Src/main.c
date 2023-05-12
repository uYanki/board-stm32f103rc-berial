#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

// q7 -> q15、q31、float32
void do_dsp_q7(void)
{
    float32_t pDst[10];
    uint32_t  pIndex;

    q31_t pDst1[10];
    q15_t pDst2[10];
    q7_t  pSrc[10];

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc[%d] = %d", pIndex, pSrc[pIndex] = rand() % 128);

    arm_q7_to_float(pSrc, pDst, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q7_to_float: pDst[%d] = %f", pIndex, pDst[pIndex]);

    arm_q7_to_q31(pSrc, pDst1, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q7_to_q31: pDst1[%d] = %d", pIndex, pDst1[pIndex]);

    arm_q7_to_q15(pSrc, pDst2, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q7_to_q15: pDst2[%d] = %d", pIndex, pDst2[pIndex]);
}

// q15 -> q7、q31、float32
void do_dsp_q15(void)
{
    float32_t pDst[10];
    uint32_t  pIndex;

    q31_t pDst1[10];
    q15_t pSrc[10];
    q7_t  pDst2[10];

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc[%d] = %d", pIndex, pSrc[pIndex] = rand() % 32678);

    arm_q15_to_float(pSrc, pDst, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q15_to_float: pDst[%d] = %f", pIndex, pDst[pIndex]);

    arm_q15_to_q31(pSrc, pDst1, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q15_to_q31: pDst1[%d] = %d", pIndex, pDst1[pIndex]);

    arm_q15_to_q7(pSrc, pDst2, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q15_to_q7: pDst2[%d] = %d", pIndex, pDst2[pIndex]);
}

// q31 -> q15、q7、float32
void do_dsp_q31(void)
{
    float32_t pDst[10];
    uint32_t  pIndex;

    q31_t pSrc[10];
    q15_t pDst1[10];
    q7_t  pDst2[10];

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc[%d] = %d", pIndex, pSrc[pIndex] = rand());

    arm_q31_to_float(pSrc, pDst, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q31_to_float: pDst[%d] = %f", pIndex, pDst[pIndex]);

    arm_q31_to_q15(pSrc, pDst1, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q31_to_q15: pDst1[%d] = %d", pIndex, pDst1[pIndex]);

    arm_q31_to_q7(pSrc, pDst2, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_q31_to_q7: pDst2[%d] = %d", pIndex, pDst2[pIndex]);
}

// 数据拷贝
void do_dsp_copy(void)
{
    float32_t pSrc[10] = {0.6557, 0.0357, 0.8491, 0.9340, 0.6787, 0.7577, 0.7431, 0.3922, 0.6555, 0.1712}, pDst[10];

    uint32_t pIndex;

    q31_t pSrc1[10], pDst1[10];
    q15_t pSrc2[10], pDst2[10];
    q7_t  pSrc3[10], pDst3[10];

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc[%d] = %f", pIndex, pSrc[pIndex]);

    arm_copy_f32(pSrc, pDst, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_copy_f32: pDst[%d] = %f", pIndex, pDst[pIndex]);

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc1[%d] = %d", pIndex, pSrc1[pIndex] = rand());

    arm_copy_q31(pSrc1, pDst1, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_copy_q31: pDst1[%d] = %d", pIndex, pDst1[pIndex]);

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc2[%d] = %d", pIndex, pSrc2[pIndex] = rand() % 32768);

    arm_copy_q15(pSrc2, pDst2, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_copy_q15: pDst2[%d] = %d", pIndex, pDst2[pIndex]);

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc3[%d] = %d", pIndex, pSrc3[pIndex] = rand() % 128);

    arm_copy_q7(pSrc3, pDst3, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_copy_q7: pDst3[%d] = %d", pIndex, pDst3[pIndex]);
}

// 数据填充
void do_dsp_fill(void)
{
    float32_t pDst[10];

    uint32_t pIndex;

    q31_t pDst1[10];
    q15_t pDst2[10];
    q7_t  pDst3[10];

    arm_fill_f32(3.33f, pDst, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_fill_f32: pDst[%d] = %f", pIndex, pDst[pIndex]);

    arm_fill_q31(0x11111111, pDst1, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_fill_q31: pDst1[%d] = %x", pIndex, pDst1[pIndex]);

    arm_fill_q15(0x1111, pDst2, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_fill_q15: pDst2[%d] = %d", pIndex, pDst2[pIndex]);

    arm_fill_q7(0x11, pDst3, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_fill_q7: pDst3[%d] = %d", pIndex, pDst3[pIndex]);
}

// 浮点数转定点数
void do_dsp_float2fix(void)
{
    float32_t pSrc[10] = {0.6557, 0.0357, 0.8491, 0.9340, 0.6787, 0.7577, 0.7431, 0.3922, 0.6555, 0.1712};

    uint32_t pIndex;

    q31_t pDst1[10];
    q15_t pDst2[10];
    q7_t  pDst3[10];

    for (pIndex = 0; pIndex < 10; pIndex++)
        println("pSrc[%d] = %f", pIndex, pSrc[pIndex]);

    arm_float_to_q31(pSrc, pDst1, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_float_to_q31: pDst[%d] = %d", pIndex, pDst1[pIndex]);

    arm_float_to_q15(pSrc, pDst2, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_float_to_q15: pDst1[%d] = %d", pIndex, pDst2[pIndex]);

    arm_float_to_q7(pSrc, pDst3, 10);
    for (pIndex = 0; pIndex < 10; pIndex++)
        println("arm_float_to_q7: pDst2[%d] = %d", pIndex, pDst3[pIndex]);
}

int main()
{
    board_init();

    do_dsp_q7();
    do_dsp_q15();
    do_dsp_q31();

    do_dsp_copy();
    do_dsp_fill();
    do_dsp_float2fix();

    while (1) {}

    return 0;
}
