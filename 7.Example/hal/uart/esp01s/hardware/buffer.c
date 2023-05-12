#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Buffer_Reset(BufferTypeDef* ring) {
    ring->head = 0;
    ring->rear = 0;
}

u16 Buffer_Length(BufferTypeDef* ring) {
    if (ring->rear >= ring->head) {
        return ring->rear - ring->head;
    } else {
        return (ring->size - ring->head) + (ring->rear - 0);
    }
}

u8 Buffer_Push(BufferTypeDef* ring, u8 data) {
    ring->buf[ring->rear] = data;
    ring->rear++;
    if (ring->rear >= ring->size) {
        ring->rear = 0;
    }
    if (ring->head == ring->rear) {
        ring->head = (ring->head + 1) % ring->size;
        return NULL;
    } else {
        return !NULL;
    }
}

u8 Buffer_Pop(BufferTypeDef* ring, u8* data) {
    if (ring->head == ring->rear) return NULL;

    *data      = ring->buf[ring->head];
    ring->head = (ring->head + 1) % ring->size;
    return !NULL;
}

u8 Buffer_Pop_All(BufferTypeDef* ring, BufferClip* clip) {
    if (ring->head == ring->rear) return NULL;

    memset(clip->data, 0x00, clip->size * sizeof(u8));
    clip->length = 0;
    if (ring->head > ring->rear) {
        while (ring->head < ring->size && clip->length <= clip->size) {
            *(clip->data + clip->length++) = ring->buf[ring->head++];
        }
        if (ring->head == ring->size) {
            ring->head = 0;
        }
    }
    while (ring->head < ring->rear && clip->length <= clip->size) {
        *(clip->data + clip->length++) = ring->buf[ring->head++];
    }
    return !NULL;
}

void Buffer_Print(BufferTypeDef* ring) {
    printf("BUFF:[%03d,%03d)", ring->head, ring->rear);
    if (ring->head == ring->rear) {
        // print nothing;
    } else if (ring->head < ring->rear) {
        for (int i = ring->head; i < ring->rear; i++) {
            printf("%c", ring->buf[i]);
        }
    } else {
        for (int i = ring->head; i < ring->size; i++) {
            printf("%c", ring->buf[i]);
        }
        for (int i = 0; i < ring->rear; i++) {
            printf("%c", ring->buf[i]);
        }
    }
    printf("\r\n");
}

void Buffer_Print_Hex(BufferTypeDef* ring) {
    printf("BUFF:[%03d,%03d)", ring->head, ring->rear);
    if (ring->head == ring->rear) {
        // print nothing;
    } else if (ring->head < ring->rear) {
        for (int i = ring->head; i < ring->rear; i++) {
            printf("%02X ", ring->buf[i]);
        }
    } else {
        for (int i = ring->head; i < ring->size; i++) {
            printf("%02X ", ring->buf[i]);
        }
        for (int i = 0; i < ring->rear; i++) {
            printf("%02X ", ring->buf[i]);
        }
    }
    printf("\r\n");
}

void Buffer_Print_All(BufferTypeDef* ring) {
    printf("BUFF:[%d,%d)", ring->head, ring->rear);
    for (int i = 0; i < ring->size; i++) {
        printf("%c", ring->buf[i]);
    }
    printf("\r\n");
}

void Buffer_Clip_Print(BufferClip* clip) {
    printf("CLIP:[%03d]", clip->length);
    for (int i = 0; i < clip->length; i++) {
        printf("%c", clip->data[i]);
    }
    printf("\r\n");
}

void Buffer_Clip_Print_Hex(BufferClip* clip) {
    printf("CLIP:[%03d]", clip->length);
    for (int i = 0; i < clip->length; i++) {
        printf("%02X ", clip->data[i]);
    }
    printf("\r\n");
}
