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

        // Проверка на отрицательные циклы
        has_negative_cycle_ = false;
        queue = std::queue<int>(); // Очистка очереди
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

    std::vector<int> getPath(int target) const {
        if (has_negative_cycle_) {
            throw std::runtime_error("Graph contains a negative cycle");
        }
        if (target < 1 || target > graph_.size()) {
            throw std::invalid_argument("Invalid target vertex");
        }
        if (distance_[target] == INT_MAX) {
            return {};
        }

        std::vector<int> path;
        for (int v = target; v != -1; v = predecessor_[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void printDistances() const {
        std::cout << "Distances from vertex " << start_ << ":\n";
        for (int v = 1; v <= graph_.size(); ++v) {
            std::cout << "To " << v << ": ";
            if (distance_[v] == INT_MAX) {
                std::cout << "INF";
            } else {
                std::cout << distance_[v];
            }
            std::cout << "\n";
        }
    }
};

int main() {
    try {
        Graph graph("list_of_edges_t7_017.txt", Graph::EDGES_LIST);
        int start_vertex = 2;
        BellmanFordMoore bfm(graph, start_vertex);

        if (bfm.hasNegativeCycle()) {
            std::cout << "Graph contains a negative cycle!\n";
        } else {
            bfm.printDistances();

            // Пример получения пути до вершины 50
            int target = 50;
            auto path = bfm.getPath(target);
            if (path.empty()) {
                std::cout << "No path from " << start_vertex << " to " << target << "\n";
            } else {
                std::cout << "Path from " << start_vertex << " to " << target << ": ";
                for (int v : path) std::cout << v << " ";
                std::cout << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
