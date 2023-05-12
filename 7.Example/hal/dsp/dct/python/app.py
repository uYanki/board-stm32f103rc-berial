# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import fftpack

# %%
# data = pd.read_csv("output_cmath.txt",sep='\t',header=None)
data = pd.read_csv("output_armmath.txt", sep='\t', header=None)

# %%
src = np.array(data[0])
dst = np.array(data[1])

# %%

full_size = len(src)
half_size = int(full_size / 2)
show_size = int(half_size / 5)

x = fftpack.fftfreq(full_size) * 6400  # 采样率 6400
y = fftpack.dct(src)
y[0] = y[0] / 2  # 直流分量多除个2

x = x[:half_size]  # 截半
y = y[:half_size]

plt.figure(figsize=(8, 8))
plt.subplots_adjust(hspace=0.3)

plt.subplot(3, 1, 1)

plt.title("wave")
plt.plot(src)

plt.subplot(3, 1, 2)
plt.title("python dct")
plt.scatter(x[:show_size], y[:show_size])

plt.subplot(3, 1, 3)
plt.title("stm32 dct")
plt.scatter(x[:show_size], dst[:show_size])

plt.show()
