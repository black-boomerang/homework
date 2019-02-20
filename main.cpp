//Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class ListGraph
{
public:
    ListGraph(int num_of_vert) : adjacencies(num_of_vert) { };

    void AddEdge(int from, int to)
    {
        adjacencies[from].push_back(to);
        adjacencies[to].push_back(from);
    }

    int VerticesCount() const
    {
        return static_cast<int>(adjacencies.size());
    }

    void GetNextVertices(int vertex, vector<int> &vertices) const
    {
        vertices.clear();
        for(int i : adjacencies[vertex]) {
            vertices.push_back(i);
        }
    }
private:
    vector<vector<int>> adjacencies;
};

int min_cycle_from(const ListGraph &graph, int vert) //минимальная длина цикла, содержащего указанную вершину
{
    int min_ccl = -1;
    int sz = graph.VerticesCount();
    queue<int> verticies;
    vector<int> dist(sz, -1); //расстояния от vert до вершин
    vector<int> parent(sz);
    verticies.push(vert);
    dist[vert] = 0;
    parent[vert] = -1;

    while(min_ccl == -1 && !verticies.empty()) //обход в ширину
    {
        int cur_vert = verticies.front();
        vector<int> next_verticies;
        graph.GetNextVertices(cur_vert, next_verticies);
        for(int next_vert : next_verticies)
        {
            if(dist[next_vert] == -1)
            {
                dist[next_vert] = dist[cur_vert] + 1;
                parent[next_vert] = cur_vert;
                verticies.push(next_vert);
            }
            else if(next_vert != parent[cur_vert])
            {
                min_ccl = dist[next_vert] + dist[cur_vert] + 1;
                break;
            }
        }
        verticies.pop();
    }

    return min_ccl;
}

int min_cycle(const ListGraph &graph) //вычисление минимальной длины цикла
{
    int min_ccl = -1;
    for(int i = 0; i < graph.VerticesCount(); ++i)
    {
        int candidate = min_cycle_from(graph, i);
        if(candidate != -1) {
            min_ccl = (min_ccl == -1) ? candidate : min(min_ccl, candidate);
        }
    }

    return min_ccl;
}

int main()
{
    int n, m;
    cin >> n >> m;

    ListGraph graph(n);
    for(int i = 0; i < m; ++i)
    {
        int vert1, vert2;
        cin >> vert1 >> vert2;
        graph.AddEdge(vert1, vert2);
    }

    int ans = min_cycle(graph);
    cout << ans;

    return 0;
}
