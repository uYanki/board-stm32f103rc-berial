#include "matrixkey.h"

void matrix_key_write(uint8_t index, key_state_t state)
{
    switch (index) {
        case 0: HAL_GPIO_WritePin(KEY_C1_GPIO_Port, KEY_C1_Pin, state); break;
        case 1: HAL_GPIO_WritePin(KEY_C2_GPIO_Port, KEY_C2_Pin, state); break;
        case 2: HAL_GPIO_WritePin(KEY_C3_GPIO_Port, KEY_C3_Pin, state); break;
        case 3: HAL_GPIO_WritePin(KEY_C4_GPIO_Port, KEY_C4_Pin, state); break;
    }
}

key_state_t matrix_key_read(uint8_t index)
{
    switch (index) {
        case 0: return HAL_GPIO_ReadPin(KEY_R1_GPIO_Port, KEY_R1_Pin);
        case 1: return HAL_GPIO_ReadPin(KEY_R2_GPIO_Port, KEY_R2_Pin);
        case 2: return HAL_GPIO_ReadPin(KEY_R3_GPIO_Port, KEY_R3_Pin);
        case 3: return HAL_GPIO_ReadPin(KEY_R4_GPIO_Port, KEY_R4_Pin);
    }
    return KEY_STATE_RELEASED;
}

//////////////////////////////////////////////////////////

void matrix_key_init()
{
    for (uint8_t i = 0; i < MATRIX_KEY_COL_COUNT; ++i)
        matrix_key_write(i, KEY_STATE_RELEASED);
}

key_state_t matrix_key_scan(uint8_t* row, uint8_t* col)
{
    uint8_t c, r;
    for (c = 0; c < MATRIX_KEY_COL_COUNT; ++c) {
        // set column state
        matrix_key_write(c, KEY_STATE_PRESSED);
        for (r = 0; r < MATRIX_KEY_ROW_COUNT; ++r) {
            // get row state

            if (matrix_key_read(r) == KEY_STATE_PRESSED) {
                HAL_Delay(10);
                // eliminate jitter
                if (matrix_key_read(r) == KEY_STATE_PRESSED) {
                    // wait for release

                    while (matrix_key_read(r) == KEY_STATE_PRESSED) {}

                    // reset column state
                    matrix_key_write(c, KEY_STATE_RELEASED);
                    // return result
                    *row = ++r, *col = ++c;
                    return KEY_STATE_PRESSED;
                }
            }
        }
        // reset column state
        matrix_key_write(c, KEY_STATE_RELEASED);
    }
    // return result
    *row = *col = 0;
    return KEY_STATE_RELEASED;
}
