#ifndef DSU_H_INCLUDED
#define DSU_H_INCLUDED
#include <vector>

using std::vector;

class DSU // система непересекающихся множеств
{
public:
    DSU(int sz);
    void make_set(int vert); // создание нового множества
    int find_set(int vert); // поиск корневого элемента множества
    void union_set(int f_vert, int s_vert); // объединение двух множеств
private:
    vector<int> parent;
    vector<int> tree_size;
};

#endif // DSU_H_INCLUDED
