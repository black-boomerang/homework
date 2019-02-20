#ifndef MATRIX_GRAPH_H_INCLUDED
#define MATRIX_GRAPH_H_INCLUDED
#include <vector>
#include "igraph.h"

using std::vector;

struct MatrixGraph : IGraph {
    vector<vector<char>> matrix;

    MatrixGraph(const IGraph *graph);
    ~MatrixGraph() { };

    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, vector<int> &vertices) const;
    void GetPrevVertices(int vertex, vector<int> &vertices) const;
};

#endif // MATRIX_GRAPH_H_INCLUDED
