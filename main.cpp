/*Дан массив из целых чисел a1, a2, ..., an (индексация с 1!). Для каждого запроса [left, right] найдите такой
подотрезок al, al+1, ..., ar этого массива (1 <= left <= l <= r <= right <= n), что сумма чисел al + al+1 + ... + ar
является максимально возможной. Требуемое время ответа на запрос - O(log n).*/
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

struct Node
{
    long long sum;
    long long max_pref;
    long long max_suf;
    long long max_sub_sum;
};

class SegmentTree // дерево отрезков
{
public:
    SegmentTree(const vector<int> &arr)
    {
        int sz = static_cast<int>(arr.size());
        int tree_sz = static_cast<int>(pow(2, ceil(log2(sz)) + 1)) - 1; // размер дерева с учётом фиктивных вершин
        nodes.resize(tree_sz);

        int i; // заполняем листья
        for(i = 0; i < sz; ++i) {
            int node_num = i + tree_sz / 2;
            nodes[node_num].sum = nodes[node_num].max_pref = nodes[node_num].max_suf = nodes[node_num].max_sub_sum = arr[i];
        }
        for(i += tree_sz / 2 + 1; i < tree_sz; ++i) {
            nodes[i].sum = nodes[i].max_pref = nodes[i].max_suf = nodes[i].max_sub_sum = LLONG_MIN;
        }

        build_tree(); // строим само дерево
    }

    int find_max_sub_sum(int left, int right) // поиск максимума на подотрезке
    {
        Node max_node = find_max_node(0, left, right, 0, nodes.size() / 2);
        return max_node.max_sub_sum;
    }
private:
    void build_tree() // посторение дерева
    {
        for(int i = nodes.size() / 2 - 1; i >= 0; --i) // считаем значения через значения "детей"
        {
            int left = i * 2 + 1, right = i * 2 + 2;
            segment_union(nodes[i], nodes[left], nodes[right]);
        }
    }

    void segment_union(Node &res_node, const Node &left_node, const Node &right_node) //объединение информации о двух подотрезках
    {
        res_node.sum = left_node.sum + right_node.sum;
        res_node.max_pref = max(left_node.max_pref, left_node.sum + right_node.max_pref);
        res_node.max_suf = max(right_node.max_suf, right_node.sum + left_node.max_suf);
        res_node.max_sub_sum = max(max(left_node.max_sub_sum, right_node.max_sub_sum),
                                       left_node.max_suf + right_node.max_pref);
    }

    Node find_max_node(int cur_vert, int left, int right, int seg_left, int seg_right)
    {
        if(left == seg_left && right == seg_right) { // подотрезок найден
            return nodes[cur_vert];
        }

        int seg_mid = (seg_left + seg_right) / 2;
        if(left > seg_mid) { // правый подотрезок
            return find_max_node(cur_vert * 2 + 2, left, right, seg_mid + 1, seg_right);
        }
        if(right <= seg_mid) { // левый подотрезок
            return find_max_node(cur_vert * 2 + 1, left, right, seg_left, seg_mid);
        }

        Node node;
        Node left_node = find_max_node(cur_vert * 2 + 1, left, seg_mid, seg_left, seg_mid);
        Node right_node = find_max_node(cur_vert * 2 + 2, seg_mid, right, seg_mid, seg_right);
        segment_union(node, left_node, right_node); // объединяем два подотрезка
        return node;
    }

    vector<Node> nodes;
};

int main()
{
    ifstream in("input.txt");

    int n;
    in >> n;
    while(!in.eof())
    {
        int m;
        in >> m;

        vector<int> arr(n);
        for(int i = 0; i < n; ++i) {
            in >> arr[i];
        }

        SegmentTree seg_tree(arr);
        for(int i = 0; i < m; ++i)
        {
            int x, y;
            in >> x >> y;
            cout << seg_tree.find_max_sub_sum(x - 1, y - 1) << endl;
        }

        in >> n;
    }

    in.close();

    return 0;
}
