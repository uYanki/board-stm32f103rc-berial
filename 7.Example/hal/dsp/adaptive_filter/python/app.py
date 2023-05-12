import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv("output.txt", sep='\t', header=None)
src = np.array(data[0])  # 原始数据
ideal = np.array(data[1])  # 理想数据
dst = np.array(data[2])  # 处理后的数据
plt.figure()
plt.subplot(3, 1, 1)
plt.plot(src)
plt.subplot(3, 1, 2)
plt.plot(ideal)
plt.subplot(3, 1, 3)
plt.plot(dst)
plt.show()
