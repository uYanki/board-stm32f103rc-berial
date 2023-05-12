#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phphash.h"
#include "xmalloc.h"

/* hash表大小的可能取值 */
static uint32_t size_table[] =
  {
    5, 11, 19, 53, 107, 223, 463, 983, 1979, 3907, 7963, 16229, 32531, 65407, 130987, 262237, 524521, 1048793, 2097397, 4194103, 8388857, 16777447, 33554201, 67108961, 134217487, 268435697, 536870683, 1073741621, 2147483399
  };

#define COUNTS_OF_SIZE_TABLE    (sizeof(size_table) / sizeof(size_table[0]))

static uint32_t hashpjw(char *key, uint32_t key_length)
{
  uint32_t h, g;
  char *p;

  h = 0;
  p = key + key_length;
  while (key < p)
  {
    h = (h << 4) + *key++;
    if ((g = (h & 0xF0000000)))
    {
      h = h ^(g >> 24);
      h = h ^ g;
    }
  }

  return h;
}

static int hash_do_rehash(LPHASHTABLE pht)
{
  LPHASHBUCKET p;
  uint32_t index;

  memset(pht->buckets, 0, sizeof(LPHASHBUCKET) * size_table[pht->size_index]);
  for (p = pht->head; p; p = p->next)
  {
    index = p->h % pht->table_size;
    p->conflict_next = 0;
    p->conflict_previous = pht->buckets[index];
    if (p->conflict_previous)
    {
      p->conflict_previous->conflict_next = p;
    }
    pht->buckets[index] = p;
  }

  return 0;
}

static int hash_do_resize(LPHASHTABLE pht)
{
  LPHASHBUCKET *pp;

  if (pht->size_index < (uint32_t)COUNTS_OF_SIZE_TABLE - 1)
  {
    pp = (LPHASHBUCKET *)xrealloc(pht->buckets, size_table[pht->size_index + 1] * sizeof(LPHASHBUCKET));
    if (pp)
    {
      pht->buckets = pp;
      pht->size_index++;
      pht->table_size = size_table[pht->size_index];
      hash_do_rehash(pht);
      return 0;
    }
    return -1;
  }

  return 0;
}

// 调用该函数之前需要自己动态分配一个hashtable
int hash_create(LPHASHTABLE pht, uint32_t size, hash_func_t hash)
{
  int i;

  for (i = 0; i < COUNTS_OF_SIZE_TABLE; ++i)
  {
    if (size <= size_table[i])
    {
      size = size_table[i];
      pht->size_index = i;
      break;
    }
  }
  if (i == COUNTS_OF_SIZE_TABLE)
  {
    size = size_table[COUNTS_OF_SIZE_TABLE - 1];
    pht->size_index = COUNTS_OF_SIZE_TABLE - 1;
  }

  pht->buckets = (LPHASHBUCKET *)xcalloc(size, sizeof(LPHASHBUCKET));/* 动态分配 指针数组 */
  if (!pht->buckets)
  {
    return -1;
  }
  pht->hash = hash ? hash : hashpjw;
  pht->elements = 0;
  pht->head = 0;
  pht->p = 0;
  pht->tail = 0;
  pht->table_size = size;

  return 0;
}

/* 向hash表中插入一项。注意当添加的key数大于hash表当前大小时会自动调整hash表大小，并重新进行hash存储。
   注意:
      hash_entry函数中data是可以为NULL，但hash_find中**data指针不能为NULL.
      hash_enry当插入的key已经存在返回-1.
*/

