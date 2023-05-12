#include "ringbuf.h"

uint8_t ringbuf_length(ringbuf* ring)
{
    int8_t len = ring->rear - ring->head;
    return (len < 0) ? (len + ring->capacity) : len;
}

uint8_t ringbuf_push(ringbuf* ring, datatype data)
{
    ring->buffer[ring->rear++] = data;
    if (ring->rear == ring->capacity)
        ring->rear = 0;
    if (ring->head == ring->rear) {
        if (++ring->head == ring->capacity)
            ring->head = 0;
        return 0;  // ring is FULL
    }
    return 1;
}

uint8_t ringbuf_pop(ringbuf* ring, datatype* data)
{
    if (ring->head == ring->rear)
        return 0;  // ring is NULL
    *data = ring->buffer[ring->head];
    if (++ring->head == ring->capacity)
        ring->head = 0;
    return 1;
}

uint16_t ringbuf_popall(ringbuf* ring, datatype* data)
{
    if (ring->rear == ring->head) return 0;
    int16_t length = ring->rear - ring->head;
    if (length > 0) {
        memcpy(data, ring->buffer + ring->head, (ring->rear - ring->head) * sizeof(datatype));
    } else {
        memcpy(data, ring->buffer + ring->head, (ring->capacity - ring->head) * sizeof(datatype));
        memcpy(data + ring->capacity - ring->head, ring->buffer, ring->rear * sizeof(datatype));
        length += ring->capacity;
    }
    ring->head = ring->rear = 0;
    return length;
}

void ringbuf_print(ringbuf* ring, uint8_t raw /* 0: start from head; 1: start from 0;*/, uint8_t type /*0:Dec,1:Hex,2:Char;*/)
{
#if PRINT_RINGBUF
    uint8_t i;
    char*   style;
    if (ring->head == ring->rear) {
        ringbuf_log("ringbuf is null\r\n");
        return;
    }
    ringbuf_log("ringbuf:[%d,%d) ", ring->head, ring->rear);
    switch (type) {
        case 0: style = "%d "; break;
        case 1: style = "%x "; break;
        case 2: style = "%c "; break;
    }
    if (raw) {
        for (i = 0; i < ring->capacity; ++i) ringbuf_log(style, ring->buffer[i]);
    } else {
        if (ring->head < ring->rear) {
            for (i = ring->head; i < ring->rear; ++i) ringbuf_log(style, ring->buffer[i]);
        } else {
            for (i = ring->head; i < ring->capacity; ++i) ringbuf_log(style, ring->buffer[i]);
            for (i = 0; i < ring->rear; ++i) ringbuf_log(style, ring->buffer[i]);
        }
    }
    ringbuf_log("\r\n");
#endif
}
