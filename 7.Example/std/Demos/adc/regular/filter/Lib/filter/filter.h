#ifndef __FILTER_H__
#define __FILTER_H__

#include <stdint.h>

uint16_t amplitude_limiter_filter(uint16_t new_value, uint16_t old_value, uint16_t limit);
uint16_t middle_value_filter(uint16_t* buf, uint16_t len);
uint16_t arithmetical_average_value_filter(uint16_t* buf, uint16_t len);
uint16_t glide_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value);
uint16_t middle_average_value_filter(uint16_t* buf, uint16_t len);
uint16_t glide_middle_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value);
uint16_t limit_glide_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value, uint16_t limit);
uint16_t weighted_glide_average_value_filter(uint16_t* buf, uint16_t len, uint16_t new_value, uint16_t* coe);

#endif
