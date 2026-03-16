# 输入设备监测 - Poll 方式

该目录包含一个使用 `poll()` 系统调用监测输入设备（如键盘、触摸屏、触控板）的 C 程序。

## 核心知识点
- **`poll()` 系统调用**：一种多路复用 I/O 模型，可以在不占用 CPU 的情况下等待文件描述符变为“就绪”状态。
- **`struct pollfd`**：用于配置要监测的文件描述符、请求的事件（POLLIN）以及返回的事件（revents）。
- **`O_NONBLOCK`**：非阻塞模式，确保在 `poll` 确认有数据后，`read` 操作能够快速清空缓冲区。

## 编译与运行
```bash
# 编译
gcc input_poll_test.c -o input_poll_test

# 运行 (以触控板为例)
sudo ./input_poll_test /dev/input/event13
```
