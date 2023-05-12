#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#define FFT_N 1024      // 定义傅里叶变换的点数
#define START_INDEX 10  // 低频过滤阈值

struct compx  // 定义一个复数结构
{
    float real;
    float imag;
};

// 向下取整
double my_floor(double x);
// 求余运算
double my_fmod(double x, double y);
// 正弦函数
double XSin(double x);
// 余弦函数
double XCos(double x);
// 开平方
int qsqrt(int a);

/*******************************************************************
函数原型：struct compx EE(struct compx b1,struct compx b2)
函数功能：对两个复数进行乘法运算
输入参数：两个以联合体定义的复数a,b
输出参数：a和b的乘积，以联合体的形式输出
*******************************************************************/
struct compx EE(struct compx a, struct compx b);
/*****************************************************************
函数原型：void FFT(struct compx *xin,int N)
函数功能：对输入的复数组进行快速傅里叶变换（FFT）
输入参数：*xin复数结构体组的首地址指针，struct型
*****************************************************************/
void FFT(struct compx* xin);

// 读取峰值
int find_max_num_index(struct compx* data, int count);

typedef struct {
    float w;
    int   init;
    float a;
} DC_FilterData;

// 直流滤波器
int dc_filter(int input, DC_FilterData* df);

typedef struct {
    float v0;
    float v1;
} BW_FilterData;

int bw_filter(int input, BW_FilterData* bw);

#endif /*__ALGORITHM_H*/
