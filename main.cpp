/*«Восьминашки» – упрощенный вариант известной головоломки «Пятнашки». Восемь костяшек, пронумерованных от 1 до 8,
расставлены по ячейкам игровой доски 3 на 3, одна ячейка при этом остается пустой. За один ход разрешается передвинуть
одну из костяшек, расположенных рядом с пустой ячейкой, на свободное место. Цель игры – для заданной начальной
конфигурации игровой доски за минимальное число ходов получить выигрышную конфигурацию*/
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

class BoardPosition // позиция доски
{
public:
    BoardPosition() : encode_pos(0), null_pos(0) { }

    BoardPosition(int encode_pos, int null_pos) : encode_pos(encode_pos), null_pos(null_pos) { }

    BoardPosition(const vector<char> &pos)
    {
        encode_pos = 0;
        for(int i = 0; i < 9; ++i)
        {
            encode_pos = encode_pos * 9 + pos[i];
            if(pos[i] == 0) {
                null_pos = i;
            }
        }
    }

    bool operator!=(BoardPosition &brd_pos)
    {
        return (encode_pos != brd_pos.encode_pos);
    }

    int Parity() const // чётность перестановки костяшек
    {
        int par = 0;
        vector<char> decode_pos;
        Decode(decode_pos);
        for(int i = 1; i < 9; ++i)
        {
            if(decode_pos[i] != 0)
            {
                for(int j = 0; j < i; ++j)
                {
                    par += decode_pos[i] < decode_pos[j];
                }
            }
        }
        return par % 2;
    }

    int IntPos() const
    {
        return encode_pos;
    }

    BoardPosition Left() const // позиция, полученная из текущей "сдвигом" пустой клетки влево
    {
        if(null_pos % 3 != 0) {
            return ChangePos(null_pos - 1);
        }
        return BoardPosition();
    }

    BoardPosition Right() const // позиция, полученная из текущей "сдвигом" пустой клетки вправо
    {
        if(null_pos % 3 != 2) {
            return ChangePos(null_pos + 1);
        }
        return BoardPosition();
    }

    BoardPosition Top() const // позиция, полученная из текущей "сдвигом" пустой клетки вверх
    {
        if(null_pos >= 3) {
            return ChangePos(null_pos - 3);
        }
        return BoardPosition();
    }

    BoardPosition Bottom() const // позиция, полученная из текущей "сдвигом" пустой клетки вниз
    {
        if(null_pos <= 5) {
            return ChangePos(null_pos + 3);
        }
        return BoardPosition();
    }

private:
    void Decode(vector<char> &decode_pos) const
    {
        int pos = encode_pos;
        decode_pos.resize(9);
        for(int i = 8; i >= 0; --i)
        {
            decode_pos[i] = pos % 9;
            pos /= 9;
        }
    }

    BoardPosition ChangePos(int new_null_pos) const // сдвиг нуля
    {
        vector<char> decode_pos;
        Decode(decode_pos);
        swap(decode_pos[null_pos], decode_pos[new_null_pos]);
        return BoardPosition(decode_pos);
    }

    int encode_pos;
    int null_pos;
};

bool solve(vector<char> &pos, vector<char> &ans) // поиск решения или его отсутствия
{
    BoardPosition cur_pos(pos);
    BoardPosition solve_pos(vector<char>({1, 2, 3, 4, 5, 6, 7, 8, 0}));
    BoardPosition nil_pos;

    if(cur_pos.Parity() == 1) {
        return false;
    }

    map<int, char> direction; // char, чтобы не ставить кучу ifов в последнем цикле
    map<int, BoardPosition> prev;
    queue<BoardPosition> positions;
    positions.push(cur_pos);
    direction[cur_pos.IntPos()] = 0;

    while(cur_pos != solve_pos) // пока не нашли решение
    {
        cur_pos = positions.front();
        positions.pop();

        // ищем соседние непройденные состояния доски:
        vector<BoardPosition> next_poses({cur_pos.Left(), cur_pos.Right(), cur_pos.Top(), cur_pos.Bottom()});
        vector<char> dirs({'L', 'R', 'U', 'D'});
        for(int i = 0; i < 4; ++i)
        {
            if(next_poses[i] != nil_pos && direction.find(next_poses[i].IntPos()) == direction.end())
            {
                positions.push(next_poses[i]);
                direction[next_poses[i].IntPos()] = dirs[i];
                prev[next_poses[i].IntPos()] = cur_pos;
            }

        }
    }

    while(direction[cur_pos.IntPos()] != 0) // восстанавливаем минимальное решение
    {
        ans.push_back(direction[cur_pos.IntPos()]);
        cur_pos = prev[cur_pos.IntPos()];
    }
    reverse(ans.begin(), ans.end());
    return true;
}

int main()
{
    vector<char> pos(9);
    for(int i = 0; i < 9; ++i)
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

