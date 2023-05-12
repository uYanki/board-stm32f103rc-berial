
#include "button.h"

#define TAG     "button"
#define DBG_LVL SDK_ERROR

static struct button* linklist = NULL;  // 链表头

static char* strncpy(char* dst, const char* src, uint32_t n);

// 按键创建
void button_create(const char* name, button_t* btn, uint8_t (*read_level)(void), uint8_t trigger_level)
{
    ASSERT(btn);

    strncpy(btn->name, name, BTN_NAME_MAX);  // 按键名
    btn->current_state = NONE_TRIGGER;       // 状态
    btn->last_state    = NONE_TRIGGER;       // 上次状态
    btn->trigger_event = NONE_TRIGGER;       // 触发事件
    btn->read_level    = read_level;         // 读电平
    btn->trigger_level = trigger_level;      // 触发电平
    btn->current_level = btn->read_level();  // 当前电平
    btn->t_debounce    = 0;

    // 添加至单链表
    btn->next = linklist;
    linklist  = btn;
}

// 事件绑定
void button_attach(button_t* btn, button_event_e event, button_cbk_t callback)
{
    uint8_t i;

    ASSERT(btn);

    if (BUTTON_ALL_RIGGER == event) {
        for (i = 0; i < number_of_event - 1; i++)
            btn->callback[i] = callback;
    } else {
        btn->callback[event] = callback;
    }
}

void button_delete(button_t* btn)
{
    struct button** curr;
    for (curr = &linklist; *curr;) {
        struct button* entry = *curr;
        if (entry == btn) {
            *curr = entry->next;
        } else {
            curr = &entry->next;
        }
    }
}

// 获取触发的事件
uint8_t button_get_event(button_t* btn)
{
    return (uint8_t)(btn->trigger_event);
}

// 获取当前状态
uint8_t button_get_state(button_t* btn)
{
    return (uint8_t)(btn->current_state);
}

// 单按键处理
void button_process(button_t* btn)
{
    uint8_t current_level = (uint8_t)btn->read_level();                                          // 获取当前按键电平
    if ((current_level != btn->current_level) && (++(btn->t_debounce) >= BUTTON_DEBOUNCE_TIME))  // 按键电平发生变化，消抖
    {
        btn->current_level = current_level;  // 更新当前按键电平
        btn->t_debounce    = 0;              // 确定了是按下
        // 如果按键是没被按下的，改变按键状态为按下(首次按下/双击按下)
        if ((btn->current_state == NONE_TRIGGER) || (btn->current_state == BUTTON_DOUBLE)) {
            btn->current_state = BUTTON_DOWM;
        }
        // 释放按键
        else if (btn->current_state == BUTTON_DOWM) {
            btn->current_state = BUTTON_UP;
            TRIGGER_CB(BUTTON_UP);  // 触发释放
            LOGD("release");
        }
    }
    switch (btn->current_state) {
        case BUTTON_DOWM:  // 按下状态
        {
            if (btn->current_level == btn->trigger_level)  // 按键按下
            {
#if CONTINUOS_TRIGGER  // 支持连续触发
                if (++(btn->t_cycle) >= BUTTON_CONTINUOS_CYCLE) {
                    btn->t_cycle       = 0;
                    btn->trigger_event = BUTTON_CONTINUOS;
                    TRIGGER_CB(BUTTON_CONTINUOS);  // 连按
                    LOGD("continuos press");
                }
#else
                btn->trigger_event = BUTTON_DOWM;
                if (++(btn->t_press) >= BUTTON_LONG_TIME)  // 释放按键前更新触发事件为长按
                {
#if LONG_FREE_TRIGGER
                    btn->trigger_event = BUTTON_LONG;
#else
                    if (++(btn->t_cycle) >= BUTTON_LONG_CYCLE)  // 连续触发长按的周期
                    {
                        btn->t_cycle       = 0;
                        btn->trigger_event = BUTTON_LONG;
                        TRIGGER_CB(BUTTON_LONG);  // 长按
                    }
#endif
                    if (btn->t_press == 0xFF)  // 更新时间溢出
                    {
                        btn->t_press = BUTTON_LONG_TIME;
                    }
                    LOGD("long press");
                }
#endif
            }
            break;
        }
        case BUTTON_UP:  // 弹起状态
        {
            if (btn->trigger_event == BUTTON_DOWM)  // 触发单击
            {
                if ((btn->t_count <= BUTTON_DOUBLE_TIME) && (btn->last_state == BUTTON_DOUBLE))  // 双击
                {
                    btn->trigger_event = BUTTON_DOUBLE;
                    TRIGGER_CB(BUTTON_DOUBLE);
                    LOGD("double press");
                    btn->current_state = NONE_TRIGGER;
                    btn->last_state    = NONE_TRIGGER;
                } else {
                    btn->t_count = 0;
                    btn->t_press = 0;  // 检测长按失败，清0
#if (SINGLE_AND_DOUBLE_TRIGGER == 0)
                    TRIGGER_CB(BUTTON_DOWM);  // 单击
#endif
                    btn->current_state = BUTTON_DOUBLE;
                    btn->last_state    = BUTTON_DOUBLE;
                }
            } else if (btn->trigger_event == BUTTON_LONG) {
#if LONG_FREE_TRIGGER
                TRIGGER_CB(BUTTON_LONG);  // 长按
#else
                TRIGGER_CB(BUTTON_LONG_FREE);  // 长按释放
#endif
                btn->t_press       = 0;
                btn->current_state = NONE_TRIGGER;
                btn->last_state    = BUTTON_LONG;
            }
#if CONTINUOS_TRIGGER
            else if (btn->trigger_event == BUTTON_CONTINUOS)  // 连按
            {
                btn->t_press = 0;
                TRIGGER_CB(BUTTON_CONTINUOS_FREE);  // 连发释放
                btn->current_state = NONE_TRIGGER;
                btn->last_state    = BUTTON_CONTINUOS;
            }
#endif
            break;
        }
        case BUTTON_DOUBLE: {
            btn->t_count++;  // 时间记录
            if (btn->t_count >= BUTTON_DOUBLE_TIME) {
                btn->current_state = NONE_TRIGGER;
                btn->last_state    = NONE_TRIGGER;
            }
#if SINGLE_AND_DOUBLE_TRIGGER
            if ((btn->t_count >= BUTTON_DOUBLE_TIME) && (btn->last_state != BUTTON_DOWM)) {
                btn->t_count = 0;
                TRIGGER_CB(BUTTON_DOWM);  // 单击
                btn->current_state = NONE_TRIGGER;
                btn->last_state    = BUTTON_DOWM;
            }
#endif
            break;
        }
        default:
            break;
    }
}

// 周期扫描(建议20~50ms调用1次)
void button_cycle_process(void)
{
    struct button* pass_btn;
    for (pass_btn = linklist; pass_btn != NULL; pass_btn = pass_btn->next) {
        button_process(pass_btn);
    }
}

// 输出所有按键名称
void button_enumrate_name(void)
{
    button_t* pass_btn;
    for (pass_btn = linklist; pass_btn != NULL; pass_btn = pass_btn->next) {
        LOGI("%s", pass_btn->name);
    }
}

/** @brief 拷贝指定长度字符串 */
static char* strncpy(char* dst, const char* src, uint32_t n)
{
    if (n != 0) {
        char*       d = dst;
        const char* s = src;
        do {
            if ((*d++ = *s++) == 0) {
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }
    return (dst);
}
