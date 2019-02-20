/*Дан ориентированный граф. Определите, какое минимальное число ребер необходимо добавить, чтобы граф стал
 сильносвязным. В графе возможны петли.*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class ListGraph
{
public:
    ListGraph(int num_of_vert) : adjacencies(num_of_vert) { };

    void AddEdge(int from, int to)
    {
        if(find(adjacencies[from].begin(), adjacencies[from].end(), to) == adjacencies[from].end()) {
            adjacencies[from].push_back(to);
        }
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

    void GetPrevVertices(int vertex, vector<int> &vertices) const
    {
        vertices.clear();
        for(int i = 0; i < static_cast<int>(adjacencies.size()); ++i)
        {
            if(i == vertex) {
                continue;
            }
            for(int j : adjacencies[i])
            {
                if(j == vertex) {
                    vertices.push_back(i);
                }
            }
        }
    }

    int CountOfNext(int vertex) const
    {
        return static_cast<int>(adjacencies[vertex].size());
    }

    int CountOfPrev(int vertex) const
    {
        int cnt = 0;
        for(int i = 0; i < static_cast<int>(adjacencies.size()); ++i)
        {
            if(i == vertex) {
                continue;
            }
            for(int j : adjacencies[i])
            {
                if(j == vertex) {
                    ++cnt;
                }
            }
        }

        return cnt;
    }
private:
    vector<vector<int>> adjacencies;
};

void order_dfs(const ListGraph &graph, vector<char> &visited, vector<int> &order, int vert)
{
    visited[vert] = 1;
    vector<int> next_verticies;
    graph.GetNextVertices(vert, next_verticies);
    for(int i : next_verticies)
    {
        if(!visited[i]) {
            order_dfs(graph, visited, order, i);
        }
    }
    order.push_back(vert);
}

void find_component(const ListGraph &graph, vector<char> &visited,
                    vector<int> &component, int vert) //поиск компоненты сильной связности
{
    visited[vert] = 1;
    component.push_back(vert);
    vector<int> prev_verticies;
    graph.GetPrevVertices(vert, prev_verticies);
    for(int i : prev_verticies)
    {
        if(!visited[i]) {
            find_component(graph, visited, component, i);
        }
    }
}

//построение графа конденсации
void build_cond_graph(const ListGraph &graph, ListGraph &cond_graph, vector<vector<int> > &components)
{
    int sz = graph.VerticesCount();
    int num_of_comps = static_cast<int>(components.size());
    vector<int> vert_comp(sz);
    for(int i = 0; i < num_of_comps; ++i) //каждой вершине сопоставляем номер компоненты связности
    {
        for(int j : components[i]) {
            vert_comp[j] = i;
        }
    }

    for(int i = 0; i < sz; ++i) //строим рёбра между вершинами графа конденсации
    {
        vector<int> next_verticies;
        graph.GetNextVertices(i, next_verticies);
        for(int j : next_verticies) {
                if(vert_comp[i] != vert_comp[j]) {
                    cond_graph.AddEdge(vert_comp[i], vert_comp[j]);
                }
        }
    }
}

int min_to_strongly(const ListGraph &graph)
{
    int sz = graph.VerticesCount();
    vector<char> visited(sz);
    vector<int> order;

    for(int i = 0; i < sz; ++i) //топологическая сортировка вершин в обратном порядке
    {
        if(!visited[i]) {
            order_dfs(graph, visited, order, i);
        }
    }

    vector<vector<int> > components;
    int num_of_comps = 0;
    visited.assign(sz, 0);
    for(int i = sz - 1; i >= 0; --i) //находим компоненты сильной связности
    {
        int vert = order[i];
        if(!visited[vert])
        {
            ++num_of_comps;
            components.push_back(vector<int> (0));
            find_component(graph, visited, components[num_of_comps - 1], vert);
        }
    }

    ListGraph cond_graph(num_of_comps);
    build_cond_graph(graph, cond_graph, components); //построение графа конденсации

    int drains = 0, sources = 0; //находим кол-во стоков и истоков, ответом будет максимум
    for(int i = 0; i < num_of_comps; ++i)
    {
        if(cond_graph.CountOfNext(i) == 0) {
            ++drains;
        }
        if(cond_graph.CountOfPrev(i) == 0) {
            ++sources;
        }
    }

    return (num_of_comps == 1) ? 0 : max(drains, sources);
}

int main()
{
    int v, e;
    cin >> v >> e;

    ListGraph graph(v);
    for(int i = 0; i < e; ++i)
    {
        int from, to;
        cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }

    int ans = min_to_strongly(graph);
    cout << ans;

    return 0;
}
