[Letter](https://nevermindzzt.github.io/2020/01/19/Letter%20shell%203.0%E5%85%A8%E6%96%B0%E5%87%BA%E5%8F%91/)，[Github](https://github.com/NevermindZZT/letter-shell) 

注：使用 MobaXterm 等命令行交互工具进行测试。

！！！该工具特别强大

- [x] Tab 键快速补全/命令提示
- [x] 查看/修改全局变量（不支持局部变量）
- [x] exec 执行未导出指令
- [x] 用户权限分级（最多256级）

---

在 demo 中，设置的

默认用户 uYanki 登录密码为 123456，

管理员 admin 登录密码为 root。（在登录默认用户后，输入 admin 进行登录）

低权限用户查看不了高权限用户的指令，高权限用户可以查看低权限用户的指令。

设置用户长时间无操作会自动上锁，即需重新输密码。