#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Graph.h"

class ConnectivityFinder {
private:
    const Graph& graph_;
    std::vector<bool> visited_;
    std::vector<std::vector<int>> components_;

    void bfs(int start, std::vector<int>& component) {
        std::queue<int> q;
        q.push(start);
        visited_[start] = true;
        component.push_back(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : graph_.adjacency_list(u)) {
                if (!visited_[v]) {
                    visited_[v] = true;
                    component.push_back(v);
                    q.push(v);
                }
            }
        }
    }

    void dfs(int u, std::vector<int>& component) {
        visited_[u] = true;
        component.push_back(u);

        for (int v : graph_.adjacency_list(u)) {
            if (!visited_[v]) {
                dfs(v, component);
            }
        }
    }

    std::vector<std::vector<int>> get_undirected_adjacency() const {
        std::vector<std::vector<int>> undir_adj(graph_.size() + 1);
        auto matrix = graph_.adjacency_matrix();

        for (int u = 1; u <= graph_.size(); ++u) {
            for (int v = 1; v <= graph_.size(); ++v) {
                if (matrix[u][v] != 0) {
                    undir_adj[u].push_back(v);
                    undir_adj[v].push_back(u);
                }
            }
        }
        return undir_adj;
    }

public:
    ConnectivityFinder(const Graph& graph) : graph_(graph) {
        visited_.resize(graph_.size() + 1, false);
    }

    std::vector<std::vector<int>> find_components_bfs() {
        visited_.assign(visited_.size(), false);
        components_.clear();

        for (int u = 1; u <= graph_.size(); ++u) {
            if (!visited_[u]) {
                std::vector<int> component;
                bfs(u, component);
                components_.push_back(component);
            }
        }
        return components_;
    }

    std::vector<std::vector<int>> find_components_dfs() {
        visited_.assign(visited_.size(), false);
        components_.clear();

        for (int u = 1; u <= graph_.size(); ++u) {
            if (!visited_[u]) {
                std::vector<int> component;
                dfs(u, component);
                components_.push_back(component);
            }
        }
        return components_;
    }

    std::vector<std::vector<int>> find_weak_components() {
        if (!graph_.is_directed()) {
            return find_components_bfs();
        }

        auto undir_adj = get_undirected_adjacency();
        visited_.assign(visited_.size(), false);
        components_.clear();

        for (int u = 1; u <= graph_.size(); ++u) {
            if (!visited_[u]) {
                std::vector<int> component;
                std::queue<int> q;
                q.push(u);
                visited_[u] = true;
                component.push_back(u);

                while (!q.empty()) {
                    int curr = q.front();
                    q.pop();

                    for (int v : undir_adj[curr]) {
                        if (!visited_[v]) {
                            visited_[v] = true;
                            component.push_back(v);
                            q.push(v);
                        }
                    }
                }
                components_.push_back(component);
            }
        }
        return components_;
    }
};

int main() {
    try {
        Graph graph("list_of_edges_t1_023.txt", Graph::EDGES_LIST);
        ConnectivityFinder finder(graph);

        std::vector<std::vector<int>> components;
        bool is_directed = graph.is_directed();

        if (is_directed) {
            components = finder.find_weak_components();
            if (components.size() == 1) {
                std::cout << "Digraph is weakly connected\n";
            } else {
                std::cout << "Digraph is not weakly connected\n";
            }
        } else {
            components = finder.find_components_bfs();
            if (components.size() == 1) {
                std::cout << "Graph is connected\n";
            } else {
                std::cout << "Graph is not connected\n";
            }
        }

        // Sort each component and then sort components by their first element
        for (auto& comp : components) {
            std::sort(comp.begin(), comp.end());
        }
        std::sort(components.begin(), components.end(),
                  [](const std::vector<int>& a, const std::vector<int>& b) {
                      return a[0] < b[0];
                  });

        std::cout << "Connected components:\n";
        for (const auto& comp : components) {
            std::cout << "[";
            for (size_t i = 0; i < comp.size(); ++i) {
                std::cout << comp[i];
                if (i < comp.size() - 1)
                    std::cout << ", ";
            }
            std::cout << "]\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
















