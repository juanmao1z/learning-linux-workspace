#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * SIGCHLD 信号处理函数
 * 当子进程退出时，内核会发送 SIGCHLD 信号到父进程。
 * 父进程在该函数中回收子进程。
 */
static void sigchld_handler(int sig)
{
    int status;
    pid_t pid;

    /**
     * WNOHANG 表示如果没有子进程退出，waitpid 将立即返回而不阻塞。
     * 使用 while 循环是为了应对多个子进程同时退出时，信号可能被合并的情况。
     */
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[异步回收] 收到信号，检测到子进程 %d 退出，并已成功回收。\n", pid);
    }
}

int main()
{
    // 1. 注册 SIGCHLD 信号的处理函数
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction 设置失败");
        return -1;
    }

    printf("[父进程] 已开启异步回收机制...\n");

    // 2. 创建一个子进程作为实验对象
    pid_t pid = fork();
    if (pid == 0) {
        // 子进程逻辑
        printf("[子进程] %d 正在运行，3 秒后退出...\n", getpid());
        sleep(3);
        exit(10); // 返回状态码 10
    }

    // 3. 主进程去忙别的事情，不再显式调用 wait()
    printf("[父进程] PID: %d，我正在忙着执行其他任务，没时间主动等子进程...\n", getpid());
    
    while (1) {
        // 模拟执行其他业务或进入睡眠
        printf("[父进程] 主循环正常运行中...\n");
        sleep(2);
    }

    return 0;
}
