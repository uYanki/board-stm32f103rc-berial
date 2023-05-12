#ifndef __ADCCHART_H__
#define __ADCCHART_H__

#include "base.h"
#include "adc.h"

typedef struct {
    bool bRepaint;
    // callback
    cbk_t lpfnPainter;
    cbk_t lpfnHandler;

    uint8_t nWidth;
    uint8_t nHeight;

    float nCurrentSample;
    float nLastSample;

    char* szTitle;

    bool bRepaintBorder;

    uint8_t chart_x;
} adc_chart_t;

adc_chart_t adcchart_initialize(
    cbk_t lpfnPainter,
    cbk_t lpfnHandler);

float adcchart_get_new_value(adc_chart_t* p);
void  adcchart_repaint_border(adc_chart_t* p);

void adcchart_callback_default_handler(adc_chart_t* p);
void adcchart_callback_default_painter(adc_chart_t* p);

#endif