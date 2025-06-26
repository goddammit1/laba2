#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "Graph.h"

class StronglyConnectedComponents {
private:
    const Graph& graph_;
    std::vector<bool> visited_;
    std::vector<std::vector<int>> components_;
    std::stack<int> order_;

    void dfs_pass1(int u) {
        visited_[u] = true;
        for (int v : graph_.adjacency_list(u)) {
            if (!visited_[v]) {
                dfs_pass1(v);
            }
        }
        order_.push(u);
    }

    void dfs_pass2(int u, std::vector<int>& component, const std::vector<std::vector<int>>& transpose) {
        visited_[u] = true;
        component.push_back(u);
        for (int v : transpose[u]) {
            if (!visited_[v]) {
                dfs_pass2(v, component, transpose);
            }
        }
    }

    std::vector<std::vector<int>> transpose_graph() const {
        int n = graph_.size();
        std::vector<std::vector<int>> transpose(n + 1);
        auto matrix = graph_.adjacency_matrix();

        for (int u = 1; u <= n; ++u) {
            for (int v = 1; v <= n; ++v) {
                if (matrix[u][v] != 0) {
                    transpose[v].push_back(u);
                }
            }
        }
        return transpose;
    }

public:
    StronglyConnectedComponents(const Graph& graph) : graph_(graph) {
        if (!graph.is_directed()) {
            throw std::invalid_argument("Graph must be directed");
        }
        int n = graph_.size();
        visited_.resize(n + 1, false);

        // Первый проход DFS для определения порядка
        for (int u = 1; u <= n; ++u) {
            if (!visited_[u]) {
                dfs_pass1(u);
            }
        }

        // Транспонирование графа
        auto transpose = transpose_graph();

        // Второй проход DFS в обратном порядке
        visited_.assign(n + 1, false);
        while (!order_.empty()) {
            int u = order_.top();
            order_.pop();
            if (!visited_[u]) {
                std::vector<int> component;
                dfs_pass2(u, component, transpose);
                components_.push_back(component);
            }
        }
    }

    const std::vector<std::vector<int>>& components() const {
        return components_;
    }

    void print_components() const {
        std::cout << "Strongly Connected Components:\n";
        for (size_t i = 0; i < components_.size(); ++i) {
            std::cout << "Component " << i + 1 << ": ";
            for (int v : components_[i]) {
                std::cout << v << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    try {
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_edges_t7_017.txt", Graph::EDGES_LIST);
        StronglyConnectedComponents scc(graph);
        scc.print_components();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}