#include "list_graph.h"
#include <algorithm>

using std::size_t;
using std::find;

ListGraph::ListGraph(int sz) : adjacencies(sz) { }

void ListGraph::AddEdge(int from, int to) // добавление ребра в граф
{
    adjacencies[from].push_back(to);
    adjacencies[to].push_back(from);
}

void ListGraph::DeleteEdge(int from, int to) // удаление ребра из графа
{
    auto it = find(adjacencies[from].begin(), adjacencies[from].end(), to);
    auto it2 = find(adjacencies[to].begin(), adjacencies[to].end(), from);
    if(it != adjacencies[from].end()) {
        adjacencies[from].erase(it);
        adjacencies[to].erase(it2);
    }
}

int ListGraph::VerticesCount() const // кол-во вершин в графе
{
    return adjacencies.size();
}

void ListGraph::GetNextVertices(int vertex, vector<int> &vertices) const // запись всех "потомков" вершины в массив
{
    vertices = adjacencies[vertex];
}
