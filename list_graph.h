#ifndef LIST_GRAPH_H_INCLUDED
#define LIST_GRAPH_H_INCLUDED
#include <vector>

using std::vector;

class ListGraph // класс графа
{
public:
    ListGraph(int sz);
    void AddEdge(int from, int to); // добавление ребра в граф
    void DeleteEdge(int from, int to); // удаление ребра из графа
    int VerticesCount() const; // кол-во вершин в графе
    void GetNextVertices(int vertex, vector<int> &vertices) const; // запись всех "потомков" вершины в массив
private:
    vector<vector<int>> adjacencies;
};

#endif // LIST_GRAPH_H_INCLUDED
