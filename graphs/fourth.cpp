#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Graph.h"

class GraphAnalyzer {
private:
    const Graph& graph_;
    std::vector<std::vector<int>> dist_matrix_;
    std::vector<int> degrees_;
    std::vector<int> eccentricities_;
    int diameter_;
    std::vector<int> peripheral_;
    int radius_;
    std::vector<int> central_;

    void init_distance_matrix() {
        int n = graph_.size();
        dist_matrix_.resize(n + 1, std::vector<int>(n + 1, std::numeric_limits<int>::max()));

        auto adj_matrix = graph_.adjacency_matrix();
        for (int u = 1; u <= n; ++u) {
            dist_matrix_[u][u] = 0;
            for (int v : graph_.adjacency_list(u)) {
                dist_matrix_[u][v] = adj_matrix[u][v];
            }
        }
    }

    void floyd_warshall() {
        int n = graph_.size();
        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i <= n; ++i) {
                if (dist_matrix_[i][k] == std::numeric_limits<int>::max()) continue;
                for (int j = 1; j <= n; ++j) {
                    if (dist_matrix_[k][j] != std::numeric_limits<int>::max()) {
                        int new_dist = dist_matrix_[i][k] + dist_matrix_[k][j];
                        if (new_dist < dist_matrix_[i][j]) {
                            dist_matrix_[i][j] = new_dist;
                        }
                    }
                }
            }
        }
    }

    void compute_degrees() {
        degrees_.resize(graph_.size() + 1, 0);
        for (int u = 1; u <= graph_.size(); ++u) {
            degrees_[u] = graph_.adjacency_list(u).size();
        }
    }

    void compute_eccentricities() {
        eccentricities_.resize(graph_.size() + 1, 0);
        int n = graph_.size();
        for (int u = 1; u <= n; ++u) {
            int max_dist = 0;
            bool is_connected = true;
            for (int v = 1; v <= n; ++v) {
                if (u == v) continue;
                if (dist_matrix_[u][v] == std::numeric_limits<int>::max()) {
                    eccentricities_[u] = std::numeric_limits<int>::max();
                    is_connected = false;
                    break;
                } else {
                    if (dist_matrix_[u][v] > max_dist) {
                        max_dist = dist_matrix_[u][v];
                    }
                }
            }
            if (is_connected) {
                eccentricities_[u] = max_dist;
            }
        }
    }

    void compute_diameter_radius() {
        diameter_ = *std::max_element(eccentricities_.begin() + 1, eccentricities_.end());
        radius_ = *std::min_element(eccentricities_.begin() + 1, eccentricities_.end());
    }

    void find_peripheral_central() {
        for (int u = 1; u <= graph_.size(); ++u) {
            if (eccentricities_[u] == diameter_) {
                peripheral_.push_back(u);
            }
            if (eccentricities_[u] == radius_) {
                central_.push_back(u);
            }
        }
    }

public:
    GraphAnalyzer(const Graph& graph) : graph_(graph) {
        if (graph.is_directed()) {
            throw std::invalid_argument("Graph must be undirected");
        }
        init_distance_matrix();
        floyd_warshall();
        compute_degrees();
        compute_eccentricities();
        compute_diameter_radius();
        find_peripheral_central();
    }

    const std::vector<int>& degrees() const { return degrees_; }
    const std::vector<int>& eccentricities() const { return eccentricities_; }
    int diameter() const { return diameter_; }
    const std::vector<int>& peripheral() const { return peripheral_; }
    int radius() const { return radius_; }
    const std::vector<int>& central() const { return central_; }
};

void print_vector(const std::vector<int>& vec, const std::string& title) {
    std::cout << title << ":\n";
    for (size_t i = 1; i < vec.size(); ++i) {
        std::cout << i << ": ";
        if (vec[i] == std::numeric_limits<int>::max()) {
            std::cout << "+Infinity";
        } else {
            std::cout << vec[i];
        }
        std::cout << "\n";
    }
}

int main() {
    try {
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/matrix_t4_011.txt", Graph::MATRIX);
        GraphAnalyzer analyzer(graph);

        // a) Степени вершин
        print_vector(analyzer.degrees(), "Vertex degrees");

        // b) Эксцентриситеты
        print_vector(analyzer.eccentricities(), "Eccentricities");

        // c) Диаметр и периферийные вершины
        std::cout << "\nDiameter: ";
        if (analyzer.diameter() == std::numeric_limits<int>::max()) {
            std::cout << "+Infinity";
        } else {
            std::cout << analyzer.diameter();
        }
        std::cout << "\nPeripheral vertices: ";
        for (int v : analyzer.peripheral()) std::cout << v << " ";

        // d) Радиус и центральные вершины
        std::cout << "\nRadius: ";
        if (analyzer.radius() == std::numeric_limits<int>::max()) {
            std::cout << "+Infinity";
        } else {
            std::cout << analyzer.radius();
        }
        std::cout << "\nCentral vertices: ";
        for (int v : analyzer.central()) std::cout << v << " ";
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
    }
    return 0;
}
