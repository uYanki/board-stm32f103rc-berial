
#include "btnplus.h"

#define EVENT_CBK(event) \
    if (handle->callback[event]) handle->callback[event]((btn_t*)handle)

// button handle list head.
static btn_t* head_handle = NULL;

/**
 * @brief  Initializes the button struct handle.
 * @param  handle: the button handle strcut.
 * @param  pin_level: read the GPIO of the connet button level.
 * @param  active_level: pressed GPIO level.
 * @retval None
 */
void button_init(btn_t* handle, uint8_t (*pin_level)(), uint8_t active_level)
{
    memset(handle, 0, sizeof(btn_t));
    handle->event        = (uint8_t)EVENT_NONE;
    handle->read_Level   = pin_level;
    handle->button_level = handle->read_Level();
    handle->active_level = active_level;
}

/**
 * @brief  Attach the button event callback function.
 * @param  handle: the button handle strcut.
 * @param  event: trigger event type.
 * @param  callback: callback function.
 * @retval None
 */
void button_attach(btn_t* handle, btn_event event, btn_cbk callback)
{
    handle->callback[event] = callback;
}

/**
 * @brief  Inquire the button event happen.
 * @param  handle: the button handle strcut.
 * @retval button event.
 */
btn_event button_event(btn_t* handle)
{
    return (btn_event)(handle->event);
}

/**
 * @brief  button driver core function, driver state machine.
 * @param  handle: the button handle strcut.
 * @retval None
 */
void button_handler(btn_t* handle)
{
    uint8_t read_gpio_level = handle->read_Level();

    // ticks counter working..
    if ((handle->state) > 0) ++handle->ticks;

    /*------------button debounce handle---------------*/
    if (read_gpio_level != handle->button_level)  // not equal to prev one
    {
        // continue read 3 times same new level change
        if (++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
            handle->button_level = read_gpio_level;
            handle->debounce_cnt = 0;
        }
    } else  // leved not change ,counter reset.
    {
        handle->debounce_cnt = 0;
    }

    /*-----------------State machine-------------------*/
    switch (handle->state) {
        case 0:
            if (handle->button_level == handle->active_level)  // start press down
            {
                handle->event = (uint8_t)EVENT_PRESS;
                EVENT_CBK(EVENT_PRESS);
                handle->ticks  = 0;
                handle->repeat = 1;
                handle->state  = 1;
            } else {
                handle->event = (uint8_t)EVENT_NONE;
            }

            break;

        case 1:
            if (handle->button_level != handle->active_level)  // released press up
            {
                handle->event = (uint8_t)EVENT_RELEASE;
                EVENT_CBK(EVENT_RELEASE);
                handle->ticks = 0;
                handle->state = 2;

            } else if (handle->ticks > LONG_TICKS) {
                handle->event = (uint8_t)EVENT_LONG_RRESS;
                EVENT_CBK(EVENT_LONG_RRESS);
                handle->state = 5;
            }

            break;

        case 2:
            if (handle->button_level == handle->active_level)  // press down again
            {
                handle->event = (uint8_t)EVENT_PRESS;
                EVENT_CBK(EVENT_PRESS);
                ++handle->repeat;
                EVENT_CBK(EVENT_REPEAT_PRESS);  // repeat hit
                handle->ticks = 0;
                handle->state = 3;
            } else if (handle->ticks > SHORT_TICKS)  // released timeout
            {
                if (handle->repeat == 1) {
                    handle->event = (uint8_t)EVENT_CLICK;
                    EVENT_CBK(EVENT_CLICK);
                } else if (handle->repeat == 2) {
                    handle->event = (uint8_t)EVENT_DOUBLE_CLICK;
                    EVENT_CBK(EVENT_DOUBLE_CLICK);  // repeat hit
                }

                handle->state = 0;
            }

            break;

        case 3:
            if (handle->button_level != handle->active_level)  // released press up
            {
                handle->event = (uint8_t)EVENT_RELEASE;
                EVENT_CBK(EVENT_RELEASE);

                if (handle->ticks < SHORT_TICKS) {
                    handle->ticks = 0;
                    handle->state = 2;  // repeat press
                } else {
                    handle->state = 0;
                }
            }

            break;

        case 5:
            if (handle->button_level == handle->active_level) {
                // continue hold trigger
                handle->event = (uint8_t)EVENT_HOLDING;
                EVENT_CBK(EVENT_HOLDING);

            } else  // releasd
            {
                handle->event = (uint8_t)EVENT_RELEASE;
                EVENT_CBK(EVENT_RELEASE);
                handle->state = 0;  // reset
            }

            break;
    }
}

/**
 * @brief  Start the button work, add the handle into work list.
 * @param  handle: target handle strcut.
 * @retval 0: succeed. -1: already exist.
 */
int button_start(btn_t* handle)
{
    btn_t* target = head_handle;

    while (target) {
        if (target == handle) return -1;  // already exist.

        target = target->next;
    }

    handle->next = head_handle;
    head_handle  = handle;
    return 0;
}

/**
 * @brief  Stop the button work, remove the handle off work list.
 * @param  handle: target handle strcut.
 * @retval None
 */
void button_stop(btn_t* handle)
{
    btn_t** curr;

    for (curr = &head_handle; *curr;) {
        btn_t* entry = *curr;

        if (entry == handle) {
            *curr = entry->next;
        } else
            curr = &entry->next;
    }
}

/**
 * @brief  background ticks, timer repeat invoking interval 5ms.
 * @param  None.
 * @retval None
 */
void button_ticks()
{
    btn_t* target;

    for (target = head_handle; target; target = target->next) {
        button_handler(target);
    }
}
