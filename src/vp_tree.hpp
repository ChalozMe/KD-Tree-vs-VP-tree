#ifndef VP_TREE_HPP
#define VP_TREE_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <cmath>
#include <limits>

#include "point.hpp"

struct VPKNNComparator {
    bool operator()(const std::pair<double, Point>& a,
                    const std::pair<double, Point>& b) const {
        return a.first < b.first; // max-heap por distancia
    }
};

class VPTree {
public:
    VPTree() : root(nullptr), node_count(0) {}

    // Construir a partir de un vector de puntos (bulk-build)
    void build(const std::vector<Point>& points);

    // Inserción simple: añade y reconstruye (suficiente para pruebas)
    void insert(const Point& p);

    // Nearest neighbor (1-NN)
    Point nearest_neighbor(const Point& q) const;

    // K nearest neighbors: devuelve pares (Point, dist)
    std::vector<std::pair<Point,double>> knn(const Point& q, int k) const;

    // Estimación simple de memoria
    size_t memoryUsage() const;

private:
    struct Node {
        Point vp;
        double radius;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(const Point& p) : vp(p), radius(0.0), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;
    size_t node_count;
    std::vector<Point> points_storage; // usado para rebuild en insert()

    std::unique_ptr<Node> buildRecursive(std::vector<Point>& pts);
    void search_nn(const Node* node, const Point& q, Point& best, double& best_d) const;

    void knn_search(const Node* node, const Point& q, int k,
                    std::priority_queue<std::pair<double,Point>,
                                        std::vector<std::pair<double,Point>>,
                                        VPKNNComparator>& heap) const;
};

#endif // VP_TREE_HPP
