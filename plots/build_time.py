import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results_build_time.csv")

plt.figure(figsize=(8,5))
plt.plot(df["Size"], df["Build_KD"], marker="o", label="KD-Tree")
plt.plot(df["Size"], df["Build_VP"], marker="s", label="VP-Tree")

plt.xlabel("Tamaño del Dataset")
plt.ylabel("Tiempo de construcción (us)")
plt.title("Tiempo de construcción del árbol")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("img/build_time.png")
plt.show()
