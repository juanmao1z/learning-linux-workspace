# 输入设备监测 - 异步通知方式 (SIGIO)

该目录包含一个使用 `SIGIO` 信号（异步通知机制）监测输入设备的 C 程序。

## 核心知识点
- **异步通知机制 (Asynchronous Notification)**：一种“信号驱动”模型，当驱动有数据时主动给应用层发信号。
- **`SIGIO` 信号**：内核向进程发送数据的“电铃”信号。
- **`signal(SIGIO, sigio_handler)`**：注册信号处理函数，处理事件。
- **`fcntl(fd, F_SETOWN, getpid())`**：告知内核哪个进程关注这个文件的信号。
- **`FASYNC` 标志位**：启用该文件的异步通知功能。

## 编译与运行
```bash
# 编译
gcc input_async_test.c -o input_async_test

# 运行 (以触控板为例)
sudo ./input_async_test /dev/input/event13
```
