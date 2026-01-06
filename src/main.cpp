#include <iostream>
#include "point.hpp"
#include "kd_tree.hpp"
#include "vp_tree.hpp"
#include "utils.hpp"
#include <fstream>

// ------------------------------------------------------------
// MAIN DE PRUEBA
// ------------------------------------------------------------
int main() {
    try {
        std::cout << "Cargando dataset...\n";

        const std::string file = "data/2000Dim_1000len.csv";

        std::vector<Point> pts = loadCSV(file);
        if (pts.empty()) {
            std::cout << "Dataset vacío.\n";
            return 0;
        }

        size_t dims = pts[0].dimension();
        std::cout << "Se cargaron " << pts.size()
                  << " puntos con " << dims << " dimensiones.\n";

        std::cout << "\nConstruyendo KD-Tree...\n";
        KDTree tree(dims);
        // Insertar todos los puntos
        for (auto &p : pts) {
            if (p.dimension() != dims) {
                std::cerr << "Error: punto con dimensión distinta.\n";
                return 1;
            }
            tree.insert(p);
        }

        std::cout << "Insercion completada.\n";
        std::cout << "Memoria aprox: " << tree.memoryUsage() << " bytes\n";

        // Probar KNN (1-NN)
        Point query = pts[5];
        std::cout << "Consultando NN de un punto del dataset...\n";
        uint64_t t0 = now_ms();

        Point best = tree.nearest_neighbor(query);

        uint64_t t1 = now_ms();
        std::cout << "Tiempo utilizado NN " << (t1 - t0) << " us\n";
        
        //std::cout << "Query: ";
        //printPoint(query,2);
        //std::cout << "\n";

        //std::cout << "NN:    ";
        //printPoint(best,2);
        //std::cout << "\n";

        double dist = Point::distance(query, best);
        std::cout << "Distancia = " << dist << "\n";

        std::cout << "Try KNN:\n";
        int K = 3;

        std::cout << "Buscando " << K << " vecinos más cercanos del punto 5...\n";
        
        uint64_t t2 = now_ms();
        auto neighbors = tree.knn(query, K);
        uint64_t t3 = now_ms();
        std::cout << "Tiempo utilizado KNN " << (t3 - t2) << " us\n";

        for (size_t i = 0; i < neighbors.size(); i++) {
            const auto& [p, dist] = neighbors[i];

            std::cout << "NN " << i+1 << ": ";
            //printPoint(p,2);
            std::cout << "   Dist = " << dist << "\n";
        }
      

        std::cout << "\nTry KNN (VP-Tree):\n";

        VPTree vptree;

        

        for (const auto& p : pts)
            vptree.insert(p);

        
        uint64_t t4 = now_ms();
        auto neighborsVP = vptree.knn(query, K);
        uint64_t t5 = now_ms();

        for (size_t i = 0; i < neighborsVP.size(); ++i) {
            const auto& [p, dist] = neighborsVP[i];

            std::cout << "NN " << i+1 << ": ";
            //printPoint(p,2);
            std::cout << "   Dist = " << dist << "\n";
        }

        std::cout << "Memoria aprox: " << vptree.memoryUsage() << " bytes\n";
        std::cout << "Tiempo utilizado KNN " << (t5 - t4) << " us\n";

        std::ofstream out("results.csv");
        out << "structure,time_us,memory_bytes\n";
        out << "KDTree," << (t1 - t0) << "," << tree.memoryUsage() << "\n";
        out << "VPTree," << (t4 - t3) << "," << vptree.memoryUsage() << "\n";
        out.close();

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
