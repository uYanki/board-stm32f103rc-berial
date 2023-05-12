#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

void do_dsp_sub(void)
{
    uint8_t i;

    /* 浮点数数组 */

    float32_t pDataA[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataB[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataDst[9];

    arm_matrix_instance_f32 pSrcA;  // 3行3列数据
    arm_matrix_instance_f32 pSrcB;  // 3行3列数据
    arm_matrix_instance_f32 pDst;

    /* 定点数q31数组 */

    q31_t pDataA1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t pDataB1[9] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    q31_t pDataDst1[9];

    arm_matrix_instance_q31 pSrcA1;  // 3行3列数据
    arm_matrix_instance_q31 pSrcB1;  // 3行3列数据
    arm_matrix_instance_q31 pDst1;

    /* 定点数q15数组 */

    q15_t pDataA2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t pDataB2[9] = {2, 2, 2, 2, 23, 2, 2, 2, 2};
    q15_t pDataDst2[9];

    arm_matrix_instance_q15 pSrcA2;  // 3行3列数据
    arm_matrix_instance_q15 pSrcB2;  // 3行3列数据
    arm_matrix_instance_q15 pDst2;

    /* 浮点数 */
    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData   = pDataA;

    pSrcB.numCols = 3;
    pSrcB.numRows = 3;
    pSrcB.pData   = pDataB;

    pDst.numCols  = 3;
    pDst.numRows  = 3;
    pDst.pData    = pDataDst;

    println("浮点数f32");
    arm_mat_sub_f32(&pSrcA, &pSrcB, &pDst);
    for (i = 0; i < 9; i++)
        println("pDataDst[%d] = %f", i, pDataDst[i]);

    /* 定点数q31 */
    pSrcA1.numCols = 3;
    pSrcA1.numRows = 3;
    pSrcA1.pData   = pDataA1;

    pSrcB1.numCols = 3;
    pSrcB1.numRows = 3;
    pSrcB1.pData   = pDataB1;

    pDst1.numCols  = 3;
    pDst1.numRows  = 3;
    pDst1.pData    = pDataDst1;

    println("*定点数q31*");
    arm_mat_sub_q31(&pSrcA1, &pSrcB1, &pDst1);
    for (i = 0; i < 9; i++)
        println("pDataDst1[%d] = %d", i, pDataDst1[i]);

    /* 定点数q15 */
    pSrcA2.numCols = 3;
    pSrcA2.numRows = 3;
    pSrcA2.pData   = pDataA2;

    pSrcB2.numCols = 3;
    pSrcB2.numRows = 3;
    pSrcB2.pData   = pDataB2;

    pDst2.numCols  = 3;
    pDst2.numRows  = 3;
    pDst2.pData    = pDataDst2;

    println("定点数q15");
    arm_mat_sub_q15(&pSrcA2, &pSrcB2, &pDst2);
    for (i = 0; i < 9; i++)
        println("pDataDst2[%d] = %d", i, pDataDst2[i]);
}

// 求和
void do_dsp_add(void)
{
    uint8_t i;

    /* 浮点数数组 */

    float32_t pDataA[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataB[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataDst[9];

    arm_matrix_instance_f32 pSrcA;  // 3行3列数据
    arm_matrix_instance_f32 pSrcB;  // 3行3列数据
    arm_matrix_instance_f32 pDst;

    /* 定点数q31数组 */

    q31_t pDataA1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t pDataB1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t pDataDst1[9];

    arm_matrix_instance_q31 pSrcA1;  // 3行3列数据
    arm_matrix_instance_q31 pSrcB1;  // 3行3列数据
    arm_matrix_instance_q31 pDst1;

    /* 定点数q15数组 */

    q15_t pDataA2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t pDataB2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t pDataDst2[9];

    arm_matrix_instance_q15 pSrcA2;  // 3行3列数据
    arm_matrix_instance_q15 pSrcB2;  // 3行3列数据
    arm_matrix_instance_q15 pDst2;

    /* 浮点数 */

    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData   = pDataA;

    pSrcB.numCols = 3;
    pSrcB.numRows = 3;
    pSrcB.pData   = pDataB;

    pDst.numCols  = 3;
    pDst.numRows  = 3;
    pDst.pData    = pDataDst;

    println("浮点数f32");
    arm_mat_add_f32(&pSrcA, &pSrcB, &pDst);
    for (i = 0; i < 9; i++)
        println("pDataDst[%d] = %f", i, pDataDst[i]);

    /* 定点数q31 */

    pSrcA1.numCols = 3;
    pSrcA1.numRows = 3;
    pSrcA1.pData   = pDataA1;

    pSrcB1.numCols = 3;
    pSrcB1.numRows = 3;
    pSrcB1.pData   = pDataB1;

    pDst1.numCols  = 3;
    pDst1.numRows  = 3;
    pDst1.pData    = pDataDst1;

    println("定点数q31");
    arm_mat_add_q31(&pSrcA1, &pSrcB1, &pDst1);
    for (i = 0; i < 9; i++)
        println("pDataDst1[%d] = %d", i, pDataDst1[i]);

    /* 定点数q15 */

    pSrcA2.numCols = 3;
    pSrcA2.numRows = 3;
    pSrcA2.pData   = pDataA2;

    pSrcB2.numCols = 3;
    pSrcB2.numRows = 3;
    pSrcB2.pData   = pDataB2;

    pDst2.numCols  = 3;
    pDst2.numRows  = 3;
    pDst2.pData    = pDataDst2;

    println("*定点数q15*");
    arm_mat_add_q15(&pSrcA2, &pSrcB2, &pDst2);
    for (i = 0; i < 9; i++)
        println("pDataDst2[%d] = %d", i, pDataDst2[i]);
}

// 初始化
void do_dsp_init(void)
{
    uint8_t i;

    /* 浮点数数组 */
    float32_t pDataA[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};

    arm_matrix_instance_f32 pSrcA;  // 3行3列数据

    /* 定点数q31数组 */
    q31_t pDataA1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};

    arm_matrix_instance_q31 pSrcA1;  // 3行3列数据

    /* 定点数q15数组 */
    q15_t pDataA2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};

    arm_matrix_instance_q15 pSrcA2;  // 3行3列数据

    /* 浮点数f32 */
    println("浮点数f32");
    arm_mat_init_f32(&pSrcA, 3, 3, pDataA);
    for (i = 0; i < 9; i++)
        println("pDataA[%d] = %f", i, pDataA[i]);

    /* 定点数q31 */
    println("定点数q31");
    arm_mat_init_q31(&pSrcA1, 3, 3, pDataA1);
    for (i = 0; i < 9; i++)
        println("pDataA1[%d] = %d", i, pDataA1[i]);

    /* 定点数q15 */
    println("定点数q15");
    arm_mat_init_q15(&pSrcA2, 3, 3, pDataA2);
    for (i = 0; i < 9; i++)
        println("pDataA2[%d] = %d", i, pDataA2[i]);
}

// 逆矩阵
void do_dsp_inverse(void)
{
    uint8_t i;

    arm_status sta;

    /* 浮点数数组 */
    float32_t pDataB[36];
    float32_t pDataA[36] = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 4.0f};

    arm_matrix_instance_f32 pSrcA;  // 6行6列数据
    arm_matrix_instance_f32 pSrcB;  // 6行6列数据;

    /* 浮点数 */
    pSrcA.numCols = 6;
    pSrcA.numRows = 6;
    pSrcA.pData   = pDataA;

    pSrcB.numCols = 6;
    pSrcB.numRows = 6;
    pSrcB.pData   = pDataB;

    sta           = arm_mat_inverse_f32(&pSrcA, &pSrcB);

    /*
        sta = ARM_MATH_SUCCESS, 即返回0，表示求逆矩阵成功。
        sta = ARM_MATH_SINGULAR, 即返回-5，表示求逆矩阵失败，也表示不可逆。
        注意，ARM提供的DSP库逆矩阵求法有局限性，通过Matlab验证是可以求逆矩阵的，而DSP库却不能正确求解。
     */

    println("----sta %d", sta);

    for (i = 0; i < 36; i++)
        println("pDataB[%d] = %f", i, pDataB[i]);
}

// 放缩
void do_dsp_scale(void)
{
    uint8_t i;

    /* 浮点数数组 */

    float32_t pDataA[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t scale     = 1.1f;
    float32_t pDataDst[9];

    arm_matrix_instance_f32 pSrcA;  // 3行3列数据
    arm_matrix_instance_f32 pDst;

    /* 定点数q31数组 */

    q31_t   pDataA1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t   scaleFract = 10;
    int32_t shift      = 0;
    q31_t   pDataDst1[9];

    arm_matrix_instance_q31 pSrcA1;  // 3行3列数据
    arm_matrix_instance_q31 pDst1;

    /* 定点数q15数组 */

    q15_t   pDataA2[9]  = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t   scaleFract1 = 10;
    int32_t shift1      = 0;
    q15_t   pDataDst2[9];

    arm_matrix_instance_q15 pSrcA2;  // 3行3列数据
    arm_matrix_instance_q15 pDst2;

    /* 浮点数 */
    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData   = pDataA;

    pDst.numCols  = 3;
    pDst.numRows  = 3;
    pDst.pData    = pDataDst;

    println("*浮点数*");
    arm_mat_scale_f32(&pSrcA, scale, &pDst);
    for (i = 0; i < 9; i++)
        println("pDataDst[%d] = %f", i, pDataDst[i]);

    /* 定点数q31 */
    pSrcA1.numCols = 3;
    pSrcA1.numRows = 3;
    pSrcA1.pData   = pDataA1;

    pDst1.numCols  = 3;
    pDst1.numRows  = 3;
    pDst1.pData    = pDataDst1;

    println("*定点数q31*");
    arm_mat_scale_q31(&pSrcA1, scaleFract, shift, &pDst1);
    for (i = 0; i < 9; i++)
        println("pDataDst1[%d] = %d", i, pDataDst1[i]);

    /* 定点数q15 */
    pSrcA2.numCols = 3;
    pSrcA2.numRows = 3;
    pSrcA2.pData   = pDataA2;

    pDst2.numCols  = 3;
    pDst2.numRows  = 3;
    pDst2.pData    = pDataDst2;

    println("*定点数q15*");
    arm_mat_scale_q15(&pSrcA2, scaleFract1, shift1, &pDst2);
    for (i = 0; i < 9; i++)
        println("pDataDst2[%d] = %d", i, pDataDst2[i]);
}

// 乘法
void do_dsp_mul(void)
{
    uint8_t i;

    /* 浮点数数组 */

    float32_t pDataA[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataB[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataDst[9];

    arm_matrix_instance_f32 pSrcA;  // 3行3列数据
    arm_matrix_instance_f32 pSrcB;  // 3行3列数据
    arm_matrix_instance_f32 pDst;

    /* 定点数q31数组 */

    q31_t pDataA1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t pDataB1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t pDataDst1[9];

    arm_matrix_instance_q31 pSrcA1;  // 3行3列数据
    arm_matrix_instance_q31 pSrcB1;  // 3行3列数据
    arm_matrix_instance_q31 pDst1;

    /* 定点数q15数组 */

    q15_t pDataA2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t pDataB2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t pDataDst2[9];

    arm_matrix_instance_q15 pSrcA2;  // 3行3列数据
    arm_matrix_instance_q15 pSrcB2;  // 3行3列数据
    arm_matrix_instance_q15 pDst2;
    q15_t                   pState[9];

    /* 浮点数 */
    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData   = pDataA;

    pSrcB.numCols = 3;
    pSrcB.numRows = 3;
    pSrcB.pData   = pDataB;

    pDst.numCols  = 3;
    pDst.numRows  = 3;
    pDst.pData    = pDataDst;

    println("*浮点数*");
    arm_mat_mult_f32(&pSrcA, &pSrcB, &pDst);
    for (i = 0; i < 9; i++)
        println("pDataDst[%d] = %f", i, pDataDst[i]);

    /* 定点数q31 */
    pSrcA1.numCols = 3;
    pSrcA1.numRows = 3;
    pSrcA1.pData   = pDataA1;

    pSrcB1.numCols = 3;
    pSrcB1.numRows = 3;
    pSrcB1.pData   = pDataB1;

    pDst1.numCols  = 3;
    pDst1.numRows  = 3;
    pDst1.pData    = pDataDst1;

    println("*定点数q31*");
    arm_mat_mult_q31(&pSrcA1, &pSrcB1, &pDst1);
    arm_mat_mult_fast_q31(&pSrcA1, &pSrcB1, &pDst1);
    for (i = 0; i < 9; i++)
        println("pDataDst1[%d] = %d", i, pDataDst1[i]);

    /* 定点数q15 */
    pSrcA2.numCols = 3;
    pSrcA2.numRows = 3;
    pSrcA2.pData   = pDataA2;

    pSrcB2.numCols = 3;
    pSrcB2.numRows = 3;
    pSrcB2.pData   = pDataB2;

    pDst2.numCols  = 3;
    pDst2.numRows  = 3;
    pDst2.pData    = pDataDst2;

    println("*定点数q15*");

    arm_mat_mult_q15(&pSrcA2, &pSrcB2, &pDst2, pState);
    arm_mat_mult_fast_q15(&pSrcA2, &pSrcB2, &pDst2, pState);
    for (i = 0; i < 9; i++)
        println("pDataDst2[%d] = %d", i, pDataDst2[i]);
}

// 转置
void do_dsp_trans(void)
{
    uint8_t i;

    /* 浮点数数组 */

    float32_t pDataA[9] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f};
    float32_t pDataDst[9];

    arm_matrix_instance_f32 pSrcA;  // 3行3列数据
    arm_matrix_instance_f32 pDst;

    /* 定点数q31数组 */

    q31_t pDataA1[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q31_t pDataDst1[9];

    arm_matrix_instance_q31 pSrcA1;  // 3行3列数据
    arm_matrix_instance_q31 pDst1;

    /* 定点数q15数组 */

    q15_t pDataA2[9] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    q15_t pDataDst2[9];

    arm_matrix_instance_q15 pSrcA2;  // 3行3列数据
    arm_matrix_instance_q15 pDst2;

    /* 浮点数 */

    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData   = pDataA;

    pDst.numCols  = 3;
    pDst.numRows  = 3;
    pDst.pData    = pDataDst;

    println("*浮点数*");
    arm_mat_trans_f32(&pSrcA, &pDst);
    for (i = 0; i < 9; i++)
        println("pDataDst[%d] = %f", i, pDataDst[i]);

    /* 定点数q31 */

    pSrcA1.numCols = 3;
    pSrcA1.numRows = 3;
    pSrcA1.pData   = pDataA1;

    pDst1.numCols  = 3;
    pDst1.numRows  = 3;
    pDst1.pData    = pDataDst1;

    println("*定点数q31*");
    arm_mat_trans_q31(&pSrcA1, &pDst1);
    for (i = 0; i < 9; i++)
        println("pDataDst1[%d] = %d", i, pDataDst1[i]);

    /* 定点数q15 */

    pSrcA2.numCols = 3;
    pSrcA2.numRows = 3;
    pSrcA2.pData   = pDataA2;

    pDst2.numCols  = 3;
    pDst2.numRows  = 3;
    pDst2.pData    = pDataDst2;

    println("*定点数q15*");
    arm_mat_trans_q15(&pSrcA2, &pDst2);
    for (i = 0; i < 9; i++)
        println("pDataDst2[%d] = %d", i, pDataDst2[i]);
}

int main()
{
    board_init();

    do_dsp_init();
    do_dsp_add();
    do_dsp_sub();
    do_dsp_inverse();
    do_dsp_mul();
    do_dsp_scale();
    do_dsp_trans();

    while (1) {}

    return 0;
}
