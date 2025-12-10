#include "kd_tree.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>


KDTree::KDTree(size_t dimensions) : root(nullptr), dims(dimensions), node_count(0) {}

void KDTree::insert(const Point &p) {
    insert_recursive(root, p, 0);
}

void KDTree::insert_recursive(std::unique_ptr<Node> &node, const Point &p, size_t depth) {
    if (!node) {
        node = std::make_unique<Node>(p, depth % dims);
        ++node_count;
        return;
    }

    size_t axis = node->axis;

    // usar operator[] de Point
    if (p[axis] < node->point[axis]) {
        insert_recursive(node->left, p, depth + 1);
    } else {
        insert_recursive(node->right, p, depth + 1);
    }
}

Point KDTree::nearest_neighbor(const Point &target) const {
    // inicializar best con un punto random; if vacío, devolver target
    if (!root) return target;
    Point best = root->point;
    double best_dist = distance_sq(best, target);
    nn_recursive(root.get(), target, best, best_dist);
    return best;
}

void KDTree::nn_recursive(const Node *node, const Point &target,
                          Point &best, double &best_dist) const {
    if (!node) return;

    double d = distance_sq(node->point, target);
    if (d < best_dist) {
        best = node->point;
        best_dist = d;
    }

    size_t axis = node->axis;
    bool go_left = target[axis] < node->point[axis];

    const Node *first = go_left ? node->left.get() : node->right.get();
    const Node *second = go_left ? node->right.get() : node->left.get();

    nn_recursive(first, target, best, best_dist);

    double diff = target[axis] - node->point[axis];
    if (diff * diff < best_dist) {
        nn_recursive(second, target, best, best_dist);
    }
}

double KDTree::distance_sq(const Point &a, const Point &b) const {
    if (a.dimension() != b.dimension()) {
        // lanzar excepción
        return std::numeric_limits<double>::infinity();
    }
    double sum = 0.0;
    for (size_t i = 0; i < dims; ++i) {
        double d = a[i] - b[i];
        sum += d * d;
    }
    return sum;
}

size_t KDTree::memoryUsage() const {
    // estimación simple: tamaño de Node * cantidad de nodos (segun GPT es asi, revisar correctitud)
    return node_count * sizeof(Node);
}




//KNN TRY
std::vector<std::pair<Point,double>> KDTree::knn(const Point& target, int k) const {
    if (!root || k <= 0) return {};

    std::priority_queue<
        std::pair<double,Point>,
        std::vector<std::pair<double,Point>>,
        KNNComparator
    > heap;

    knn_recursive(root.get(), target, k, heap);

    std::vector<std::pair<Point,double>> result;
    while (!heap.empty()) {
        auto [d, p] = heap.top();
        heap.pop();
        result.push_back({p, std::sqrt(d)});
    }

    std::reverse(result.begin(), result.end());
    return result;
}

void KDTree::knn_recursive(
    const Node* node,
    const Point& target,
    int k,
    std::priority_queue<
        std::pair<double,Point>,
        std::vector<std::pair<double,Point>>,
        KNNComparator
    >& heap
) const {
    if (!node) return;

    double d = distance_sq(node->point, target);

    if (d > 1e-12) {
        if ((int)heap.size() < k)
            heap.push({d, node->point});
        else if (d < heap.top().first) {
            heap.pop();
            heap.push({d, node->point});
        }
    }

    size_t axis = node->axis;
    bool go_left = target[axis] < node->point[axis];

    const Node* first  = go_left ? node->left.get()  : node->right.get();
    const Node* second = go_left ? node->right.get() : node->left.get();

    knn_recursive(first, target, k, heap);

    double diff = target[axis] - node->point[axis];
    double diff_sq = diff * diff;

    if ((int)heap.size() < k || diff_sq < heap.top().first)
        knn_recursive(second, target, k, heap);
}
