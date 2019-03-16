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

    for(int i = 0; i < sz; ++i) // проверка на наличия арбитража от каждой вершины
    {
        if(is_arbitrage_from(rates, i))
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
