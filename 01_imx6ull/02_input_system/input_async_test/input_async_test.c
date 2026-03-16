#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <errno.h>

/* 定义全局变量，以便信号处理函数访问 */
static int fd;

/**
 * SIGIO 信号处理函数
 * 当驱动程序中有数据可读时，内核会向本进程发送 SIGIO 信号，
 * 从而触发本函数的执行。
 */
static void sigio_handler(int sig)
{
    struct input_event ev;

    // 在信号处理函数中循环读取所有已就绪的输入事件
    // 注意：fd 必须是以非阻塞 (O_NONBLOCK) 模式打开的
    while (read(fd, &ev, sizeof(ev)) == sizeof(ev)) {
        if (ev.type != EV_SYN) {
            printf("[异步信号捕获] 类型: 0x%04x, 编码: 0x%04x, 数值: %d\n", 
                    ev.type, ev.code, ev.value);
        } else {
            printf("---------- 帧同步 (SYN_REPORT) ----------\n");
        }
    }
}

/**
 * 程序功能：使用信号驱动的异步通知方式监测输入设备
 * 用法：sudo ./input_async_test <设备路径>
 */
int main(int argc, char **argv)
{
    int flags;

    // 检查命令行参数
    if (argc != 2) {
        printf("用法: sudo %s <设备路径>\n", argv[0]);
        printf("示例: sudo %s /dev/input/event13\n", argv[0]);
        return -1;
    }

    // 1. 以只读和非阻塞模式打开设备
    fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("打开设备失败");
        return -1;
    }

    // 2. 注册信号处理函数，捕获 SIGIO 信号
    signal(SIGIO, sigio_handler);

    // 3. 将本进程的 PID 告知内核，设置该文件描述符的所有者
    // 这样内核才知道应该把信号发给哪个进程
    if (fcntl(fd, F_SETOWN, getpid()) < 0) {
        perror("设置 F_SETOWN 失败");
        return -1;
    }

    // 4. 启用异步通知机制 (FASYNC)
    // 获取当前标志位并添加 FASYNC (或 O_ASYNC) 标志
    flags = fcntl(fd, F_GETFL);
    if (fcntl(fd, F_SETFL, flags | FASYNC) < 0) {
        perror("启用异步通知失败");
        return -1;
    }

    printf("异步通知模式已开启: %s (按 Ctrl+C 退出)\n", argv[1]);
    printf("主程序现在可以去执行其他任务或进入休眠...\n");

    // 5. 主循环：模拟主程序在处理其他业务，而不阻塞在读操作上
    while (1) {
        // 使用 pause() 让主进程进入低功耗休眠，直到捕获到信号
        pause();
        // 如果这里有其他业务逻辑，可以在信号处理完毕后继续执行
        // printf("主循环：捕获到信号并处理完毕，继续休眠...\n");
    }

    close(fd);
    return 0;
}
