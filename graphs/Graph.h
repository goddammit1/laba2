//
// Created by goddammit on 10.03.2025.
//

#ifndef UNTITLED2_GRAPH_H
#define UNTITLED2_GRAPH_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <stdexcept>

struct NoneType {};
constexpr NoneType None = NoneType();

class Graph {
private:
    int size_{};
    bool is_directed_;
    std::vector<std::vector<int>> adjacency_matrix_;

    void LoadMatrix(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
        file >> size_;
        adjacency_matrix_.resize(size_ + 1, std::vector<int>(size_ + 1, 0));
        for (int u = 1; u <= size_; ++u) {
            for (int v = 1; v <= size_; ++v) {
                file >> adjacency_matrix_[u][v];
            }
        }
        file.close();
    }

    void LoadEdgesList(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
        file >> size_;
        adjacency_matrix_.resize(size_ + 1, std::vector<int>(size_ + 1, 0));
        std::string line;
        std::getline(file, line); // Consume the remaining part of the first line
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int u, v, weight = 1;
            iss >> u >> v;
            if (!(iss >> weight)) {
                weight = 1;
            }
            adjacency_matrix_[u][v] = weight;
        }
        file.close();
    }

    void LoadAdjacencyList(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
        file >> size_;
        adjacency_matrix_.resize(size_ + 1, std::vector<int>(size_ + 1, 0));
        std::string line;
        std::getline(file, line); // Consume the remaining part of the first line
        for (int u = 1; u <= size_; ++u) {
            std::getline(file, line);
            std::istringstream iss(line);
            std::string token;
            while (iss >> token) {
                size_t colon_pos = token.find(':');
                int v, weight = 1;
                if (colon_pos != std::string::npos) {
                    v = std::stoi(token.substr(0, colon_pos));
                    weight = std::stoi(token.substr(colon_pos + 1));
                } else {
                    v = std::stoi(token);
                }
                adjacency_matrix_[u][v] = weight;
            }
        }
        file.close();
    }

    [[nodiscard]] bool CheckDirected() const {
        for (int u = 1; u <= size_; ++u) {
            for (int v = 1; v <= size_; ++v) {
                if (adjacency_matrix_[u][v] != adjacency_matrix_[v][u]) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    enum FileType {
        MATRIX,
        EDGES_LIST,
        ADJACENCY_LIST
    };

    Graph(const std::string& path, FileType type) {
        switch (type) {
            case MATRIX:
                LoadMatrix(path);
                break;
            case EDGES_LIST:
                LoadEdgesList(path);
                break;
            case ADJACENCY_LIST:
                LoadAdjacencyList(path);
                break;
            default:
                throw std::invalid_argument("Invalid file type");
        }
        is_directed_ = CheckDirected();
    }

    [[nodiscard]] int size() const {
        return size_;
    }

    [[nodiscard]] int weight(int u, int v) const {
        if (u < 1 || u > size_ || v < 1 || v > size_) {
            throw std::out_of_range("Vertex index out of range");
        }
        return adjacency_matrix_[u][v];
    }

    bool is_edge(int u, int v) const {
        return weight(u, v) != 0;
    }

    [[nodiscard]] std::vector<std::vector<int>> adjacency_matrix() const {
        std::vector<std::vector<int>> matrix(size_ + 1, std::vector<int>(size_ + 1, 0));
        for (int u = 1; u <= size_; ++u) {
            for (int v = 1; v <= size_; ++v) {
                matrix[u][v] = adjacency_matrix_[u][v];
            }
        }
        return matrix;
    }

    [[nodiscard]] std::vector<int> adjacency_list(int v) const {
        if (v < 1 || v > size_) {
            throw std::out_of_range("Vertex index out of range");
        }
        std::vector<int> list;
        for (int u = 1; u <= size_; ++u) {
            if (adjacency_matrix_[v][u] != 0) {
                list.push_back(u);
            }
        }
        return list;
    }

    [[nodiscard]] std::vector<std::tuple<int, int, int>> list_of_edges() const {
        std::vector<std::tuple<int, int, int>> edges;
        for (int u = 1; u <= size_; ++u) {
            for (int v = 1; v <= size_; ++v) {
                if (adjacency_matrix_[u][v] != 0) {
                    if (is_directed_ || u <= v) {
                        edges.emplace_back(u, v, adjacency_matrix_[u][v]);
                    }
                }
            }
        }
        return edges;
    }

    [[nodiscard]] std::vector<std::tuple<int, int, int>> list_of_edges(int v) const {
        if (v < 1 || v > size_) {
            throw std::out_of_range("Vertex index out of range");
        }
        std::vector<std::tuple<int, int, int>> edges;
        for (int u = 1; u <= size_; ++u) {
            if (adjacency_matrix_[v][u] != 0) {
                edges.emplace_back(v, u, adjacency_matrix_[v][u]);
            }
        }
        return edges;
    }

    [[nodiscard]] bool is_directed() const {
        return is_directed_;
    }
};

#endif //UNTITLED2_GRAPH_H
