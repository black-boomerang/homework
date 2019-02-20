#include "list_graph.h"

using std::size_t;

ListGraph::ListGraph(const IGraph *graph)
{
    int sz = graph->VerticesCount();
    adjacencies.resize(sz);
    for(int i = 0; i < sz; ++i) {
        graph->GetNextVertices(i, adjacencies[i]);
    }
}

void ListGraph::AddEdge(int from, int to)
{
    adjacencies[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return adjacencies.size();
}

void ListGraph::GetNextVertices(int vertex, vector<int> &vertices) const
{
    vertices = adjacencies[vertex];
}

void ListGraph::GetPrevVertices(int vertex, vector<int> &vertices) const
{
    vertices.clear();
    for(size_t i = 0; i < adjacencies.size(); ++i)
    {
        if(i == vertex) {
            continue;
        }
        for(size_t j = 0; j < adjacencies[i].size(); ++j)
        {
            if(adjacencies[i][j] == vertex)
            {
                vertices.push_back(i);
                break;
            }
        }
    }
}
