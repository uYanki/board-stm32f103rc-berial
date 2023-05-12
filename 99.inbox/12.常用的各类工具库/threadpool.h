#ifndef  THREAD_POOL_H
#define  THREAD_POOL_H

typedef void(* thread_func_t)(void *args);
typedef struct ThreadPool  thread_pool_t;

#ifdef __cplusplus
extern "C"
{
#endif

  thread_pool_t*  thread_pool_create(int min, int max, int idle_min,int idle_max);
  int  thread_pool_do(thread_pool_t *tp, thread_func_t func, void *args);
  int  thread_pool_destroy(thread_pool_t *tp);

#ifdef __cplusplus
}
#endif


#endif /* THREAD_POOL_H */
