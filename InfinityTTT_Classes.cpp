#include "InfinityTTT_Classes.h"
#include <iostream>
#include <limits>
#include <cstdlib>
using namespace std;

InfinityTTT_Board::InfinityTTT_Board() : Board<char>(3, 3), total_moves(0) {
    for (auto& r : board)
        for (auto& c : r)
            c = '-';
}

bool InfinityTTT_Board::in_bounds(int x, int y) const {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

bool InfinityTTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (!in_bounds(x, y)) return false;
    if (board[x][y] != '-') return false;

    board[x][y] = sym;
    move_order.push_back({ x,y });
    total_moves++;

    if (check_three_in_row(sym)) return true;

    if (total_moves % 3 == 0 && !move_order.empty()) {
        auto old = move_order.front();
        move_order.pop_front();
        board[old.first][old.second] = '-';
    }
    return true;
}

bool InfinityTTT_Board::check_three_in_row(char s) const {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
        if (board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
    }
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    return false;
}

bool InfinityTTT_Board::is_win(Player<char>* p) {
    return check_three_in_row(p->get_symbol());
}

bool InfinityTTT_Board::is_lose(Player<char>* p) {
    return false;
}

bool InfinityTTT_Board::is_draw(Player<char>* p) {
    bool empty = false;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '-') empty = true;
    return !empty && !check_three_in_row('X') && !check_three_in_row('O');
}

bool InfinityTTT_Board::game_is_over(Player<char>* p) {
    if (check_three_in_row('X') || check_three_in_row('O')) return true;

    bool empty = false;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '-') empty = true;
    return !empty;
}

pair<int, int> InfinityTTT_Board::random_move() const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '-') empty.push_back({ i,j });
    if (empty.empty()) return { -1,-1 };
    return empty[rand() % empty.size()];
}

pair<int, int> InfinityTTT_Board::smart_move(char sym) const {
    return random_move();
}

Player<char>* InfinityTTT_UI::create_player(string& name, char symbol, PlayerType type) {
    if (symbol == '?' || symbol == '\0') {
        cout << name << ", choose symbol: ";
        cin >> symbol;
    }
    return new Player<char>(name, symbol, type);
}

Move<char>* InfinityTTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::COMPUTER) {
        auto b = dynamic_cast<InfinityTTT_Board*>(p->get_board_ptr());
        auto mv = b->random_move();
        return new Move<char>(mv.first, mv.second, p->get_symbol());
    }
    int x, y;
    cout << p->get_name() << " (" << p->get_symbol() << ") move (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, p->get_symbol());
}
