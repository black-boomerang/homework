/*Полный ориентированный взвешенный граф задан матрицей смежности. Постройте матрицу кратчайших путей между его
вершинами. Гарантируется, что в графе нет циклов отрицательного веса.*/
#include <iostream>
#include <vector>
#include <fstream>
#include <climits>

using namespace std;

class MatrixGraph
{
public:
    MatrixGraph(int sz) : matrix(vector<vector<int>> (sz, vector<int> (sz))) { }

    vector<int> &operator[](int vert)
    {
        return matrix[vert];
    }

    void AddEdge(int from, int to, int dist)
    {
        matrix[from][to] = dist;
    }

    int VerticesCount() const
    {
        return static_cast<int>(matrix.size());
    }

private:
    vector<vector<int>> matrix;
};

void floyd_algo(MatrixGraph &graph, vector<vector<int>> &short_dist_matrix) // алгоритм Флойда
{
    int sz = graph.VerticesCount();
    for(int i = 0; i < sz; ++i)
    {
        for(int j = 0; j < sz; ++j) {
            short_dist_matrix[i][j] = graph[i][j];
        }
    }

    for(int k = 0; k < sz; ++k)
    {
        for(int i = 0; i < sz; ++i)
        {
            for(int j = 0; j < sz; ++j) {
                short_dist_matrix[i][j] = min(short_dist_matrix[i][j],
                                              short_dist_matrix[i][k] + short_dist_matrix[k][j]);
            }
        }
    }
}

int main()
{
    ifstream in("floyd.in");
    int n;
    in >> n;

    MatrixGraph graph(n);
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            int dist;
            in >> dist;
            graph.AddEdge(i, j, dist);
        }
    }

    in.close();

    vector<vector<int>> short_dist_matrix(n, vector<int> (n));
    floyd_algo(graph, short_dist_matrix);

    ofstream out("floyd.out");
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            out << short_dist_matrix[i][j] << " ";
        }
        out << endl;
    }

    out.close();

    return 0;
}
