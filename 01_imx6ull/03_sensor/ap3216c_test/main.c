#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <errno.h>

/* 引入 i2c-tools 的头文件 */
#include <i2c/smbus.h>

/*
 * AP3216C 传感器关键参数
 * 地址: 0x1E
 */

int main(int argc, char *argv[]) {
    int file;
    char *filename;
    int addr = 0x1E;

    // 检查命令行参数
    if (argc < 2) {
        printf("用法: %s <I2C设备路径>\n", argv[0]);
        printf("示例: %s /dev/i2c-0\n", argv[0]);
        return 1;
    }

    filename = argv[1];

    // 1. 打开 I2C 设备文件
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the bus");
        return 1;
    }

    // 2. 设置从机地址
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(file);
        return 1;
    }

    // 3. 使用 i2c-tools 的 SMBus 函数初始化 AP3216C
    // 复位传感器: 向寄存器 0x00 写入 0x04
    if (i2c_smbus_write_byte_data(file, 0x00, 0x04) < 0) {
        perror("Failed to reset AP3216C via SMBus");
    }
    usleep(50000); 

    // 开启全模式 (ALS+PS+IR): 向寄存器 0x00 写入 0x03
    if (i2c_smbus_write_byte_data(file, 0x00, 0x03) < 0) {
        perror("Failed to enable sensor modes via SMBus");
    }
    usleep(50000);

    printf("Reading AP3216C data on %s using i2c-tools (SMBus)...\n", filename);
    printf("Total 20 readings planned...\n");

    // 4. 循环读取传感器数据 (限制为 20 次)
    int count = 0;
    while(count < 20) {
        int ir_low, ir_high, als_low, als_high, ps_low, ps_high;
        unsigned short ir, als, ps;

        // 使用 i2c_smbus_read_byte_data 读取各寄存器
        ir_low   = i2c_smbus_read_byte_data(file, 0x0A);
        ir_high  = i2c_smbus_read_byte_data(file, 0x0B);
        als_low  = i2c_smbus_read_byte_data(file, 0x0C);
        als_high = i2c_smbus_read_byte_data(file, 0x0D);
        ps_low   = i2c_smbus_read_byte_data(file, 0x0E);
        ps_high  = i2c_smbus_read_byte_data(file, 0x0F);

        if (ir_low < 0 || ir_high < 0 || als_low < 0 || als_high < 0 || ps_low < 0 || ps_high < 0) {
            printf("[%d] Error reading data!\n", count + 1);
        } else {
            ir = (ir_high << 8) | ir_low;
            als = (als_high << 8) | als_low;
            ps = ((ps_high & 0x3F) << 4) | (ps_low & 0x0F);

            printf("[%d] IR: %-4d, ALS: %-5d, PS: %-4d\n", count + 1, ir, als, ps);
        }
        
        count++;
        sleep(1);
    }

    printf("20 readings completed. Exiting...\n");

    close(file);
    return 0;
}
