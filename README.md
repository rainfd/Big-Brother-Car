# Big Brother 智能车电磁组工程

-------

# 使用的IDE
* IAR 7.2 以上版本

# 使用芯片型号
* MK60系列

# 使用固件库
* 超核固件库

# 文件说明
//device
  8700_2100.c 九轴陀螺仪加速度地磁计
  DataScope_DP.c 虚拟示波器
  LQ_OLED.c IIC OLED
//user
  calaulate.c 角度转换 PID控制器
  counter.c 红外码盘
  filter.c 滤波算法
  main.c 主程序 初始化 数据显示
  motor.c 电机控制
  timer.c 定时器 中断
