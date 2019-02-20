/*Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
 Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).*/
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class ListGraph {
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

int count_of_ways(const ListGraph &graph, int from, int to) //кол-во кратчайших путей из from  в to
{
    int sz = graph.VerticesCount();
    queue<int> verticies;
    vector<int> ways_to_vert(sz);
    vector<int> dist(sz, -1);

    verticies.push(from);
    dist[from] = 0;
    ways_to_vert[from] = 1;

    int cur_vert = -1;
    while(cur_vert != to && !verticies.empty())
    {
        cur_vert = verticies.front();
        vector<int> next_verticies;
        graph.GetNextVertices(cur_vert, next_verticies);
        for(int next_vert : next_verticies)
        {
            if(dist[next_vert] == -1)
            {
                dist[next_vert] = dist[cur_vert] + 1;
                verticies.push(next_vert);
            }
            if(dist[next_vert] == dist[cur_vert] + 1) {
                ways_to_vert[next_vert] += ways_to_vert[cur_vert];
            }
        }
        verticies.pop();
    }

    return ways_to_vert[to];
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

    int from, to;
    cin >> from >> to;

    int ans = count_of_ways(graph, from, to);
    cout << ans;

    return 0;
}
