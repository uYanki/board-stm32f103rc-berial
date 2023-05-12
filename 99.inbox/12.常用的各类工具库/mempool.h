/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef MEM_POOL_H
#define MEM_POOL_H

typedef struct mem_pool mem_pool_t;

#ifdef __cplusplus
extern "C" {
#endif

mem_pool_t* mp_create(size_t memb_count, size_t memb_size);
void*       mp_alloc(mem_pool_t* pmp);
void        mp_free(mem_pool_t* pmp, void* p);
void        mp_delete(mem_pool_t* pmp);

#ifdef __cplusplus
}
#endif

#endif
