import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv("output.txt", header=None)
pts = np.array(data)
plt.plot(pts)
plt.show()
