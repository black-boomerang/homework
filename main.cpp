/*Задано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
Требуется ответить на m (1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.*/
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class ListGraph
{
public:
    ListGraph(int sz) : adjacencies(sz) { }

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
        vertices = adjacencies[vertex];
    }
private:
    vector<vector<int>> adjacencies;
};

// поиск времён захода и выхода из вершин поиском в глубину
void find_times(const ListGraph &tree, int vert, vector<int> &time_in, vector<int> &time_out, int &time)
{
    time_in[vert] = time;
    ++time;

    vector<int> next_verticies;
    tree.GetNextVertices(vert, next_verticies);
    for(int next : next_verticies) {
        find_times(tree, next, time_in, time_out, time);
    }

    time_out[vert] = time;
    ++time;
}

// поиск предков
void find_previous(const ListGraph &tree, int vert, vector<vector<int>> &prevs)
{
    for(size_t deg = 1; deg < prevs[vert].size(); ++deg) { // считаем всех предков 2^deg степени
        prevs[vert][deg] = prevs[prevs[vert][deg - 1]][deg - 1];
    }

    vector<int> next_verticies;
    tree.GetNextVertices(vert, next_verticies);
    for(int next : next_verticies)
    {
        prevs[next][0] = vert;
        find_previous(tree, next, prevs);
    }
}

// проверка, является ли одна вершина предком другой
bool is_previous(int probable_parent, int probable_child, const vector<int> &time_in, const vector<int> &time_out)
{
    return (time_in[probable_child] >= time_in[probable_parent] &&
            time_out[probable_child] <= time_out[probable_parent]);
}

// наименьший общий предок вершин
int find_lca(int vert_1, int vert_2, const vector<int> &time_in, const vector<int> &time_out,
             const vector<vector<int>> &prevs)
{
    // проверяем является ли одна из вершин предком другой
    if(is_previous(vert_1, vert_2, time_in, time_out)) {
        return vert_1;
    }
    if(is_previous(vert_2, vert_1, time_in, time_out)) {
        return vert_2;
    }

    // ищем наименьшего общего предка
    int max_uncle = vert_1;
    for(int i = static_cast<int>(prevs[vert_1].size()) - 1; i >= 0; --i)
    {
        if(!is_previous(prevs[max_uncle][i], vert_2, time_in, time_out)) {
            max_uncle = prevs[max_uncle][i];
        }
    }

    return prevs[max_uncle][0];
}

// поиск суммы наименьших общих предков
long long find_lca_sum(const ListGraph &tree, int m, long long a1, long long a2, long long x, long long y, long long z)
{
    // препроцессинг
    int sz = tree.VerticesCount();
    vector<int> time_in(sz), time_out(sz);
    int time = 0;
    find_times(tree, 0, time_in, time_out, time); // ищем времена захода и выхода из вершин

    vector<vector<int>> prevs(sz, vector<int> (ceil(log2(sz)) + 1));
    prevs[0][0] = 0;
    find_previous(tree, 0, prevs);

    int lca = 0;
    long long sum = 0;
    for(int i = 0; i < m; ++i) // пересчитываем номера вершин и считаем lca
    {
        lca = find_lca((a1 + lca) % sz, a2, time_in, time_out, prevs);
        sum += lca;
        a1 = (x * a1 + y * a2 + z) % sz;
        a2 = (x * a2 + y * a1 + z) % sz;
    }

    return sum;
}

int main()
{
    int n, m;
    cin >> n >> m;

    ListGraph tree(n);
    for(int i = 1; i < n; ++i)
    {
        int prev;
        cin >> prev;
        tree.AddEdge(prev, i);
    }

    long long a1, a2, x, y, z;
    cin >> a1 >> a2 >> x >> y >> z;

    long long ans = find_lca_sum(tree, m, a1, a2, x, y, z);
    cout << ans;

    return 0;
}
