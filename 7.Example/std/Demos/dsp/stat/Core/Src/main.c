#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

// 最小值
void do_dsp_min(void)
{
    float32_t pSrc[10] = {0.6948f, 0.3171f, 0.9502f, 0.0344f, 0.4387f, 0.3816f, 0.7655f, 0.7952f, 0.1869f, 0.4898f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q31_t pResult1;

    q15_t pSrc2[10];
    q15_t pResult2;

    q7_t pSrc3[10];
    q7_t pResult3;

    arm_min_f32(pSrc, 10, &pResult, &pIndex);
    println("arm_min_f32 : pResult = %f  pIndex = %d", pResult, pIndex);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_min_q31(pSrc1, 10, &pResult1, &pIndex);
    println("arm_min_q31 : pResult = %d  pIndex = %d", pResult1, pIndex);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_min_q15(pSrc2, 10, &pResult2, &pIndex);
    println("arm_min_q15 : pResult = %d  pIndex = %d", pResult2, pIndex);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc3[pIndex] = rand() % 128;

    arm_min_q7(pSrc3, 10, &pResult3, &pIndex);
    println("arm_min_q7 : pResult = %d  pIndex = %d", pResult3, pIndex);
}

// 最大值
void do_dsp_max(void)
{
    float32_t pSrc[10] = {0.6948f, 0.3171f, 0.9502f, 0.0344f, 0.4387f, 0.3816f, 0.7655f, 0.7952f, 0.1869f, 0.4898f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q31_t pResult1;

    q15_t pSrc2[10];
    q15_t pResult2;

    q7_t pSrc3[10];
    q7_t pResult3;

    arm_max_f32(pSrc, 10, &pResult, &pIndex);
    println("arm_max_f32 : pResult = %f  pIndex = %d", pResult, pIndex);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_max_q31(pSrc1, 10, &pResult1, &pIndex);
    println("arm_max_q31 : pResult = %d  pIndex = %d", pResult1, pIndex);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_max_q15(pSrc2, 10, &pResult2, &pIndex);
    println("arm_max_q15 : pResult = %d  pIndex = %d", pResult2, pIndex);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc3[pIndex] = rand() % 128;

    arm_max_q7(pSrc3, 10, &pResult3, &pIndex);
    println("arm_max_q7 : pResult = %d  pIndex = %d", pResult3, pIndex);
}

// 平均值
void do_dsp_mean(void)
{
    float32_t pSrc[10] = {0.6948f, 0.3171f, 0.9502f, 0.0344f, 0.4387f, 0.3816f, 0.7655f, 0.7952f, 0.1869f, 0.4898f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q31_t pResult1;

    q15_t pSrc2[10];
    q15_t pResult2;

    q7_t pSrc3[10];
    q7_t pResult3;

    arm_mean_f32(pSrc, 10, &pResult);
    println("arm_mean_f32 : pResult = %f", pResult);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_mean_q31(pSrc1, 10, &pResult1);
    println("arm_mean_q31 : pResult = %d", pResult1);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_mean_q15(pSrc2, 10, &pResult2);
    println("arm_mean_q15 : pResult = %d", pResult2);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc3[pIndex] = rand() % 128;

    arm_mean_q7(pSrc3, 10, &pResult3);
    println("arm_mean_q7 : pResult = %d", pResult3);
}

// 功率
void do_dsp_power(void)
{
    float32_t pSrc[10] = {0.6948f, 0.3171f, 0.9502f, 0.0344f, 0.4387f, 0.3816f, 0.7655f, 0.7952f, 0.1869f, 0.4898f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q63_t pResult1;

    q15_t pSrc2[10];
    q63_t pResult2;

    q7_t  pSrc3[10];
    q31_t pResult3;

    arm_power_f32(pSrc, 10, &pResult);
    println("arm_power_f32 : pResult = %f", pResult);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_power_q31(pSrc1, 10, &pResult1);
    println("arm_power_q31 : pResult = %lld", pResult1);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_power_q15(pSrc2, 10, &pResult2);
    println("arm_power_q15 : pResult = %lld", pResult2);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc3[pIndex] = rand() % 128;

    arm_power_q7(pSrc3, 10, &pResult3);
    println("arm_power_q7 : pResult = %d", pResult3);
}

// 标准差
void do_dsp_std(void)
{
    float32_t pSrc[10] = {0.6557f, 0.0357f, 0.8491f, 0.9340f, 0.6787f, 0.7577f, 0.7431f, 0.3922f, 0.6555f, 0.1712f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q31_t pResult1;

    q15_t pSrc2[10];
    q15_t pResult2;

    arm_std_f32(pSrc, 10, &pResult);
    println("arm_std_f32 : pResult = %f", pResult);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_std_q31(pSrc1, 10, &pResult1);
    println("arm_std_q31 : pResult = %d", pResult1);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_std_q15(pSrc2, 10, &pResult2);
    println("arm_std_q15 : pResult = %d", pResult2);
}

// 均方根
void do_dsp_rms(void)
{
    float32_t pSrc[10] = {0.7060f, 0.0318f, 0.2769f, 0.0462f, 0.0971f, 0.8235f, 0.6948f, 0.3171f, 0.9502f, 0.0344f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q31_t pResult1;

    q15_t pSrc2[10];
    q15_t pResult2;

    arm_rms_f32(pSrc, 10, &pResult);
    println("arm_rms_f32 : pResult = %f", pResult);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_rms_q31(pSrc1, 10, &pResult1);
    println("arm_rms_q31 : pResult = %d", pResult1);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_rms_q15(pSrc2, 10, &pResult2);
    println("arm_rms_q15 : pResult = %d", pResult2);
}

// 方差
void do_dsp_var(void)
{
    float32_t pSrc[10] = {0.4387f, 0.3816f, 0.7655f, 0.7952f, 0.1869f, 0.4898f, 0.4456f, 0.6463f, 0.7094f, 0.7547f};
    float32_t pResult;
    uint32_t  pIndex;

    q31_t pSrc1[10];
    q31_t pResult1;

    q15_t pSrc2[10];
    q15_t pResult2;

    arm_var_f32(pSrc, 10, &pResult);
    println("arm_var_f32 : pResult = %f", pResult);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc1[pIndex] = rand();

    arm_var_q31(pSrc1, 10, &pResult1);
    println("arm_var_q31 : pResult = %d", pResult1);

    for (pIndex = 0; pIndex < 10; pIndex++)
        pSrc2[pIndex] = rand() % 32768;

    arm_var_q15(pSrc2, 10, &pResult2);
    println("arm_var_q15 : pResult = %d", pResult2);
}

int main()
{
    board_init();

    do_dsp_min();
    do_dsp_max();
    do_dsp_mean();
    do_dsp_power();

    do_dsp_std();
    do_dsp_rms();
    do_dsp_var();

    while (1) {}

    return 0;
}
