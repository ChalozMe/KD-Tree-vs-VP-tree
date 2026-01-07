#include "utils.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>


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
        int col = 0;

        while (std::getline(ss, cell, ',')) {
            // ignorar id (columna 0)
            if (col == 0) {
                col++;
                continue;
            }

            try {
                values.push_back(std::stod(cell));
            } catch (...) {
                // ignora title u otros no numéricos
            }

            col++;
        }

        if (!values.empty()) {
            points.emplace_back(values);
        }
    }

    return points;
}


// ------------------------------------------------------------
// PPRINT (pretty-print c:)
// ------------------------------------------------------------
void printPoint(const Point& p, int precision) {
    std::cout << "[ ";
    std::cout << std::fixed << std::setprecision(precision);

    for (size_t i = 0; i < p.dimension(); ++i) {
        std::cout << p[i];
        if (i + 1 < p.dimension()) std::cout << ", ";
    }

    std::cout << " ]";
}
void printVectorPoint(const std::vector<Point>& pts) {
    for (size_t i = 0; i < pts.size(); ++i) {
        std::cout << i << ": ";
        printPoint(pts[i],2);
        std::cout << "\n";
    }
}

// ------------------------------------------------------------
// Tiempo (ms)
// ------------------------------------------------------------
uint64_t now_ms() {
    using namespace std::chrono;
    return duration_cast<microseconds>(
        steady_clock::now().time_since_epoch()
    ).count();
}

// ------------------------------------------------------------
// Ayudas
// ------------------------------------------------------------

void printMenu() {
    std::cout << "\n===== MENU =====\n";
    std::cout << "1. Construir KD-Tree\n";
    std::cout << "2. Construir VP-Tree\n";
    std::cout << "3. Buscar NN (KD-Tree)\n"; //ELIMINAR
    std::cout << "4. Buscar KNN (KD-Tree)\n";
    std::cout << "5. Buscar KNN (VP-Tree)\n";
    std::cout << "6. Mostrar memoria usada(Estimado)\n";
    std::cout << "7. Comparativas\n";
    std::cout << "0. Salir\n";
    std::cout << "Opcion: ";
}

void CompMenu() {
    std::cout << "\n===== COMPARATIVAS =====\n";
    std::cout << "1. Tiempo de busqueda KNN vs Dimensionalidad\n";
    std::cout << "2. Tiempo de busqueda KNN vs Tamano del dataset\n";
    std::cout << "3. Numero de Nodos Visitados\n";
    std::cout << "4. Tiempo de Construccion del arbol\n";
    std::cout << "5. Tiempo de Insercion en Diferentes Tamanos\n";
    std::cout << "0. Volver al menú principal\n";
    std::cout << "Opcion: ";
}

void runPython(const std::string& script) {
#ifdef _WIN32
    std::string cmd = "python " + script;
#else
    std::string cmd = "python3 " + script;
#endif
    system(cmd.c_str());
}


void writeCSVRow(std::ofstream& out,
                 const std::vector<double>& values) {
    for (size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i + 1 < values.size()) out << ",";
    }
    out << "\n";
}