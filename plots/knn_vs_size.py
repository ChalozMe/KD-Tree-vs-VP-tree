import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results_knn_vs_size.csv")

plt.figure(figsize=(8,5))
plt.plot(df["Size"], df["Time_KD"], marker="o", label="KD-Tree")
plt.plot(df["Size"], df["Time_VP"], marker="s", label="VP-Tree")

plt.xlabel("Tamaño del Dataset")
plt.ylabel("Tiempo de búsqueda KNN (us)")
plt.title("Tiempo KNN vs Tamaño del Dataset")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("img/knn_vs_size.png")
plt.show()
