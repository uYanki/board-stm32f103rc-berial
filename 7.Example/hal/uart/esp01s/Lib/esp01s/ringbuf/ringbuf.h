#ifndef __RINGBUF_H_
#define __RINGBUF_H_

#include <stdint.h>
#include <string.h>

#define PRINT_RINGBUF 1
#if PRINT_RINGBUF
#include <stdio.h>
#endif

#define datatype char

typedef struct {
    datatype* buffer;
    uint8_t   capacity;  // 容量
    uint16_t  head;
    uint16_t  rear;
} ringbuf;  // fifo

#define ringbuf_log printf

uint16_t ring_buffer_length(ringbuf* ring);

uint8_t ringbuf_push(ringbuf* ring, datatype data);  // push_back
uint8_t ringbuf_pop(ringbuf* ring, datatype* data);  // pop_front
// uint16_t ringbuf_pops(ringbuf* ring, datatype* data, uint16_t size);
uint16_t ringbuf_popall(ringbuf* ring, datatype* data);

void ringbuf_print(ringbuf* ring, uint8_t raw, uint8_t type);

#endif
