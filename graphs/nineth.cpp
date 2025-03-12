#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "Graph.h"

class MSTFinder {
private:
    const Graph& graph_;
    std::vector<bool> included_;
    std::vector<int> parent_;
    std::vector<int> min_weight_;
    std::vector<std::tuple<int, int, int>> mst_edges_;
    int total_weight_;

    int find_min_weight_vertex() {
        int min_weight = INT_MAX;
        int min_vertex = -1;

        for (int v = 1; v <= graph_.size(); ++v) {
            if (!included_[v] && min_weight_[v] < min_weight) {
                min_weight = min_weight_[v];
                min_vertex = v;
            }
        }
        return min_vertex;
    }

    void prim() {
        int n = graph_.size();
        min_weight_.assign(n + 1, INT_MAX);
        parent_.assign(n + 1, -1);
        included_.assign(n + 1, false);

        min_weight_[1] = 0; // Начинаем с вершины 1

        for (int i = 1; i <= n; ++i) {
            int u = find_min_weight_vertex();
            if (u == -1) {
                throw std::runtime_error("Graph is disconnected");
            }

            included_[u] = true;

            // Обновляем веса для смежных вершин
            for (int v = 1; v <= n; ++v) {
                if (graph_.is_edge(u, v) && !included_[v] && graph_.weight(u, v) < min_weight_[v]) {
                    parent_[v] = u;
                    min_weight_[v] = graph_.weight(u, v);
                }
            }
        }

        // Собираем рёбра MST
        total_weight_ = 0;
        for (int v = 2; v <= n; ++v) {
            if (parent_[v] != -1) {
                mst_edges_.emplace_back(parent_[v], v, min_weight_[v]);
                total_weight_ += min_weight_[v];
            }
        }
    }

public:
    MSTFinder(const Graph& graph) : graph_(graph), total_weight_(0) {
        if (graph.is_directed()) {
            throw std::invalid_argument("Graph must be undirected");
        }
        prim();
    }

    const std::vector<std::tuple<int, int, int>>& edges() const { return mst_edges_; }
    int total_weight() const { return total_weight_; }

    void print_mst() const {
        std::cout << "Minimum Spanning Tree Edges (Total Weight: " << total_weight_ << "):\n";
        for (const auto& edge : mst_edges_) {
            std::cout << std::get<0>(edge) << " - " << std::get<1>(edge)
                      << " (w=" << std::get<2>(edge) << ")\n";
        }
    }
};

int main() {
    try {
        Graph graph("matrix_t7_017.txt", Graph::MATRIX);
        MSTFinder mst(graph);
        mst.print_mst();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}