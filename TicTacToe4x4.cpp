#include "TicTacToe4x4.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ================= Board =================

template <typename T>
TicTacToe4x4_Board<T>::TicTacToe4x4_Board() : Board<T>(4, 4) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            this->board[i][j] = ' ';
    srand((unsigned)time(nullptr));
}

template <typename T>
bool TicTacToe4x4_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x(), y = move->get_y();
    if (x < 0 || x >= 4 || y < 0 || y >= 4) return false;
    if (this->board[x][y] != ' ') return false;
    this->board[x][y] = move->get_symbol();
    this->n_moves++;
    return true;
}

template <typename T>
bool TicTacToe4x4_Board<T>::is_win(Player<T>* player) {
    T s = player->get_symbol();

    // check rows + columns (3 in a row)
    for (int i = 0; i < 4; i++)
        for (int j = 0; j <= 1; j++) {
            if (this->board[i][j] == s && this->board[i][j + 1] == s && this->board[i][j + 2] == s)
                return true;
            if (this->board[j][i] == s && this->board[j + 1][i] == s && this->board[j + 2][i] == s)
                return true;
        }

    // diagonals
    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++) {
            if (this->board[i][j] == s && this->board[i + 1][j + 1] == s && this->board[i + 2][j + 2] == s)
                return true;
            if (this->board[i + 2][j] == s && this->board[i + 1][j + 1] == s && this->board[i][j + 2] == s)
                return true;
        }

    return false;
}

template <typename T>
bool TicTacToe4x4_Board<T>::is_lose(Player<T>* player) { return false; }

template <typename T>
bool TicTacToe4x4_Board<T>::is_draw(Player<T>* player) {
    return this->n_moves >= 16 && !is_win(player);
}

template <typename T>
bool TicTacToe4x4_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// ----------- AI HELPERS -----------

template <typename T>
pair<int, int> TicTacToe4x4_Board<T>::random_move() const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (this->board[i][j] == ' ')
                empty.push_back({ i,j });
    if (empty.empty()) return { -1,-1 };
    return empty[rand() % empty.size()];
}

// Smart AI — heuristic
template <typename T>
pair<int, int> TicTacToe4x4_Board<T>::smart_move(T sym) const {
    vector<pair<int, int>> empty;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (this->board[i][j] == ' ')
                empty.push_back({ i,j });

    if (empty.empty()) return { -1,-1 };

    // 1) Try winning move
    for (auto& p : empty) {
        int r = p.first, c = p.second;
        auto& cell = const_cast<T&>(this->board[r][c]);
        cell = sym;

        // simulate
        bool win = false;
        // check win with simple scan
        for (int i = 0; i < 4; i++)
            for (int j = 0; j <= 1; j++) {
                if (this->board[i][j] == sym && this->board[i][j + 1] == sym && this->board[i][j + 2] == sym)
                    win = true;
                if (this->board[j][i] == sym && this->board[j + 1][i] == sym && this->board[j + 2][i] == sym)
                    win = true;
            }
        for (int i = 0; i <= 1; i++)
            for (int j = 0; j <= 1; j++) {
                if (this->board[i][j] == sym && this->board[i + 1][j + 1] == sym && this->board[i + 2][j + 2] == sym)
                    win = true;
                if (this->board[i + 2][j] == sym && this->board[i + 1][j + 1] == sym && this->board[i][j + 2] == sym)
                    win = true;
            }

        cell = ' ';
        if (win) return p;
    }

    // 2) Block opponent if they could win
    T opp = (sym == 'X') ? 'O' : 'X';

    for (auto& p : empty) {
        int r = p.first, c = p.second;
        auto& cell = const_cast<T&>(this->board[r][c]);
        cell = opp;

        bool oppWin = false;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j <= 1; j++) {
                if (this->board[i][j] == opp && this->board[i][j + 1] == opp && this->board[i][j + 2] == opp)
                    oppWin = true;
                if (this->board[j][i] == opp && this->board[j + 1][i] == opp && this->board[j + 2][i] == opp)
                    oppWin = true;
            }
        for (int i = 0; i <= 1; i++)
            for (int j = 0; j <= 1; j++) {
                if (this->board[i][j] == opp && this->board[i + 1][j + 1] == opp && this->board[i + 2][j + 2] == opp)
                    oppWin = true;
                if (this->board[i + 2][j] == opp && this->board[i + 1][j + 1] == opp && this->board[i][j + 2] == opp)
                    oppWin = true;
            }

        cell = ' ';
        if (oppWin) return p;
    }

    // fallback
    return random_move();
}

// ================= UI =================

template <typename T>
TicTacToe4x4_UI<T>::TicTacToe4x4_UI() : UI<T>(3) {}

// Add Smart AI selection
template <typename T>
Player<T>* TicTacToe4x4_UI<T>::create_player(std::string& name, T symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        int ai;
        cout << "Choose AI for " << name << " (1 = Random, 2 = Smart): ";
        cin >> ai;
        if (ai == 2) name += " [AI:Smart]";
        else name += " [AI:Random]";
    }
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* TicTacToe4x4_UI<T>::get_move(Player<T>* player) {
    Board<T>* b = player->get_board_ptr();

    if (player->get_type() == PlayerType::COMPUTER) {

        pair<int, int> mv;
        if (player->get_name().find("[AI:Smart]") != string::npos)
            mv = static_cast<TicTacToe4x4_Board<T>*>(b)->smart_move(player->get_symbol());
        else
            mv = static_cast<TicTacToe4x4_Board<T>*>(b)->random_move();

        cout << player->get_name() << " plays: " << mv.first << " " << mv.second << endl;
        return new Move<T>(mv.first, mv.second, player->get_symbol());
    }

    // human
    int r, c;
    cout << player->get_name() << " enter row col (0-3): ";
    cin >> r >> c;
    return new Move<T>(r, c, player->get_symbol());
}

// explicit instantiation
template class TicTacToe4x4_Board<char>;
template class TicTacToe4x4_UI<char>;
