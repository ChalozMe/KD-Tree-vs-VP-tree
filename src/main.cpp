#include <iostream>
#include "point.hpp"
#include "kd_tree.hpp"
#include "vp_tree.hpp"
#include "utils.hpp"

// ------------------------------------------------------------
// MAIN DE PRUEBA
// ------------------------------------------------------------
int main() {
    try {
        std::cout << "Cargando dataset...\n";

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
        Point query = pts[5];
        std::cout << "Consultando NN de un punto del dataset...\n";

        Point best = tree.nearest_neighbor(query);

        std::cout << "Query: ";
        printPoint(query);
        std::cout << "\n";

        std::cout << "NN:    ";
        printPoint(best);
        std::cout << "\n";

        double dist = Point::distance(query, best);
        std::cout << "Distancia = " << dist << "\n";

        std::cout << "Try KNN:\n";
        int K = 3;

        std::cout << "Buscando " << K << " vecinos más cercanos del punto 5...\n";
        auto neighbors = tree.knn(query, K);

        for (size_t i = 0; i < neighbors.size(); i++) {
            const auto& [p, dist] = neighbors[i];

            std::cout << "NN " << i+1 << ": ";
            printPoint(p);
            std::cout << "   Dist = " << dist << "\n";
        }

        std::cout << "\nTry KNN (VP-Tree):\n";

        VPTree vptree;

        for (const auto& p : pts)
            vptree.insert(p);

        auto neighborsVP = vptree.knn(query, K);

        for (size_t i = 0; i < neighborsVP.size(); ++i) {
            const auto& [p, dist] = neighborsVP[i];

            std::cout << "NN " << i+1 << ": ";
            printPoint(p);
            std::cout << "   Dist = " << dist << "\n";
        }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
