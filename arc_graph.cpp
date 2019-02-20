#include "arc_graph.h"

using std::size_t;

ArcGraph::ArcGraph(const IGraph *graph)
{
    int sz = graph->VerticesCount();
    for(int i = 0; i < sz; ++i)
    {
        vector<int> vertices;
        graph->GetNextVertices(i, vertices);
        for(size_t j = 0; j < vertices.size(); ++j) {
            edges.push_back({i, vertices[j]});
        }
    }
}

void ArcGraph::AddEdge(int from, int to)
{
    edges.push_back({from, to});
}

int ArcGraph::VerticesCount() const
{
    int cnt = 0;
    for(size_t i = 0; i < edges.size(); ++i)
    {
        cnt = (edges[i].from > cnt) ? edges[i].from : cnt;
        cnt = (edges[i].to > cnt) ? edges[i].to : cnt;
    }
    return cnt + 1;
}

void ArcGraph::GetNextVertices(int vertex, vector<int> &vertices) const
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        if(edges[i].from == vertex) {
            vertices.push_back(edges[i].to);
        }
    }
}

void ArcGraph::GetPrevVertices(int vertex, vector<int> &vertices) const
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        if(edges[i].to == vertex) {
            vertices.push_back(edges[i].from);
        }
    }
}
