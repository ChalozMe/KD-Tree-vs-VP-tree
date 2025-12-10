#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>

class Point
{
    private:
        std::vector<double> coords;
    public:
        Point()= default;
        Point(const std::vector<double>& v) : coords(v) {}

        inline int dimension() const 
        {
            return coords.size();
        }

        inline double operator[](int i) const
        {
            return coords[i];
        }

        inline std::vector<double> getCoords() const
        {
            return coords;
        }

        //Dist ecluidiana
        static double distance(const Point& a, const Point& b)
        {
            if (a.dimension() != b.dimension())
                throw std::runtime_error("Dimensiones diferentes entre ambos puntos");
            
            double sum = 0.0;
            for (int i = 0; i < a.dimension(); i++)
            {
                double diff = a[i] - b[i];
                sum += diff * diff;
            }
            return std::sqrt(sum);
        }
};