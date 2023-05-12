#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "threadpool.h"

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

/* 线程双向链表: 
   线程池中有两个线程双向链表: 空闲线程双向链表、busy线程双向链表。*/
struct ThreadList;

/* 线程节点: 线程池中的每一个线程都有一个相应的控制数据结构即线程节点*/
struct ThreadNode
{
  struct ThreadNode *prev, *next;
  thread_pool_t     *pool;/* 指向线程池数据结构*/
  pthread_t         tid;
  pthread_cond_t    cond;
  pthread_mutex_t   mutex;
  thread_func_t     func; /* 线程函数，空闲线程与busy线程的区分标志是该线程节点是否有线程函数*/
  void              *args;/* 线程函数的参数*/
};

struct ThreadList
{
  struct ThreadNode *head, *tail;
  int      num;
};

struct ThreadPool
{
  sem_t     lock ;  /* 线程池数据结构是多个线程的共享资源，需要互斥地访问*/
  struct ThreadList idle, busy;
  int      min_num, max_num, idle_min, idle_max;
};

static int thread_list_init(struct ThreadList *list)
{
  int retval;

  assert(list != NULL);
  list->head = NULL;
  list->tail = NULL;
  list->num = 0;

  return retval;
}

static int thread_list_add(struct ThreadList *list,struct ThreadNode *node)
{
  int retval;

  assert(list != NULL && node != NULL);
  node->next = list->head;
  list->head = node;
  node->prev = NULL;
  if (node->next)
    node->next->prev = node;
  else
    list->tail = node;
  list->num ++;

  return retval;
}

static int thread_list_del(struct ThreadList *list,struct ThreadNode *node)
{
  int retval;

  assert(list != NULL && node != NULL);
  if (node->next)
    node->next->prev = node->prev;
  else
    list->tail = node->prev;
  if (node->prev)
    node->prev->next = node->next;
  else
    list->head = node->next;
  list->num --;

  return retval;
}

static struct ThreadNode* thread_list_del_first(struct ThreadList *list)
{
  struct ThreadNode *node;

  assert(list->num > 0);
  node = list->head;
  thread_list_del(list, node);

  return node;
}

static int thread_node_init(struct ThreadNode *node,thread_func_t func, void *args, thread_pool_t *pool)
{
  int retval;

  assert(node != NULL);
  node->pool = pool;
  node->prev = NULL;
  node->next = NULL;
  node->func = func;
  node->args = args;
  node->tid = (pthread_t)-1;
  pthread_cond_init(&node->cond, NULL);
  pthread_mutex_init(&node->mutex, NULL);

  return retval;
};

static void thread_node_destroy(struct ThreadNode *node)
{
  assert(node != NULL);

  pthread_mutex_destroy(&node->mutex);
  pthread_cond_destroy(&node->cond);
  free(node);
}

static int thread_create_do(thread_pool_t *tp, thread_func_t func,void *args);

static void* thread_pool_main(void *args)
{
  struct ThreadNode *node = (struct ThreadNode *)args;
  thread_pool_t *tp;
  int curr_num;

  assert(node != NULL);
  tp = node->pool;
  /* task loop */
  while (1)
  {
    /* if no task, wait */
    if (!node->func)
    {
      pthread_cleanup_push((void(*)(void*))pthread_mutex_unlock,
                           (void*)&node->mutex)
      pthread_mutex_lock(&node->mutex);
      pthread_cond_wait(&node->cond, &node->mutex);
      pthread_cleanup_pop(1);
    }

    /* do the task. */
    node->func(node->args);
    node->func = NULL;

    sem_wait(&tp->lock);
    thread_list_del(&tp->busy, node);
    /* If there are enough threads, I just exit */
    curr_num = tp->idle.num + 1 + tp->busy.num;
    if (tp->idle.num >= tp->idle_max
        && curr_num > tp->min_num)
    {
      sem_post(&tp->lock);
      thread_node_destroy(node);
      pthread_detach(pthread_self());
      pthread_exit(NULL);
    }
    /* The pre-created threads aren't enough, so I create more.
     * It's better than creating them at the task beginning. */
    else if (curr_num < tp->max_num)
    {
      int num, i;

      num = MIN(tp->busy.num + tp->idle_min, tp->max_num);

      for (i = curr_num; i < num; i ++)
      {
        if (thread_create_do(tp, NULL, NULL) != 0)
        {
          tp->max_num = i;
          break;
        }
      }
    }
    thread_list_add(&tp->idle, node);
    sem_post(&tp->lock);
  }

  return NULL;
};

