#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "point.hpp"


std::vector<Point> loadCSV(const std::string& filename);


void printPoint(const Point& p, int precision);


void printVectorPoint(const std::vector<Point>& pts);

void printMenu();
void CompMenu();
void runPython(const std::string& script);
void writeCSVRow(std::ofstream& out,
                 const std::vector<double>& values);

// Tiempo en milisegundos
uint64_t now_ms();
