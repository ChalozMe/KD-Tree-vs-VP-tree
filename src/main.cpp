#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include "point.hpp"
#include "kd_tree.hpp"
#include "vp_tree.hpp"


// ------------------------------------------------------------
// Cargar CSV numérico a vectores<double>
// ------------------------------------------------------------
std::vector<Point> loadCSV(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    std::string line;
    bool header_skipped = false;

    while (std::getline(file, line)) {
        if (!header_skipped) { 
            header_skipped = true; 
            continue; 
        }

        std::stringstream ss(line);
        std::string cell;
        std::vector<double> values;

        while (std::getline(ss, cell, ',')) {
            try {
                values.push_back(std::stod(cell));
            } catch (...) {
                // si no es numérico lo skipea
            }
        }

        if (!values.empty())
            points.emplace_back(values);
    }

    return points;
}

// ------------------------------------------------------------
// MAIN DE PRUEBA
// ------------------------------------------------------------
int main() {
    try {
        std::cout << "Cargando dataset...\n";

        // set dataset
        const std::string file = "data/5Dim_20len.csv";

        std::vector<Point> pts = loadCSV(file);
        if (pts.empty()) {
            std::cout << "Dataset vacío.\n";
            return 0;
        }

        size_t dims = pts[0].dimension();
        std::cout << "Se cargaron " << pts.size()
                  << " puntos con " << dims << " dimensiones.\n";

        KDTree tree(dims);

        // Insertar todos los puntos
        for (auto &p : pts) {
            if (p.dimension() != dims) {
                std::cerr << "Error: punto con dimensión distinta.\n";
                return 1;
            }
            tree.insert(p);
        }

        std::cout << "Inserción completada.\n";
        std::cout << "Memoria aprox: " << tree.memoryUsage() << " bytes\n";

        // Probar KNN (1-NN)
        Point query = pts[5]; // punto arbitrario
        std::cout << "Consultando NN de un punto del dataset...\n";

        Point best = tree.nearest_neighbor(query);

        // Mostrar vector
        std::cout << "Query: [ ";
        for (size_t i = 0; i < query.dimension(); i++)
            std::cout << query[i] << " ";
        std::cout << "]\n";

        std::cout << "NN:    [ ";
        for (size_t i = 0; i < best.dimension(); i++)
            std::cout << best[i] << " ";
        std::cout << "]\n";

        double dist = Point::distance(query, best);
        std::cout << "Distancia = " << dist << "\n";



        std::cout << "Try KNN:" << "\n";
        int K = 3;
        //Point query = pts[5];  // punto arbitrario

        std::cout << "Buscando " << K << " vecinos más cercanos del punto 5...\n";

        auto neighbors = tree.knn(query, K);

        for (size_t i = 0; i < neighbors.size(); i++) {
            const auto& [p, dist] = neighbors[i];

            std::cout << "NN " << i+1 << ": [ ";
            for (size_t d = 0; d < p.dimension(); d++)
                std::cout << p[d] << " ";
            std::cout << "]  Dist = " << dist << "\n";
        }

        std::cout << "\nTry KNN (VP-Tree):\n";
        // Crear VP-Tree
        VPTree vptree;

        // Insertar puntos
        for (const auto& p : pts)
            vptree.insert(p);

        // Probar KNN
        auto neighborsVP = vptree.knn(query, K);

        // Mostrar resultados
        for (size_t i = 0; i < neighborsVP.size(); ++i) {
            const auto& pr = neighborsVP[i];
            const Point& p = pr.first;
            double dist = pr.second;

            std::cout << "NN " << i+1 << ": [ ";
            for (size_t d = 0; d < p.dimension(); ++d)
                std::cout << p[d] << " ";
            std::cout << "]  Dist = " << dist << "\n";
        }


    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    

    return 0;
}
