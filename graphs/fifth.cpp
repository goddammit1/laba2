#include <iostream>
#include <vector>
#include <queue>
#include "Graph.h"

class BipartiteChecker {
private:
    const Graph& graph_;
    std::vector<int> colors_; // -1: не посещена, 0/1: цвет
    std::vector<int> partA_, partB_;
    bool is_bipartite_;

    bool bfs_check(int start) {
        std::queue<int> q;
        q.push(start);
        colors_[start] = 0;
        partA_.push_back(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : graph_.adjacency_list(u)) {
                if (colors_[v] == colors_[u]) {
                    return false;
                }
                if (colors_[v] == -1) {
                    colors_[v] = 1 - colors_[u];
                    (colors_[v] == 0 ? partA_ : partB_).push_back(v);
                    q.push(v);
                }
            }
        }
        return true;
    }

public:
    BipartiteChecker(const Graph& graph) : graph_(graph), is_bipartite_(true) {
        if (graph.is_directed()) {
            throw std::invalid_argument("Graph must be undirected");
        }
        colors_.resize(graph.size() + 1, -1);

        for (int u = 1; u <= graph.size() && is_bipartite_; ++u) {
            if (colors_[u] == -1) {
                if (!bfs_check(u)) {
                    is_bipartite_ = false;
                    partA_.clear();
                    partB_.clear();
                }
            }
        }
    }

    bool is_bipartite() const { return is_bipartite_; }
    const std::vector<int>& partA() const { return partA_; }
    const std::vector<int>& partB() const { return partB_; }
};

int main() {
    try {
        Graph graph("list_of_edges_t1_023.txt", Graph::EDGES_LIST);
        BipartiteChecker checker(graph);

        if (checker.is_bipartite()) {
            std::cout << "Graph is bipartite\nPart A: ";
            for (int v : checker.partA()) std::cout << v << " ";
            std::cout << "\nPart B: ";
            for (int v : checker.partB()) std::cout << v << " ";
        } else {
            std::cout << "Graph is NOT bipartite";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
    }
    return 0;
}
