import numpy as np
import matplotlib.pyplot as plt
import random

#read from file
words = []
x_values = []


#with open('/Users/snehabelkhale/Documents/word2vec-master/src/scores.txt', 'r') as f:
with open('scores.txt', 'r') as f:
    for line in f:
        items = line.split()
        words.append(items[0])
        x_values.append(float(items[1]))

fig = plt.figure()
ax = fig.add_subplot(111)



for idx, xval in enumerate(x_values):

    ax.annotate(words[idx], xy=(1, 1), xytext=(xval*50, float(idx)/100),
            )
ax.set_ylim(-.5,1.5)
ax.set_xlim(-15,20)
plt.xlabel("he <---> she  axis ")
plt.show()
