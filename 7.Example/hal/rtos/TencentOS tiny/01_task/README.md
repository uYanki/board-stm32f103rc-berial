[REF](https://www.bookstack.cn/read/TencentOS-tiny/spilt.2.spilt.2.bc888230ee452d75.md) 

**相关函数**

```c
tos_task_create();   // 创建
tos_task_suspend();  // 挂起
tos_task_resume();   // 恢复
tos_task_destroy();  // 销毁
```

**任务创建**

```c
k_err_t tos_task_create(k_task_t *task, 
                        char *name,
                        k_task_entry_t entry,
                        void *arg,
                        k_prio_t prio,
                        k_stack_t *stk_base,
                        size_t stk_size,
                        k_timeslice_t timeslice);
```

* entry：函数入口。

```c
void entry_task(void* arg){}
```

* arg：参数。

注意生命周期，在调用函数 `tos_knl_start()` 前（系统启动运行内核前），缓冲变量所在的去代码块不能被释放。

* prio：优先级。值越小，优先级越高。

系统创建的空闲任务的优先级被分配为 `TOS_CFG_TASK_PRIO_MAX - 1`，

用户创建的任务的任务优先级须在 [0, TOS_CFG_TASK_PRIO_MAX - 2] 范围内。 

最大优先级数 `TOS_CFG_TASK_PRIO_MAX` 可在 `tos_config.h` 中配置，至少大于等于8。

* timeslice：时间片大小。

传入值为 0 时，会被设置为 `TOS_CFG_CPU_TICK_PER_SECOND / 10`

**实例代码**

```c
#include "tos_k.h"

// task param

#define STK_SIZE_TASK 512

k_task_t  task;
k_stack_t stack_task[STK_SIZE_TASK];
uint32_t  task_arg; // arg

// custom param

typedef struct vector3 {
    float x, y, z;
} vector3_t;

vector3_t vec3 = {.x = 1.2f, .y = 2.3f, .z = 3.4f};

// task entry

void task_entry(void* arg);

void task_entry(void* arg)
{
    vector3_t* vec3 = (vector3_t*)arg; // print arg 
    tos_kprintln("vec3: %f\t%f\t%f", vec3->x, vec3->y, vec3->z);

    while (1) {
        tos_kprintln("helloworld");
        tos_task_delay(1000);
    }
}

// main

void application_entry(void* arg)
{
    tos_task_create(&task, "task_name", task_entry, (void*)&vec3, 4, stack_task, STK_SIZE_TASK, 0);
}
```

---

**延时函数** `tos_task_delay(n)`：延时 n 个 systick。

每秒钟的系统滴答数 `TOS_CFG_CPU_TICK_PER_SECOND` = 1000，即 1000 个 systick 代表 1s。

所以 tos_task_delay(1000) 就是延时 1000 个 systick，对应为 1s。

