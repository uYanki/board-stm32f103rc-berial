### 互斥锁

互斥量又称互斥锁，一般用于共享资源的互斥排他性访问保护。

互斥量在任意时刻处于且仅会处于解锁或锁定状态，当一个任务获取到一把锁后（互斥量锁定），其他任务再尝试获得这把锁时会失败或进入阻塞状态，当该任务释放持有的锁时（互斥量解锁），会唤醒一个正阻塞等待此互斥量的任务，被唤醒的任务将会获取这把锁。

在多任务运行环境中，有些共享资源不具有多线程可重入性，对于这类不希望被多任务同时访问的资源（临界资源），可以采用互斥量来进行保护。

简单理解：不希望有个大块内存在还没写完的时候就被读取，或者在还没读取完时就写入新的数据。

**相关函数**

```c
tos_mutex_create();      // 创建
tos_mutex_pend();        // 等待(无限等待)
tos_mutex_pend_timed();  // 等待(超时机制)
tos_mutex_post();        // 释放
tos_mutex_destroy();     // 销毁
```

功能开启：`tos_config.h` 中的 `TOS_CFG_MUTEX_EN`

**实例代码**

```c
k_mutex_t locker;

void write_critical_resource(){}
void read_critical_resource(){}

void task_writer()
{
    while (1) {
        tos_mutex_pend(&locker);
        write_critical_resource(); // 写临界区
        tos_mutex_post(&locker);
    }
}

void task_reader()
{
    while (1) {
        tos_mutex_pend(&locker);
        read_critical_resource(); // 读临界区
        tos_mutex_post(&locker);
    }
}

void application_entry(void* arg)
{
    tos_mutex_create(&critical_resource_locker); 
}
```