#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>
#include "Graph.h"

std::pair<std::vector<int>, std::vector<int>> checkBipartition(const Graph& graph) {
    int size = graph.size();
    std::vector<int> color(size + 1, 0); // 0 - uncolored, 1 and 2 are colors
    std::queue<int> q;

    for (int u = 1; u <= size; ++u) {
        if (color[u] == 0) {
            color[u] = 1;
            q.push(u);
            while (!q.empty()) {
                int current = q.front();
                q.pop();
                for (int neighbor : graph.adjacency_list(current)) {
                    if (color[neighbor] == 0) {
                        color[neighbor] = (color[current] == 1) ? 2 : 1;
                        q.push(neighbor);
                    } else if (color[neighbor] == color[current]) {
                        throw std::runtime_error("Graph is not bipartite.");
                    }
                }
            }
        }
    }

    std::vector<int> U, V;
    for (int u = 1; u <= size; ++u) {
        if (color[u] == 1) U.push_back(u);
        else V.push_back(u);
    }

    return {U, V};
}

bool dfsKuhn(int u, const Graph& graph, std::vector<int>& match, std::vector<bool>& visited) {
    if (visited[u]) return false;
    visited[u] = true;

    for (int v : graph.adjacency_list(u)) {
        if (match[v] == -1 || dfsKuhn(match[v], graph, match, visited)) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

std::pair<int, std::vector<std::pair<int, int>>> findMaxMatching(const Graph& graph) {
    if (graph.is_directed()) {
        throw std::runtime_error("Graph must be undirected for bipartite matching.");
    }

    auto [U, V] = checkBipartition(graph);

    int n = graph.size();
    std::vector<int> match(n + 1, -1); // Tracks matches for vertices in V
    int maxMatching = 0;

    for (int u : U) {
        std::vector<bool> visited(n + 1, false);
        if (dfsKuhn(u, graph, match, visited)) {
            maxMatching++;
        }
    }

    // Build mateU: for u in U, the vertex v in V matched to u, or -1 if unmatched.
    std::vector<int> mateU(n+1, -1);
    for (int v = 1; v <= n; ++v) {
        if (match[v] != -1) {
            mateU[match[v]] = v;
        }
    }

    // Collect matching pairs: for each u in U that is matched, add (u, mateU[u])
    std::vector<std::pair<int, int>> matchingPairs;
    for (int u : U) {
        if (mateU[u] != -1) {
            matchingPairs.push_back({u, mateU[u]});
        }
    }

    return {maxMatching, matchingPairs};
}

int main() {
    try {
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_adjacency_t13_012.txt", Graph::ADJACENCY_LIST);
        auto [matchingSize, matchingPairs] = findMaxMatching(graph);
        std::cout << "Size of maximum matching: " << matchingSize << "." << std::endl;
        std::cout << "Maximum matching:\n{";
        for (const auto& edge : matchingPairs) {
            std::cout << "(" << edge.first << ", " << edge.second << "), ";
        }
        std::cout << "}" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}