import subprocess
import re
import numpy as np
import matplotlib.pyplot as plt

programName = "numericalIntegration"

# arraySize is in MB.
def runAndGetResult(threadCount: int, trapezes: int, runs: int):
    times = np.zeros((runs), dtype=float)
    for i in range (0, runs):
        processResult = subprocess.run([f"./{programName}", f"{threadCount}", f"{trapezes}"], capture_output=True)
        result = processResult.stdout.decode()
        times[i] = re.search("[+-]?([0-9]*[.])?[0-9]+", result)[0]
    return times.mean()


# Compile the program (just in case)
subprocess.run(["g++", "-std=c++20", "-pthread", f"{programName}.cpp", "-o", f"{programName}"])

threads = np.arange(1, 16 + 1, 1)
trapezes = np.arange(1, 2_000_000 + 125_000, 125_000)

completionTimes = np.zeros((len(threads), len(trapezes)), dtype=float) 

# Run the program and retreive the time taken.
for t, threadCount in enumerate(threads):
    for n, trapezeCount in enumerate(trapezes):
        completionTime = float(runAndGetResult(threadCount, trapezeCount, 10))
        completionTimes[t][n] = completionTime


figure, ax = plt.subplots()
ax.set_ylabel("Time [s]")
ax.set_xlabel("Trapezes")
for threadCount, timesForThread in zip(threads, completionTimes):
    ax.plot(trapezes, timesForThread, label=f"{threadCount} threads")

ax.legend()
plt.savefig("exercise-1-time.png", dpi=400, bbox_inches='tight')
np.savetxt("data.csv", completionTimes)