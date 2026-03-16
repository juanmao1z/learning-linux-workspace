# 僵尸进程 (Zombie Process) 的产生与处理

该目录包含两个使用 C 语言编写的程序，演示了如何有效处理 Linux 系统中的僵尸进程。

## 什么是僵尸进程？
僵尸进程（Zombie Process，状态码为 `Z` 或显示为 `<defunct>`）是一个已经结束运行，但其父进程尚未调用 `wait()` 或 `waitpid()` 系统调用来获取其终止状态的进程。

- **危害**：虽然不占 CPU 和内存，但会占用 **进程 ID (PID)**。如果 PID 资源耗尽，系统将无法创建新进程。

## 目录代码说明

### 1. 同步回收方案 (`wait_solution.c`)
- **核心函数**：`waitpid(pid, &status, 0)`
- **特点**：父进程会**阻塞（暂停运行）**，直到子进程退出。
- **适用场景**：父进程必须在子进程完成后才能继续执行下一步任务。

### 2. 异步回收方案 (`signal_solution.c`)
- **核心函数**：`signal(SIGCHLD, sigchld_handler)` 或 `sigaction()`
- **特点**：父进程**不会阻塞**，而是注册一个信号处理函数。当子进程退出时，内核会发出 `SIGCHLD` 信号，触发处理函数自动调用 `waitpid()` 回收子进程。
- **适用场景**：生产环境中最常用的方案，父进程可以同时处理多个业务。

## 为什么不直接忽略信号 (`SIG_IGN`)？
虽然可以通过 `signal(SIGCHLD, SIG_IGN)` 让内核自动回收，但这种方式会导致父进程无法获取子进程的**退出状态码 (Exit Code)**，且在某些老旧 Unix 系统上存在兼容性问题。

## 编译与运行
```bash
# 1. 编译代码
gcc wait_solution.c -o wait_solution
gcc signal_solution.c -o signal_solution

# 2. 运行实验
sudo ./wait_solution
sudo ./signal_solution
```

## 调试命令
在程序运行期间，你可以打开另一个终端，使用以下命令观察进程状态：
```bash
ps -ef | grep defunct
# 或者直接查看状态码
ps aux | grep 'Z'
```
