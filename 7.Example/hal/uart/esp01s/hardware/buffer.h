#ifndef __BUFFER_H_
#define __BUFFER_H_

#include "stm32f4xx.h"

typedef struct
{
    u8* buf;
    u16 size;
    u16 head;
    u16 rear;
} BufferTypeDef;

typedef struct
{
    u8  size;
    u8  length;
    u8* data;
} BufferClip;

void Buffer_Reset(BufferTypeDef* buff);
u16  Buffer_Length(BufferTypeDef* buff);
u8   Buffer_Push(BufferTypeDef* buff, u8 data);
u8   Buffer_Pop(BufferTypeDef* buff, u8* data);
u8   Buffer_Pop_All(BufferTypeDef* buff, BufferClip* clip);
void Buffer_Print(BufferTypeDef* buff);
void Buffer_Print_Hex(BufferTypeDef* buff);
void Buffer_Print_All(BufferTypeDef* buff);

void Buffer_Clip_Print(BufferClip* clip);
void Buffer_Clip_Print_Hex(BufferClip* clip);

#endif
