/*Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.
Для решения задачи используйте структуру данных Sparse Table. Требуемое время обработки каждого диапазона O(1).
Время подготовки структуры данных O(n*log(n)).*/
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

struct TwoStatistics // первые две статистики
{
    long long first_statistic;
    long long  second_statistic;
    int first_pos; // храним позицию минимального элемента, чтобы учитывать пересечения отрезков
};

class SparseTable // разреженная таблица
{
public:
    SparseTable(const vector<long long> &arr)
    {
        size_t sz = arr.size();
        int max_deg = static_cast<int>(log2(sz)) + 1;
        table.resize(max_deg, vector<TwoStatistics> (sz));

        for(size_t i = 0; i < sz; ++i)
        {
            table[0][i].first_statistic = arr[i];
            table[0][i].second_statistic = LLONG_MAX;
            table[0][i].first_pos = i;
        }

        for(int k = 1; k < max_deg; ++k) // цикл по степеням двойки
        {
            long long two_power = static_cast<long long>(pow(2, k - 1));
            for(size_t i = 0; i < sz - two_power; ++i) // заполнение отрезков длины 2^k
            {
                TwoStatistics &values_1 = table[k - 1][i];
                TwoStatistics &values_2 = table[k - 1][i + two_power];
                if(values_1.first_statistic < values_2.first_statistic)
                {
                    table[k][i].first_statistic = values_1.first_statistic;
                    table[k][i].first_pos = values_1.first_pos;
                }
                else
                {
                    table[k][i].first_statistic = values_2.first_statistic;
                    table[k][i].first_pos = values_2.first_pos;
                }
                table[k][i].second_statistic = find_second_of_values(values_1, values_2);
            }
        }
    }

    int find_second_on_interval(int left, int right) const // поиск второй статистики на отрезке
    {
        int deg = static_cast<int>(log2(right - left + 1));
        return find_second_of_values(table[deg][left], table[deg][right - static_cast<long long>(pow(2, deg)) + 1]);
    }
private:
    // поиск второй статистики из четырёх элементов
    int find_second_of_values(const TwoStatistics &values_1, const TwoStatistics &values_2) const
    {
        if(values_1.first_pos == values_2.first_pos) { // минимальный элемент лежит в пересечении
            return min(values_1.second_statistic, values_2.second_statistic);
        }
        if(values_2.first_statistic >= values_1.second_statistic) {
            return values_1.second_statistic;
        }
        if(values_1.first_statistic >= values_2.second_statistic) {
            return values_2.second_statistic;
        }

        return max(values_1.first_statistic, values_2.first_statistic);
    }

    vector<vector<TwoStatistics>> table;
};

int main()
{
    int n, m;
    cin >> n >> m;

    vector<long long> arr(n);
    for(int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    SparseTable sparse_table(arr);

    for(int i = 0; i < m; ++i)
    {
        int x, y;
        cin >> x >> y;
        cout << sparse_table.find_second_on_interval(x - 1, y - 1) << endl;
    }

    return 0;
}
