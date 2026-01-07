#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>   // system()
#include <sstream>
#include <iomanip>

#include "point.hpp"
#include "kd_tree.hpp"
#include "vp_tree.hpp"
#include "utils.hpp"

int main() {
    try {
        const std::string dir = "data/";
        std::string file;
        std::cout << "Ingrese el nombre del archivo: ";
        std::cin >> file;
        file = dir + file;

        std::cout << "Cargando dataset...\n";
        std::vector<Point> pts = loadCSV(file);

        if (pts.empty()) {
            std::cerr << "Dataset vacío.\n";
            return 1;
        }

        size_t dims = pts[0].dimension();
        std::cout << "Se cargaron " << pts.size()
                  << " puntos con " << dims << " dimensiones.\n";

        //PARA QUE SEA DIMANICO ESTO PONER DESDE INPUT, POR AHORA SOLO PRUEBA
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
                kdTree.build(pts);
                /*
                for (const auto& p : pts)
                    kdTree.insert(p);
                */
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

                size_t visiteDKD = 0;

                uint64_t t0 = now_ms();
                auto neighbors = kdTree.knn(query, K,visiteDKD);
                uint64_t t1 = now_ms();
                std::cout << "Nodos visitados (KD-Tree): " << visiteDKD << "\n";
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
                size_t visiteDVP = 0;
                uint64_t t0 = now_ms();
                auto neighbors = vpTree.knn(query, K,visiteDVP);
                uint64_t t1 = now_ms();
                std::cout << "Nodos visitados (VP-Tree): " << visiteDVP << "\n";
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

            
            case 7: {
                CompMenu();
                int comp_option;
                std::cin >> comp_option;
                switch(comp_option) {
                    case 1: {
                        std::vector<int> dims = {5,10,20,50,100};
                        std::ofstream out("results/results_knn_vs_dim.csv");
                        out << "Dim,Time_KD,Time_VP\n";

                        for (int d : dims) {
                            std::string fname = "data/set_KNNvsD/" + std::to_string(d) + "Dim_"+std::to_string(50000)+"len.csv";
                            auto data = loadCSV(fname);
                            Point q = data[0];

                            KDTree kd(d);
                            VPTree vp;

                            kd.build(data);
                            vp.build(data);

                            size_t vkd = 0, vvp = 0;

                            uint64_t t0 = now_ms();
                            kd.knn(q, 5, vkd);
                            uint64_t t1 = now_ms();

                            uint64_t t2 = now_ms();
                            vp.knn(q, 5, vvp);
                            uint64_t t3 = now_ms();

                            writeCSVRow(out, {
                                (double)d,
                                (double)(t1 - t0),
                                (double)(t3 - t2)
                            });
                        }

                        out.close();
                        runPython("plots/knn_vs_dim.py");
                        break;
                    }

                    case 2: {
                        std::vector<int> sizes = {1000,10000,50000,100000};
                        std::ofstream out("results/results_knn_vs_size.csv");
                        out << "Size,Time_KD,Time_VP\n";

                        for (int s : sizes) {
                            std::string fname = "data/set_KNNvsTAM/" + std::to_string(20) + "Dim_"+std::to_string(s)+"len.csv";
                            auto data = loadCSV(fname);
                            size_t d = data[0].dimension();
                            Point q = data[0];

                            KDTree kd(d);
                            VPTree vp;

                            kd.build(data);
                            vp.build(data);

                            size_t vkd = 0, vvp = 0;

                            uint64_t t0 = now_ms();
                            kd.knn(q, 5, vkd);
                            uint64_t t1 = now_ms();

                            uint64_t t2 = now_ms();
                            vp.knn(q, 5, vvp);
                            uint64_t t3 = now_ms();

                            writeCSVRow(out, {
                                (double)s,
                                (double)(t1 - t0),
                                (double)(t3 - t2)
                            });
                        }

                        out.close();
                        runPython("plots/knn_vs_size.py");
                        break;
                    }

                    case 3: {
                        std::vector<int> sizes = {1000,5000,10000};
                        std::ofstream out("results/results_nodes_visited.csv");
                        out << "Size,Visited_KD,Visited_VP\n";

                        for (int s : sizes) {
                            std::string fname = "data/set_N_Visited/" + std::to_string(20) + "Dim_"+std::to_string(s)+"len.csv";
                            auto data = loadCSV(fname);
                            size_t d = data[0].dimension();
                            Point q = data[0];

                            KDTree kd(d);
                            VPTree vp;
                            kd.build(data);
                            vp.build(data);

                            size_t vkd = 0, vvp = 0;
                            kd.knn(q, 5, vkd);
                            vp.knn(q, 5, vvp);

                            writeCSVRow(out, {
                                (double)s,
                                (double)vkd,
                                (double)vvp
                            });
                        }

                        out.close();
                        runPython("plots/nodes_visited.py");
                        break;
                    }

                    case 4: {
                        std::vector<int> sizes = {1000,10000,50000,100000};
                        std::ofstream out("results/results_build_time.csv");
                        out << "Size,Build_KD,Build_VP\n";

                        for (int s : sizes) {
                            std::string fname = "data/set_BuildTime/" + std::to_string(20) + "Dim_"+std::to_string(s)+"len.csv";
                            auto data = loadCSV(fname);
                            size_t d = data[0].dimension();

                            uint64_t t0 = now_ms();
                            KDTree kd(d);
                            kd.build(data);
                            uint64_t t1 = now_ms();

                            uint64_t t2 = now_ms();
                            VPTree vp;
                            vp.build(data);
                            uint64_t t3 = now_ms();

                            writeCSVRow(out, {
                                (double)s,
                                (double)(t1 - t0),
                                (double)(t3 - t2)
                            });
                        }

                        out.close();
                        runPython("plots/build_time.py");
                        break;
                    }

                    case 5: {
                        std::vector<int> sizes = {1000, 10000, 50000, 100000};
                        std::ofstream out("results/results_memory_usage.csv");
                        out << "Size,Memory_KD,Memory_VP\n"; // cambiar encabezado

                        for (int s : sizes) {
                            std::string fname =
                            "data/set_Memory/" + std::to_string(20) + "Dim_" +
                            std::to_string(s) + "len.csv";

                            auto data = loadCSV(fname);
                            size_t d = data[0].dimension();

                            KDTree kd(d);
                            kd.build(data);
                            size_t mem_kd = kd.memoryUsage() / 1024;

                            VPTree vp;
                            vp.build(data);
                            size_t mem_vp = vp.memoryUsage() / 1024;

                            writeCSVRow(out, {
                                (double)s,
                                        (double)mem_kd,
                                        (double)mem_vp
                            });
                        }

                        out.close();
                        runPython("plots/memory_usage.py");
                        break;
                    }

                    case 0:
                        //Volver al menu principal
                        break;
                    default:
                        std::cout << "Opcion invalida.\n";
                }
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

///anotar experimento, 
