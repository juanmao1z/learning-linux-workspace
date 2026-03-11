# 多点触控 - 双指距离计算 (Tslib)

该目录包含一个基于 `tslib` 库编写的多点触控应用程序，用于实时计算两根手指之间的距离。

## 核心知识点
- **`tslib` 库**：嵌入式 Linux 中处理触摸屏数据的工业级标准库，简化了 MT（多点触控）协议的解析。
- **`ts_read_mt()`**：一次性读取多根手指的坐标信息，将数据填充到 `ts_sample_mt` 结构体中。
- **多点触控协议 (Slots)**：内核使用“槽位”来并行管理多个触摸点（Slot 0、Slot 1 等）。
- **`tracking_id`**：用于跟踪同一个手指的持续动作（按住、移动、抬起）。

## 编译与运行
确保已安装 `libts-dev`：
```bash
sudo apt-get install libts-dev
```

编译并运行：
```bash
# 编译并链接库
gcc mt_cal_distance.c -o mt_cal_distance -lts

# 运行 (需要指定 TSLIB_TSDEVICE)
sudo TSLIB_TSDEVICE=/dev/input/event13 ./mt_cal_distance
```
