import csv
import matplotlib.pyplot as plt

RESULTS_FILE = "results.csv"


def load_results(filename):
    structures = []
    times = []
    memories = []

    with open(filename, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            structures.append(row["structure"])
            times.append(float(row["time_us"]))
            memories.append(float(row["memory_bytes"]))

    return structures, times, memories


def plot_time(structures, times):
    plt.figure()
    plt.bar(structures, times)
    plt.xlabel("Estructura de datos")
    plt.ylabel("Tiempo (μs)")
    plt.title("Comparación de tiempo KD-Tree vs VP-Tree")
    plt.tight_layout()
    plt.show()


def plot_memory(structures, memories):
    plt.figure()
    plt.bar(structures, memories)
    plt.xlabel("Estructura de datos")
    plt.ylabel("Memoria (bytes)")
    plt.title("Comparación de memoria KD-Tree vs VP-Tree")
    plt.tight_layout()
    plt.show()


def main():
    structures, times, memories = load_results(RESULTS_FILE)

    plot_time(structures, times)
    plot_memory(structures, memories)


if __name__ == "__main__":
    main()
