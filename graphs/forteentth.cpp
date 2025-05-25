//
// Created by goddammit on 25.05.2025.
//
#include "Graph.h"
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include <iostream>

class AntColony {
private:
    const Graph& graph_;                // Ссылка на граф
    int num_ants_;                      // Количество муравьев
    double alpha_;                      // Влияние феромонов
    double beta_;                       // Влияние эвристики
    double rho_;                        // Коэффициент испарения феромонов
    double initial_pheromone_;          // Начальный уровень феромонов
    int max_iterations_;                // Максимальное число итераций
    std::vector<std::vector<double>> pheromones_; // Матрица феромонов
    std::vector<int> best_path_;        // Лучший найденный маршрут
    double best_path_length_;           // Длина лучшего маршрута
    std::mt19937 rng_;                  // Генератор случайных чисел

    // Инициализация матрицы феромонов
    void InitializePheromones() {
        pheromones_.resize(graph_.size() + 1, std::vector<double>(graph_.size() + 1, initial_pheromone_));
    }

    // Построение маршрута одним муравьем
    std::vector<int> ConstructPath() {
        std::vector<int> path;
        std::vector<bool> visited(graph_.size() + 1, false);
        int current = std::uniform_int_distribution<int>(1, graph_.size())(rng_);
        path.push_back(current);
        visited[current] = true;

        while (path.size() < graph_.size()) {
            std::vector<int> candidates;
            for (int v = 1; v <= graph_.size(); ++v) {
                if (!visited[v] && graph_.is_edge(current, v)) {
                    candidates.push_back(v);
                }
            }
            if (candidates.empty()) {
                break; // Нет доступных непосещенных вершин
            }

            std::vector<double> probabilities;
            double total = 0.0;
            for (int v : candidates) {
                double tau = pheromones_[current][v];
                double eta = 1.0 / graph_.weight(current, v);
                double p = std::pow(tau, alpha_) * std::pow(eta, beta_);
                probabilities.push_back(p);
                total += p;
            }
            for (double& p : probabilities) {
                p /= total;
            }

            std::discrete_distribution<int> dist(probabilities.begin(), probabilities.end());
            int next = candidates[dist(rng_)];
            path.push_back(next);
            visited[next] = true;
            current = next;
        }
        return path;
    }

    // Обновление феромонов
    void UpdatePheromones(const std::vector<std::vector<int>>& paths, const std::vector<double>& path_lengths) {
        // Испарение феромонов
        for (int u = 1; u <= graph_.size(); ++u) {
            for (int v = 1; v <= graph_.size(); ++v) {
                if (graph_.is_edge(u, v)) {
                    pheromones_[u][v] *= (1 - rho_);
                }
            }
        }
        // Отложение феромонов
        for (int k = 0; k < num_ants_; ++k) {
            if (paths[k].size() == graph_.size()) {
                double delta = 1.0 / path_lengths[k];
                for (size_t i = 0; i < paths[k].size() - 1; ++i) {
                    int u = paths[k][i];
                    int v = paths[k][i + 1];
                    pheromones_[u][v] += delta;
                    if (!graph_.is_directed()) {
                        pheromones_[v][u] += delta;
                    }
                }
            }
        }
    }

    // Вычисление длины маршрута
    double CalculatePathLength(const std::vector<int>& path) {
        double length = 0.0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            length += graph_.weight(path[i], path[i + 1]);
        }
        return length;
    }

public:
    // Конструктор
    AntColony(const Graph& graph, int num_ants, double alpha, double beta, double rho,
              double initial_pheromone, int max_iterations)
            : graph_(graph), num_ants_(num_ants), alpha_(alpha), beta_(beta), rho_(rho),
              initial_pheromone_(initial_pheromone), max_iterations_(max_iterations),
              best_path_length_(std::numeric_limits<double>::max()),
              rng_(std::random_device{}()) {
        InitializePheromones();
    }

    // Запуск алгоритма
    void Run() {
        for (int iter = 0; iter < max_iterations_; ++iter) {
            std::vector<std::vector<int>> paths;
            std::vector<double> path_lengths;
            for (int k = 0; k < num_ants_; ++k) {
                std::vector<int> path = ConstructPath();
                if (path.size() == graph_.size()) {
                    double length = CalculatePathLength(path);
                    paths.push_back(path);
                    path_lengths.push_back(length);
                    if (length < best_path_length_) {
                        best_path_ = path;
                        best_path_length_ = length;
                    }
                }
            }
            UpdatePheromones(paths, path_lengths);
        }
    }

    // Получение лучшего маршрута
    std::vector<int> GetBestPath() const {
        return best_path_;
    }

    // Получение длины лучшего маршрута
    double GetBestPathLength() const {
        return best_path_length_;
    }
};

int main() {
    // Загрузка графа из файла в формате матрицы смежности
    Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/matrix_t15_009.txt", Graph::MATRIX);

    // Создание объекта ACO с параметрами:
    // 10 муравьев, alpha=1.0, beta=2.0, rho=0.1, начальный феромон=0.1, 100 итераций
    AntColony aco(graph, 10, 1.0, 2.0, 0.1, 0.1, 100);

    // Запуск алгоритма
    aco.Run();

    // Получение результатов
    std::vector<int> best_path = aco.GetBestPath();
    double best_length = aco.GetBestPathLength();

    // Вывод результатов
    if (!best_path.empty()) {
        std::cout << "Лучший маршрут: ";
        for (int v : best_path) {
            std::cout << v << " ";
        }
        std::cout << "\nДлина маршрута: " << best_length << std::endl;
    } else {
        std::cout << "Маршрут, проходящий через все вершины, не найден." << std::endl;
    }

    return 0;
}
