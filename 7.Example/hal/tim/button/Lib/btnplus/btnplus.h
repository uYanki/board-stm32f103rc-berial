#ifndef __BUTTON_PLUS_H__
#define __BUTTON_PLUS_H__

#include "stdint.h"
#include "string.h"

#define TICKS_INTERVAL 5  // ms
#define DEBOUNCE_TICKS 3  // MAX 8
#define SHORT_TICKS    (300 / TICKS_INTERVAL)
#define LONG_TICKS     (1000 / TICKS_INTERVAL)

typedef void (*btn_cbk)(void*);

typedef enum {
    EVENT_PRESS = 0,
    EVENT_RELEASE,
    EVENT_REPEAT_PRESS,
    EVENT_CLICK,         // SINGLE
    EVENT_DOUBLE_CLICK,  // DOUBLE
    EVENT_LONG_RRESS,
    EVENT_HOLDING,
    count_of_event,
    EVENT_NONE
} btn_event;

typedef struct button {
    uint16_t ticks;
    uint8_t  repeat : 4;
    uint8_t  event : 4;  // current event
    uint8_t  state : 3;
    uint8_t  debounce_cnt : 3;
    uint8_t  active_level : 1;
    uint8_t  button_level : 1;

    uint8_t (*read_Level)(void);
    btn_cbk callback[count_of_event];

    struct button* next;
} btn_t;

#ifdef __cplusplus
extern "C" {
#endif

void      button_init(btn_t* handle, uint8_t (*pin_level)(), uint8_t active_level);
void      button_attach(btn_t* handle, btn_event event, btn_cbk callback);
btn_event button_event(btn_t* handle);
int       button_start(btn_t* handle);
void      button_stop(btn_t* handle);
void      button_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
