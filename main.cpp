// Реализуйте структуру данных “массив строк” на основе декартового дерева по неявному ключу.
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Node // элемент дерева
{
    string value;
    int sz; // кол-во вершин в поддереве
    int priority;
    Node *left;
    Node *right;
};

class Treap // декартово дерево по неявному ключу
{
public:
    Treap() : root(nullptr) { }

    ~Treap()
    {
        Del_Treap(root);
    }

    void InsertAt(int position, const string &value) // вставка нового значения
    {
        Node *new_node = new Node;
        new_node->value = value;
        new_node->priority = rand();
        new_node->sz = 1;
        new_node->left = new_node->right = nullptr;

        Node *left_treap = nullptr, *right_treap = nullptr;
        Split(root, position, left_treap, right_treap); // разрезаем дерево по ключу pos
        Merge(root, left_treap, new_node); // сливаем левое дерево с новым элементом
        Merge(root, root, right_treap); // сливаем с правым деревом
    }

    void DeleteAt(int position) // удаление элемента по индексу
    {
        Node *left_treap = nullptr, *right_treap = nullptr, *node = nullptr;
        Split(root, position, left_treap, right_treap);
        Split(right_treap, 1, node, right_treap);
        Merge(root, left_treap, right_treap);
        delete node;
    }

    string GetAt(int position) const // получение элемента по индексу
    {
        Node *node = SearchAt(position);
        return node->value;
    }
private:
    Node *SearchAt(int position) const // поиск элемента
    {
        int cur_position = position;
        Node *cur_node = root;
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

    int GetTreapSz(const Node *treap) const // размевар поддере
    {
        return treap ? treap->sz : 0;
    }

    void ChangeSz(Node *treap) // изменение размера дерева
    {
        if(!treap) {
            return;
        }
        treap->sz = GetTreapSz(treap->left) + GetTreapSz(treap->right) + 1;
    }

    void Merge(Node *&treap, Node *left_treap, Node *right_treap)
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

    void Split(Node *treap, int key, Node *&left_treap, Node *&right_treap)
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

    void Del_Treap(Node *node) // удаление дерева
    {
        if(node == nullptr) {
            return;
        }

        Del_Treap(node->left); // удаление детей
        Del_Treap(node->right);
        delete node;
    }

    Node *root;
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
