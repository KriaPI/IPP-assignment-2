import subprocess
import re
import numpy as np
import matplotlib.pyplot as plt

programName = "integralApproximator"

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

def benchmark(name: str, start: int, end: int, stepsize: int):
    threads = 2 ** np.arange(0, 6)
    trapezes = np.arange(start, end + 1, stepsize)

    completionTimes = np.zeros((len(threads), len(trapezes)), dtype=float) 

    # Run the program and retreive the time taken.
    for t, threadCount in enumerate(threads):
        for n, trapezeCount in enumerate(trapezes):
            completionTime = float(runAndGetResult(threadCount, trapezeCount, 10))
            completionTimes[t][n] = completionTime

    completionTimes = completionTimes * 1000

    figure, ax = plt.subplots(layout='constrained')
    ax.set_ylabel("Time [ms]")
    ax.set_xlabel("Trapezes")
    colors = plt.cm.turbo(np.linspace(0, 1, len(threads)))

    for t, (threadCount, timesForThread) in enumerate(zip(threads, completionTimes)):
        ax.plot(trapezes, timesForThread, color=colors[t], label=f"{threadCount} threads")

    box = ax.get_position()
    ax.set_position([box.x0, box.y0 + box.height * 0.1,
                 box.width, box.height * 0.9])

    ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),
          fancybox=True, shadow=True, ncol=5)
    plt.savefig(f"{name}.png", dpi=400, bbox_inches='tight')
    np.savetxt(f"{name}.csv", completionTimes, fmt="%.2f")


benchmark(name="exercise-1-low", start=1, end=64, stepsize=4)
benchmark(name="exercise-1-high", start=10_000, end=500_000, stepsize=10_000)