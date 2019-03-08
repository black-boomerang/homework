/*Требуется отыскать самый выгодный маршрут между городами. Требуемое время работы O((N+M)logN), где
 N-количество городов, M-известных дорог между ними*/
#include <iostream>
#include <vector>
#include <set>
#include <climits>

using namespace std;

class ListGraph
{
public:
    ListGraph(int num_of_vert) : adjacencies(num_of_vert) { };

    void AddEdge(int from, int to, int dist)
    {
        adjacencies[from].push_back(make_pair(to, dist));
        adjacencies[to].push_back(make_pair(from, dist));
    }

    int VerticesCount() const
    {
        return static_cast<int>(adjacencies.size());
    }

    void GetNextVertices(int vertex, vector<pair<int, int>> &vertices) const
    {
        vertices.clear();
        for(pair<int, int> i : adjacencies[vertex]) {
            vertices.push_back(i);
        }
    }
private:
    vector<vector<pair<int, int>>> adjacencies;
};

int shortest_path(const ListGraph &graph, int from, int to) //поиск кратчайшего пути между вершинами
{
    int sz = graph.VerticesCount();
    vector<int> dist_to_verts(sz, INT_MAX); //массив для хранения кратчайших путей до вершин
    set<pair<int, int>> set_dist;
    set_dist.insert(make_pair(0, from));
    dist_to_verts[from] = 0;

    int short_dist = 0;
    while(!set_dist.empty()) //ищем пути
    {
        auto item = set_dist.begin();
        int vert = item->second; //номер текущей вершины
        int dist = item->first; //расстояние до текущей вершины
        set_dist.erase(item);
        if(vert == to)
        {
            short_dist = dist;
            break;
        }

        vector<pair<int, int>> next_verticies;
        graph.GetNextVertices(vert, next_verticies);
        for(pair<int, int> next : next_verticies) //выполняем релаксацию вершин
        {
            int next_vert = next.first;
            int dist_to_next = next.second;
            if(dist_to_verts[vert] + dist_to_next < dist_to_verts[next_vert]) //улучшаем расстояние до вершины
            {
                set_dist.erase(make_pair(dist_to_verts[next_vert], next_vert));
                dist_to_verts[next_vert] = dist_to_verts[vert] + dist_to_next;
                set_dist.insert(make_pair(dist_to_verts[next_vert], next_vert));
            }
        }
    }

    return short_dist;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ListGraph graph(n);
    for(int i = 0; i < m; ++i)
    {
        int from, to, dist;
        cin >> from >> to >> dist;
        graph.AddEdge(from, to, dist);
    }
    int from, to;
    cin >> from >> to;

    int ans = shortest_path(graph, from, to);
    cout << ans << endl;

    return 0;
}
