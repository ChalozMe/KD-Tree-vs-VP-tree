import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results_knn_vs_dim.csv")

plt.figure(figsize=(8,5))
plt.plot(df["Dim"], df["Time_KD"], marker="o", label="KD-Tree")
plt.plot(df["Dim"], df["Time_VP"], marker="s", label="VP-Tree")

plt.xlabel("Dimensionalidad")
plt.ylabel("Tiempo de búsqueda KNN (us)")
plt.title("Tiempo KNN vs Dimensionalidad")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("img/knn_vs_dim.png")
plt.show()
