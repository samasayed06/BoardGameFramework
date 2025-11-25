#include "Obstacles_Classes.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// ================== Obstacles_Board ======================

Obstacles_Board::Obstacles_Board() : Board<char>(6, 6) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = '-';
    n_moves = 0;
    // seed random once (if not seeded by main)
    srand((unsigned)time(nullptr));
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
    vector<pair<int, int>> empty_cells;

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-')
                empty_cells.push_back({ i, j });

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

// -------------------- AI helpers --------------------

// pick random empty cell
pair<int, int> Obstacles_Board::random_move() const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-')
                empty.push_back({ i,j });
    if (empty.empty()) return { -1,-1 };
    return empty[rand() % empty.size()];
}

// simple heuristic: 1) immediate win, 2) block opponent immediate win, 3) random
pair<int, int> Obstacles_Board::smart_move(char symbol) const {
    char opp = (symbol == 'X') ? 'O' : 'X';

    // 1) immediate win
    for (int i = 0; i < rows; ++i) for (int j = 0; j < columns; ++j) {
        if (board[i][j] != '-') continue;
        // simulate
        const_cast<char&>(board[i][j]) = symbol; // temporarily modify (safe because we restore)
        bool win = has_four_in_row(symbol);
        const_cast<char&>(board[i][j]) = '-';
        if (win) return { i,j };
    }

    // 2) block opponent's immediate win
    for (int i = 0; i < rows; ++i) for (int j = 0; j < columns; ++j) {
        if (board[i][j] != '-') continue;
        const_cast<char&>(board[i][j]) = opp;
        bool oppWin = has_four_in_row(opp);
        const_cast<char&>(board[i][j]) = '-';
        if (oppWin) return { i,j };
    }

    // 3) fallback random
    return random_move();
}

// ================== Obstacles_UI ======================

Obstacles_UI::Obstacles_UI() : UI<char>(3) {
    cout << "\n--- Obstacles Tic-Tac-Toe (6x6) ---\n";
    cout << "Obstacles '#' appear randomly after each round.\n";
    cout << "First to align 4 wins.\n\n";
}

Player<char>* Obstacles_UI::create_player(string& name, char symbol, PlayerType type) {
    // Force symbol input if not given by framework
    if (symbol == '?' || symbol == '\0') {
        cout << name << ", choose your symbol (X/O): ";
        cin >> symbol;
    }

    if (type == PlayerType::COMPUTER) {
        int ai;
        cout << "Choose AI for " << name << " (1=Random, 2=Smart): ";
        cin >> ai;
        if (ai == 2) name += " [AI:Smart]";
        else name += " [AI:Random]";
    }

    return new Player<char>(name, symbol, type);
}

Move<char>* Obstacles_UI::get_move(Player<char>* player) {
    int x, y;

    // لو اللاعب كمبيوتر
    if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* bptr = player->get_board_ptr();
        Obstacles_Board* ob = dynamic_cast<Obstacles_Board*>(bptr);
        pair<int, int> mv = { -1,-1 };

        if (ob) {
            string nm = player->get_name();
            if (nm.find("[AI:Smart]") != string::npos) {
                mv = ob->smart_move(player->get_symbol());
            }
            else {
                mv = ob->random_move();
            }
        }
        else {
            // fallback: naive random
            do {
                x = rand() % player->get_board_ptr()->get_rows();
                y = rand() % player->get_board_ptr()->get_columns();
            } while (player->get_board_ptr()->get_cell(x, y) != '-');
            return new Move<char>(x, y, player->get_symbol());
        }

        if (mv.first == -1) {
            // no move found (shouldn't normally happen) -> fallback random
            mv = ob ? ob->random_move() : make_pair(0, 0);
        }

        cout << "\n" << player->get_name()
            << " (Computer) played: (" << mv.first << "," << mv.second << ")\n";

        return new Move<char>(mv.first, mv.second, player->get_symbol());
    }

    // لو اللاعب إنسان
    cout << player->get_name() << " (" << player->get_symbol()
        << ") enter move (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}

