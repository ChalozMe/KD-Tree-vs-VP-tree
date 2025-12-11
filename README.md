# KNN Comparison using KD-Tree and VP-Tree

Comando Magico para provar:
```bash
g++ -std=c++17 -O2 -Wall -Isrc src/*.cpp -o test
```


## Descripción del Trabajo
Este proyecto implementa y compara dos estructuras de datos multidimensionales para realizar búsquedas **K-Nearest Neighbors (KNN)** sobre vectores de características.  
Se evalúan métricas de **tiempo de inserción**, **tiempo de búsqueda** y **uso de memoria**, utilizando datasets generados sintéticamente.

## Estructuras Implementadas

### KD-Tree
Árbol de partición espacial que divide recursivamente el espacio mediante hiperplanos perpendiculares a los ejes.  
Es adecuado para datos con baja dimensionalidad, pero su rendimiento se degrada significativamente cuando la dimensionalidad crece.

### VP-Tree (Vantage-Point Tree)
Árbol métrico que utiliza puntos de referencia (vantage points) y particiones radiales basadas en distancias.  
Mantiene un mejor rendimiento en espacios de mayor dimensionalidad mientras la distancia utilizada cumpla la desigualdad triangular.

---

## Estructura del Proyecto

```bash
proyecto-knn/
├── README.md
├── .gitignore
│
├── data/
│ └── <datasets_generados.csv>
│
├── src/
│ ├── point.hpp
│ ├── kd_tree.hpp
│ ├── kd_tree.cpp
│ ├── vp_tree.hpp
│ ├── vp_tree.cpp
│ ├── utils.hpp
│ └── main.cpp
│
├── scripts/
│ └── generator.py
│
└── CMakeLists.txt
```