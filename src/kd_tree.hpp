#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include <vector>
#include <memory>
#include <queue>
#include <utility>
#include <cmath>
#include "point.hpp"

struct KNNComparator {
    bool operator()(const std::pair<double,Point>& a,
                    const std::pair<double,Point>& b) const {
        return a.first < b.first;   // max-heap
    }
};


class KDTree {
public:
    KDTree(size_t dimensions);
    void insert(const Point &p);
    Point nearest_neighbor(const Point &target) const;
    size_t memoryUsage() const; // estimación simple segun gpt,revisen
    std::vector<std::pair<Point,double>> knn(const Point& target, int k) const;


private:
    struct Node {
    Point point;
    size_t axis;                     // 1. axis primero
    std::unique_ptr<Node> left;      // 2. left después
    std::unique_ptr<Node> right;

    Node(const Point& p, size_t axis)
        : point(p), axis(axis), left(nullptr), right(nullptr)
    {}
};

    std::unique_ptr<Node> root;
    size_t dims;
    size_t node_count = 0;

    void insert_recursive(std::unique_ptr<Node> &node, const Point &p, size_t depth);
    void nn_recursive(const Node *node, const Point &target,
                      Point &best, double &best_dist) const;

    double distance_sq(const Point &a, const Point &b) const;
    void knn_recursive(
        const Node* node,
        const Point& target,
        int k,
        std::priority_queue<
            std::pair<double,Point>,
            std::vector<std::pair<double,Point>>,
            KNNComparator
        >& heap
    ) const;


};

#endif 
