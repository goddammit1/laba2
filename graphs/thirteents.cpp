//
// Created by goddammit on 25.05.2025.
//

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include "Graph.h"

using namespace std;

int bfs(const vector<vector<int>>& residual_graph, int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, numeric_limits<int>::max()});

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int v = 0; v < residual_graph.size(); ++v) {
            if (parent[v] == -1 && residual_graph[u][v] > 0) {
                parent[v] = u;
                int new_flow = min(flow, residual_graph[u][v]);
                if (v == t)
                    return new_flow;
                q.push({v, new_flow});
            }
        }
    }
    return 0;
}

int ford_fulkerson(const Graph& graph, int s, int t) {
    if (!graph.is_directed())
        throw runtime_error("Flow network must be directed");

    int n = graph.size();
    vector<vector<int>> residual_graph(n + 1, vector<int>(n + 1, 0));

    // Initialize residual graph
    for (int u = 1; u <= n; ++u) {
        for (int v = 1; v <= n; ++v) {
            residual_graph[u][v] = graph.weight(u, v);
        }
    }

    vector<int> parent(n + 1);
    int max_flow = 0;
    int new_flow;

    while (new_flow = bfs(residual_graph, s, t, parent)) {
        max_flow += new_flow;
        int v = t;

        while (v != s) {
            int u = parent[v];
            residual_graph[u][v] -= new_flow;
            residual_graph[v][u] += new_flow;
            v = u;
        }
    }

    return max_flow;
}

int main() {
    try {
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_edges_t14_010.txt", Graph::EDGES_LIST);
        int source = 333;  // Исток
        int sink = 418;   // Сток

        int maximum_flow = ford_fulkerson(graph, source, sink);
        cout << "Maximum flow from " << source << " to " << sink
             << " is: " << maximum_flow << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
