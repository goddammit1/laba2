#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include "Graph.h"

class BellmanFordMoore {
private:
    const Graph& graph_;
    int start_;
    std::vector<int> distance_;
    std::vector<int> predecessor_;
    bool has_negative_cycle_;

    void initialize() {
        int n = graph_.size();
        distance_.assign(n + 1, INT_MAX);
        predecessor_.assign(n + 1, -1);
        distance_[start_] = 0;
    }

    void relaxEdges() {
        int n = graph_.size();
        std::queue<int> queue;
        std::vector<bool> in_queue(n + 1, false);

        queue.push(start_);
        in_queue[start_] = true;

        for (int i = 0; i < n - 1; ++i) {
            int size = queue.size();
            if (size == 0) break;

            for (int j = 0; j < size; ++j) {
                int u = queue.front();
                queue.pop();
                in_queue[u] = false;

                for (int v : graph_.adjacency_list(u)) {
                    int weight = graph_.weight(u, v);
                    if (distance_[u] != INT_MAX && distance_[v] > distance_[u] + weight) {
                        distance_[v] = distance_[u] + weight;
                        predecessor_[v] = u;
                        if (!in_queue[v]) {
                            queue.push(v);
                            in_queue[v] = true;
                        }
                    }
                }
            }
        }

        has_negative_cycle_ = false;
        for (int u = 1; u <= n; ++u) {
            for (int v : graph_.adjacency_list(u)) {
                if (distance_[u] != INT_MAX && distance_[v] > distance_[u] + graph_.weight(u, v)) {
                    has_negative_cycle_ = true;
                    return;
                }
            }
        }
    }

public:
    BellmanFordMoore(const Graph& graph, int start)
            : graph_(graph), start_(start), has_negative_cycle_(false) {
        if (start < 1 || start > graph.size()) {
            throw std::invalid_argument("Invalid start vertex");
        }
        initialize();
        relaxEdges();
    }

    bool hasNegativeCycle() const { return has_negative_cycle_; }

    void printDistances() const {
        std::cout << "Shotest paths lengths from " << start_ << ":\n{";
        int n = graph_.size();
        for (int v = 1; v <= n; ++v) {
            if (v > 1) std::cout << ", ";
            std::cout << v << ": ";
            if (distance_[v] == INT_MAX) {
                std::cout << "+Infinity";
            } else {
                std::cout << distance_[v];
            }
        }
        std::cout << "}" << std::endl;
    }
};

int main() {
    try {

        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_edges_t11_008.txt", Graph::EDGES_LIST);
        int start_vertex = 13;
        BellmanFordMoore bfm(graph, start_vertex);

        if (bfm.hasNegativeCycle()) {
            std::cout << "Graph contains a negative cycle!\n";
        } else {
            bfm.printDistances();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
