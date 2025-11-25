#include "InfinityTTT_Classes.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <limits>

using namespace std;

/* ---------------- Board implementation ---------------- */

InfinityTTT_Board::InfinityTTT_Board() : Board<char>(3, 3), total_moves(0) {
    for (auto& r : board)
        for (auto& c : r)
            c = '-';
    move_order.clear();
}

bool InfinityTTT_Board::in_bounds(int x, int y) const {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

bool InfinityTTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (!in_bounds(x, y)) return false;
    if (board[x][y] != '-') return false; // occupied

    // place
    board[x][y] = sym;
    move_order.push_back({ x,y });
    total_moves++;

    // immediate win check (winner should be detected before any removal)
    if (check_three_in_row(sym)) {
        return true;
    }

    // If not winning, apply removal rule after every 3 moves
    if (total_moves % 3 == 0 && !move_order.empty()) {
        auto p = move_order.front();
        move_order.pop_front();
        board[p.first][p.second] = '-';
    }

    return true;
}

bool InfinityTTT_Board::check_three_in_row(char sym) const {
    // rows and cols
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym) return true;
        if (board[0][i] == sym && board[1][i] == sym && board[2][i] == sym) return true;
    }
    // diags
    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) return true;
    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym) return true;
    return false;
}

bool InfinityTTT_Board::is_win(Player<char>* player) {
    if (!player) return false;
    return check_three_in_row(player->get_symbol());
}

bool InfinityTTT_Board::is_lose(Player<char>* player) {
    return false; // not used
}

bool InfinityTTT_Board::is_draw(Player<char>* /*player*/) {
    // draw if no empty cells and no winner
    bool anyEmpty = false;
    for (auto& r : board)
        for (auto& c : r)
            if (c == '-') anyEmpty = true;
    if (anyEmpty) return false;
    return !check_three_in_row('X') && !check_three_in_row('O');
}

bool InfinityTTT_Board::game_is_over(Player<char>* /*player*/) {
    if (check_three_in_row('X') || check_three_in_row('O')) return true;
    bool anyEmpty = false;
    for (auto& r : board)
        for (auto& c : r)
            if (c == '-') anyEmpty = true;
    return !anyEmpty;
}

/* ---------------- AI helpers ---------------- */

pair<int, int> InfinityTTT_Board::random_move() const {
    vector<pair<int, int>> empties;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == '-') empties.push_back({ i,j });
    if (empties.empty()) return { -1,-1 };
    int idx = rand() % empties.size();
    return empties[idx];
}

pair<int, int> InfinityTTT_Board::smart_move(char symbol) const {
    // collect empties
    vector<pair<int, int>> empties;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == '-') empties.push_back({ i,j });
    if (empties.empty()) return { -1,-1 };

    // prefer immediate winning move
    for (auto& m : empties) {
        char sim[3][3];
        for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) sim[i][j] = board[i][j];
        deque<pair<int, int>> sim_order = move_order;
        int sim_total = total_moves;

        sim[m.first][m.second] = symbol;
        sim_order.push_back(m);
        sim_total++;

        auto check_win_sim = [&](char s)->bool {
            for (int i = 0; i < 3; ++i) {
                if (sim[i][0] == s && sim[i][1] == s && sim[i][2] == s) return true;
                if (sim[0][i] == s && sim[1][i] == s && sim[2][i] == s) return true;
            }
            if (sim[0][0] == s && sim[1][1] == s && sim[2][2] == s) return true;
            if (sim[0][2] == s && sim[1][1] == s && sim[2][0] == s) return true;
            return false;
            };
        if (check_win_sim(symbol)) return m;
    }

    // otherwise use minimax (depth-limited)
    int bestScore = numeric_limits<int>::min();
    pair<int, int> bestMove = empties[0];

    for (auto& m : empties) {
        char sim_board[3][3];
        for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) sim_board[i][j] = board[i][j];
        deque<pair<int, int>> sim_order = move_order;
        int sim_total = total_moves;

        sim_board[m.first][m.second] = symbol;
        sim_order.push_back(m);
        sim_total++;

        // apply removal if needed
        if (sim_total % 3 == 0 && !sim_order.empty()) {
            auto old = sim_order.front(); sim_order.pop_front();
            sim_board[old.first][old.second] = '-';
        }

        char opponent = (symbol == 'X') ? 'O' : 'X';
        int score = minimax_sim(sim_board, sim_order, sim_total, symbol, opponent, 4); // depth limit 4

        if (score > bestScore) {
            bestScore = score; bestMove = m;
        }
    }
    return bestMove;
}

