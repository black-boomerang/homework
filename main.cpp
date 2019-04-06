/*Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе
с помощью алгоритма Крускала.*/
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Edge // ребро дерева
{
    int first_vert;
    int second_vert;
    int weight;
};

class DSU // система непересекающихся множеств
{
public:
    DSU(int sz) : parent(sz), tree_size(sz)
    {
        for(int i = 0; i < sz; ++i)
        {
            parent[i] = i;
            tree_size[i] = 1;
        }
    }

    void make_set(int vert) // создание нового множества
    {
        parent[vert] = vert;
        tree_size[vert] = vert;
    }

    int find_set(int vert) // поиск корневого элемента множества
    {
        if(parent[vert] == vert) {
            return vert;
        }

        parent[vert] = find_set(parent[vert]);
        return parent[vert];
    }

    void union_set(int f_vert, int s_vert) // объединение двух множеств
    {
        int f_tree = find_set(f_vert);
        int s_tree = find_set(s_vert);

        if(f_tree != s_tree)
        {
            if(tree_size[f_tree] < tree_size[s_tree]) {
                swap(f_tree, s_tree);
            }
            parent[s_tree] = f_tree;
            tree_size[f_tree] += tree_size[s_tree];
        }
    }
private:
    vector<int> parent;
    vector<int> tree_size;
};

bool comparator(const Edge &e1, const Edge &e2)
{
    return (e1.weight < e2.weight);
}

int kruskala_alg(vector<Edge> &graph, int n) // алгоритм Крускала
{
    int sz = static_cast<int>(graph.size());
    DSU dsu(n);
    int min_cost = 0;
    sort(graph.begin(), graph.end(), comparator);

    for(int i = 0; i < sz; ++i)
    {
        if(dsu.find_set(graph[i].first_vert) != dsu.find_set(graph[i].second_vert))
        {
            min_cost += graph[i].weight;
            dsu.union_set(graph[i].first_vert, graph[i].second_vert);
        }
    }

    return min_cost;
}

int main()
{
    int n, m;
    cin >> n >> m;

    vector<Edge> graph;
    for(int i = 0; i < m; ++i)
    {
        int f_vert, s_vert, weight;
        cin >> f_vert >> s_vert >> weight;
        graph.push_back({f_vert - 1, s_vert - 1, weight});
    }

    int ans = kruskala_alg(graph, n);
    cout << ans;

    return 0;
}
