# 泰山派 RK3566 驱动 Makefile
KERN_DIR    ?= /home/zz/taisan_Pi_linux_sdk/tspi_sdk/kernel/
CROSS_COMPILE := aarch64-linux-gnu-
ARCH         := arm64

obj-m += hello_drv.o

PWD       := $(shell pwd)
BUILD_DIR := $(PWD)/build
DEPLOY_DIR := /root/hello_drv

.PHONY: all clean deploy

all:
	@mkdir -p $(BUILD_DIR)
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	$(CROSS_COMPILE)gcc -o $(BUILD_DIR)/hello_drv_test hello_drv_test.c
	@mv $(PWD)/*.ko $(PWD)/*.mod.c $(PWD)/*.o $(PWD)/.*.cmd $(PWD)/*.symvers $(PWD)/*.order $(BUILD_DIR) 2>/dev/null || true
	@echo "------------------------------------------------"
	@echo "编译完成！产物位于: $(BUILD_DIR)"
	@echo "------------------------------------------------"

clean:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules clean ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	rm -rf $(BUILD_DIR)

deploy: all
	adb shell mkdir -p $(DEPLOY_DIR)
	adb push $(BUILD_DIR)/hello_drv.ko $(DEPLOY_DIR)/
	adb push $(BUILD_DIR)/hello_drv_test $(DEPLOY_DIR)/
	@echo "------------------------------------------------"
	@echo "已部署到泰山派 $(DEPLOY_DIR)/"
	@echo "------------------------------------------------"
