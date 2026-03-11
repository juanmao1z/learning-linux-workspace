#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <linux/input.h>
#include <errno.h>

/**
 * 程序功能：使用 poll 函数异步监测输入设备事件
 * 用法：sudo ./input_poll_test <设备路径>
 * 说明：每个输入事件由 struct input_event 结构表示。
 */
int main(int argc, char **argv)
{
    int fd;
    struct pollfd fds[1];
    struct input_event ev;
    int ret;

    // 检查命令行参数，确保传入了正确的设备路径
    if (argc != 2) {
        printf("用法: sudo %s <设备路径>\n", argv[0]);
        printf("示例: sudo %s /dev/input/event13\n", argv[0]);
        return -1;
    }
    // 以只读和非阻塞模式 (O_NONBLOCK) 打开输入设备节点
    // 配合 poll 使用时，可以确保 read 操作不会阻塞主线程
    fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("打开设备失败");
        return -1;
    }

    // 配置 poll 结构体参数
    fds[0].fd = fd;
    fds[0].events = POLLIN; // 设置监测类型为：有可读数据输入

    printf("已进入监测状态: %s (按 Ctrl+C 退出监测)\n", argv[1]);

    while (1) {
        /**
         * 调用 poll 等待事件响应
         * 参数 5000 代表最大等待时间为 5000 毫秒（5秒）
         * 如果在此期间有数据输入，则返回大于 0 的整数
         */
        ret = poll(fds, 1, 5000);

        if (ret > 0) {
            // 如果返回正数且发生的是读事件 (POLLIN)
            if (fds[0].revents & POLLIN) {
                // 持续读取缓冲区，直到全部解析为 struct input_event 包
                while (read(fd, &ev, sizeof(ev)) == sizeof(ev)) {
                    // 过滤并处理同步标记位 (EV_SYN)，打印核心数据
                    if (ev.type != EV_SYN) {
                        printf("[捕获数据包] 类型: 0x%04x, 编码: 0x%04x, 数值: %d\n", 
                                ev.type, ev.code, ev.value);
                    } else {
                        printf("---------- 帧同步 (SYN_REPORT) ----------\n");
                    }
                }
            }
        } else if (ret == 0) {
            // 超过 5 秒没有任何输入时执行此处代码
            printf("系统提示: 5秒内没有任何监测动作，程序继续等待...\n");
        } else {
            // poll 调用因为某种系统原因报错
            perror("Poll 系统调用出错");
            break;
        }
    }

    // 安全关闭文件句柄，回收系统资源
    close(fd);
    return 0;
}
