# 2D Plotting
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
# plt.show()
# plt.imshow(data)
plt.show()












# from mpl_toolkits import mplot3d
# import numpy as np
# import matplotlib.pyplot as plt
# fig = plt.figure()
# ax = plt.axes(projection='3d')

# with open("out.txt") as textFile:
#     lines = [line.split() for line in textFile]

# # for r in lines:
# #    for c in r:
# #       print(c,end = " ")
# #    print()

# ax.set_xlabel('X-axis', fontweight ='bold')
# ax.set_ylabel('Y-axis', fontweight ='bold')
# ax.set_zlabel('Z-axis', fontweight ='bold')


# for i in range(len(lines)):
#     x = lines[i][0]
#     y = lines[i][1]
#     z = lines[i][2]
#     ax.scatter3D(x, y,z, c=1, cmap='pink', linewidth=0.5);

# #plt.imshow(lines)
# plt.show()



# data = [[1, 2, 3],
#         [4, 5, 6],
#         [7, 8, 9]]

# plt.imshow(data)
# plt.show()
#################################

# from mpl_toolkits import mplot3d
# import numpy as np
# import matplotlib.pyplot as plt
# fig = plt.figure()
# ax = plt.axes(projection='3d')

# # # Data for a three-dimensional line
# # zline = np.linspace(0, 2, 100)
# # xline = np.linspace(1, 16, 100)
# # yline = np.linspace(2, 17, 100)
# # ax.plot3D(xline, yline, zline, 'red')

# #Data for three-dimensional scattered points

# array3d = [[[0, 0, 0], [0, 0, 0], [0, 0, 0]],
#  [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
#  [[0, 0, 0], [0, 0, 0], [0, 0, 0]]]

# counterr = 0

# for i in range(len(array3d)):
#     for j in range(len(array3d[i])):
#             for k in range(len(array3d[i][j])):
#                 array3d[i][j][k] = counterr 
#                 counterr = counterr + 1  
# # zdata = 1
# # xdata = 2
# # ydata = 3
# for i in range(len(array3d)):
#     for j in range(len(array3d[i])):
#             for k in range(len(array3d[i][j])):
#                 ax.scatter3D(array3d[i][j][k], array3d[i][j][k], array3d[i][j][k], c=array3d[i][j][k], cmap='pink');

# ax.set_xlabel('X-axis', fontweight ='bold')
# ax.set_ylabel('Y-axis', fontweight ='bold')
# ax.set_zlabel('Z-axis', fontweight ='bold')

# plt.show()