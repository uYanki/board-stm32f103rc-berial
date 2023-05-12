#include "Filters.h"

float v[2];

void DCRemover_init(float* alpha, float* dcw)
{
    *alpha = 0;
    *dcw   = 0;
}
void DCRemover1(float alpha_, float* alpha, float* dcw)
{
    *alpha = alpha_;
    *dcw   = 0;
}

float step(float x, float* alpha, float* dcw)
{
    float olddcw = *dcw;
    *dcw         = x + (*alpha) * (*dcw);
    return *dcw - olddcw;
}

float getDCW(float* dcw)
{
    return *dcw;
}
void FilterBuLp()
{
    v[0] = 0.0;
}

float FBstep(float x)
{
    v[0] = v[1];
    v[1] = (2.452372752527856026e-1 * x) + (0.50952544949442879485 * v[0]);
    return (v[0] + v[1]);
}
