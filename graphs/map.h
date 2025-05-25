//
// Created by goddammit on 25.05.2025.
//

#ifndef UNTITLED2_LABIRINT_H
#define UNTITLED2_LABIRINT_H

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <iostream>

class Map {
private:
    std::vector<std::vector<int>> data;
    int rows;
    int cols;

public:
    Map(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }

        // Чтение первой строки для получения размеров
        std::string line;
        if (!std::getline(file, line)) {
            throw std::runtime_error("Empty file");
        }
        std::istringstream sizeStream(line);
        sizeStream >> rows >> cols;
        if (rows <= 0 || cols <= 0) {
            throw std::runtime_error("Invalid map dimensions");
        }

        // Чтение остальных строк как данных матрицы
        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            if (row.size() != static_cast<size_t>(cols)) {
                throw std::runtime_error("Inconsistent row length");
            }
            data.push_back(row);
        }

        if (data.size() != static_cast<size_t>(rows)) {
            throw std::runtime_error("Row count mismatch");
        }
    }

    int operator()(int i, int j) const {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[i][j];
    }

    std::pair<int, int> size() const {
        return {rows, cols};
    }

    std::vector<std::pair<int, int>> neighbors(int i, int j) const {
        std::vector<std::pair<int, int>> result;
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            return result;
        }

        const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& dir : directions) {
            int ni = i + dir[0];
            int nj = j + dir[1];
            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                if (data[ni][nj] != 0) {
                    result.emplace_back(ni, nj);
                }
            }
        }

        return result;
    }
};



#endif //UNTITLED2_LABIRINT_H
