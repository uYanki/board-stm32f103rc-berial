#ifndef BUTTON_H
#define BUTTON_H

#include "board.h"

#define BTN_NAME_MAX              32  // 名字最大为32字节

/* 按键消抖时间40ms, 建议调用周期为20ms
 只有连续检测到40ms状态不变才认为有效，包括弹起和按下两种事件
*/

#define CONTINUOS_TRIGGER         0  // 是否支持连续触发，连发的话就不要检测单双击与长按了

/* 是否支持单击&双击同时存在触发，如果选择开启宏定义的话，单双击都回调，只不过单击会延迟响应，
   因为必须判断单击之后是否触发了双击否则，延迟时间是双击间隔时间 BUTTON_DOUBLE_TIME。
   而如果不开启这个宏定义，建议工程中只存在单击/双击中的一个，否则，在双击响应的时候会触发一次单击，
   因为双击必须是有一次按下并且释放之后才产生的 */
#define SINGLE_AND_DOUBLE_TRIGGER 1

/* 是否支持长按释放才触发，如果打开这个宏定义，那么长按释放之后才触发单次长按，
   否则在长按指定时间就一直触发长按，触发周期由 BUTTON_LONG_CYCLE 决定 */
#define LONG_FREE_TRIGGER         0

#define BUTTON_DEBOUNCE_TIME      2   // 消抖时间          (n-1)*调用周期
#define BUTTON_CONTINUOS_CYCLE    1   // 连按触发周期时间  (n-1)*调用周期
#define BUTTON_LONG_CYCLE         1   // 长按触发周期时间  (n-1)*调用周期
#define BUTTON_DOUBLE_TIME        15  // 双击间隔时间      (n-1)*调用周期  建议在200-600ms
#define BUTTON_LONG_TIME          50  /* 持续n秒 ((n-1)*调用周期 ms)，认为长按事件 */

#define TRIGGER_CB(event)     \
    if (btn->callback[event]) \
    btn->callback[event]((button_t*)btn)

typedef void (*button_cbk_t)(void*);  // 按键事件回调

typedef enum {
    BUTTON_DOWM = 0,
    BUTTON_UP,
    BUTTON_DOUBLE,
    BUTTON_LONG,
    BUTTON_LONG_FREE,
    BUTTON_CONTINUOS,
    BUTTON_CONTINUOS_FREE,
    BUTTON_ALL_RIGGER,
    number_of_event,
    NONE_TRIGGER
} button_event_e;

typedef struct button {
    uint8_t (*read_level)(void);  // 读按键电平

    char name[BTN_NAME_MAX];

    uint8_t current_state : 4;  // 当前状态 current state, 按下或弹起
    uint8_t last_state    : 4;  // 上次状态 last state, 双击判断
    uint8_t trigger_level : 2;  // 触发电平 trigger level
    uint8_t current_level : 2;  // 当前电平 current level

    uint8_t trigger_event;  // 当前触发的事件

    button_cbk_t callback[number_of_event];

    uint8_t t_cycle;  // 连续按键周期

    uint8_t t_count;     // 计时
    uint8_t t_debounce;  // 消抖时间

    uint8_t t_press;  // 按键持续按下时间

    struct button* next;

} button_t;

void button_create(const char* name, button_t* btn, uint8_t (*read_level)(void), uint8_t trigger_level);
void button_attach(button_t* btn, button_event_e btn_event, button_cbk_t btn_callback);
void button_process(button_t* btn);
void button_cycle_process(void);
void button_delete(button_t* btn);

void button_enumrate_name(void);

uint8_t button_get_event(button_t* btn);
uint8_t button_get_state(button_t* btn);

#if 0

// 通用回调

void button_process_callback(void* btn)
{
    uint8_t event = button_get_event(btn);

    switch (event) {
        case BUTTON_DOWM: {
            break;
        }
        case BUTTON_UP: {
            break;
        }
        case BUTTON_DOUBLE: {
            break;
        }
        case BUTTON_LONG: {
            break;
        }
        case BUTTON_LONG_FREE: {
            break;
        }
        case BUTTON_CONTINUOS: {
            break;
        }
        case BUTTON_CONTINUOS_FREE: {
            break;
        }
    }
}

#endif

#endif
