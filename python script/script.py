import matplotlib.pyplot as plt
import numpy as np
from array import *

with open("out.txt") as textFile:
    lines = [line.split() for line in textFile]

for i in range(len(lines)):
    for j in range(len(lines[i])):
        lines[i][j] = float(lines[i][j])    

for r in lines:
   for c in r:
      print(c,end = " ")
   print()

plt.imshow(lines)
plt.show()