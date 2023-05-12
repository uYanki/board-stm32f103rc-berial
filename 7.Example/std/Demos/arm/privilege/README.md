非特权模式下，在中断外操作内核上的寄存器会触发 `HardFault_Hander`。

---

![hard fault](README.assets/hard_fault.png)

屏蔽 while(1) 掉，执行完 `HardFault_Hander` 里的内容，PC 值会指向 `HardFault_Hander`，即其本身就是个循环。

![no loop](README.assets/no_loop.png)

---

![special register](README.assets/special_register.png)

![permission](README.assets/permission.png)

