#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <unordered_set>
#include "Graph.h"

class FloydWarshallAnalyzer {
private:
    const Graph& graph_;
    std::vector<std::vector<int>> dist_;
    std::vector<std::vector<int>> next_;

    void initialize() {
        int n = graph_.size();
        dist_.resize(n + 1, std::vector<int>(n + 1, INT_MAX));
        next_.resize(n + 1, std::vector<int>(n + 1, -1));

        // Инициализация матриц
        for (int u = 1; u <= n; ++u) {
            dist_[u][u] = 0;
            for (int v : graph_.adjacency_list(u)) {
                dist_[u][v] = graph_.weight(u, v);
                next_[u][v] = v;
            }
        }
    }

    void computeShortestPaths() {
        int n = graph_.size();
        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i <= n; ++i) {
                if (dist_[i][k] == INT_MAX) continue;
                for (int j = 1; j <= n; ++j) {
                    if (dist_[k][j] != INT_MAX && dist_[i][j] > dist_[i][k] + dist_[k][j]) {
                        dist_[i][j] = dist_[i][k] + dist_[k][j];
                        next_[i][j] = next_[i][k];
                    }
                }
            }
        }
    }

public:
    FloydWarshallAnalyzer(const Graph& graph) : graph_(graph) {
        initialize();
        computeShortestPaths();
    }

    // Восстановление пути между u и v
    std::vector<int> reconstructPath(int u, int v) {
        if (next_[u][v] == -1) return {};

        std::vector<int> path;
        path.push_back(u);
        while (u != v) {
            u = next_[u][v];
            path.push_back(u);
        }
        return path;
    }

    // Анализ компонент связности
    void analyzeComponents() {
        int n = graph_.size();
        std::vector<bool> visited(n + 1, false);
        std::vector<std::vector<int>> components;

        // Поиск компонент связности
        for (int u = 1; u <= n; ++u) {
            if (!visited[u]) {
                std::vector<int> component;
                std::unordered_set<int> compSet;

                // Собираем все вершины в компоненте
                for (int v = 1; v <= n; ++v) {
                    if (!visited[v] && (dist_[u][v] != INT_MAX || dist_[v][u] != INT_MAX)) {
                        visited[v] = true;
                        component.push_back(v);
                        compSet.insert(v);
                    }
                }
                components.push_back(component);
            }
        }

        // Вывод информации о связности
        std::cout << "Graph is " << (components.size() == 1 ? "" : "NOT ")
                  << "connected and contains " << components.size() << " connected components.\n\n";

        // Анализ каждой компоненты
        for (const auto& comp : components) {
            // Вывод вершин
            std::cout << "Vertices numbers:\n\t";
            for (int v : comp) std::cout << v << "\t";
            std::cout << "\n";

            // Вывод степеней вершин
            std::cout << "Vertices degrees:\n\t";
            for (int v : comp) std::cout << graph_.adjacency_list(v).size() << "\t";
            std::cout << "\n";

            // Вычисление эксцентриситетов
            std::cout << "Eccentricity:\n\t";
            std::vector<int> ecc(comp.size());
            for (int i = 0; i < comp.size(); ++i) {
                int maxDist = 0;
                for (int j = 0; j < comp.size(); ++j) {
                    if (dist_[comp[i]][comp[j]] != INT_MAX) {
                        maxDist = std::max(maxDist, dist_[comp[i]][comp[j]]);
                    }
                }
                ecc[i] = maxDist;
                std::cout << ecc[i] << "\t";
            }
            std::cout << "\n";

            // Вычисление радиуса и центра
            int radius = *std::min_element(ecc.begin(), ecc.end());
            int diameter = *std::max_element(ecc.begin(), ecc.end());

            std::vector<int> central;
            for (int i = 0; i < comp.size(); ++i) {
                if (ecc[i] == radius) central.push_back(comp[i]);
            }

            std::vector<int> peripherial;
            for (int i = 0; i < comp.size(); ++i) {
                if (ecc[i] == diameter) peripherial.push_back(comp[i]);
            }

            // Вывод характеристик
            std::cout << "R = " << radius << ".0\n";
            std::cout << "Central vertices:\n[";
            for (int i = 0; i < central.size(); ++i) {
                std::cout << central[i];
                if (i < central.size() - 1) std::cout << ", ";
            }
            std::cout << "]\n";

            std::cout << "D = " << diameter << ".0\n";
            std::cout << "Peripherial vertices:\n[";
            for (int i = 0; i < peripherial.size(); ++i) {
                std::cout << peripherial[i];
                if (i < peripherial.size() - 1) std::cout << ", ";
            }
            std::cout << "]\n\n";
        }
    }
};

int main() {
    try {
        // Убедитесь, что путь к файлу корректный
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_edges_t10_008.txt", Graph::EDGES_LIST);
        FloydWarshallAnalyzer analyzer(graph);

        // Пример вывода пути
        int u = 2, v = 50;
        auto path = analyzer.reconstructPath(u, v);
        if (!path.empty()) {
            std::cout << "Path from " << u << " to " << v << ": ";
            for (int node : path) std::cout << node << " ";
            std::cout << "\n\n";
        } else {
            std::cout << "No path between " << u << " and " << v << "\n\n";
        }

        // Анализ компонент
        analyzer.analyzeComponents();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}


