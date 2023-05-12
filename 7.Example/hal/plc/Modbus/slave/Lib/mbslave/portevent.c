
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Variables ----------------------------------------*/
static eMBEventType eQueuedEvent;
static BOOL         xEventInQueue;

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortEventInit(void)
{
    xEventInQueue = FALSE;
    return TRUE;
}

BOOL xMBPortEventPost(eMBEventType eEvent)
{
    xEventInQueue = TRUE;
    eQueuedEvent  = eEvent;
    return TRUE;
}

BOOL xMBPortEventGet(eMBEventType* eEvent)
{
    BOOL xEventHappened = FALSE;

    if (xEventInQueue) {
        *eEvent        = eQueuedEvent;
        xEventInQueue  = FALSE;
        xEventHappened = TRUE;
    }
    return xEventHappened;
}
