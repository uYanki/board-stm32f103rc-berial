##### 逻辑优先级

* FreeRTOS：优先级数值越小，逻辑优先级越低。（IdleTask Prio = 0）
* uC/OS-III：优先级数值越小，逻辑优先级越高。（IdleTask Prio = MAX-1）
* Unix：优先级数值越小，逻辑优先级越高。

---

注：使用 `vTaskPrioritySet` 变更任务优先级会立即触发任务调度。