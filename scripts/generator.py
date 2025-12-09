import csv
import random
import numpy as np

# CONFIGURACIÓN -------------------------
N = 20                                          # cantidad de datos
DIM_GENRES = 2                                  # número de géneros (inflar dimensiones)
SEED = 23
TOTAL_DIM = DIM_GENRES + 3                      # anho rating y duracion son fijos
OUTPUT = f"data/{TOTAL_DIM}DIm_{N}len.csv"

# END CONFIG----------------------------

random.seed(SEED)
np.random.seed(SEED)

# Funciones

def generate_genre_vector(dim):
    return np.random.dirichlet([1]*dim)         #Vector de géneros que suma 1.

def normalize(x, minv, maxv):
    return (x - minv) / (maxv - minv)

# Generacion de dataset, todo random y normalizado

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

    # reducir a 2 decimales
    for g in range(DIM_GENRES):
        row[f"genre_{g}"] = round(float(genres[g]), 2)

    row["year_norm"] = round(year_n, 2)
    row["rating_norm"] = round(rating_n, 2)
    row["duration_norm"] = round(duration_n, 2)

    rows.append(row)

# GUARDAR CSV ----------------------------

fieldnames = ["id", "title"] \
             + [f"genre_{g}" for g in range(DIM_GENRES)] \
             + ["year_norm", "rating_norm", "duration_norm"]

with open(OUTPUT, "w", newline="") as f:
    writer = csv.DictWriter(f, fieldnames)
    writer.writeheader()
    writer.writerows(rows)

print(f"Dataset generado: {OUTPUT} ({N} filas, {DIM_GENRES+3} dimensiones)")
