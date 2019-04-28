/*Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости)
с помощью минимального остовного дерева, построенного в первой задаче.*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <climits>
#include "list_graph.h"
#include "dsu.h"

using namespace std;

struct Edge // ребро графа
{
    int first_vert;
    int second_vert;
    double weight;

    bool operator<(const Edge &edge) const
    {
        return (weight < edge.weight);
    }
};

struct Vertex // вершина графа
{
    double x;
    double y;
};

// генерация точек на плоскости
void generate_points(vector<Vertex> &verticies, normal_distribution<double> &distrib, default_random_engine &generator, int vert_num)
{
    verticies.resize(vert_num);

    for(int i = 0; i < vert_num; ++i) // генерируем точки с дисперсией 1
    {
        verticies[i].x = distrib(generator);
        verticies[i].y = distrib(generator);
    }
}

void get_edges(const vector<Vertex> &verticies, vector<Edge> &edges) // создание списка рёбер
{
    int vert_num = static_cast<int>(verticies.size());

    for(int i = 1; i < vert_num; ++i) // создание массива рёбер
    {
        for(int j = 0; j < i; ++j)
        {
            double dist = hypot(verticies[i].x - verticies[j].x, verticies[i].y - verticies[j].y);
            edges.push_back({j, i, dist});
        }
    }
}

void get_matrix(const vector<Edge> &edges, vector<vector<double>> &matrix) // создание матрицы расстояний
{
    for(Edge edge : edges) {
        matrix[edge.first_vert][edge.second_vert] = matrix[edge.second_vert][edge.first_vert] = edge.weight;
    }
}

// нахождение веса цикла, проходящего через заданные вершины
double cycle_through_verts(const vector<vector<double>> &distance_matrix, vector<int> &verticies)
{
    int vert_num = static_cast<int>(distance_matrix.size());
    double cycle_length = distance_matrix[verticies[vert_num - 1]][verticies[0]];

    for(int i = 0; i < vert_num - 1; ++i) {
        cycle_length += distance_matrix[verticies[i]][verticies[i + 1]];
    }

    return cycle_length;
}

// нахождение минимального остовного дерева
void find_min_span_tree(vector<Edge> &edges, ListGraph &span_graph, int n)
{
    int sz = static_cast<int>(edges.size());
    DSU dsu(n);
    sort(edges.begin(), edges.end());

    for(int i = 0; i < sz; ++i)
    {
        if(dsu.find_set(edges[i].first_vert) != dsu.find_set(edges[i].second_vert))
        {
            span_graph.AddEdge(edges[i].first_vert, edges[i].second_vert);
            dsu.union_set(edges[i].first_vert, edges[i].second_vert);
        }
    }
}

// эйлеров обход графа, нахождение порядка обхода
void find_order(ListGraph &graph, int vert, vector<int> &order, vector<char> &visited)
{
    if(!visited[vert]) {
        order.push_back(vert);
    }
    visited[vert] = 1;

    vector<int> next_verticies;
    graph.GetNextVertices(vert, next_verticies);

    for(int next : next_verticies)
    {
        graph.DeleteEdge(vert, next);
        find_order(graph, next, order, visited);
    }
}

// рекурсивное нахождение степени каждой вершины
void find_verticies_degree(const ListGraph &graph, int vert, vector<int> &vertex_degree)
{
    int vert_num = graph.VerticesCount();
    for(int i = 0; i < vert_num; ++i)
    {
        vector<int> next_verticies;
        graph.GetNextVertices(i, next_verticies);
        vertex_degree[i] = static_cast<int>(next_verticies.size());
    }
}

// поиск совершенного паросочетания минимального веса
void find_min_matching(const vector<int> &odd_verticies, const vector<vector<double>> &distance_matrix,
                       vector<int> &min_matching)
{
    // преобразуем в двудольный граф
    int vert_num = static_cast<int>(odd_verticies.size());
    vector<vector<double>> new_dist_matrix(vert_num + 1, vector<double> (vert_num + 1));
    for(int i = 1; i <= vert_num; ++i)
    {
        for(int j = 1; j <= vert_num; ++j)
        {
            if(i == j) {
                new_dist_matrix[i][j] = INT_MAX;
            }
            else {
                new_dist_matrix[i][j] = distance_matrix[odd_verticies[i - 1]][odd_verticies[j - 1]];
            }
            new_dist_matrix[j][i] = new_dist_matrix[i][j];
        }
    }

    vector<double> capacity_1(vert_num + 1), capacity_2(vert_num + 1); // потенциалы вершин
    vector<int> matching(vert_num + 1), prev(vert_num + 1);
    for(int i = 1; i <= vert_num; ++i)
    {
        matching[0] = i;
        int cur_vert = 0;
        vector<char> visited(vert_num + 1);
        vector<double> auxiliary_min(vert_num + 1, INT_MAX);
        while(matching[cur_vert] != 0) // пока можно увелис=чить мощность паросочетаний
        {
            visited[cur_vert] = 1;
            int cur_matching = matching[cur_vert];
            double changing = INT_MAX;
            int next_vert;
            for(int j = 1; j <= vert_num; ++j) // находим оптимальное изменение потенциала
            {
                if(visited[j]) {
                    continue;
                }
                double cur_min = new_dist_matrix[cur_matching][j] - capacity_1[cur_matching] - capacity_2[j];
                if(cur_min < auxiliary_min[j])
                {
                    auxiliary_min[j] = cur_min;
                    prev[j] = cur_vert;
                }
                if(auxiliary_min[j] < changing)
                {
                    changing = auxiliary_min[j];
                    next_vert = j;
                }
            }

            for(int j = 0; j <= vert_num; ++j) // пересчёт потенциала
            {
                if(visited[j])
                {
                    capacity_1[matching[j]] += changing;
                    capacity_2[j] -= changing;
                }
                else {
                    auxiliary_min[j] -= changing;
                }
            }

            cur_vert = next_vert;
        }

        while(cur_vert != 0) // восстановление паросочетаний
        {
            matching[cur_vert] = matching[prev[cur_vert]];
            cur_vert = prev[cur_vert];
        }
    }

    for(int i = 1; i <= vert_num; ++i) { // восстановление ответа
        min_matching[matching[i] - 1] = i - 1;
    }
}

// приближённое решение задачи
double approximated_min_cycle(vector<Edge> &edges, const vector<vector<double>> &distance_matrix)
{
    int vert_num = static_cast<int>(distance_matrix.size());

    ListGraph graph(vert_num);
    find_min_span_tree(edges, graph, vert_num);

    // Поиск вершшин нечётной степени
    vector<int> vertex_degree(vert_num);
    find_verticies_degree(graph, 0, vertex_degree);
    vector<int> odd_verticies;
    for(int i = 0; i < vert_num; ++i)
    {
        if(vertex_degree[i] % 2 == 1) {
            odd_verticies.push_back(i);
        }
    }

    // Поиск совершенного паросочетания минимального веса
    int odd_vert_num = static_cast<int>(odd_verticies.size());
    vector<int> min_matching(odd_vert_num);
    find_min_matching(odd_verticies, distance_matrix, min_matching);
    vector<char> odd_visited(odd_vert_num);
    for(int i = 0; i < odd_vert_num; ++i)
    {
        if(!odd_visited[i] && !odd_visited[min_matching[i]])
        {
            odd_visited[i] = odd_visited[min_matching[i]] = 1;
            graph.AddEdge(odd_verticies[i], odd_verticies[min_matching[i]]);
        }
    }

    // Поиск эйлерова цикла
    vector<int> verticies;
    vector<char> visited(vert_num);
    find_order(graph, 0, verticies, visited);

    return cycle_through_verts(distance_matrix, verticies);
}

// перебор всевозможных гамильтоновых циклов
double min_hamiltonian_cycle(const vector<vector<double>> &distance_matrix, vector<int> &verticies, vector<char> &vert_in_path)
{
    int vert_num = static_cast<int>(distance_matrix.size());
    int cur_vert_num = static_cast<int>(verticies.size());

    if(cur_vert_num == vert_num) {
        return cycle_through_verts(distance_matrix, verticies);
    }

    double min_cycle = INT_MAX;

    for(int i = 0; i < vert_num; ++i) // проверка наличия всех вершин в цикле
    {
        if(!vert_in_path[i])
        {
            vert_in_path[i] = 1;
            verticies.push_back(i);
            min_cycle = min(min_cycle, min_hamiltonian_cycle(distance_matrix, verticies, vert_in_path));
            verticies.pop_back();
            vert_in_path[i] = 0;
        }
    }

    return min_cycle;
}

// точное решение задачи
double real_min_cycle(const vector<vector<double>> &distance_matrix)
{
    vector<int> verticies;
    vector<char> vert_in_path(static_cast<int>(distance_matrix.size()));
    double min_cycle = min_hamiltonian_cycle(distance_matrix, verticies, vert_in_path);

    return min_cycle;
}

// основная функция
void salesman_approximation()
{
    default_random_engine generator(time(0));
    normal_distribution<double> distrib(0.0, 1.0);
    const int experiments_num = 10;

    for(int n = 2; n <= 10; ++n) // эксперименты для разных n
    {
        vector<double> approximation_quality(experiments_num);
        for(int i = 0; i < experiments_num; ++i)
        {
            vector<Vertex> verticies;
            generate_points(verticies, distrib, generator, n);

            vector<Edge> edges;
            get_edges(verticies, edges);

            vector<vector<double>> distance_matrix(n, vector<double> (n));
            get_matrix(edges, distance_matrix);

            double approximated_ans = approximated_min_cycle(edges, distance_matrix);
            double real_ans = real_min_cycle(distance_matrix);
            approximation_quality[i] = (real_ans != 0) ? approximated_ans / real_ans : 1.0; // качество приближения
        }

        // нахождение среднего значения и среднеквадратичного отклонения
        double average_quality = accumulate(approximation_quality.begin(), approximation_quality.end(), 0.0) / experiments_num;
        double standard_deviation = 0.0;
        for(double i : approximation_quality) {
            standard_deviation += (i - average_quality) * (i - average_quality);
        }
        standard_deviation = sqrt(standard_deviation / experiments_num);

        cout << "Number of points: " << n << endl;
        cout << "Average quality: " << average_quality << endl;
        cout << "Standard deviation: " << standard_deviation << endl << endl;
    }
}

int main()
{
    salesman_approximation();

    return 0;
}