int hash_entry(LPHASHTABLE pht, char *key, uint32_t key_length, void *data)
{
  uint32_t h;
  uint32_t index;
  LPHASHBUCKET p;

  h = pht->hash(key, key_length);
  index = h % pht->table_size;

  for (p = pht->buckets[index]; p; p = p->conflict_previous)
  {   /* key 在hash表中对应的存储位置不为空 */
    if (p->h == h && p->key_length == key_length)
    {
      if (!memcmp(p->key, key, key_length))
      {
        return -1;
      }
    }
  }

  /* key在hash表中不存在 */
  p = (LPHASHBUCKET)xmalloc(sizeof(HASHBUCKET) - 1 + key_length);
  memcpy(p->key, key, key_length);
  p->key_length = key_length;
  p->h = h;
  p->data = data;

  p->conflict_next = 0;
  p->conflict_previous = pht->buckets[index];
  if (p->conflict_previous)
  {
    p->conflict_previous->conflict_next = p;
  }
  p->previous = pht->tail;
  p->next = 0;
  pht->tail = p;
  if (p->previous)
  {
    p->previous->next = p;
  }
  if (!pht->head)
  {
    pht->head = p;
  }

  pht->buckets[index] = p;
  ++pht->elements;
  if (pht->elements > pht->table_size)
  {
    hash_do_resize(pht);
  }

  return 0;
}

int hash_find(LPHASHTABLE pht, char *key, uint32_t key_length, void **data)
{
  uint32_t h;
  uint32_t index;
  LPHASHBUCKET p;

  h = pht->hash(key, key_length);
  index = h % pht->table_size;

  for (p = pht->buckets[index]; p; p = p->conflict_previous)
  {
    if (p->h == h && p->key_length == key_length && !memcmp(p->key, key, key_length))
    {
      *data = p->data;
      return 0;
    }
  }

  return -1;
}

// 要删除的key不存在返回-1
int hash_remove(LPHASHTABLE pht, char *key, uint32_t key_length)
{
  uint32_t h;
  uint32_t index;
  LPHASHBUCKET p;

  h = pht->hash(key, key_length);
  index = h % pht->table_size;

  for (p = pht->buckets[index]; p; p = p->conflict_previous)
  {
    if (p->h == h && p->key_length == key_length && !memcmp(p->key, key, key_length))
    {
      if (p->conflict_previous)
      {
        p->conflict_previous->conflict_next = p->conflict_next;
      }
      if (p->conflict_next)
      {
        p->conflict_next->conflict_previous = p->conflict_previous;
      }
      if (p->previous)
      {
        p->previous->next = p->next;
      }
      if (p->next)
      {
        p->next->previous = p->previous;
      }
      if (pht->buckets[index] == p)
      {
        pht->buckets[index] = p->conflict_previous;
      }
      if (pht->head == p)
      {
        pht->head = p->next;
      }
      if (pht->tail == p)
      {
        pht->tail = p->previous;
      }
      --pht->elements;
      xfree(p);

      return 0;
    }
  }

  return -1;
}

// 修改的是key的data部分，如果要修改的key不存在则返回-1
int hash_update(LPHASHTABLE pht, char *key, uint32_t key_length, void *data)
{
  uint32_t h;
  uint32_t index;
  LPHASHBUCKET p;

  h = pht->hash(key, key_length);
  index = h % pht->table_size;

  for (p = pht->buckets[index]; p; p = p->conflict_previous)
  {
    if (p->h == h && p->key_length == key_length && !memcmp(p->key, key, key_length))
    {
      p->data = data;
      return 0;
    }
  }

  return -1;
}

// hash_destroy函数中不会释放data以及其中的动态内存，如果data是动态内存或者其中包含动态内容的话。
int hash_destroy(LPHASHTABLE pht)
{
  LPHASHBUCKET p, q;

  p = pht->head;
  while (p)
  {
    q = p;
    p = p->next;
    xfree(q);
  }
  xfree(pht->buckets);

  return 0;
}

#ifdef TEST_PHPHASH
#include "md5.h"
int main()
{
  uint32_t l;
  uint32_t l2;
  uint8_t m1[17];
  uint8_t m2[17];
  md5("liuxingzhi", 10, m1);
  md5("LIUXINGZHI", 10, m2);
  l = hashpjw(m1, 16);
  l2 = hashpjw(m2, 16);

  printf("%d,%d\n", l, l2);

  return 0;
}
#endif
