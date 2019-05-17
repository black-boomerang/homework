// Реализуйте структуру данных “массив строк” на основе декартового дерева по неявному ключу.
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>

using namespace std;

struct Node // элемент дерева
{
    string value;
    int sz; // кол-во вершин в поддереве
    int priority;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
};

class Treap // декартово дерево по неявному ключу
{
public:
    Treap() : root(nullptr) { }

    void InsertAt(int position, const string &value) // вставка нового значения
    {
        shared_ptr<Node> new_node = make_shared<Node>();
        new_node->value = value;
        new_node->priority = rand();
        new_node->sz = 1;
        new_node->left = new_node->right = nullptr;

        shared_ptr<Node> left_treap(nullptr);
        shared_ptr<Node> right_treap(nullptr);
        Split(root, position, left_treap, right_treap); // разрезаем дерево по ключу pos
        Merge(root, left_treap, new_node); // сливаем левое дерево с новым элементом
        Merge(root, root, right_treap); // сливаем с правым деревом
    }

    void DeleteAt(int position) // удаление элемента по индексу
    {
        shared_ptr<Node> left_treap(nullptr);
        shared_ptr<Node> right_treap(nullptr);
        shared_ptr<Node> node(nullptr);
        Split(root, position, left_treap, right_treap);
        Split(right_treap, 1, node, right_treap);
        Merge(root, left_treap, right_treap);
    }

    string GetAt(int position) const // получение элемента по индексу
    {
        shared_ptr<Node> node(SearchAt(position));
        return node->value;
    }
private:
    shared_ptr<Node> SearchAt(int position) const // поиск элемента
    {
        int cur_position = position;
        shared_ptr<Node> cur_node(root);
        int left_sz = GetTreapSz(root->left);
        while(left_sz != cur_position) // пока искомая позиция не равна разверу левого поддерева
        {
            if(left_sz < cur_position)
            {
                cur_position -= left_sz + 1;
                cur_node = cur_node->right;
            }
            else {
                cur_node = cur_node->left;
            }

            left_sz = GetTreapSz(cur_node->left);
        }

        return cur_node;
    }

    int GetTreapSz(const shared_ptr<Node> treap) const // размевар поддере
    {
        return treap ? treap->sz : 0;
    }

    void ChangeSz(shared_ptr<Node> treap) // изменение размера дерева
    {
        if(!treap) {
            return;
        }
        treap->sz = GetTreapSz(treap->left) + GetTreapSz(treap->right) + 1;
    }

    void Merge(shared_ptr<Node> &treap, shared_ptr<Node> left_treap, shared_ptr<Node> right_treap)
    {
        // если какое-то из деревьев пусто, то искомое дерево равно другому
        if(!left_treap)
        {
            treap = right_treap;
            ChangeSz(treap);
            return;
        }
        if(!right_treap)
        {
            treap = left_treap;
            ChangeSz(treap);
            return;
        }

         // в зависимости от приоритета сливаем ребёнка одного из деревьев с другим деревом
        if(left_treap->priority < right_treap->priority)
        {
            Merge(right_treap->left, left_treap, right_treap->left);
            treap = right_treap;
        }
        else
        {
            Merge(left_treap->right, left_treap->right, right_treap);
            treap = left_treap;
        }

        // меняем кол-во вершин в поддереве
        ChangeSz(treap);
    }

    void Split(shared_ptr<Node> treap, int key, shared_ptr<Node> &left_treap, shared_ptr<Node> &right_treap)
    {
        if(!treap) // разрезать нечего
        {
            left_treap = nullptr;
            right_treap = nullptr;
            return;
        }

        if(GetTreapSz(treap->left) < key) // в зависимости от неявного ключа разрезаем правое или левое поддерево
        {
            Split(treap->right, key - GetTreapSz(treap->left) - 1, treap->right, right_treap);
            left_treap = treap;
        }
        else
        {
            Split(treap->left, key, left_treap, treap->left);
            right_treap = treap;
        }

        // меняем кол-во вершин в поддереве
        ChangeSz(treap);
    }

    shared_ptr<Node> root;
};

int main()
{
    srand(time(0));

    int n;
    cin >> n;

    Treap arr;
    for(int i = 0; i < n; ++i)
    {
        char operation;
        int pos;
        cin >> operation >> pos;
        if(operation == '+')
        {
            string str;
            cin >> str;
            arr.InsertAt(pos, str);
        }
        else if(operation == '-')
        {
            string str;
            getline(cin, str);
            int second_pos = (str.size() == 0) ? pos : atoi(str.c_str());
            for(int j = pos; j <= second_pos; ++j) {
                arr.DeleteAt(pos);
            }
        }
        else if(operation == '?') {
            cout << arr.GetAt(pos) << endl;
        }
    }

    return 0;
}
