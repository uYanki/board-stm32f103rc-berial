#ifndef __MEMORY_H__  // use sram
#define __MEMORY_H__

#include <stdint.h>

#define MEM_TOTAL_SIZE (12 * 1024)                        // 内存大小 KB
#define MEM_BLOCK_SIZE 32                                 // 块大小 Byte
#define MEM_TABLE_SIZE (MEM_TOTAL_SIZE / MEM_BLOCK_SIZE)  // 表大小

uint8_t mem_perused(void);                      // 使用率
void*   mem_alloc(uint32_t szie);               // 分配
void*   mem_realloc(void* ptr, uint32_t szie);  // 重分配
void    mem_free(void* ptr);                    // 释放

void mem_set(void* src, uint8_t val, uint32_t len);  // 填充
void mem_cpy(void* dest, void* src, uint32_t len);   // 复制

#endif