#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>

/**
 * 程序功能：使用 select 函数异步监测输入设备事件
 * 用法：sudo ./input_select_test <设备路径>
 * 说明：select 是一种传统的多路复用 IO 模型，常用于监控多个文件描述符的状态。
 */
int main(int argc, char **argv)
{
    int fd;
    fd_set readfds;         // 定义文件描述符集合，用于读监测
    struct timeval tv;      // 定义超时时间结构体
    struct input_event ev;
    int ret;

    // 检查命令行参数，确保传入了正确的设备路径
    if (argc != 2) {
        printf("用法: sudo %s <设备路径>\n", argv[0]);
        printf("示例: sudo %s /dev/input/event13\n", argv[0]);
        return -1;
    }

    // 以只读和非阻塞模式 (O_NONBLOCK) 打开输入设备节点
    // 非阻塞模式对于确保 select 后的 read 操作不卡死至关重要
    fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("打开设备失败");
        return -1;
    }

    printf("正在监测设备: %s (使用 select 方式, 按 Ctrl+C 退出)\n", argv[1]);

    while (1) {
        /**
         * 重点注意：由于 select 会修改传入的集合和时间结构体，
         * 因此在每次循环开始前，必须重新初始化它们。
         */
        
        // 1. 清空集合并将当前文件描述符 fd 加入集合
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        // 2. 重新设置超时时间为 5 秒
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        /**
         * 3. 调用 select 函数进行等待
         * 第一个参数是最大文件描述符值加 1 (fd + 1)
         * 第二个参数是读集合，我们关心是否有数据可以读取
         * 如果在 5 秒内有输入动作，select 将返回大于 0 的整数
         */
        ret = select(fd + 1, &readfds, NULL, NULL, &tv);

        if (ret > 0) {
            // 4. 使用 FD_ISSET 检查该文件描述符是否真的是“已就绪”状态
            if (FD_ISSET(fd, &readfds)) {
                // 读取缓冲区中所有的 struct input_event 结构
                while (read(fd, &ev, sizeof(ev)) == sizeof(ev)) {
                    // 打印事件信息，跳过 EV_SYN 同步事件以减少干扰
                    if (ev.type != EV_SYN) {
                        printf("[select 捕获] 类型: 0x%04x, 编码: 0x%04x, 数值: %d\n", 
                                ev.type, ev.code, ev.value);
                    } else {
                        printf("---------- 帧同步 (SYN_REPORT) ----------\n");
                    }
                }
            }
        } else if (ret == 0) {
            // select 超时退出（5秒无输入）
            printf("select 提示: 5秒内无任何输入动作，正在重试等待...\n");
        } else {
            // 系统调用发生错误（可能是被信号中断）
            if (errno == EINTR) continue;
            perror("select 调用报错");
            break;
        }
    }

    // 关闭打开的文件，释放系统句柄
    close(fd);
    return 0;
}
