#include "Obstacles_Classes.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

// ================== Obstacles_Board ======================

Obstacles_Board::Obstacles_Board() : Board<char>(6, 6) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = '-';
    n_moves = 0;
}

bool Obstacles_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        cout << "Invalid position.\n";
        return false;
    }
    if (board[x][y] == '#') {
        cout << "This cell is blocked by an obstacle (#).\n";
        return false;
    }
    if (board[x][y] != '-') {
        cout << "Cell is not empty.\n";
        return false;
    }

    board[x][y] = sym;
    n_moves++;

    // حساب عدد الخلايا الفارغة
    int empty_count = 0;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-')
                empty_count++;

    // بعد كل جولتين، نضيف عائقين بشرط وجود أماكن كافية
    if (n_moves % 2 == 0 && empty_count > 2)
        add_random_obstacles(2);

    return true;
}

void Obstacles_Board::add_random_obstacles(int count) {
    vector<pair<int,int>> empty_cells;

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-')
                empty_cells.push_back({i, j});

    if (empty_cells.empty())
        return;

    for (int k = 0; k < count && !empty_cells.empty(); ++k) {
        int idx = rand() % empty_cells.size();
        int x = empty_cells[idx].first;
        int y = empty_cells[idx].second;
        board[x][y] = '#';
        empty_cells.erase(empty_cells.begin() + idx);
    }
}

// فحص 4 في صف
bool Obstacles_Board::has_four_in_row(char sym) const {

    // horizontal
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j <= columns - 4; ++j) {
            if (board[i][j] == sym &&
                board[i][j + 1] == sym &&
                board[i][j + 2] == sym &&
                board[i][j + 3] == sym)
                return true;
        }
    }

    // vertical
    for (int i = 0; i <= rows - 4; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == sym &&
                board[i + 1][j] == sym &&
                board[i + 2][j] == sym &&
                board[i + 3][j] == sym)
                return true;
        }
    }

    // diagonal "\"
    for (int i = 0; i <= rows - 4; ++i) {
        for (int j = 0; j <= columns - 4; ++j) {
            if (board[i][j] == sym &&
                board[i + 1][j + 1] == sym &&
                board[i + 2][j + 2] == sym &&
                board[i + 3][j + 3] == sym)
                return true;
        }
    }

    // diagonal "/"
    for (int i = 0; i <= rows - 4; ++i) {
        for (int j = 3; j < columns; ++j) {
            if (board[i][j] == sym &&
                board[i + 1][j - 1] == sym &&
                board[i + 2][j - 2] == sym &&
                board[i + 3][j - 3] == sym)
                return true;
        }
    }

    return false;
}

bool Obstacles_Board::is_win(Player<char>* player) {
    return has_four_in_row(player->get_symbol());
}

bool Obstacles_Board::is_lose(Player<char>* player) {
    return false;
}

bool Obstacles_Board::is_draw(Player<char>* player) {
    bool full = true;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-') {
                full = false;
                break;
            }

    if (!full) return false;
    return !(has_four_in_row('X') || has_four_in_row('O'));
}

bool Obstacles_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ================== Obstacles_UI ======================

Obstacles_UI::Obstacles_UI() : UI<char>(3) {
    cout << "\n--- Obstacles Tic-Tac-Toe (6x6) ---\n";
    cout << "Obstacles '#' appear randomly after each round.\n";
    cout << "First to align 4 wins.\n\n";
}

Move<char>* Obstacles_UI::get_move(Player<char>* player) {
    int x, y;

    // لو اللاعب كمبيوتر
    if (player->get_type() == PlayerType::COMPUTER) {
        do {
            x = rand() % player->get_board_ptr()->get_rows();
            y = rand() % player->get_board_ptr()->get_columns();
        }
        while (player->get_board_ptr()->get_cell(x, y) != '-'); 

        cout << "\n" << player->get_name()
             << " (Computer) played: (" << x << "," << y << ")\n";

        return new Move<char>(x, y, player->get_symbol());
    }

    // لو اللاعب إنسان
    cout << player->get_name() << " (" << player->get_symbol()
         << ") enter move (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}
