/*Необходимо написать торгового советника для поиска арбитража*/
#include <iostream>
#include <vector>

using namespace std;

struct Edge
{
    int from;
    int to;
    double rate;
};

class ArcGraph
{
public:
   ArcGraph(int vert_cnt) : vert_count(vert_cnt) { }

    void AddEdge(int from, int to, double rate)
    {
        graph_edges.push_back({from, to, rate});
    }

    int VerticesCount() const
    {
        return vert_count;
    }

    void GetAllEdges(vector<Edge> &edges) const
    {
        edges = graph_edges;
    }

private:
    vector<Edge> graph_edges;
    int vert_count;
};

class ListGraph
{
public:
    ListGraph(int num_of_vert) : adjacencies(num_of_vert) { };

    void AddEdge(int from, int to)
    {
        adjacencies[from].push_back(to);
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

void find_all_components(const ListGraph &graph, vector<vector<int> > &components)
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
}

bool is_arbitrage_from(const ArcGraph &rates, int from) // проверка на наличие арбитража от заданной вершины
{
    int sz = rates.VerticesCount();
    vector<double> dist_to_verts(sz, -1); // массив для хранения "дорожайших" путей до вершин
    dist_to_verts[from] = 1.0;

    int stage = 0;
    bool was_relax = true; // была ли проведена релаксация
    while(was_relax && stage <= sz) //
    {
        was_relax = false;
        vector<Edge> all_edges;
        rates.GetAllEdges(all_edges);
        for(Edge edge : all_edges) // попытка провести релаксацию вдоль каждого ребра
        {
            if(dist_to_verts[edge.from] >= 0.0 && dist_to_verts[edge.from] * edge.rate > dist_to_verts[edge.to])
            {
                dist_to_verts[edge.to] = dist_to_verts[edge.from] * edge.rate;
                was_relax = true;
            }
        }
        ++stage;
    }

    if(stage == sz + 1) { // арбитраж был найден
        return true;
    }
    return false;
}

bool is_arbitrage(const ArcGraph &rates) // проверка на наличие арбитража
{
    int sz = rates.VerticesCount();
    bool is_arb = false;

    ListGraph graph(sz); // преобразование в ListGraph
    vector<Edge> all_edges;
    rates.GetAllEdges(all_edges);
    for(Edge edge : all_edges) {
        graph.AddEdge(edge.from, edge.to);
    }

    vector<vector<int> > components;
    find_all_components(graph, components);

    for(size_t i = 0; i < components.size(); ++i) // проверка на наличия арбитража в каждой компоненте
    {
        if(is_arbitrage_from(rates, components[i][0]))
        {
            is_arb = true;
            break;
        }
    }

    return is_arb;
}

int main()
{
    int n;
    cin >> n;
    ArcGraph rates(n); // граф курсов валют
    ListGraph graph(n);
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(i == j) {
                continue;
            }

            double coef;
            cin >> coef;
            if(coef >= 0) {
                rates.AddEdge(i, j, coef);
            }
        }
    }

    if(is_arbitrage(rates)) {
        cout << "YES";
    }
    else {
        cout << "NO";
    }

    return 0;
}
