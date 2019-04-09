import matplotlib.pyplot as plt
import numpy as np

n = 100

csv = np.genfromtxt ('ACA.out', delimiter="")

aa = np.zeros((n,n))

for i in range(n):
  for j in range(n):
    aa[i,j] = csv[i*n + j]

plt.matshow(aa)
plt.show()
