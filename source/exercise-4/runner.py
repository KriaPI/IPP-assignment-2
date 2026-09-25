import subprocess
import re
import numpy as np
import matplotlib.pyplot as plt

programName = "benchmark_example"

def runAndGetResult(threadCount: int):
    # Matches floats/integers located strictly at the end of each line
    pattern = r"[+-]?(?:\d*\.\d+|\d+)\s*$"

    processResult = subprocess.run(
        [f"./{programName}", f"{threadCount}"], capture_output=True, text=True
    )

    # Extract all floating-point numbers across lines
    matches = re.findall(pattern, processResult.stdout, flags=re.MULTILINE)

    # Convert extracted strings to floats inside a NumPy array
    return np.array(matches, dtype=float)


# Compile the program (just in case)
subprocess.run(["g++", "-std=c++20", "-O3", f"{programName}.cpp", "-o", f"{programName}"])

def plot(plotIndex, row, threads):
    figure, ax = plt.subplots(layout='constrained')
    ax.set_ylabel("Kilo Operations / second")
    ax.set_xlabel("Threads")
    colors = plt.cm.turbo(np.linspace(0, 1, 4))

    fileNames = ["read", "update", "mixed"]
    titles = ["Course-grained list", "Fine-grained list", "Course-grained with TATAS", "Fine-grained with TATAS"]


    for index, (array, title)  in enumerate(zip(row, titles)):
        ax.plot(threads, array, color=colors[index], label=title)


    box = ax.get_position()
    ax.set_position([box.x0, box.y0 + box.height * 0.1,
                     box.width, box.height * 0.9])

    ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),
              fancybox=True, shadow=True, ncol=5)
    plt.savefig(f"exercise4-{fileNames[plotIndex]}.png", dpi=400, bbox_inches='tight')

def benchmark(name: str):
    threads = 2 ** np.arange(0, 5)
    print(threads)

    # Thousands of operations per second.
    operations = []

    # Run the program and retreive the time taken.
    for threadCount in threads:
        values = runAndGetResult(threadCount)
        reshaped_values = values.reshape(-1, 3).T
        operations.append(reshaped_values)

    operations = np.dstack(operations)

    for index, row in enumerate(operations):
        print(row)
        plot(index, row, threads)
    

benchmark(name="exercise-4")