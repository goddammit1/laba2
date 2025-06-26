#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <tuple>
#include "Graph.h"

using namespace std;

int bfs(const vector<vector<int>>& residual_graph, int s, int t, vector<int>& parent) {
    int n = residual_graph.size() - 1; // Количество вершин (без 0)
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, numeric_limits<int>::max()});

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        // Обходим только вершины 1..n
        for (int v = 1; v <= n; ++v) {
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

int ford_fulkerson(const Graph& graph, int s, int t, vector<tuple<int, int, int>>& flow_edges) {
    if (!graph.is_directed())
        throw runtime_error("Flow network must be directed");

    int n = graph.size();
    vector<vector<int>> residual_graph(n + 1, vector<int>(n + 1, 0));

    // Инициализируем остаточную сеть
    for (int u = 1; u <= n; ++u) {
        for (int v = 1; v <= n; ++v) {
            residual_graph[u][v] = graph.weight(u, v);
        }
    }

    vector<int> parent(n + 1);
    int max_flow = 0;
    int new_flow;

    while ((new_flow = bfs(residual_graph, s, t, parent))){
        max_flow += new_flow;
        int v = t;

        while (v != s) {
            int u = parent[v];
            residual_graph[u][v] -= new_flow;
            residual_graph[v][u] += new_flow;
            v = u;
        }
    }

    // Собираем информацию о потоках
    for (int u = 1; u <= n; ++u) {
        for (int v = 1; v <= n; ++v) {
            int capacity = graph.weight(u, v);
            if (capacity > 0) { // Если ребро существовало в исходном графе
                int flow_val = capacity - residual_graph[u][v];
                if (flow_val > 0) {
                    flow_edges.push_back(make_tuple(u, v, flow_val));
                }
            }
        }
    }

    // Сортируем рёбра для удобного вывода
    sort(flow_edges.begin(), flow_edges.end());

    return max_flow;
}

int main() {
    try {
        Graph graph("C:/Users/goddammit/Documents/GitHub/laba2/graphs/list_of_edges_t14_009.txt", Graph::EDGES_LIST); // Убедитесь в правильности пути
        int source = 176;
        int sink = 289;

        vector<tuple<int, int, int>> flow_edges;
        int maximum_flow = ford_fulkerson(graph, source, sink, flow_edges);

        cout << "Maximum flow value: " << maximum_flow << "." << endl;
        cout << "Source: " << source << ", sink: " << sink << "." << endl;
        cout << "Flow:" << endl;

        for (const auto& edge : flow_edges) {
            cout << get<0>(edge) << "-" << get<1>(edge) << " : " << get<2>(edge) << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
