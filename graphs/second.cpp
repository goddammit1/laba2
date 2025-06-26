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
    std::vector<int> articulation_points_;  // Changed to vector
    std::vector<bool> is_ap_added_;         // Track added articulation points

    void dfs(int u, int parent = -1) {
        visited_[u] = true;
        entry_time_[u] = low_[u] = timer_++;
        int children = 0;
        bool is_articulation = false;

        for (int v : graph_.adjacency_list(u)) {
            if (v == parent) continue;

            if (!visited_[v]) {
                dfs(v, u);
                low_[u] = std::min(low_[u], low_[v]);

                if (low_[v] > entry_time_[u]) {
                    bridges_.insert({std::min(u, v), std::max(u, v)});
                }

                if (low_[v] >= entry_time_[u] && parent != -1) {
                    is_articulation = true;
                }
                children++;
            } else {
                low_[u] = std::min(low_[u], entry_time_[v]);
            }
        }

        if (parent == -1 && children > 1) {
            is_articulation = true;
        }

        if (is_articulation && !is_ap_added_[u]) {
            articulation_points_.push_back(u);
            is_ap_added_[u] = true;
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
        is_ap_added_.assign(n + 1, false);  // Initialize
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

    const std::vector<int>& get_articulation_points() const {  // Return vector
        return articulation_points_;
    }
};

void print_results(const std::set<std::pair<int, int>>& bridges,
                   const std::vector<int>& articulation_points) {
    // Print bridges in sorted order
    std::cout << "Bridges:\n";
    if (bridges.empty()) {
        std::cout << "[]\n";
    } else {
        std::cout << "[";
        auto it = bridges.begin();
        std::cout << it->first << "-" << it->second;
        for (++it; it != bridges.end(); ++it) {
            std::cout << ", " << it->first << "-" << it->second;
        }
        std::cout << "]\n";
    }

    // Print articulation points in discovery order
    std::cout << "Cut vertices:\n";
    if (articulation_points.empty()) {
        std::cout << "[]\n";
    } else {
        std::cout << "[";
        std::cout << articulation_points[0];
        for (int i = 1; i < articulation_points.size(); ++i) {
            std::cout << ", " << articulation_points[i];
        }
        std::cout << "]\n";
    }
}

int main() {
    try {
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