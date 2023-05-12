#include "memory.h"

uint32_t _mem_alloc(uint32_t size);
uint8_t  _mem_free(uint32_t offset);

static uint8_t  _mem_buff[MEM_TOTAL_SIZE] = {0};  // 内存池 __align(4)
static uint16_t _mem_map[MEM_TABLE_SIZE]  = {0};  // 状态表

/**
 * @brief 填充内存
 * @param dest 目标地址
 * @param val  填充值
 * @param len  长度(以字节为单位)
 */
void mem_set(void* dest, uint8_t val, uint32_t len)
{
    uint8_t* _dest = dest;
    while (len--) *_dest++ = val;
}

/**
 * @brief 复制内存
 * @param dest 目标地址
 * @param src  源地址
 * @param len  长度(以字节为单位)
 */
void mem_cpy(void* dest, void* src, uint32_t len)
{
    uint8_t *_dest = dest, *_src = src;
    while (len--) *_dest++ = *_src++;
}

/**
 * @brief 获取内存使用率
 * @return 使用率(0~100)
 */
uint8_t mem_perused(void)
{
    uint32_t used = 0;
    uint32_t idx  = MEM_TABLE_SIZE;
    while (idx--)
        if (_mem_map[idx])
            ++used;
    return (used * 100) / MEM_TABLE_SIZE;
}

/**
 * @brief 申请内存(内部使用)
 * @param size 需分配的大小
 * @return other:success 0XFFFFFFFF:fault
 */
uint32_t _mem_alloc(uint32_t size)
{
    if (size == 0) return 0XFFFFFFFF;  // 不需分配

    uint16_t blk_cnt = (size / MEM_BLOCK_SIZE) + ((size % MEM_BLOCK_SIZE) ? 1 : 0);  // 需分配的内存块数量

    uint16_t nblk_cnt = 0;  // 连续空白内存块数
    uint32_t offset   = MEM_TABLE_SIZE;

    while (offset--) {
        (_mem_map[offset]) ? (nblk_cnt = 0) : (++nblk_cnt);
        if (nblk_cnt == blk_cnt) {
            while (nblk_cnt--)  // 标注为非空
                _mem_map[offset + nblk_cnt] = nblk_cnt;
            return offset * MEM_BLOCK_SIZE;
        }
    }

    return 0XFFFFFFFF;  // 未找到符合条件的内存块
}

/**
 * @brief 释放内存(内部使用)
 * @param offset 内存地址偏移
 * @return 0:success 1:fault
 */
uint8_t _mem_free(uint32_t offset)
{
    if (offset >= MEM_TOTAL_SIZE) return 1;  // 超出范围

    uint32_t idx = offset / MEM_BLOCK_SIZE;  // 块索引
    uint32_t cnt = _mem_map[idx];            // 块数量
    while (cnt--) _mem_map[idx++] = 0;       // 清零

    return 0;
}

/**
 * @brief 申请内存(外部使用)
 * @param size 需分配的大小
 * @return 内存首地址
 */
void* mem_alloc(uint32_t size)
{
    uint32_t offset = _mem_alloc(size);
    return (offset == 0XFFFFFFFF) ? (0) : (_mem_buff + offset);
}

/**
 * @brief 申请内存(外部使用)
 * @param ptr 旧内存首地址
 * @param size 需分配的大小
 * @return 新内存首地址
 */
void* mem_realloc(void* ptr, uint32_t size)
{
    void* p = mem_alloc(size);
    if (p == 0) return 0;
    mem_cpy(p, ptr, size);
    mem_free(size);
    return p;
}

/**
 * @brief 释放内存(外部使用)
 * @param ptr 内存首地址
 * @return 0:success 1:fault
 */
void mem_free(void* ptr)
{
    if (ptr == 0) return;
    _mem_free((uint32_t)ptr - (uint32_t)_mem_buff);
}