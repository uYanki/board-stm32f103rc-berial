// https://www.bookstack.cn/read/TencentOS-tiny/spilt.3.spilt.2.bc888230ee452d75.md#2.3.2%20%E4%BF%A1%E5%8F%B7%E9%87%8F

#include "main.h"
#include "tos_k.h"
#include "tos_config.h"

#define USE_MUTEX 1  // <--- 0/1

#define STK_SIZE_TASK_WRITER 512
#define STK_SIZE_TASK_READER 512

k_stack_t stack_task_writer[STK_SIZE_TASK_WRITER];
k_stack_t stack_task_reader[STK_SIZE_TASK_READER];

k_task_t task_writer;
k_task_t task_reader;

extern void entry_task_writer(void* arg);
extern void entry_task_reader(void* arg);

#if USE_MUTEX

k_mutex_t critical_resource_locker;

#endif

// 临界区
static uint32_t critical_resource[3];

static void write_critical_resource(int salt)
{
    size_t i = 0;
    for (i = 0; i < 3; ++i) {
        critical_resource[i] = salt;
        tos_task_delay(100);
    }
}

static void read_critical_resource(void)
{
    size_t i = 0;
    for (i = 0; i < 3; ++i)
        printf("%d\t", critical_resource[i]);
    printf("\n");
}

void entry_task_writer(void* arg)
{
    size_t  salt = 0;
    k_err_t err;

    while (K_TRUE) {
#if USE_MUTEX
        err = tos_mutex_pend(&critical_resource_locker);  // 获取互斥锁
        if (err == K_ERR_NONE) {
#endif
            // 向临界区写入数据
            write_critical_resource(salt);
#if USE_MUTEX
            tos_mutex_post(&critical_resource_locker);  // 释放互斥锁
        }
#endif
        tos_task_delay(300);
        ++salt;
    }
}

void entry_task_reader(void* arg)
{
    k_err_t err;

    while (K_TRUE) {
#if USE_MUTEX
        err = tos_mutex_pend(&critical_resource_locker);  // 获取互斥锁
        if (err == K_ERR_NONE) {
#endif
            // 从临界区读取数据
            read_critical_resource();
#if USE_MUTEX
            tos_mutex_post(&critical_resource_locker);  // 释放互斥锁
        }
#endif
        tos_task_delay(100);
    }
}

void application_entry(void* arg)
{
#if USE_MUTEX
    tos_mutex_create(&critical_resource_locker);  // 创建互斥锁
#endif
    tos_task_create(&task_writer, "writer", entry_task_writer, NULL, 4, stack_task_writer, STK_SIZE_TASK_WRITER, 0);
    tos_task_create(&task_reader, "reader", entry_task_reader, NULL, 4, stack_task_reader, STK_SIZE_TASK_READER, 0);
}
