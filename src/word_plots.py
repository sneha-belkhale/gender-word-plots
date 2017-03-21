import numpy as np
import matplotlib.pyplot as plt
import random

#read from scores file
words = []
x_values = []


with open('scores.txt', 'r') as f:
    for line in f:
        items = line.split()
        words.append(items[0])
        x_values.append(float(items[1]))

fig = plt.figure()
ax = fig.add_subplot(111)

scale = 100

for idx, xval in enumerate(x_values):

    ax.annotate(words[idx], xy=(1, 1), xytext=(xval*scale, float(idx)/len(x_values)),
            )

ax.set_xlim(np.min(x_values)*scale-1,np.max(x_values)*scale+1)
plt.xlabel("he <---> she  axis ")
plt.show()
