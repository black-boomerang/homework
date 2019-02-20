#include "set_graph.h"

using std::size_t;

SetGraph::SetGraph(const IGraph *graph)
{
    int sz = graph->VerticesCount();
    adjacencies.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        vector<int> verticies;
        graph->GetNextVertices(i, verticies);
        adjacencies[i].insert(verticies.begin(), verticies.end());
    }
}

void SetGraph::AddEdge(int from, int to)
{
    adjacencies[from].insert(to);
}

int SetGraph::VerticesCount() const
{
    return adjacencies.size();
}

void SetGraph::GetNextVertices(int vertex, vector<int> &vertices) const
{
    vertices.clear();
    vertices.insert(vertices.begin(), adjacencies[vertex].begin(), adjacencies[vertex].end());
}

void SetGraph::GetPrevVertices(int vertex, vector<int> &vertices) const
{
    vertices.clear();
    for(size_t i = 0; i < adjacencies.size(); ++i)
    {
        if(i != vertex && adjacencies[i].find(vertex) != adjacencies[i].end()) {
                vertices.push_back(i);
        }
    }
}
