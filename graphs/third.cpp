#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include "Graph.h"

class SpanningTree {
private:
    const Graph& graph_;
    std::vector<bool> visited_;
    std::vector<std::pair<int, int>> tree_edges_;
    int start_vertex_;

    void bfs(int start) {
        std::queue<int> q;
        q.push(start);
        visited_[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : graph_.adjacency_list(u)) {
                if (!visited_[v]) {
                    visited_[v] = true;
                    tree_edges_.emplace_back(u, v);
                    q.push(v);
                }
            }
        }
    }

    void dfs(int u) {
        visited_[u] = true;

        for (int v : graph_.adjacency_list(u)) {
            if (!visited_[v]) {
                tree_edges_.emplace_back(u, v);
                dfs(v);
            }
        }
    }

public:
    enum Algorithm { BFS, DFS };

    SpanningTree(const Graph& graph, int start_vertex = 1, Algorithm algo = BFS)
            : graph_(graph), start_vertex_(start_vertex) {
        if (graph.is_directed()) {
            throw std::invalid_argument("Graph must be undirected for spanning tree");
        }
        visited_.resize(graph.size() + 1, false);

        switch (algo) {
            case BFS: bfs(start_vertex); break;
            case DFS: dfs(start_vertex); break;
        }

        // Проверка связности
        for (int u = 1; u <= graph.size(); ++u) {
            if (!visited_[u]) {
                throw std::runtime_error("Graph is disconnected");
            }
        }
    }

    const std::vector<std::pair<int, int>>& edges() const {
        return tree_edges_;
    }

    void print_tree() const {
        std::cout << "Spanning tree edges (" << tree_edges_.size() << "):\n";
        for (const auto& edge : tree_edges_) {
            std::cout << edge.first << " - " << edge.second << "\n";
        }
    }
};

int main() {
    try {
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_edges_t3_001.txt", Graph::EDGES_LIST);

        // Построение BFS-остова
        std::cout << "=== BFS Spanning Tree ===\n";
        SpanningTree bfs_tree(graph, 1, SpanningTree::BFS);
        bfs_tree.print_tree();

        // Построение DFS-остова
        std::cout << "\n=== DFS Spanning Tree ===\n";
        SpanningTree dfs_tree(graph, 1, SpanningTree::DFS);
        dfs_tree.print_tree();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
