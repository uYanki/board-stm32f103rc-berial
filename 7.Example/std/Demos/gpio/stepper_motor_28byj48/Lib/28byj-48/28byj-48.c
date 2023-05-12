#include "28byj-48.h"

#define CYCLE_ANGLE       360
#define STEPPER_ONE_CYCLE 64
#define APP_TASK_PRIORITY 5
#define CYCLE_STEP        8
#define STEP_ANGLE        5.625

#define PIN_A             PBout(12)
#define PIN_B             PBout(13)
#define PIN_C             PBout(15)
#define PIN_D             PCout(6)

void stepper_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  // 推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// Rotate [target] cycle [dir,false:counterclockwise,true:clockwise] in [time] ms
void stepper_docycle(uint32_t target, bool dir)
{
    static uint8_t CCW[CYCLE_STEP] = {0b1110, 0b1100, 0b1101, 0b1001, 0b1011, 0b0011, 0b0111, 0b0110};  // Counterclockwise beat
    static uint8_t CW[CYCLE_STEP]  = {0b0110, 0b0111, 0b0011, 0b1011, 0b1001, 0b1101, 0b1100, 0b1110};  // Clockwise beat

    uint8_t* work = dir ? CCW : CW;

    for (int cycle = 0; cycle < target; cycle++) {
        for (int j = 0; j < CYCLE_STEP; ++j) {
            for (int i = 0; i < CYCLE_STEP; ++i) {
                PIN_A = (work[i] & (1 << 0)) ? 1 : 0;
                PIN_B = (work[i] & (1 << 1)) ? 1 : 0;
                PIN_C = (work[i] & (1 << 2)) ? 1 : 0;
                PIN_D = (work[i] & (1 << 3)) ? 1 : 0;
                delay_ms(1);  // minimum response time
            }
        }
    }

    PIN_A = PIN_B = PIN_C = PIN_D = 0;
}

/**
 * @param angle relative angle
 * @param dir false:counterclockwise; true:clockwise;
 */
void steeper_rotate(uint32_t angle, bool dir)
{
    stepper_docycle(angle * STEPPER_ONE_CYCLE / CYCLE_ANGLE, dir);
}
