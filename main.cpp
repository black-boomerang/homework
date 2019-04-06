/*Задан неориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью.
Найдите максимальный поток из вершины с номером 1 в вершину с номером n.*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

using namespace std;

class MatrixGraph
{
public:
    MatrixGraph(int sz) : matrix(sz, vector<int> (sz)) { }

    void AddEdge(int from, int to, int cost)
    {
        matrix[from][to] = cost;
    }

    int VerticesCount() const
    {
        return static_cast<int>(matrix.size());
    }

    vector<int> &operator[](int vertex)
    {
        return matrix[vertex];
    }

    void GetNextVertices(int vertex, vector<int> &vertices) const
    {
        for(int i = 0; i < VerticesCount(); ++i)
        {
            if(matrix[vertex][i]) {
                vertices.push_back(i);
            }
        }
    }
private:
    vector<vector<int>> matrix;
};

bool find_min_path(const MatrixGraph &graph, int from, int to, vector<int> &path) // поиск кратчайшего пути между вершинами
{
    int sz = graph.VerticesCount();
    vector<int> parent(sz);
    vector<char> visited(sz);
    queue<int> vert_q;
    vert_q.push(from);
    parent[from] = -1;
    visited[from] = 1;

    while(!vert_q.empty())
    {
        int cur_vert = vert_q.front();
        if(cur_vert == to) {
            break;
        }

        vert_q.pop();
        vector<int> next_verticies;
        graph.GetNextVertices(cur_vert, next_verticies);
        for(int i : next_verticies)
        {
            if(!visited[i])
            {
                visited[i] = 1;
                parent[i] = cur_vert;
                vert_q.push(i);
            }
        }
    }

    if(vert_q.empty()) {
        return false;
    }
    else
    {
        int cur_vert = to;
        while(cur_vert != -1)
        {
            path.push_back(cur_vert);
            cur_vert = parent[cur_vert];
        }
        reverse(path.begin(), path.end());

        return true;
    }
}

int max_flow(const MatrixGraph &graph, int source, int drain)
{
    MatrixGraph residual_graph = graph;
    int sz = graph.VerticesCount();
    vector<vector<int>> flows(sz, vector<int> (sz));
    for(int i = 0; i < sz; ++i)
    {
        for(int j = 0; j < sz; ++j) {
            flows[i][j] = 0;
        }
    }

    vector<int> path;
    while(find_min_path(residual_graph, source, drain, path)) // пока существует кратчайший путь из истока в сток
    {
        int min_cost = INT_MAX;
        for(size_t i = 0; i < path.size() - 1; ++i) { // ищем минимальную пропускную способность в кратчайшем пути
            min_cost = min(min_cost, residual_graph[path[i]][path[i + 1]]);
        }

        for(size_t i = 0; i < path.size() - 1; ++i) // модифицируем рёбра в остаточной сети
        {
            flows[path[i]][path[i + 1]] += min_cost;
            flows[path[i + 1]][path[i]] -= min_cost;
            residual_graph[path[i]][path[i + 1]] -= min_cost;
            residual_graph[path[i + 1]][path[i]] += min_cost;
        }

        path.clear();
    }

    int max_flw = 0;
    for(int i = 0; i < sz; ++i) { // находим максимальный поток
        max_flw += flows[source][i];
    }

    return max_flw;
}

int main()
{
    int n, m;
    cin >> n >> m;

    MatrixGraph graph(n);
    for(int i = 0; i < m; ++i)
    {
        int from, to, cost;
        cin >> from >> to >> cost;
        --from;
        --to;
        graph.AddEdge(from, to, cost);
    }

    int ans = max_flow(graph, 0, n - 1);
    cout << ans << endl;

    return 0;
}
