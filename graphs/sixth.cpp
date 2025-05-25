#include "map.h"
#include <queue>
#include <unordered_map>
#include <iostream>
#include <algorithm>

std::vector<std::pair<int, int>> find_path(const Map& map, std::pair<int, int> start, std::pair<int, int> end) {
    std::queue<std::pair<int, int>> q;
    std::unordered_map<int, std::unordered_map<int, std::pair<int, int>>> parent;

    q.push(start);
    parent[start.first][start.second] = {-1, -1};

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        if (current == end) {
            std::vector<std::pair<int, int>> path;
            while (current.first != -1) {
                path.push_back(current);
                current = parent[current.first][current.second];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& neighbor : map.neighbors(current.first, current.second)) {
            if (!parent[neighbor.first].count(neighbor.second)) {
                parent[neighbor.first][neighbor.second] = current;
                q.push(neighbor);
            }
        }
    }

    return {};
}

int main() {
    try {
        Map map("C:/Users/goddammit/Documents/GitHub/laba2/graphs/maze_t6_007.txt");
        auto [rows, cols] = map.size();
        std::cout << "Map size: " << rows << "x" << cols << "\n";

        std::pair<int, int> start = {4361, 21};
        std::pair<int, int> end = { 2571, 17};

        auto path = find_path(map, start, end);

        if (!path.empty()) {
            std::cout << "Path found (" << path.size() << " steps):\n";
            for (const auto& p : path) {
                std::cout << "(" << p.first << ", " << p.second << ") ";
            }
            std::cout << "\n";
        } else {
            std::cout << "No path exists\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}