/* Note: It must be called with tp->lock locked */
static int thread_wake_do(thread_pool_t *tp, thread_func_t func,void *args)
{
  struct ThreadNode *node;

  assert(tp != NULL);
  node = thread_list_del_first(&tp->idle);
  node->func = func;
  node->args = args;
  thread_list_add(&tp->busy, node);
  pthread_cond_signal(&node->cond);

  return 0;
}

/* Note: It must be called with tp->lock locked */
static int thread_create_do(thread_pool_t *tp, thread_func_t func,void *args)
{
  int retval;
  struct ThreadNode *node;

  assert(tp != NULL);
  node = malloc(sizeof(*node));
  if (!node) return -1;
  thread_node_init(node, func, args, tp);
  if (func)
    thread_list_add(&tp->busy, node);
  else
    thread_list_add(&tp->idle, node);
  retval = pthread_create(&node->tid, NULL,
                          thread_pool_main, node);
  if (retval != 0)
  {
    if (func)
      thread_list_del(&tp->busy, node);
    else
      thread_list_del(&tp->idle, node);
    thread_node_destroy(node);
  }

  return retval;
}

/*
    创建一个线程池，线程池里的线程都阻塞，只有当其中某个
    线程对应的线程节点被设置了线程函数，这个线程才能执行。
*/
thread_pool_t* thread_pool_create(int min, int max, int idle_min,int idle_max)
{
  thread_pool_t *tp;
  int retval;
  int i, pred;
  sigset_t sig, old_sig;

  tp = malloc(sizeof(thread_pool_t));
  if (!tp) return NULL;
  tp->min_num = min;
  tp->max_num = max;
  tp->idle_min = idle_min;
  tp->idle_max = idle_max;
  sem_init(&tp->lock , 0, 1);
  thread_list_init(&tp->idle);
  thread_list_init(&tp->busy);

  pred = MAX(min, idle_min);
  /* block all the signal */
  sigfillset(&sig);
  pthread_sigmask(SIG_SETMASK, &sig, &old_sig);
  sem_wait(&tp->lock);
  for (i = 0; i < pred; i ++)
  {
    retval = thread_create_do(tp, NULL, NULL);
    if (retval != 0)
    {
      if (i == 0)
      {
        sem_post(&tp->lock);
        sem_destroy(&tp->lock);
        pthread_sigmask(SIG_SETMASK, &old_sig, NULL);
        free(tp);
        return NULL;
      }
      tp->min_num = tp->max_num = tp->idle_min = tp->idle_max  = i;
      break;
    }
  }
  sem_post(&tp->lock);
  pthread_sigmask(SIG_SETMASK, &old_sig, NULL);

  return tp;
}

/*
   指定线程函数。
*/
int  thread_pool_do(thread_pool_t *tp, thread_func_t func, void *args)
{
  int retval;

  assert(tp != NULL && func != NULL);

  sem_wait(&tp->lock);
  if (tp->idle.num > 0)
  {
    retval = thread_wake_do(tp, func, args);
  }
  else if (tp->idle.num + tp->busy.num < tp->max_num)
  {
    sigset_t sig, old_sig;

    sigfillset(&sig);
    pthread_sigmask(SIG_SETMASK, &sig, &old_sig);
    retval = thread_create_do(tp, func, args);
    if (retval != 0)
      tp->max_num = tp->idle.num + tp->busy.num;
    pthread_sigmask(SIG_SETMASK, &old_sig, NULL);
  }
  else
  {
    retval = -1;
  }
  sem_post(&tp->lock);

  return retval;
}

int  thread_pool_destroy(thread_pool_t *tp)
{
  struct ThreadNode *node, *tmp;

  assert(tp != NULL);
  sem_wait(&tp->lock);
  /* cancel all the threads */
  for (node = tp->idle.head; node; node = tmp)
  {
    tmp = node->next;
    pthread_cancel(node->tid);
    pthread_join(node->tid, NULL);
    thread_list_del(&tp->idle, node);
    thread_node_destroy(node);
  }
  for (node = tp->busy.head; node; node = tmp)
  {
    tmp = node->next;
    pthread_cancel(node->tid);
    pthread_join(node->tid, NULL);
    thread_list_del(&tp->busy, node);
    thread_node_destroy(node);
  }
  sem_post(&tp->lock);
  sem_destroy(&tp->lock);
  free(tp);

  return 0;
}
