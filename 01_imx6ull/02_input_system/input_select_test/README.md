# 输入设备监测 - Select 方式

该目录包含一个使用 `select()` 系统调用监测输入设备（如键盘、触摸屏、触控板）的 C 程序。

## 核心知识点
- **`select()` 系统调用**：经典的 I/O 多路复用函数，几乎在所有 Unix/Linux 平台上可用。
- **`fd_set` 文件描述符集合**：需要每次循环前重新初始化。
- **`struct timeval`**：用于设置监测的最大等待时间（超时时间）。
- **`FD_ZERO`, `FD_SET`, `FD_ISSET`**：文件描述符集合操作宏。

## 编译与运行
```bash
# 编译
gcc input_select_test.c -o input_select_test

# 运行 (以触控板为例)
sudo ./input_select_test /dev/input/event13
```
