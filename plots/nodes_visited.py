import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results_nodes_visited.csv")

plt.figure(figsize=(8,5))
plt.plot(df["Size"], df["Visited_KD"], marker="o", label="KD-Tree")
plt.plot(df["Size"], df["Visited_VP"], marker="s", label="VP-Tree")

plt.xlabel("Tamaño del Dataset")
plt.ylabel("Número de nodos visitados")
plt.title("Nodos visitados en KNN (K=5)")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("img/nodes_visited.png")
plt.show()
