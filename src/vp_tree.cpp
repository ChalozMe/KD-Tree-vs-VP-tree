#include "vp_tree.hpp"

#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <limits>
#include <functional>


// Helper: compute distance squared (avoid sqrt frequently)
static double dist_sq(const Point& a, const Point& b) {
    if (a.dimension() != b.dimension()) return std::numeric_limits<double>::infinity();
    double s = 0.0;
    for (size_t i = 0; i < a.dimension(); ++i) {
        double d = a[i] - b[i];
        s += d * d;
    }
    return s;
}

void VPTree::build(const std::vector<Point>& points) {
    points_storage = points; // guardar copia
    std::vector<Point> pts = points_storage; // trabajar sobre copia
    root = buildRecursive(pts);
    // contar nodos (simple recorrido)
    std::function<size_t(const Node*)> count = [&](const Node* n)->size_t {
        if (!n) return 0;
        return 1 + count(n->left.get()) + count(n->right.get());
    };
    node_count = count(root.get());
}

std::unique_ptr<VPTree::Node> VPTree::buildRecursive(std::vector<Point>& pts) {
    if (pts.empty()) return nullptr;
    // elegir vantage point: último elemento (simple)
    Point vp = pts.back();
    pts.pop_back();

    auto node = std::make_unique<Node>(vp);
    if (pts.empty()) return node;

    // calcular distancias al vantage point
    std::vector<double> dists;
    dists.reserve(pts.size());
    for (const auto& p : pts) dists.push_back(std::sqrt(dist_sq(vp, p)));

    // mediana de distancias
    size_t mid = dists.size() / 2;
    std::nth_element(dists.begin(), dists.begin() + mid, dists.end());
    node->radius = dists[mid];

    // particionar puntos en left/right según radius
    std::vector<Point> leftPts, rightPts;
    leftPts.reserve(pts.size());
    rightPts.reserve(pts.size());
    for (const auto& p : pts) {
        double d = std::sqrt(dist_sq(vp, p));
        if (d <= node->radius) leftPts.push_back(p);
        else rightPts.push_back(p);
    }

    node->left = buildRecursive(leftPts);
    node->right = buildRecursive(rightPts);
    return node;
}

void VPTree::insert(const Point& p) {
    points_storage.push_back(p);
    // rebuild completo (ineficiente pero correcto para pruebas)
    std::vector<Point> copy = points_storage;
    root = buildRecursive(copy);
    // actualizar contador
    std::function<size_t(const Node*)> count = [&](const Node* n)->size_t {
        if (!n) return 0;
        return 1 + count(n->left.get()) + count(n->right.get());
    };
    node_count = count(root.get());
}

Point VPTree::nearest_neighbor(const Point& q) const {
    if (!root) return q;
    Point best = root->vp;
    double best_d = dist_sq(best, q);
    search_nn(root.get(), q, best, best_d);
    return best;
}

void VPTree::search_nn(const Node* node, const Point& q, Point& best, double& best_d) const {
    if (!node) return;
    double d = dist_sq(q, node->vp);
    if (d < best_d && d > 1e-12) { // evitar mismo punto exacto
        best = node->vp;
        best_d = d;
    }

    double dist_to_vp = std::sqrt(d);
    // escoger rama primaria
    const Node* first = nullptr;
    const Node* second = nullptr;
    if (dist_to_vp <= node->radius) {
        first = node->left.get();
        second = node->right.get();
    } else {
        first = node->right.get();
        second = node->left.get();
    }

    if (first) search_nn(first, q, best, best_d);

    double r = node->radius;
    double bound = std::sqrt(best_d);
    // condiciones de poda usando desigualdad triangular
    if (second) {
        if (std::abs(dist_to_vp - r) <= bound) {
            search_nn(second, q, best, best_d);
        }
    }
}

std::vector<std::pair<Point,double>> VPTree::knn(const Point& q, int k) const {
    std::vector<std::pair<Point,double>> empty;
    if (!root || k <= 0) return empty;

    std::priority_queue<std::pair<double,Point>,
                        std::vector<std::pair<double,Point>>,
                        VPKNNComparator> heap;

    knn_search(root.get(), q, k, heap);

    std::vector<std::pair<Point,double>> result;
    while (!heap.empty()) {
        auto pr = heap.top(); heap.pop();
        result.push_back({pr.second, std::sqrt(pr.first)}); // pr.first es dist^2
    }
    std::reverse(result.begin(), result.end());
    return result;
}

void VPTree::knn_search(const Node* node, const Point& q, int k,
                        std::priority_queue<std::pair<double,Point>,
                                            std::vector<std::pair<double,Point>>,
                                            VPKNNComparator>& heap) const {
    if (!node) return;

    double d2 = dist_sq(q, node->vp);

    if (d2 > 1e-12) {
        if ((int)heap.size() < k) {
            heap.push({d2, node->vp});
        } else if (d2 < heap.top().first) {
            heap.pop();
            heap.push({d2, node->vp});
        }
    }

    double dist_to_vp = std::sqrt(d2);
    const Node* first = nullptr;
    const Node* second = nullptr;
    if (dist_to_vp <= node->radius) {
        first = node->left.get();
        second = node->right.get();
    } else {
        first = node->right.get();
        second = node->left.get();
    }

    if (first) knn_search(first, q, k, heap);

    double bound = heap.empty() ? std::numeric_limits<double>::infinity() : std::sqrt(heap.top().first);
    // poda: solo explorar second si |dist_to_vp - radius| <= bound
    if (second) {
        if (std::abs(dist_to_vp - node->radius) <= bound) {
            knn_search(second, q, k, heap);
        }
    }
}

size_t VPTree::memoryUsage() const {
    return node_count * sizeof(Node);
}
