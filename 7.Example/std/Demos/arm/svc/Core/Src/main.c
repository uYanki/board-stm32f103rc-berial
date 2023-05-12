#include "board.h"

#define TAG        "main"
#define DBG_LVL    SDK_DEBUG

// SVC 软中断(最多4个参数)

#define SVC_PARAMS 4

// clang-format off
void __svc(0) do_svc(
    uint32_t r0
#if SVC_PARAMS > 1
    , uint32_t r1
#if SVC_PARAMS > 2
    , uint32_t r2
#if SVC_PARAMS > 3
    , uint32_t r3
#endif
#endif
#endif
);
// clang-format on

int main(void)
{
    board_init();

    // clang-format off
    do_svc(
        0x11223344
#if SVC_PARAMS > 1
        , 0x55667788
#if SVC_PARAMS > 2
        , 0xAABBCCDD
#if SVC_PARAMS > 3
        , 0x12345678
#endif
#endif
#endif
    );
    // clang-format on

    for (;;)
        ;
}
