#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
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
                for (int j = 1; j <= n; ++j) {
                    if (dist_[i][k] != INT_MAX && dist_[k][j] != INT_MAX &&
                        dist_[i][j] > dist_[i][k] + dist_[k][j]) {
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

    // a) Восстановление пути между u и v
    std::vector<int> reconstructPath(int u, int v) {
        if (dist_[u][v] == INT_MAX) return {};

        std::vector<int> path;
        path.push_back(u);
        while (u != v) {
            u = next_[u][v];
            path.push_back(u);
        }
        return path;
    }

    // b) Характеристики компонент связности
    void analyzeComponents() {
        int n = graph_.size();
        std::vector<bool> visited(n + 1, false);

        for (int u = 1; u <= n; ++u) {
            if (!visited[u]) {
                std::vector<int> component;
                // Поиск компоненты через достижимость
                for (int v = 1; v <= n; ++v) {
                    if (dist_[u][v] != INT_MAX && !visited[v]) {
                        component.push_back(v);
                        visited[v] = true;
                    }
                }

                // Вычисление характеристик
                int diameter = 0, radius = INT_MAX;
                std::vector<int> central;

                for (int v : component) {
                    int maxDist = 0;
                    for (int w : component) {
                        maxDist = std::max(maxDist, dist_[v][w]);
                    }
                    diameter = std::max(diameter, maxDist);
                    if (maxDist < radius) {
                        radius = maxDist;
                        central = {v};
                    } else if (maxDist == radius) {
                        central.push_back(v);
                    }
                }

                // Вывод результатов
                std::cout << "Компонента: ";
                for (int v : component) std::cout << v << " ";
                std::cout << "\nДиаметр: " << diameter << "\nРадиус: " << radius
                          << "\nЦентральные вершины: ";
                for (int v : central) std::cout << v << " ";
                std::cout << "\n\n";
            }
        }
    }
};

int main() {
    try {
        Graph graph("list_of_edges_t7_017.txt", Graph::EDGES_LIST);
        FloydWarshallAnalyzer analyzer(graph);

        // Пример: путь от 2 до 50
        int u = 2, v = 50;
        auto path = analyzer.reconstructPath(u, v);
        std::cout << "Путь от " << u << " до " << v << ": ";
        for (int node : path) std::cout << node << " ";
        std::cout << "\n\n";

        // Анализ компонент
        analyzer.analyzeComponents();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}