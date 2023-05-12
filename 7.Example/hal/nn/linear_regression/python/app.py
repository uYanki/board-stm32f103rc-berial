# %%
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt

# %%
px = [187.1, 179.5, 157.0, 197.0, 239.4, 217.8, 227.1, 233.4, 242.0, 251.9, 230.0, 271.8]
py = [25.4, 22.8, 20.6, 21.8, 32.4, 24.4, 29.3, 27.9, 27.8, 34.2, 29.2, 30.0]

# %%
slope = 0.108141
intercept = 3.412968
lx = np.linspace(min(px)-50, max(px)+50, 10)
ly = slope*lx+intercept
print(slope, intercept)
plt.plot(lx, ly)
plt.scatter(px, py)
plt.show()

# %%
slope, intercept, r_value, p_value, std_err = stats.linregress(px, py)
lx = np.linspace(min(px)-50, max(px)+50, 10)
ly = slope*lx+intercept
print(slope, intercept)
plt.plot(lx, ly)
plt.scatter(px, py)
plt.show()
