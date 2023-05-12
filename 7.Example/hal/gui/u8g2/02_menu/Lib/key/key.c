
#include "key.h"

key_id_t    key_id;  // current
key_state_t keys[KEY_ID_END];

bool key_val(key_id_t id)
{
    switch (id) {
        case KEY_ID_OK: return HAL_GPIO_ReadPin(KEY_C1_GPIO_Port, KEY_C1_Pin) == GPIO_PIN_RESET;    // b
        case KEY_ID_PREV: return HAL_GPIO_ReadPin(KEY_C3_GPIO_Port, KEY_C3_Pin) == GPIO_PIN_RESET;  // d
        case KEY_ID_NEXT: return HAL_GPIO_ReadPin(KEY_C2_GPIO_Port, KEY_C2_Pin) == GPIO_PIN_RESET;  // c
        default: break;
    }
    return false;
}

void key_init(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(keys); ++i)
        keys[i].cur = keys[i].last = key_val(i + KEY_ID_START);
}

key_id_t key_scan(void)
{
    for (key_id_t i = 0; i <= ARRAY_SIZE(keys); ++i) {
        keys[i].cur = key_val(i + KEY_ID_START);
        if (keys[i].last != keys[i].cur) {
            keys[i].last = keys[i].cur;
            printv("%d", keys[i].cur);
            if (keys[i].cur)
                return key_id = i + KEY_ID_START;  // key pressed
        }
    }
    return KEY_ID_NONE;
}
