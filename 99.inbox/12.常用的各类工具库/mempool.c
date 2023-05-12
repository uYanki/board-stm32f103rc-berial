/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdlib.h>
#include <string.h>
#include "xmalloc.h"

/* figure:
                    mem_block                   mem_block
   mem_block_head -->|PTR|PTR,unit|PTR,unit|...|-->|PTR|PTR,unit|PTR,unit|...
   free_unit_head -------^--------^--------------------^.....
*/
typedef struct mem_pool
{
  void * p_mem_block_head;/* 指向内存块链表的头结点 */
  void * p_free_unit_head;/* 指向空闲单元链表的头结点 */
  size_t alloc_unit_size;
  size_t alloc_unit_count;
  size_t mem_block_size;
  size_t mem_block_count;
  /* 从内存池中分配和释放unit都是mem_block_head指向的内存块中的free unit */
}mem_pool_t;

mem_pool_t * mp_create(size_t memb_count,size_t memb_size)
{
  mem_pool_t *pmp;
  char *p, *n;
  size_t ptr_size,unit_size,mem_block_size;
  int i;

  ptr_size = sizeof(void *);
  if (memb_count <= 0 || memb_size <= 0)
    return NULL;

  unit_size = memb_size + ptr_size;
  mem_block_size = memb_count * unit_size + ptr_size;

  pmp =(mem_pool_t *) xmalloc0(sizeof(mem_pool_t));
  p = (char *)xmalloc0(mem_block_size);

  pmp->alloc_unit_count = memb_count;
  pmp->alloc_unit_size = unit_size;
  pmp->mem_block_size = mem_block_size;
  pmp->mem_block_count = 1;

  /* 构造内存块链表 */
  memcpy((void *)p,&pmp->p_mem_block_head,ptr_size);
  pmp->p_mem_block_head = (void *)p;

  /* 构造空闲单元链表 */
  memcpy(p + ptr_size + (memb_count - 1) * unit_size,&pmp->p_free_unit_head,ptr_size);
  pmp->p_free_unit_head = (void *)(p + ptr_size);

  /* 初始化内存块,把内存块中的空闲单元链接起来 */
  for (i=memb_count-1;i>=1;i--)
  {
    n = p + ptr_size + i * unit_size;
    memcpy(p + ptr_size + (i - 1) * unit_size,&n,ptr_size);
  }

  return pmp;
}

void * mp_alloc(mem_pool_t * pmp)
{
  char *p,*n;
  int i;
  size_t ptr_size = sizeof(void *);

  if (pmp->p_free_unit_head == NULL)
  {
    /* 空闲单元链表为空 */
    /* 重新分配一个内存块 */
    p = (char *)xmalloc0(pmp->mem_block_size);

    /* 构造内存块链表 */
    memcpy(p,&pmp->p_mem_block_head,ptr_size);
    pmp->p_mem_block_head = (void *)p;

    /* 构造空闲单元链表 */
    memcpy(p + ptr_size + (pmp->alloc_unit_count - 1) * pmp->alloc_unit_size,
           &pmp->p_free_unit_head,ptr_size);
    pmp->p_free_unit_head = (void *)(p + ptr_size);

    /* 初始化内存块,把内存块中的空闲单元链接起来 */
    for (i=pmp->alloc_unit_count-1;i>=1;i--)
    {
      n = p + ptr_size + i * pmp->alloc_unit_size;
      memcpy(p + ptr_size + (i - 1) * pmp->alloc_unit_size,&n,ptr_size);
    }

    pmp->mem_block_count++;

  }

  /* 还有空闲单元 */
  /* 从空闲单元链表的头部删除一个节点 */

  p = pmp->p_free_unit_head;
  memcpy(&pmp->p_free_unit_head,p,ptr_size);
  return (void *)(p + ptr_size);/* attention */
}

void mp_free(mem_pool_t *pmp,void *p)
{
  void *n;
  size_t ptr_size = sizeof(void *);
  char *pc = (char *)p;

  n = pmp->p_free_unit_head;
  pmp->p_free_unit_head = (void *)(pc - ptr_size);
  memcpy(pc - ptr_size,&n,ptr_size);

}

void mp_delete(mem_pool_t *pmp)
{
  void *p;

  /* 释放内存块 */
  while (pmp->p_mem_block_head != NULL)
  {
    memcpy(&p, pmp->p_mem_block_head,sizeof(void *));
    xfree(pmp->p_mem_block_head);
    pmp->p_mem_block_head = p;
  }

  /* 释放mem_pool_t数据结构 */
  if (pmp != NULL)
    xfree(pmp);
}

#ifdef TEST_MEMPOOL
int main()
{
  int i;
  mem_pool_t *pmp;
  pmp = mp_create(3,4);
  int *pi;
  int *pi2;
  for (i=1;i<5;i++)
  {
    pi = mp_alloc(pmp);
    if (i == 3)
      pi2=pi;
    *pi = i;
  }
  mp_free(pmp,pi2);
  mp_delete(pmp);
  return 0;
}
#endif
