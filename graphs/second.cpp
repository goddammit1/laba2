#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "Graph.h"

class BridgeArticulationFinder {
private:
    const Graph& graph_;
    std::vector<int> entry_time_;
    std::vector<int> low_;
    std::vector<bool> visited_;
    int timer_;
    std::set<std::pair<int, int>> bridges_;
    std::set<int> articulation_points_;

    void dfs(int u, int parent = -1) {
        visited_[u] = true;
        entry_time_[u] = low_[u] = timer_++;
        int children = 0;
        bool is_articulation = false;

        for (int v : graph_.adjacency_list(u)) {
            if (v == parent) continue; // Пропускаем родительскую вершину

            if (!visited_[v]) {
                dfs(v, u);
                low_[u] = std::min(low_[u], low_[v]);

                // Проверка на мост
                if (low_[v] > entry_time_[u]) {
                    bridges_.insert({std::min(u, v), std::max(u, v)});
                }

                // Проверка на шарнир
                if (low_[v] >= entry_time_[u] && parent != -1) {
                    is_articulation = true;
                }
                children++;
            } else {
                low_[u] = std::min(low_[u], entry_time_[v]);
            }
        }

        // Шарнир, если корень с >1 потомком
        if (parent == -1 && children > 1) {
            is_articulation = true;
        }

        if (is_articulation) {
            articulation_points_.insert(u);
        }
    }

public:
    BridgeArticulationFinder(const Graph& graph) : graph_(graph) {
        if (graph.is_directed()) {
            throw std::invalid_argument("Graph must be undirected");
        }
        int n = graph.size();
        entry_time_.resize(n + 1);
        low_.resize(n + 1);
        visited_.assign(n + 1, false);
        timer_ = 0;
    }

    void find() {
        for (int u = 1; u <= graph_.size(); ++u) {
            if (!visited_[u]) {
                dfs(u);
            }
        }
    }

    const std::set<std::pair<int, int>>& get_bridges() const {
        return bridges_;
    }

    const std::set<int>& get_articulation_points() const {
        return articulation_points_;
    }
};

void print_results(const std::set<std::pair<int, int>>& bridges,
                   const std::set<int>& articulation_points) {
    std::cout << "Bridges (" << bridges.size() << "):\n";
    for (const auto& bridge : bridges) {
        std::cout << bridge.first << "-" << bridge.second << "\n";
    }

    std::cout << "\nArticulation Points (" << articulation_points.size() << "):\n";
    for (int ap : articulation_points) {
        std::cout << ap << " ";
    }
    std::cout << "\n";
}

int main() {
    try {
        // Загрузка графа (предполагается, что он неориентированный)
        Graph graph("list_of_edges_t2_016.txt", Graph::EDGES_LIST);
        BridgeArticulationFinder finder(graph);
        finder.find();

        auto bridges = finder.get_bridges();
        auto articulation_points = finder.get_articulation_points();

        print_results(bridges, articulation_points);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}