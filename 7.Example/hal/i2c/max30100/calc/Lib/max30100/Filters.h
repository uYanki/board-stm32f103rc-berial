

#ifndef MAX30100_FILTERS_H
#define MAX30100_FILTERS_H
extern float v[2];

// http://sam-koblenski.blogspot.de/2015/11/everyday-dsp-for-programmers-dc-and.html

typedef struct {
    float alpha;
    float dcw;
} DCRemover;

float FBstep(float x);
void  FilterBuLp(void);
float getDCW(float* dcw);
float step(float x, float* alpha, float* dcw);
void  DCRemover1(float alpha_, float* alpha, float* dcw);
void  DCRemover_init(float* alpha, float* dcw);

#endif
