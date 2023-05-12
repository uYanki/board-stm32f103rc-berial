#include "adcchart.h"

adc_chart_t adcchart_initialize(
    cbk_t lpfnPainter,
    cbk_t lpfnHandler)
{
    adc_chart_t m = {
        .lpfnPainter    = lpfnPainter == 0 ? adcchart_callback_default_painter : lpfnPainter,
        .lpfnHandler    = lpfnHandler == 0 ? adcchart_callback_default_handler : lpfnHandler,
        .nWidth         = CONFIG_SCREEN_WIDTH,
        .nHeight        = CONFIG_SCREEN_HEIGHT,
        .nCurrentSample = 0,
        .nLastSample    = 0,
        .szTitle        = "Real time angle :",
        .bRepaintBorder = true,
        .bRepaint       = true,
        .chart_x        = 0,
    };
    return m;
}

void adcchart_callback_default_handler(adc_chart_t* p)
{
    println("def");

    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default: {
            break;
        }
    }
}

void adcchart_repaint_border(adc_chart_t* p)
{
    p->bRepaintBorder = true;
}

void adcchart_drawborder(adc_chart_t* p)
{
    if (p->bRepaintBorder) {
        p->bRepaintBorder = false;

        _clear();

        _draw_str(4, 12, p->szTitle);

        _draw_rounded_rect(4, 18, 120, 46, 8);

        _set_color(2);

        // 轴

        _draw_hline(10, 58, 108);
        _draw_vline(10, 24, 34);

        // 箭头
        _draw_pixel(7, 27);
        _draw_pixel(8, 26);
        _draw_pixel(9, 25);

        _draw_pixel(116, 59);
        _draw_pixel(115, 60);
        _draw_pixel(114, 61);

        _set_color(1);

        p->nLastSample = adcchart_get_new_value(&p);
    }
}

float adcchart_get_new_value(adc_chart_t* p)
{
    HAL_ADC_Start(&hadc1);
    return (float)abs(HAL_ADC_GetValue(&hadc1) - 3700) / 10 + 30;
}

void adcchart_callback_default_painter(adc_chart_t* p)
{
    adcchart_drawborder(p);

    _draw_rect(96, 0, 30, 14);

    _draw_vline(p->chart_x + 10, 59, 3);
    if (p->chart_x == 100) p->chart_x = 0;

    _draw_vline(p->chart_x + 11, 24, 34);
    _draw_vline(p->chart_x + 12, 24, 34);
    _draw_vline(p->chart_x + 13, 24, 34);
    _draw_vline(p->chart_x + 14, 24, 34);

    // 异或绘制
    _set_color(2);
    p->nCurrentSample = adcchart_get_new_value(&p);
    _draw_line(p->chart_x + 11, 58 - (int)(p->nLastSample) / 2, p->chart_x + 12, 58 - (int)(p->nCurrentSample) / 2);
    _draw_vline(p->chart_x + 12, 59, 3);
    p->nLastSample = p->nCurrentSample;
    p->chart_x += 2;
    _draw_rect(96, 0, 30, 14);
    _set_color(1);

    _draw_num(96, 12, "%.2f", p->nCurrentSample);

    _update();

    p->bRepaint = true;
}
