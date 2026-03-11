#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * 程序功能：通过 waitpid 解决僵尸进程
 */
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 失败");
        return -1;
    } else if (pid == 0) {
        // 子进程逻辑
        printf("[子进程] PID: %d，正在运行并准备退出...\n", getpid());
        sleep(2);
        printf("[子进程] 退出。\n");
        exit(0);
    } else {
        // 父进程逻辑
        printf("[父进程] 正在等待子进程 %d 结束...\n", pid);
        
        /**
         * waitpid 会阻塞父进程，直到指定的子进程退出。
         * 此时内核会回收子进程的所有残留信息，防止其变成僵尸。
         */
        int status;
        waitpid(pid, &status, 0); 
        
        if (WIFEXITED(status)) {
            printf("[父进程] 检测到子进程正常退出，状态码: %d\n", WEXITSTATUS(status));
        }
        
        printf("[父进程] 任务完成，准备退出。\n");
    }

    return 0;
}
