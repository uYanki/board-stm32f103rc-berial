
#ifndef __ROTARY_ENCODER_H__
#define __ROTARY_ENCODER_H__

#include "board.h"

typedef uint8_t (*read_pin_level)(void);

typedef enum {
    ROTARY_ENCODER_INCREMENT,  // increment
    ROTARY_ENCODER_DECREMENT,  // decrement
    ROTARY_ENCODER_UNCHANGE    // unchange
} re_dir_e;

typedef enum {
    ROTARY_ENCODER_MODE_0,
    ROTARY_ENCODER_MODE_1
} re_mode_e;

typedef struct {
    // public_dat
    int32_t  abs, diff;
    re_dir_e dir;

    // public_cfg
    re_mode_e mode;

    // public_cbk
    read_pin_level read_a;
    read_pin_level read_b;

    // private_dat
    uint8_t last_a;
    int32_t count;
} rotary_encoder_t;

void     rotary_encoder_init(rotary_encoder_t* encoder, re_mode_e mode, read_pin_level read_a, read_pin_level read_b);
void     rotary_encoder_handler(rotary_encoder_t* encoder);
re_dir_e rotary_encoder_get(rotary_encoder_t* encoder);

#endif