int InfinityTTT_Board::minimax_sim(char sim_board[3][3], deque<pair<int, int>> sim_order, int sim_total_moves,
    char maximizer, char current_player, int depth) const {
    auto check_win_sim = [&](char s)->bool {
        for (int i = 0; i < 3; ++i) {
            if (sim_board[i][0] == s && sim_board[i][1] == s && sim_board[i][2] == s) return true;
            if (sim_board[0][i] == s && sim_board[1][i] == s && sim_board[2][i] == s) return true;
        }
        if (sim_board[0][0] == s && sim_board[1][1] == s && sim_board[2][2] == s) return true;
        if (sim_board[0][2] == s && sim_board[1][1] == s && sim_board[2][0] == s) return true;
        return false;
        };

    if (check_win_sim(maximizer)) return 1000;
    char opponent = (maximizer == 'X') ? 'O' : 'X';
    if (check_win_sim(opponent)) return -1000;

    vector<pair<int, int>> empties;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) if (sim_board[i][j] == '-') empties.push_back({ i,j });
    if (empties.empty() || depth == 0) return 0;

    bool isMax = (current_player == maximizer);
    int best = isMax ? numeric_limits<int>::min() : numeric_limits<int>::max();

    for (auto& m : empties) {
        sim_board[m.first][m.second] = current_player;
        sim_order.push_back(m);
        sim_total_moves++;

        if (check_win_sim(current_player)) {
            int val = (current_player == maximizer) ? 1000 : -1000;
            sim_order.pop_back();
            sim_board[m.first][m.second] = '-';
            sim_total_moves--;
            if (isMax) best = max(best, val); else best = min(best, val);
            if ((isMax && best == 1000) || (!isMax && best == -1000)) return best;
            continue;
        }

        pair<int, int> removed_pair = { -1,-1 };
        char removed_char = 0;
        if (sim_total_moves % 3 == 0 && !sim_order.empty()) {
            removed_pair = sim_order.front();
            sim_order.pop_front();
            removed_char = sim_board[removed_pair.first][removed_pair.second];
            sim_board[removed_pair.first][removed_pair.second] = '-';
        }

        int val = minimax_sim(sim_board, sim_order, sim_total_moves, maximizer, (current_player == 'X' ? 'O' : 'X'), depth - 1);

        if (removed_pair.first != -1) {
            sim_board[removed_pair.first][removed_pair.second] = removed_char;
            sim_order.push_front(removed_pair);
        }

        sim_board[m.first][m.second] = '-';
        sim_order.pop_back();
        sim_total_moves--;

        if (isMax) best = max(best, val); else best = min(best, val);
    }

    return best;
}

/* ---------------- UI ---------------- */

Player<char>* InfinityTTT_UI::create_player(string& name, char symbol, PlayerType type) {
    char sym = symbol;
    if (sym == '?' || sym == '\0') {
        cout << name << ", choose your symbol (single char, e.g. X or O): ";
        cin >> sym;
        if (sym == '\0') sym = 'X';
    }

    if (type == PlayerType::COMPUTER) {
        int choice = 1;
        cout << "Choose AI for " << name << " (1 = Random, 2 = Smart): ";
        cin >> choice;
        if (choice == 2) name += " [AI:Smart]";
        else name += " [AI:Random]";
    }

    return new Player<char>(name, sym, type);
}

Move<char>* InfinityTTT_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::COMPUTER) {
        string nm = player->get_name();
        Board<char>* bptr = player->get_board_ptr();
        InfinityTTT_Board* ib = dynamic_cast<InfinityTTT_Board*>(bptr);
        if (!ib) {
            // fallback: search base board if possible
            Board<char>* base = bptr;
            try {
                for (int i = 0; i < (int)base->get_rows(); ++i) {
                    for (int j = 0; j < (int)base->get_columns(); ++j) {
                        if (base->get_cell(i, j) == '-') return new Move<char>(i, j, player->get_symbol());
                    }
                }
            }
            catch (...) {
                return new Move<char>(0, 0, player->get_symbol());
            }
            return new Move<char>(0, 0, player->get_symbol());
        }

        if (nm.find("[AI:Smart]") != string::npos) {
            auto p = ib->smart_move(player->get_symbol());
            return new Move<char>(p.first, p.second, player->get_symbol());
        }
        else {
            auto p = ib->random_move();
            return new Move<char>(p.first, p.second, player->get_symbol());
        }
    }

    cout << player->get_name() << " (" << player->get_symbol() << ") enter (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}
