# 交叉编译器路径
CROSS_COMPILE = /home/zz/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/arm-buildroot-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc

# 路径定义
I2C_TOOLS_DIR = i2c-tools-4.2
INCLUDE_DIR = $(I2C_TOOLS_DIR)/include
SMBUS_SRC = $(I2C_TOOLS_DIR)/lib/smbus.c

# 编译选项
# -I 指定头文件搜索路径
# -static 强制进行静态链接
# -Wall 显示所有警告
CFLAGS = -I $(INCLUDE_DIR) -Wall
LDFLAGS = -static

# 目标文件
TARGET = ap3216c_smbus_static

# 源码文件
SRCS = main.c $(SMBUS_SRC)

# 默认目标
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)
	@echo "编译完成: $(TARGET) (静态链接)"

# 清理目标
clean:
	rm -f $(TARGET)

# 上传并运行测试 (可选)
test: $(TARGET)
	adb push $(TARGET) /tmp/
	adb shell "chmod +x /tmp/$(TARGET) && /tmp/$(TARGET) /dev/i2c-0"

.PHONY: all clean test
