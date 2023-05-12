import numpy as np
import math
POINT_NUM = 256  # 点数
AMPLITUDE = 2048  # 幅值
n = np.linspace(0, 2*math.pi, POINT_NUM)
vals = map(math.sin, n)  # [-1,1]
wave = []
for i in vals:
    v = round((i+1) * AMPLITUDE)  # [0,2] * amplitude
    if v > 4095:
        v = 4095
    wave.append(v)
print(list(map(int, wave)))
