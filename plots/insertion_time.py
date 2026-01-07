import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results_insertion_time.csv")

plt.figure(figsize=(8,5))
plt.plot(df["Size"], df["Insert_KD"], marker="o", label="KD-Tree")
plt.plot(df["Size"], df["Insert_VP"], marker="s", label="VP-Tree")
plt.xlabel("Dimensionalidad")
plt.ylabel("Tiempo de Insercion (us)")
plt.title("Tiempo de Insercion en Diferentes Tamanos de Dimensionalidad")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("img/insertion_time.png")
plt.show()
