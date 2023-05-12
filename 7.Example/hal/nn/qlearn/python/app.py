# %%
import pandas as pd
import matplotlib.pyplot as plt

# %%
data = pd.read_csv("output.txt", sep='\t', header=None)

# %%
plt.figure(figsize=(14, 14))
plt.subplots_adjust(left=None, bottom=None, right=None, top=None,  wspace=0.1, hspace=0.5)

for i in range(14):
    plt.subplot(7, 2, i+1)
    plt.title(f"COL{i}")
    plt.plot(data[i])

plt.show()

# %%
plt.figure(figsize=(14, 7))

for i in range(14):
    plt.plot(data[i])

plt.show()
