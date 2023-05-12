#include "filter.h"

uint16_t amplitude_limiter_filter(uint16_t new_value, uint16_t old_value, uint16_t limit)
{
    if ((new_value - old_value) > limit || ((old_value - new_value) < limit)) {
        return old_value;
    } else {
        return new_value;
    }
}

uint16_t middle_value_filter(uint16_t* buf, uint16_t len)
{
    uint16_t i, j;
    uint16_t tmp = 0;

    /* 冒泡排序 */
    for (j = 0; j < len - 1; ++j) {
        for (i = 0; i < len - 1 - j; ++i) {
            if (buf[i] > buf[i + 1]) {
                tmp        = buf[i];
                buf[i]     = buf[i + 1];
                buf[i + 1] = tmp;
            }
        }
    }

    return buf[(len - 1) >> 1];
}

uint16_t arithmetical_average_value_filter(uint16_t* buf, uint16_t len)
{
    uint16_t i;
    uint32_t sum = 0;

    for (i = 0; i < len; i++) {
        sum += buf[i];
    }

    return (sum + len / 2) / len;
}

uint16_t glide_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value)
{
    uint16_t i;
    uint32_t sum = 0;

    for (i = 0; i < len - 1; i++) {
        buf[i] = buf[i + 1];
        sum += buf[i];
    }
    buf[len - 1] = new_value;
    sum += buf[len - 1];

    return (sum + len / 2) / len;
}

uint16_t middle_average_value_filter(uint16_t* buf, uint16_t len)
{
    uint16_t tmp;
    uint32_t sum = 0;
    uint16_t i, j;

    /* 冒泡排序 */
    for (j = 0; j < len - 1; ++j) {
        for (i = 0; i < len - 1 - j; ++i) {
            if (buf[i] > buf[i + 1]) {
                tmp        = buf[i];
                buf[i]     = buf[i + 1];
                buf[i + 1] = tmp;
            }
        }
    }

    for (i = 1; i < len - 1; i++) {
        sum += buf[i];
    }

    return (sum + (len - 2) / 2) / (len - 2);
}

uint16_t glide_middle_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value)
{
    uint16_t min;
    uint16_t max;
    uint32_t sum;
    uint16_t i;

    sum = max = min = new_value;

    for (i = 0; i < len - 1; i++) {
        buf[i] = buf[i + 1];
        sum += buf[i];
        if (buf[i] > max)
            max = buf[i];
        else if (buf[i] < min)
            min = buf[i];
    }

    buf[len - 1] = new_value;
    sum          = sum - max - min;

    return (sum + (len - 2) / 2) / (len - 2);
}

uint16_t limit_glide_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value, uint16_t limit)
{
    uint16_t i;
    uint32_t sum = 0;

    if (((new_value - buf[len - 1]) > limit) ||
        ((buf[len - 1] - new_value) > limit)) {
        new_value = buf[len - 1];
    }

    for (i = 0; i < len - 1; i++) {
        buf[i] = buf[i + 1];
        sum += buf[i];
    }

    buf[len - 1] = new_value;
    sum += buf[len - 1];

    return (sum + len / 2) / len;

    // return glide_average_value_filter(buf, len, new_value);
}

uint16_t weighted_glide_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value, uint16_t* coe)
{
    uint16_t i;
    uint32_t sum     = 0;
    uint32_t coe_sum = 0;

    for (i = 0; i < len - 1; i++) {
        buf[i] = buf[i + 1];
        sum += buf[i] * coe[i];
        coe_sum += coe[i];
    }

    buf[len - 1] = new_value;
    sum += buf[len - 1] * coe[len - 1];
    coe_sum += coe[len - 1];

    sum = (sum + coe_sum / 2) / coe_sum;

    return (sum + len / 2) / len;
}

#if 0

#define ELIMINATE_DITHERING_TIMES 10

uint16_t eliminate_dithering_filter(uint16_t new_value, uint16_t old_value)
{
    uint16_t i;
    for (i = 0; i < ELIMINATE_DITHERING_TIMES; i++) {
        /* code */
    }
}

#endif