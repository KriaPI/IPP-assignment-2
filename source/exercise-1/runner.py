import subprocess
import re
import numpy as np
import matplotlib.colors as mcolors
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt

# arraySize is in MB.
def runAndGetResult(threadCount: int, arraySize: int):
    processResult = subprocess.run(["./performance", f"{threadCount}", f"{arraySize}"], capture_output=True)
    result = processResult.stdout.decode()
    return re.search("[+-]?([0-9]*[.])?[0-9]+", result)[0]




# Compile the program (just in case)
subprocess.run(["g++", "-std=c++11", "-pthread", "performance.cpp", "-o", "performance"])

executionRange = [1, 2, 4, 8, 16, 32]
entries = len(executionRange)
executionLabels = [f"{p}" for p in executionRange]
completionTimes = np.zeros((entries, entries), dtype=float) 

for t, threads in enumerate(executionRange):
    for n, megaBytes in enumerate(executionRange):
        print(f"{threads}, {megaBytes}")
        completionTime = float(runAndGetResult(threads, megaBytes))
        completionTimes[t][n] = completionTime
         
print(completionTimes)

# Create index grid for uniform bar spacing
xpos, ypos = np.meshgrid(np.arange(entries), np.arange(entries), indexing='ij')

# Flatten 2D matrices into 1D arrays
xpos = xpos.flatten()
ypos = ypos.flatten()
zpos = np.zeros_like(xpos)

dx = dy = 0.5
dz = completionTimes.flatten()

# Normalize heights and map them to a colormap ending in red
norm = mcolors.Normalize(vmin=dz.min(), vmax=dz.max())
cmap = plt.cm.YlOrRd  # Transitions from light yellow (low) to deep red (high)
colors = cmap(norm(dz))

figure = plt.figure()
ax = figure.add_subplot(projection='3d')

# Plot 3D bars using the color array
ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color=colors, shade=True)

# Add a colorbar legend
mappable = plt.cm.ScalarMappable(norm=norm, cmap=cmap)
mappable.set_array(dz)
figure.colorbar(mappable, ax=ax, shrink=0.5, aspect=5, label="Seconds")

# Set categorical tick labels
ax.set_xticks(np.arange(entries) + dx / 2)
ax.set_xticklabels(executionLabels)
ax.set_yticks(np.arange(entries) + dy / 2)
ax.set_yticklabels(executionLabels)

# Set axis labels
ax.set_xlabel("Threads")
ax.set_ylabel("Array size [MB]")
ax.set_zlabel("Seconds")

# Save the figure to file
plt.savefig("3d_bar_plot.png", dpi=300, bbox_inches='tight')
plt.close(figure)