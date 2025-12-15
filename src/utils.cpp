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

        while (std::getline(ss, cell, ',')) {
            try {
                values.push_back(std::stod(cell));
            } catch (...) {
                // ignorar no numerico
            }
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
