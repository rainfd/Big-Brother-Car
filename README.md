# Big Brother 智能车电磁组工程

-------

# 使用的IDE
* IAR 7.2 以上版本

# 使用芯片型号
* MK60系列

# 使用固件库
* 超核固件库

# 文件说明

device
* 8700_2100.c 九轴陀螺仪加速度地磁计
* DataScope_DP.c 虚拟示波器
* LQ_OLED.c IIC OLED

user
* calaulate.c 角度转换 PID控制器
* filter.c 滤波算法
* main.c 主程序 初始化 数据显示
* timer.c 定时器 中断 


user_device
* counter.c 红外码盘
* motor.c 电机控制
* bluetooth.c 蓝牙收发
* ad.c 电磁检测 

-----

因硬件部分拖延很长时间，所以比赛就中途放弃。这个工程参照了官方的方案，程序框架基本确定，主要是将检测和电机处理等几个部分放进一个中断再按时间次序调度。电磁部分没有完成。
