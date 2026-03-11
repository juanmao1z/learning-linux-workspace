#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <tslib.h>

/**
 * 函数功能：计算两点之间的距离（平方值）
 * 参数：point1, point2 - tslib 提供的多点触控采样结构体
 * 返回：距离的平方 (dx^2 + dy^2)
 */
int distance(struct ts_sample_mt *point1, struct ts_sample_mt *point2)
{
    int x = point1->x - point2->x;
    int y = point1->y - point2->y;
    return x*x + y*y;
}

int main(int argc, char **argv)
{
    struct tsdev *ts;
    int i, ret;
    struct ts_sample_mt **samp_mt;      // 存储当前采样的多点数据
    struct ts_sample_mt **pre_samp_mt;  // 存储历史记录的多点数据
    int max_slots;                      // 硬件支持的最大触摸点数
    int point_pressed[20];              // 记录当前按下的手指索引
    struct input_absinfo slot;          // 用于获取触摸槽位信息
    int touch_cnt = 0;                  // 记录当前按下的手指数量

    // 1. 初始化 tslib，自动检测触摸设备
    // NULL 表示使用环境变量 TSLIB_TSDEVICE 定义的设备
    ts = ts_setup(NULL, 0);
    if (!ts)
    {
        printf("ts_setup 出错，请检查 TSLIB_TSDEVICE 环境变量或设备权限\n");
        return -1;
    }

    // 2. 通过 ioctl 获取硬件支持的最大触摸点数（Slots）
    if (ioctl(ts_fd(ts), EVIOCGABS(ABS_MT_SLOT), &slot) < 0) {
        perror("获取 ABS_MT_SLOT 信息失败");
        ts_close(ts);
        return errno;
    }

    // 计算槽位数量（例如：如果 minimum 为 0，maximum 为 4，则有 5 个槽位）
    max_slots = slot.maximum + 1 - slot.minimum;
    printf("硬件支持的最大触摸点数: %d\n", max_slots);

    // 3. 为采样数据分配内存
    // tslib 的 ts_read_mt 要求传入二级指针
    samp_mt = malloc(sizeof(struct ts_sample_mt *));
    samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));

    pre_samp_mt = malloc(sizeof(struct ts_sample_mt *));
    pre_samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));

    // 初始化历史数据，将有效标志设为 0
    for (i = 0; i < max_slots; i++)
        pre_samp_mt[0][i].valid = 0;

    printf("开始监测... 请使用两个手指操作触控板\n");

    while (1)
    {
        // 4. 读取多点触控数据
        // 参数 1 表示每次读取 1 帧数据
        ret = ts_read_mt(ts, samp_mt, max_slots, 1);
        if (ret < 0) {
            perror("读取触摸数据失败");
            break;
        }

        // 5. 更新历史记录：如果当前点有效，则保存到 pre_samp_mt 中
        for (i = 0; i < max_slots; i++)
        {
            if (samp_mt[0][i].valid)
            {
                memcpy(&pre_samp_mt[0][i], &samp_mt[0][i], sizeof(struct ts_sample_mt));
            }
        }

        // 6. 统计当前按在屏幕上的手指数量
        touch_cnt = 0;
        for (i = 0; i < max_slots; i++)
        {
            // 只有当 valid 为 1 且 tracking_id 不为 -1 时，才表示手指正在按下
            if (pre_samp_mt[0][i].valid && pre_samp_mt[0][i].tracking_id != -1)
                point_pressed[touch_cnt++] = i;
        }

        // 7. 如果正好有两根手指，计算并输出距离
        if (touch_cnt == 2)
        {
            int d = distance(&pre_samp_mt[0][point_pressed[0]], &pre_samp_mt[0][point_pressed[1]]);
            printf("双指检测成功！距离平方值: %08d\n", d);
        }
    }
    
    // 释放资源
    free(samp_mt[0]); free(samp_mt);
    free(pre_samp_mt[0]); free(pre_samp_mt);
    ts_close(ts);
    
    return 0;
}
