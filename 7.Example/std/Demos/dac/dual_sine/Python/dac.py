#! python3
# coding=utf-8

"""
Python版本：3.x

设置TIM_Period、TIM_Prescaler、point
计算正弦波频率。

运行方式：
在命令行中输入：python dac.py 
"""

TIM_Period = 19  # 定时周期数
TIM_Prescaler = 0  # 定时时钟分频
point = 32  # 单周期信号点数

Tsysclk = 1/72000000  # 系统时钟周期
Ttim = Tsysclk * (TIM_Prescaler+1)  # 定时时钟周期
Tupdate = Ttim * (TIM_Period + 1)  # 定时器触发周期
Tsin = Tupdate * point  # 一个正弦波周期内的dac点数
fsin = 1/Tsin  # 正弦波频率

print("正弦波频率=%.2f" % fsin)
