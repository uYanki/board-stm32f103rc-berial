import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv("output.txt", sep='\t', header=None)

src = np.array(data[0])  # 采集值
dst = np.array(data[2])  # 预测值

fig = plt.figure(num=1)
plt.plot(src)
plt.plot(dst)
plt.show()
