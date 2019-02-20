#ifndef SET_GRAPH_H_INCLUDED
#define SET_GRAPH_H_INCLUDED

#include <vector>
#include "igraph.h"
#include <unordered_set>

using std::vector;
using std::unordered_set;

struct SetGraph : IGraph {
    vector<unordered_set<int>> adjacencies;

    SetGraph(const IGraph *graph);
    ~SetGraph() { };

    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, vector<int> &vertices) const;
    void GetPrevVertices(int vertex, vector<int> &vertices) const;
};

#endif // SET_GRAPH_H_INCLUDED
