#include <iostream>
#include <fstream>
#include <vector>

#include "point.hpp"
#include "kd_tree.hpp"
#include "vp_tree.hpp"
#include "utils.hpp"

void printMenu() {
    std::cout << "\n===== MENU =====\n";
    std::cout << "1. Construir KD-Tree\n";
    std::cout << "2. Construir VP-Tree\n";
    std::cout << "3. Buscar NN (KD-Tree)\n";
    std::cout << "4. Buscar KNN (KD-Tree)\n";
    std::cout << "5. Buscar KNN (VP-Tree)\n";
    std::cout << "6. Mostrar memoria usada\n";
    std::cout << "0. Salir\n";
    std::cout << "Opcion: ";
}

int main() {
    try {
        const std::string file = "data/2000Dim_1000len.csv";

        std::cout << "Cargando dataset...\n";
        std::vector<Point> pts = loadCSV(file);

        if (pts.empty()) {
            std::cerr << "Dataset vacío.\n";
            return 1;
        }

        size_t dims = pts[0].dimension();
        std::cout << "Se cargaron " << pts.size()
                  << " puntos con " << dims << " dimensiones.\n";

        Point query = pts[5];

        KDTree kdTree(dims);
        VPTree vpTree;

        bool kd_built = false;
        bool vp_built = false;

        int option;
        do {
            printMenu();
            std::cin >> option;

            switch (option) {

            case 1: {
                std::cout << "Construyendo KD-Tree...\n";
                uint64_t t0 = now_ms();
                for (const auto& p : pts)
                    kdTree.insert(p);
                uint64_t t1 = now_ms();

                kd_built = true;
                std::cout << "KD-Tree construido en "
                          << (t1 - t0) << " us\n";
                break;
            }

            case 2: {
                std::cout << "Construyendo VP-Tree...\n";
                uint64_t t0 = now_ms();
                vpTree.build(pts);
                uint64_t t1 = now_ms();

                vp_built = true;
                std::cout << "VP-Tree construido en "
                          << (t1 - t0) << " us\n";
                break;
            }

            case 3: {
                if (!kd_built) {
                    std::cout << "Primero construye el KD-Tree.\n";
                    break;
                }
                uint64_t t0 = now_ms();
                Point best = kdTree.nearest_neighbor(query);
                uint64_t t1 = now_ms();

                std::cout << "NN encontrado\n";
                std::cout << "Distancia: "
                          << Point::distance(query, best) << "\n";
                std::cout << "Tiempo: " << (t1 - t0) << " us\n";
                break;
            }

            case 4: {
                if (!kd_built) {
                    std::cout << "Primero construye el KD-Tree.\n";
                    break;
                }
                int K;
                std::cout << "Ingrese K: ";
                std::cin >> K;

                uint64_t t0 = now_ms();
                auto neighbors = kdTree.knn(query, K);
                uint64_t t1 = now_ms();

                for (size_t i = 0; i < neighbors.size(); ++i)
                    std::cout << "NN " << i+1
                              << " dist = " << neighbors[i].second << "\n";

                std::cout << "Tiempo: " << (t1 - t0) << " us\n";
                break;
            }

            case 5: {
                if (!vp_built) {
                    std::cout << "Primero construye el VP-Tree.\n";
                    break;
                }
                int K;
                std::cout << "Ingrese K: ";
                std::cin >> K;

                uint64_t t0 = now_ms();
                auto neighbors = vpTree.knn(query, K);
                uint64_t t1 = now_ms();

                for (size_t i = 0; i < neighbors.size(); ++i)
                    std::cout << "NN " << i+1
                              << " dist = " << neighbors[i].second << "\n";

                std::cout << "Tiempo: " << (t1 - t0) << " us\n";
                break;
            }

            case 6: {
                if (kd_built)
                    std::cout << "KD-Tree memoria: "
                              << kdTree.memoryUsage() << " bytes\n";
                if (vp_built)
                    std::cout << "VP-Tree memoria: "
                              << vpTree.memoryUsage() << " bytes\n";
                break;
            }

            case 0:
                std::cout << "Saliendo...\n";
                break;

            default:
                std::cout << "Opcion invalida.\n";
            }

        } while (option != 0);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
