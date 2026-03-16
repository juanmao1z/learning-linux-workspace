# AP3216C I2C 传感器测试程序

该程序基于 Linux I2C-Tools 的 SMBus 库开发，用于在 IMX6ULL 等嵌入式开发板上测试 AP3216C 传感器（环境光、红外、接近距离）。

## 目录结构
- `main.c`: 主测试程序，包含 20 次循环读取逻辑。
- `i2c-tools-4.2/`: 依赖的 I2C-Tools 源码。
- `Makefile`: 自动化编译脚本（支持交叉编译与静态链接）。

## 如何编译

### 1. 修改 Makefile (可选)
如果你的交叉编译器路径不同，请修改 `Makefile` 中的 `CROSS_COMPILE` 变量。

### 2. 执行编译
在当前目录下运行：
```bash
make
```
编译完成后会生成静态链接的二进制文件 `ap3216c_smbus_static`。

## 如何运行

### 使用 ADB 自动测试
```bash
make test
```
*该命令会自动将程序 push 到开发板的 /tmp 目录并在 /dev/i2c-0 上运行。*

### 手动运行
1. 上传文件：
   ```bash
   adb push ap3216c_smbus_static /tmp/
   ```
2. 在开发板执行：
   ```bash
   chmod +x /tmp/ap3216c_smbus_static
   /tmp/ap3216c_smbus_static /dev/i2c-0
   ```

## 输出说明
程序会连续读取 20 条数据并自动退出，包含：
- **IR**: 红外强度
- **ALS**: 环境光照强度
- **PS**: 接近距离
