# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# %%
data = pd.read_csv("output.txt", sep='\t', header=None)

# %%
ia_in = np.array(data[0])  # 原数据
ib_in = np.array(data[1])
ia_out = np.array(data[2])  # 克拉克变换后的数据
ib_out = np.array(data[3])


# %%
plt.figure(figsize=(10, 17))
plt.subplot(4, 1, 1)
plt.title("ia_in")
plt.plot(ia_in)
plt.subplot(4, 1, 2)
plt.title("ib_in")
plt.plot(ib_in)
plt.subplot(4, 1, 3)
plt.title("ia_out")
plt.plot(ia_out)
plt.subplot(4, 1, 4)
plt.title("ib_out")
plt.plot(ib_out)
plt.show()
