#include "rotary_encoder.h"

void rotary_encoder_init(rotary_encoder_t* encoder, re_mode_e mode, read_pin_level read_a, read_pin_level read_b)
{
    encoder->mode = mode;

    encoder->read_a = read_a;
    encoder->read_b = read_b;

    encoder->count  = 0;
    encoder->diff   = 0;
    encoder->abs    = 0;
    encoder->last_a = 1;
}

void rotary_encoder_handler(rotary_encoder_t* encoder) // 这个方法不太好
{
    uint8_t now_a = encoder->read_a();
    uint8_t now_b = encoder->read_b();

    if (encoder->last_a != now_a) {
        if ((encoder->last_a = now_a) == 0) {
            if (encoder->mode == ROTARY_ENCODER_MODE_0) {
                now_b ? --encoder->count : ++encoder->count;
            } else {
                now_b ? ++encoder->count : --encoder->count;
            }
        }
    }
}

re_dir_e rotary_encoder_get(rotary_encoder_t* encoder)
{
    encoder->diff = encoder->count - encoder->abs;
    encoder->abs  = encoder->count;

    if (encoder->diff < 0) {
        encoder->dir = ROTARY_ENCODER_DECREMENT;
    } else if (encoder->diff > 0) {
        encoder->dir = ROTARY_ENCODER_INCREMENT;
    } else {
        encoder->dir = ROTARY_ENCODER_UNCHANGE;
    }

    return encoder->dir;
}
