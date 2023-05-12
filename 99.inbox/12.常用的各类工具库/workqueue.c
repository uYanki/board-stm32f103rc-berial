/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdlib.h>
#include <assert.h>

#include <pthread.h>

#include "threadpool.h"
#include "workqueue.h"

/* 每个任务都在队列中分配一个结点*/
struct WorkQueueItem {
    thread_func_t func;
    thread_func_t cleanup;
    void*         args;
    work_queue_t* wq;
};

struct WorkQueue {
    pthread_mutex_t        mutex; /* 互斥地访问工作队列数据结构*/
    struct WorkQueueItem** q;
    int                    q_size, curr_size, head_idx, tail_idx;
    thread_pool_t*         tp;
};

work_queue_t* work_queue_create(int max, int tp_min, int tp_max, int tp_idle_min, int tp_idle_max)
{
    work_queue_t* q;

    assert(max > 0);
    q = malloc(sizeof(*q));
    if (!q) return NULL;
    pthread_mutex_init(&q->mutex, NULL);

    /* 创建max个WorkQueueItem指针*/
    q->q = malloc(sizeof(struct WorkQueueItem*) * max);
    if (!q->q) goto err;

    q->q_size    = max;
    q->curr_size = 0;
    q->head_idx = q->tail_idx = 0;

    /* 创建一个线程池*/
    q->tp                     = thread_pool_create(tp_min, tp_max, tp_idle_min, tp_idle_max);
    if (!q->tp) goto err2;

    return q;

err2:
    free(q->q);
err:
    pthread_mutex_destroy(&q->mutex);
    free(q);

    return NULL;
}

static void work_queue_main(void* args)
{
    struct WorkQueueItem* it;
    work_queue_t*         wq;

    it = (struct WorkQueueItem*)args;
    wq = it->wq;

    do {
        it->func(it->args);
        free(it);
        it = NULL;
        pthread_cleanup_push((void (*)(void*)) & pthread_mutex_unlock,
                             &wq->mutex);
        pthread_mutex_lock(&wq->mutex);
        if (wq->curr_size > 0) {
            it           = wq->q[wq->head_idx];
            wq->head_idx = (wq->head_idx + 1) % wq->q_size;
            wq->curr_size--;
        }
        pthread_cleanup_pop(1);
    } while (it);
}

/* 一个线程函数就是一个任务，从线程池中激活一个线程来执行该任务*/
int work_queue_enqueue(work_queue_t* q, thread_func_t func, thread_func_t cleanup, void* args)
{
    struct WorkQueueItem* it;
    int                   retval = 0;
    assert(q && func);

    /* 分配一个工作队列结点并初始化*/
    it = malloc(sizeof(*it));
    if (!it) return -1;
    it->func    = func;
    it->cleanup = cleanup;
    it->args    = args;
    it->wq      = q;

    /* 指定线程函数*/
    if (thread_pool_do(q->tp, work_queue_main, (void*)it) == 0)
        return 0;

    pthread_cleanup_push((void (*)(void*)) & pthread_mutex_unlock, &q->mutex);
    pthread_mutex_lock(&q->mutex);
    if (q->curr_size < q->q_size) {
        q->q[q->tail_idx] = it;
        q->tail_idx       = (q->tail_idx + 1) % q->q_size;
        q->curr_size++;
    } else {
        free(it);
        retval = -1;
    }
    pthread_cleanup_pop(1);

    return retval;
}

int work_queue_destroy(work_queue_t* q)
{
    struct WorkQueueItem* it;

    assert(q);

    thread_pool_destroy(q->tp);
    pthread_mutex_destroy(&q->mutex);
    while (q->curr_size > 0) {
        it = q->q[q->head_idx];
        if (it->cleanup)
            it->cleanup(it->args);
        free(it);
        q->head_idx = (q->head_idx + 1) % q->q_size;
        q->curr_size--;
    }
    free(q->q);
    free(q);

    return 0;
}

#ifdef TEST
void task(void* args)
{
    usleep(random() % 1000);
}

int main(void)
{
#ifdef WQ
    XSWorkQueue* q;
#elif defined(TP)
    XSThreadPool* p;
#else
    pthread_t tid;
#endif
    int  i, retval;
    int* args;

#ifdef WQ
    printf("t_wq\n");
    q = xs_work_queue_create(50, 50, 100, 10, 10);
#elif defined(TP)
    printf("t_tp\n");
    p = xs_thread_pool_create(50, 100, 10, 10);
#else
    printf("t_th\n");
#endif
    for (i = 0; i < 1000; i++) {
#ifdef WQ
        retval = xs_work_queue_enqueue(q, task, NULL, NULL);
#elif defined(TP)
        retval = xs_thread_pool_do(p, task, NULL);
#else
        retval = pthread_create(&tid, NULL, (void* (*)(void*))task, NULL);
        pthread_detach(tid);
#endif
        if (retval != 0)
            printf("err = %d\n", i);
        usleep(random() % 1);
    }
    usleep(random() % 1000);
    printf("terminate all the tasks\n");
#ifdef WQ
    xs_work_queue_destroy(q);
#elif defined(TP)
    xs_thread_pool_destroy(p);
#endif
    printf("ok\n");

    return 0;
}

#endif
