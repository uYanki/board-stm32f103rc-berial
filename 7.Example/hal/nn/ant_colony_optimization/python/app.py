# %%
import numpy as np
import matplotlib.pyplot as plt

# %%
matrixs = []

with open("output.txt") as f:
    lines = f.readlines()
    matrix = []
    cutoff = 0
    for line in lines:
        if cutoff > 0:
            bits = line.strip().split(' ')
            matrix.append([int(i) for i in bits])
            cutoff -= 1
            if cutoff == 0:
                matrixs.append(matrix)
                matrix = []
        elif line.startswith("mSTEPS"):
            cutoff = 10  # 截取往下的10行

# %%
plt.figure(figsize=(10, 15))
plt.subplots_adjust(wspace=0.4, hspace=1)
for index, matrix in enumerate(matrixs):
    plt.subplot(10, 5, index+1)
    plt.title(f"{index+1}")
    plt.imshow(matrix)
