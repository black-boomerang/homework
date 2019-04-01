/*Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ]
 [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку. Необходимо найти минимальное решение.*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

class BoardPosition // позиция доски
{
public:
    BoardPosition() : encode_pos(0), null_pos(0) { }

    BoardPosition(const vector<char> &pos)
    {
        encode_pos = 0;
        for(int i = 0; i < 16; ++i)
        {
            encode_pos = (encode_pos << 4) + static_cast<unsigned long long>(pos[i]);
            if(pos[i] == 0) {
                null_pos = i;
            }
        }
    }

    BoardPosition(unsigned long long pos, int null_p)
    {
        encode_pos = pos;
        null_pos = null_p;
    }

    bool operator==(BoardPosition &brd_pos) //сравнение двух позиций
    {
        return (encode_pos == brd_pos.encode_pos);
    }

    bool IsSolve() const // существование решения
    {
        int par = 0;
        vector<char> decode_pos;
        Decode(decode_pos);

        for(int i = 0; i < 16; ++i)
        {
            if(decode_pos[i] != 0)
            {
                for(int j = 0; j < i; ++j) {
                    par += (decode_pos[i] < decode_pos[j]);
                }
            }
            else {
                par += i / 4;
            }
        }

        return par % 2 == 1;
    }

    unsigned long long LongPos() const
    {
        return encode_pos
    }

    BoardPosition Left() const // позиция со "смещённым влево" нулём
    {
        if(null_pos % 4 != 0) {
            return BoardPosition(ChangePos(null_pos, null_pos - 1), null_pos - 1);
        }
        return BoardPosition();
    }

    BoardPosition Right() const // позиция со "смещённым вправо" нулём
    {
        if(null_pos % 4 != 3) {
            return BoardPosition(ChangePos(null_pos, null_pos + 1), null_pos + 1);
        }
        return BoardPosition();
    }

    BoardPosition Top() const // позиция со "смещённым вверх" нулём
    {
        if(null_pos >= 4) {
            return BoardPosition(ChangePos(null_pos, null_pos - 4), null_pos - 4);
        }
        return BoardPosition();
    }

    BoardPosition Bottom() const // позиция со "смещённым вниз" нулём
    {
        if(null_pos <= 11) {
            return BoardPosition(ChangePos(null_pos, null_pos + 4), null_pos + 4);
        }
        return BoardPosition();
    }

    int GetHeuristic() const  // эвристика для позиции
    {
        vector<char> decode_pos;
        Decode(decode_pos);
        int priority = 0;
        vector<char> linear_conf(16);
        int last_move = 1;

        for(int i = 0; i < 16; ++i)
        {
            int cur_cell = decode_pos[i];
            if(cur_cell == 0) {
                continue;
            }
            --cur_cell;
            priority += abs(cur_cell % 4 - i % 4)  + abs(cur_cell / 4 - i / 4); // манхэттенское расстояние

            if(cur_cell / 4 == i / 4) // линейный конфликт
            {
                for(int k = (i / 4) * 4; k < i; ++k)
                {
                    if(decode_pos[k] == 0) {
                        continue;
                    }
                    int prev_pos = decode_pos[k] - 1;
                    if(prev_pos / 4 == i / 4 && prev_pos > cur_cell) {
                        priority += 2;
                        linear_conf[k] = linear_conf[i] = 1;
                    }
                }
            }
            if(cur_cell % 4 == i % 4)
            {
                for(int k = i % 4; k < i; k += 4) {
                    if(decode_pos[k] == 0) {
                        continue;
                    }
                    int prev_pos = decode_pos[k] - 1;
                    if(prev_pos % 4 == i % 4 && prev_pos > cur_cell) {
                        priority += 2;
                        linear_conf[k] = linear_conf[i] = 1;
                    }
                }
            }

            if(cur_cell == 12 && i <= 12) {
                last_move = 0;
            }
            else if(cur_cell == 15 && i % 4 != 3) {
                last_move = 0;
            }
        }

        priority += last_move * 2; // последний ход

        if(((decode_pos[1] == 2 && !linear_conf[1]) && (decode_pos[4] == 5 && !linear_conf[4])) // угловые фишки
            && decode_pos[0] != 1) {
            priority += 2;
        }
        if(((decode_pos[2] == 3 && !linear_conf[2]) && (decode_pos[7] == 8 && !linear_conf[7]))
            && decode_pos[3] != 4) {
            priority += 2;
        }
        if(((decode_pos[8] == 9 && !linear_conf[8]) && (decode_pos[13] == 14 && !linear_conf[13]))
            && decode_pos[12] != 13) {
            priority += 2;
        }

        return priority;
    }

    void show() const // для отладки
    {
        vector<char> decode_pos;
        Decode(decode_pos);
        for(int i = 0; i < 16; ++i)
        {
            cout << (int)decode_pos[i] << " ";
            if(i % 4 == 3) {
                cout << endl;
            }
        }
        cout << endl;
    }
private:
    void Decode(vector<char> &decode_pos) const // преобразование в массив 4*4
    {
        unsigned long long pos = encode_pos;
        decode_pos.resize(16);
        for(int i = 0; i < 16; ++i)
        {
            decode_pos[15 - i] = pos & 15;
            pos >>= 4;
        }
    }

    unsigned long long ChangePos(int nil_pos, int second_pos) const // сдвинуть нулевую ячейку
    {
        unsigned long long new_pos = encode_pos;
        unsigned long long byte = 1ull << (4 * (15 - second_pos));
        for(int i = 0; i < 3; ++i) {
            byte = (byte << 1) | byte;
        }
        new_pos &= ~byte;
        byte &= encode_pos;
        if(nil_pos < second_pos) {
            byte <<= 4 * (second_pos - nil_pos);
        }
        else {
            byte >>= 4 * (nil_pos - second_pos);
        }
        new_pos |= byte;
        return new_pos;
    }

    unsigned long long encode_pos;
    int null_pos;
};

int ida_search(BoardPosition &cur_pos, int g_score, int threshold, vector<char> &ans,
               BoardPosition &solve_pos, BoardPosition &nil_pos, BoardPosition &prev)
{

    if(cur_pos == solve_pos) { // решение найдено
        return -1;
    }

    int f_score = g_score + cur_pos.GetHeuristic();
    if(f_score > threshold) {
        return f_score;
    }

    int min_score = INT_MAX;
    vector<BoardPosition> next_poses({cur_pos.Left(), cur_pos.Right(), cur_pos.Top(), cur_pos.Bottom()});
    vector<char> dirs({'R', 'L', 'D', 'U'});
    for(int i = 0; i < 4; ++i)
    {
        if(next_poses[i] == nil_pos || next_poses[i] == prev) {
            continue;
        }
        int temp = ida_search(next_poses[i], g_score + 1, threshold, ans, solve_pos, nil_pos, cur_pos);
        if(temp == -1)
        {
            ans.push_back(dirs[i]); // сохраняем ответ
            return -1;
        }
        else if(temp < min_score) {
            min_score = temp;
        }
    }

    return min_score;
}

bool solve(vector<char> &pos, vector<char> &ans) // нахождение решения или его отсутствия
{
    BoardPosition cur_pos(pos);
    BoardPosition solve_pos(vector<char>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}));
    BoardPosition nil_pos(0, 0);

    if(!cur_pos.IsSolve()) {
        return false;
    }

    int threshold = cur_pos.GetHeuristic(); // пороговое значение ida
    while(true)
    {
        int temp = ida_search(cur_pos, 0, threshold, ans, solve_pos, nil_pos, nil_pos);
        if(temp == -1) { // решение найдено
            break;
        }
        threshold = temp;
    }

    reverse(ans.begin(), ans.end());
    return true;
}

int main()
{
    vector<char> pos(16);
    for(int i = 0; i < 16; ++i)
    {
        int int_pos;
        cin >> int_pos;
        pos[i] = static_cast<char>(int_pos);
    }

    vector<char> ans;
    if(solve(pos, ans)) {
        cout << ans.size() << endl;
        for(char i : ans) {
            cout << i;
        }
    }
    else {
        cout << "-1";
    }

    return 0;
}
