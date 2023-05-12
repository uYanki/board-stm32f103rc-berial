/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

typedef struct WorkQueue work_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

work_queue_t* work_queue_create(int max, int tp_min, int tp_max, int tp_idle_min, int tp_idle_max);
int           work_queue_enqueue(work_queue_t* q, thread_func_t func, thread_func_t cleanup, void* args);
int           work_queue_destroy(work_queue_t* q);

#ifdef __cplusplus
}
#endif

#endif
