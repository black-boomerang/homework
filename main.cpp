/*В одном из отделов крупной организации работает n человек. Как практически все сотрудники этой организации, они любят
пить чай в перерывах между работой. При этом они достаточно дисциплинированы и делают в день ровно один перерыв, во
время которого пьют чай. Для того, чтобы этот перерыв был максимально приятным, каждый из сотрудников этого отдела обязательно
пьет чай одного из своих любимых сортов. В разные дни сотрудник может пить чай разных сортов. Для удобства пронумеруем сорта
чая числами от 1 до m. Недавно сотрудники отдела купили себе большой набор чайных пакетиков, который содержит a1 пакетиков
чая сорта номер 1, a2 пакетиков чая сорта номер 2, ..., am пакетиков чая сорта номер m. Теперь они хотят знать, на какое
максимальное число дней им может хватить купленного набора так, чтобы в каждый из дней каждому из сотрудников доставался
пакетик чая одного из его любимых сортов. Каждый сотрудник отдела пьет в день ровно одну чашку чая, которую заваривает из
одного пакетика. При этом пакетики чая не завариваются повторно.*/
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

// проверяем, хватит ли набора чая на заданное кол-во дней
bool is_enough(MatrixGraph &graph, int days, int people_num, int sorts_num)
{
    for(int i = 1; i <= people_num; ++i) {
        graph[0][i] = days;
    }

    for(int i = 1; i <= sorts_num; ++i) {
        graph[people_num + i][graph.VerticesCount() - 1];
    }

    int max_f = max_flow(graph, 0, graph.VerticesCount() - 1);
    return (max_f == days * people_num);
}

int max_days(const vector<int> &num_of_each_sort, const vector<vector<int>> &teas) // максимальное число дней, на которые хватит набора
{
    int people_num = static_cast<int>(teas.size());
    int sorts_num = static_cast<int>(num_of_each_sort.size()); // кол-во различных сортов чая

    MatrixGraph graph(people_num + sorts_num + 2);

    int all_teas = 0;
    for(int i = 0; i < sorts_num; ++i) {
        all_teas += num_of_each_sort[i];
    }

    for(int i = 0; i < people_num; ++i)
    {
        for(size_t j = 0; j < teas[i].size(); ++j) {
            graph.AddEdge(i + 1, people_num + teas[i][j], num_of_each_sort[teas[i][j] - 1]);
        }
    }

    for(int i = 1; i <= sorts_num; ++i) {
        graph.AddEdge(people_num + i, graph.VerticesCount() - 1, num_of_each_sort[i - 1]);
    }

    int left = 0, right = all_teas, mid;
    while(right - left > 1) // бинарный поиск по числу дней
    {
        mid = right - (right - left) / 2;
        if(is_enough(graph, mid, people_num, sorts_num)) {
            left = mid;
        }
        else {
            right = mid;
        }
    }

    if(is_enough(graph, right, people_num, sorts_num)) {
        return right;
    }
    else {
        return left;
    }
}

int main()
{
    int n, m;
    cin >> n >> m;

    vector<int> a(m);
    for(int i = 0; i < m; ++i) {
        cin >> a[i];
    }

    vector<vector<int>> teas(n);
    for(int i = 0; i < n; ++i)
    {
        int k;
        cin >> k;
        teas[i].resize(k);
        for(int j = 0; j < k; ++j) {
            cin >> teas[i][j];
        }
    }

    int ans = max_days(a, teas);
    cout << ans << endl;

    return 0;
}
