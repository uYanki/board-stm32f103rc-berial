#include "blood.h"
#include "usart.h"
uint16_t g_fft_index = 0;  // fft输入输出下标

struct compx s1[FFT_N + 16];  // FFT输入和输出：从S[1]开始存放，根据大小自己定义
struct compx s2[FFT_N + 16];  // FFT输入和输出：从S[1]开始存放，根据大小自己定义

struct {
    float Hp;    // 血红蛋白
    float HpO2;  // 氧合血红蛋白
} g_BloodWave;   // 血液波形数据

BloodData g_blooddata = {0};  // 血液数据存储

#define CORRECTED_VALUE 47  // 标定血液氧气含量

/*funcation start ------------------------------------------------------------*/
// 血液检测信息更新
void blood_data_update(void)
{
    max30102_fifo_t fifo;
    // 标志位被使能时 读取FIFO
    g_fft_index = 0;
    while (g_fft_index < FFT_N) {
        if (MAX30102_CheckFIFO() == HAL_OK) {
            if (MAX30102_ReadFIFO2(&fifo) == HAL_OK)
                // 将数据写入fft输入并清除输出
                if (g_fft_index < FFT_N) {
                    // 将数据写入fft输入并清除输出
                    s1[g_fft_index].real = fifo.Red;
                    s1[g_fft_index].imag = 0;
                    s2[g_fft_index].real = fifo.IR;
                    s2[g_fft_index].imag = 0;
                    g_fft_index++;
                }
        }
    }
}

// 血液信息转换
void blood_data_translate(void)
{
    float    n_denom;
    uint16_t i;
    // 4 pt Moving Average

    // 直流滤波
    float dc_red = 0;
    float dc_ir  = 0;
    float ac_red = 0;
    float ac_ir  = 0;

    for (i = 0; i < FFT_N; i++) {
        dc_red += s1[i].real;
        dc_ir += s2[i].real;
    }
    dc_red = dc_red / FFT_N;
    dc_ir  = dc_ir / FFT_N;
    for (i = 0; i < FFT_N; i++) {
        s1[i].real = s1[i].real - dc_red;
        s2[i].real = s2[i].real - dc_ir;
    }

    // 移动平均滤波
    for (i = 1; i < FFT_N - 1; i++) {
        n_denom    = (s1[i - 1].real + 2 * s1[i].real + s1[i + 1].real);
        s1[i].real = n_denom / 4.00;

        n_denom    = (s2[i - 1].real + 2 * s2[i].real + s2[i + 1].real);
        s2[i].real = n_denom / 4.00;
    }
    // 八点平均滤波
    for (i = 0; i < FFT_N - 8; i++) {
        n_denom    = (s1[i].real + s1[i + 1].real + s1[i + 2].real + s1[i + 3].real + s1[i + 4].real + s1[i + 5].real + s1[i + 6].real + s1[i + 7].real);
        s1[i].real = n_denom / 8.00;

        n_denom    = (s2[i].real + s2[i + 1].real + s2[i + 2].real + s2[i + 3].real + s2[i + 4].real + s2[i + 5].real + s2[i + 6].real + s2[i + 7].real);
        s2[i].real = n_denom / 8.00;
    }

    // 开始变换显示
    g_fft_index = 0;
    // 快速傅里叶变换
    FFT(s1);
    FFT(s2);

    // 解平方
    for (i = 0; i < FFT_N; i++) {
        s1[i].real = sqrtf(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag);
        s1[i].real = sqrtf(s2[i].real * s2[i].real + s2[i].imag * s2[i].imag);
    }
    // 计算交流分量
    for (i = 1; i < FFT_N; i++) {
        ac_red += s1[i].real;
        ac_ir += s2[i].real;
    }

    //	for(i = 0;i < 50;i++)
    //	{
    //		if(s1[i].real<=10)
    //			break;
    //	}

    // printf("%d\r\n",(int)i);
    // 读取峰值点的横坐标  结果的物理意义为
    int s1_max_index = find_max_num_index(s1, 30);
    int s2_max_index = find_max_num_index(s2, 30);

    // 检查HbO2和Hb的变化频率是否一致
    //	if(i>=45)
    //	{
    // 心率计算
    //		uint16_t Heart_Rate = 60.00 * SAMPLES_PER_SECOND * s1_max_index / FFT_N;

    float Heart_Rate = 60.00 * ((100.0 * s1_max_index) / 512.00);

    g_blooddata.heart = Heart_Rate;

    // 血氧含量计算
    //  double R = (s2[average].real * s1[0].real)/(s1[s1_max_index].real * s2[0].real);
    //  sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;

    // double R = (s1[s1_max_index].real/s1[0].real)/(s2[s2_max_index].real/s2[0].real);

    // double sp02_num =-16.666666*R*R+ 8.333333 *R + 100;
    // g_blooddata.SpO2 = sp02_num;
    // 血氧含量计算
    // float sp02_num = (s2[s1_max_index].real * s1[0].real)/(s1[s1_max_index].real * s2[0].real);

    // sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;
    //( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max)

    float R          = (ac_ir * dc_red) / (ac_red * dc_ir);
    float sp02_num   = -45.060 * R * R + 30.354 * R + 94.845;
    g_blooddata.SpO2 = sp02_num;

    // 状态正常
    //	}
    //	else //数据发生异常
    //	{
    //		g_blooddata.heart = 0;
    //		g_blooddata.SpO2 	= 0;
    //	}
    // 结束变换显示
}

void blood_Loop(void)
{
    blood_data_update();
    blood_data_translate();
    if (g_blooddata.SpO2 > 99.99) g_blooddata.SpO2 = 99.99;
    printf("HR=%d/min\tSpO2=%.2f\r\n", g_blooddata.heart, g_blooddata.SpO2);
    HAL_Delay(10);
}
