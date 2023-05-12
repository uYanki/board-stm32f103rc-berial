#include "board.h"

#include "arm_math.h"
#include "arm_const_structs.h"

#define SAMPLES_SIZE 256

// #define SAMPLES_SIZE 512

typedef struct {
    uint16_t data[SAMPLES_SIZE];
} frame_t;

int main()
{
    uint16_t i, j;

    uint8_t* p;
    frame_t  frame;

    board_init();

    p = (uint8_t*)&frame;
    for (i = 0; i < SAMPLES_SIZE; ++i) {
        frame.data[i] = arm_sin_q15(i * 128);
    }

    while (1) {
        // 帧同步
        if (getchar() == 13) {
            putchar(13);
            // 传输数据
            for (i = 0; i < 5; ++i, j += 2) {
                if (j == SAMPLES_SIZE) j = 0;
                putchar(p[j + 0]);
                putchar(p[j + 1]);
            }
        }
    }

    return 0;
}
