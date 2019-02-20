#include "matrix_graph.h"

using std::size_t;

MatrixGraph::MatrixGraph(const IGraph *graph)
{
    int sz = graph->VerticesCount();
    matrix.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        vector<int> vertices;
        graph->GetNextVertices(i, vertices);
        matrix[i].resize(sz);
        for(size_t j = 0; j < vertices.size(); ++j) {
            matrix[i][vertices[j]] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to)
{
    matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const
{
    return matrix.size();
}

void MatrixGraph::GetNextVertices(int vertex, vector<int> &vertices) const
{
    for(size_t i = 0; i < matrix[vertex].size(); ++i)
    {
        if(matrix[vertex][i]) {
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int vertex, vector<int> &vertices) const
{
    for(size_t i = 0; i < matrix[vertex].size(); ++i)
    {
        if(matrix[i][vertex]) {
            vertices.push_back(i);
        }
    }
}
