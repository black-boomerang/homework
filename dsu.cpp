#include "dsu.h"

using std::swap;

DSU::DSU(int sz) : parent(sz), tree_size(sz)
{
    for(int i = 0; i < sz; ++i)
    {
        parent[i] = i;
        tree_size[i] = 1;
    }
}

void DSU::make_set(int vert) // создание нового множества
{
    parent[vert] = vert;
    tree_size[vert] = vert;
}

int DSU::find_set(int vert) // поиск корневого элемента множества
{
    if(parent[vert] == vert) {
        return vert;
    }

    parent[vert] = find_set(parent[vert]);
    return parent[vert];
}

void DSU::union_set(int f_vert, int s_vert) // объединение двух множеств
{
    int f_tree = find_set(f_vert);
    int s_tree = find_set(s_vert);

    if(f_tree != s_tree)
    {
        if(tree_size[f_tree] < tree_size[s_tree]) {
            swap(f_tree, s_tree);
        }
        parent[s_tree] = f_tree;
        tree_size[f_tree] += tree_size[s_tree];
    }
}
