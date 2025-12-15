import csv
import random
import numpy as np

# CONFIGURACIÓN -------------------------
N = 4000                          # cantidad de datos
DIM_GENRES = 37                    # número de géneros
SEED = 23
TOTAL_DIM = DIM_GENRES + 3         # año, rating y duración
OUTPUT = f"data/{TOTAL_DIM}Dim_{N}len.csv"
# --------------------------------------

random.seed(SEED)
np.random.seed(SEED)

# FUNCIONES -----------------------------

def generate_genre_vector(dim):
    # Vector de géneros que suma 1
    return np.random.dirichlet([1] * dim)

def normalize(x, minv, maxv):
    return (x - minv) / (maxv - minv)

# GENERACIÓN ----------------------------

rows = []

for i in range(N):
    genres = generate_genre_vector(DIM_GENRES)

    # características originales
    year = random.randint(1950, 2025)
    rating = random.uniform(1, 10)
    duration = random.randint(60, 240)

    # normalización
    year_n = normalize(year, 1950, 2025)
    rating_n = normalize(rating, 1, 10)
    duration_n = normalize(duration, 60, 240)

    row = {
        "id": i,
        "title": f"Movie_{i}",
    }

    # SIN redondeo
    for g in range(DIM_GENRES):
        row[f"genre_{g}"] = float(genres[g])

    row["year_norm"] = year_n
    row["rating_norm"] = rating_n
    row["duration_norm"] = duration_n

    rows.append(row)

# GUARDAR CSV ----------------------------

fieldnames = (
    ["id", "title"]
    + [f"genre_{g}" for g in range(DIM_GENRES)]
    + ["year_norm", "rating_norm", "duration_norm"]
)

with open(OUTPUT, "w", newline="") as f:
    writer = csv.DictWriter(f, fieldnames)
    writer.writeheader()
    writer.writerows(rows)

print(f"Dataset generado: {OUTPUT} ({N} filas, {TOTAL_DIM} dimensiones)")
