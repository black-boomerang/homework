#ifndef LIST_GRAPH_H_INCLUDED
#define LIST_GRAPH_H_INCLUDED
#include <vector>
#include "igraph.h"

using std::vector;

struct ListGraph : IGraph {
    vector<vector<int>> adjacencies;

    ListGraph(const IGraph *graph);
    ~ListGraph() { };

    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, vector<int> &vertices) const;
    void GetPrevVertices(int vertex, vector<int> &vertices) const;
};

#endif // LIST_GRAPH_H_INCLUDED
