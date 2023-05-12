#include "board.h"

#include "EventRecorder.h"

#define TAG          "Event Recorder"
#define DBG_LVL      SDK_DEBUG

/** TAG
 * E = 0x45
 * v = 0x76
 * e = 0x65
 * n = 0x6e
 * t = 0x74
 *   = 0x20
 * R = 0x52
 * e = 0x65
 * c = 0x63
 * o = 0x6f
 * r = 0x72
 * d = 0x64
 * e = 0x65
 * r = 0x72
 */

#define WKUP_PRESSED SET

/*
// 定时精度测量
void TIMx_IRQHandler( void )
{
    if(TIM_GetITStatus(TIMx, TIM_IT_Update) == SET)
    {
        EventStopC(0);
        EventStartC(0);
        TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    }
}
*/

int main(void)
{
    uint8_t  last_key = 0, curr_key = 0;
    uint32_t t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0;

    EventRecorderInitialize(EventRecordAll, 1U);
    EventRecorderStart();

    board_init();

    // EventStartC(0);

    timer_init(2, 7200, 2e4);  // 2s

    for (;;) {
        if (timer_check(2)) {
            // no delay
            EventStartA(0);
            EventStopA(0);

            // delay 100us
            EventStartA(2);
            delay_us(100);
            EventStopA(2);

            // delay 50ms
            EventStartA(3);
            delay_ms(50);
            EventStopA(3);

            // delay 300ms
            EventStartB(4);
            delay_ms(300);
            EventStopB(4);

            ++t0;
            EventStartAv(5, t0, t0);
            delay_ms(30);
            EventStopAv(5, t0, t0);
        }

        curr_key = GPIO_ReadInputDataBit(KEY_GPIO_Port, KEY_GPIO_Pin);

        if (last_key != curr_key) {
            if (WKUP_PRESSED == (last_key = curr_key)) {
                t1 += 1;
                t2 += 2;
                EventRecord2(1 + EventLevelAPI, t1, t2);  // 数值*2
                t3 += 3;
                t4 += 4;
                EventRecord4(2 + EventLevelOp, t1, t2, t3, t4);       // 数值*4
                EventRecordData(3 + EventLevelOp, TAG, sizeof(TAG));  // 字符串
            }
        }
    }
}
