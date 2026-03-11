# Learning Linux Workspace

该项目用于记录嵌入式 Linux 的学习过程，涵盖了内核源码分析、字符设备驱动、应用层多路复用、进程管理以及硬件通信原理等实验代码。

## 项目结构与实验索引

### 1. 输入子系统与 I/O 多路复用 (Input Subsystem & I/O Multiplexing)
*   **[input_poll_test](./input_poll_test)**: 使用 `poll()` 系统调用异步监测输入设备（如触控板、键盘）事件。
*   **[input_select_test](./input_select_test)**: 使用传统的 `select()` 函数实现输入设备多路复用。
*   **[input_async_test](./input_async_test)**: 使用信号驱动的异步通知机制 (`SIGIO/FASYNC`) 监测硬件事件。
*   **[mt_distance_test](./mt_distance_test)**: 基于 `tslib` 库实现的多点触控实验，实时计算双指间的物理距离（平方值）。

### 2. 进程管理与系统编程 (Process Management)
*   **[zombie_solution](./zombie_solution)**: 深入研究僵尸进程的产生原因，并提供同步 (`waitpid`) 和 异步 (`SIGCHLD`) 两种回收方案。

### 3. 底层原理探索 (Under-the-hood Research)
*   **内核结构体分析**: 详细解析了 `struct file` (已打开文件实例)、`struct input_dev` (输入设备抽象) 等核心结构。
*   **硬件协议理解**: 记录了 UART 串口通信中“起始位”的物理电平实现与接收端的采样同步逻辑。
*   **内存统计差异**: 探讨了 `top` 调用的 VIRT/RES 与图形化系统监视器在内存计算口径上的区别。

## 开发环境
*   **OS**: Ubuntu 22.04 LTS (Linux Kernel 5.15+)
*   **SDK**: i.MX6ULL 100ask SDK (Linux Kernel 4.9.88)
*   **工具链**: gcc, gdb, tslib, evtest

---
*保持好奇，持续探索 Linux 内核的奥秘。*
