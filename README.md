# Linux 学习工作区 (Linux Learning Workspace)

本仓库用于存放 Linux 驱动开发和系统编程的学习项目、实验代码及相关解决方案。

## 目录结构说明

### 01_imx6ull/ (i.MX6ULL 平台)
该目录存放针对 NXP i.MX6ULL 开发板（ARM32 架构，内核 4.9.88）的学习内容。
*   `01_char_driver/`: 基础字符设备驱动（如 hello_drv）。
*   `02_input_system/`: Linux 输入系统实验（Poll, Select, Async, 多点触摸等）。
*   `03_sensor/`: 传感器驱动实验（如 AP3216C）。

### 02_tspi/ (泰山派平台)
该目录存放针对 泰山派 RK3566 开发板（ARM64 架构，内核 4.19.232）的学习内容。
*   `01_char_driver/`: 适配泰山派的字符设备驱动。

### 99_general/ (通用方案)
**Misc (Miscellaneous)** 意为“杂项”。此处存放不限于特定硬件平台的通用 Linux 编程方案或小工具。
*   `zombie_solution/`: 僵尸进程处理方案。

---

## 编译指南 (Compilation Guide)

在进行交叉编译前，请务必在终端中设置对应的交叉编译器路径。

### i.MX6ULL (Buildroot)
```bash
export PATH=$PATH:/home/zz/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/
# 然后在对应的项目目录下执行 make
```

### 泰山派 (RK3566)
```bash
export PATH=$PATH:/home/zz/taisan_Pi_linux_sdk/tspi_sdk/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/
# 然后在对应的项目目录下执行 make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
```

---
*祝学习愉快！保持代码整洁是成为优秀工程师的第一步。*
