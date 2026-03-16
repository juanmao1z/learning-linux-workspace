
# 1. 内核源码路径
KERN_DIR ?= /home/zz/100ask_imx6ull-sdk/Linux-4.9.88
CROSS_COMPILE ?= arm-buildroot-linux-gnueabihf-

# 2. 目标文件定义
obj-m += hello_drv.o
BUILD_DIR := $(shell pwd)/build

all:
	# 创建 build 目录
	mkdir -p $(BUILD_DIR)
	# 编译内核模块
	make -C $(KERN_DIR) M=$(shell pwd) modules ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE)
	# 编译测试程序并直接放入 build 目录
	$(CROSS_COMPILE)gcc -o $(BUILD_DIR)/hello_drv_test hello_drv_test.c 
	# 将生成的 .ko 移动到 build 目录
	mv *.ko *.mod.c *.o .*.cmd *.symvers *.order $(BUILD_DIR) 2>/dev/null || true
	@echo "------------------------------------------------"
	@echo "编译完成！产物位于: $(BUILD_DIR)"
	@echo "------------------------------------------------"

clean:
	make -C $(KERN_DIR) M=$(shell pwd) modules clean
	rm -rf $(BUILD_DIR)
	rm -f modules.order
