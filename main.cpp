//Дан невзвешенный неориентированный граф. Определить, является ли он двудольным. Требуемая сложность O(V+E).
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

bool is_bipartite(const ListGraph &graph) //проверка графа на двудольность
{
    int sz = graph.VerticesCount();
    queue<int> verticies;
    vector<int> group(sz, -1);

    for(int i = 0; i < sz; ++i) //проходим по всем вершинам и проверяем определение двудольности
    {
        if(group[i] != -1) {
            continue;
        }

        verticies.push(i);
        group[i] = 0;

        while(!verticies.empty()) //обход в ширину
        {
            int cur_vert = verticies.front();
            vector<int> next_verticies;
            graph.GetNextVertices(cur_vert, next_verticies);
            for(int next_vert : next_verticies)
            {
                if(group[next_vert] == -1)
                {
                    group[next_vert] = 1 - group[cur_vert];
                    verticies.push(next_vert);
                }
                if(group[next_vert] == group[cur_vert]) {
                    return false;
                }
            }
            verticies.pop();
        }
    }

    return true;
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

    if(is_bipartite(graph))
    {
        cout << "YES";
    }
    else {
        cout << "NO";
    }

    return 0;
}
