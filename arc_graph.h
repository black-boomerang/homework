#ifndef ARC_GRAPH_H_INCLUDED
#define ARC_GRAPH_H_INCLUDED
#include <vector>
#include "igraph.h"

using std::vector;

struct Edge {
    int from;
    int to;
};

struct ArcGraph : IGraph {
    vector<Edge> edges;

    ArcGraph(const IGraph *graph);
    ~ArcGraph() { };

    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, vector<int> &vertices) const;
    void GetPrevVertices(int vertex, vector<int> &vertices) const;
};

#endif // ARC_GRAPH_H_INCLUDED
