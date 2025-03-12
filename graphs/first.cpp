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

    // Для слабой связности в орграфе: создаём неориентированное представление
    std::vector<std::vector<int>> get_undirected_adjacency() const {
        std::vector<std::vector<int>> undir_adj(graph_.size() + 1);
        auto matrix = graph_.adjacency_matrix();

        for (int u = 1; u <= graph_.size(); ++u) {
            for (int v = 1; v <= graph_.size(); ++v) {
                if (matrix[u][v] != 0) {
                    undir_adj[u].push_back(v);
                    undir_adj[v].push_back(u); // Добавляем обратное ребро
                }
            }
        }
        return undir_adj;
    }

public:
    ConnectivityFinder(const Graph& graph) : graph_(graph) {
        visited_.resize(graph_.size() + 1, false);
    }

    // Поиск компонент связности (BFS)
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

    // Поиск компонент связности (DFS)
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

    // Поиск слабых компонент связности для орграфа
    std::vector<std::vector<int>> find_weak_components() {
        if (!graph_.is_directed()) {
            return find_components_bfs(); // Если граф не ориентирован
        }

        // Создаём неориентированное представление
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

void print_components(const std::vector<std::vector<int>>& components) {
    for (size_t i = 0; i < components.size(); ++i) {
        std::cout << "Component " << i + 1 << ": ";
        for (int v : components[i]) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    try {
        Graph graph("list_of_edges_t1_023.txt", Graph::EDGES_LIST);
        ConnectivityFinder finder(graph);

        // Поиск компонент связности BFS
        auto components_bfs = finder.find_components_bfs();
        std::cout << "BFS Components (" << components_bfs.size() << "):\n";
        print_components(components_bfs);

        // Поиск компонент связности DFS
        auto components_dfs = finder.find_components_dfs();
        std::cout << "\nDFS Components (" << components_dfs.size() << "):\n";
        print_components(components_dfs);

        // Поиск слабых компонент для орграфа
        auto weak_components = finder.find_weak_components();
        std::cout << "\nWeak Components (" << weak_components.size() << "):\n";
        print_components(weak_components);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
















