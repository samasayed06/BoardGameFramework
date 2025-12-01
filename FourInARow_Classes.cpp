#include "FourInARow_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;


FourInARow_Board::FourInARow_Board() : Board<char>(6, 7) {
    // initialize board cells to blank ' '
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = ' ';
    srand(static_cast<unsigned>(time(nullptr)));
}

// returns lowest empty row index for column, or -1 if full
int FourInARow_Board::lowest_row_in_column(int col) const {
    if (col < 0 || col >= columns) return -1;
    for (int r = rows - 1; r >= 0; --r) {
        if (board[r][col] == ' ') return r;
    }
    return -1;
}

// Update: Move<char> expected to contain (row, col, symbol)
bool FourInARow_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (c < 0 || c >= columns) return false;
    int targetRow = lowest_row_in_column(c);
    if (targetRow == -1) return false; // column full

    // allow UI to pass any row; we place at the lowest available
    r = targetRow;

    if (!in_bounds(r, c)) return false;
    if (board[r][c] != ' ') return false;
    if (sym != 'X' && sym != 'O') return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

// check 4 in a direction containing (r,c): counts in both directions
bool FourInARow_Board::check_direction(int r, int c, int dr, int dc, char sym) const {
    int count = 1; // include (r,c)
    int rr = r + dr, cc = c + dc;
    while (in_bounds(rr, cc) && board[rr][cc] == sym) {
        ++count; rr += dr; cc += dc;
    }
    rr = r - dr; cc = c - dc;
    while (in_bounds(rr, cc) && board[rr][cc] == sym) {
        ++count; rr -= dr; cc -= dc;
    }
    return count >= 4;
}

// scans the board for any 4-in-a-row for symbol sym
bool FourInARow_Board::has_four_for(char sym) const {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (board[r][c] != sym) continue;
            // horizontal (0,1)
            if (check_direction(r, c, 0, 1, sym)) return true;
            // vertical (1,0)
            if (check_direction(r, c, 1, 0, sym)) return true;
            // diag down-right (1,1)
            if (check_direction(r, c, 1, 1, sym)) return true;
            // diag down-left (1,-1)
            if (check_direction(r, c, 1, -1, sym)) return true;
        }
    }
    return false;
}

bool FourInARow_Board::is_win(Player<char>* player) {
    return has_four_for(player->get_symbol());
}
bool FourInARow_Board::is_lose(Player<char>* player) {
    char opp = (player->get_symbol() == 'X') ? 'O' : 'X';
    return has_four_for(opp);
}
bool FourInARow_Board::is_draw(Player<char>* ) {
    // draw if no legal moves and no winner
    if (has_four_for('X') || has_four_for('O')) return false;
    for (int c = 0; c < columns; ++c)
        if (lowest_row_in_column(c) != -1) return false;
    return true;
}
bool FourInARow_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}


vector<int> FourInARow_Board::legal_columns() const {
    vector<int> cols;
    for (int c = 0; c < columns; ++c)
        if (lowest_row_in_column(c) != -1) cols.push_back(c);
    return cols;
}

// Random AI: pick a random legal column and return the move {row,col}
pair<int, int> FourInARow_Board::random_move(char ) const {
    auto cols = legal_columns();
    if (cols.empty()) return { -1,-1 };
    int chosen = cols[rand() % static_cast<int>(cols.size())];
    int r = lowest_row_in_column(chosen);
    return { r, chosen };
}


static const int WIN_SCORE = 1000000;
static const int LOSS_SCORE = -1000000;

int FourInARow_Board::evaluate_board(char playerSym, char oppSym) const {
    // Quick terminal checks
    if (has_four_for(playerSym)) return WIN_SCORE;
    if (has_four_for(oppSym)) return LOSS_SCORE;

    // Heuristic: count open sequences of length 2 and 3 for player and opponent
    auto score_for = [&](char sym)->int {
        int score = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c + 3 < columns; ++c) {
                int cnt = 0, blanks = 0;
                for (int k = 0; k < 4; ++k) {
                    char cur = board[r][c + k];
                    if (cur == sym) ++cnt;
                    else if (cur == ' ') ++blanks;
                }
                if (cnt > 0 && cnt + blanks == 4) {
                    if (cnt == 3) score += 100;
                    else if (cnt == 2) score += 10;
                    else score += 1;
                }
            }
        }
        for (int c = 0; c < columns; ++c) {
            for (int r = 0; r + 3 < rows; ++r) {
                int cnt = 0, blanks = 0;
                for (int k = 0; k < 4; ++k) {
                    char cur = board[r + k][c];
                    if (cur == sym) ++cnt;
                    else if (cur == ' ') ++blanks;
                }
                if (cnt > 0 && cnt + blanks == 4) {
                    if (cnt == 3) score += 120;
                    else if (cnt == 2) score += 12;
                    else score += 1;
                }
            }
        }
        // diag down-right
        for (int r = 0; r + 3 < rows; ++r) {
            for (int c = 0; c + 3 < columns; ++c) {
                int cnt = 0, blanks = 0;
                for (int k = 0; k < 4; ++k) {
                    char cur = board[r + k][c + k];
                    if (cur == sym) ++cnt;
                    else if (cur == ' ') ++blanks;
                }
                if (cnt > 0 && cnt + blanks == 4) {
                    if (cnt == 3) score += 110;
                    else if (cnt == 2) score += 11;
                    else score += 1;
                }
            }
        }
        // diag down-left
        for (int r = 0; r + 3 < rows; ++r) {
            for (int c = 3; c < columns; ++c) {
                int cnt = 0, blanks = 0;
                for (int k = 0; k < 4; ++k) {
                    char cur = board[r + k][c - k];
                    if (cur == sym) ++cnt;
                    else if (cur == ' ') ++blanks;
                }
                if (cnt > 0 && cnt + blanks == 4) {
                    if (cnt == 3) score += 110;
                    else if (cnt == 2) score += 11;
                    else score += 1;
                }
            }
        }
        return score;
        };

    int pscore = score_for(playerSym);
    int oscore = score_for(oppSym);
    return pscore - oscore;
}

int FourInARow_Board::minimax(int depth, bool maximizingPlayer, char playerSym, char oppSym, int alpha, int beta) {
    // Terminal checks
    if (has_four_for(playerSym)) return WIN_SCORE;
    if (has_four_for(oppSym)) return LOSS_SCORE;
    auto cols_now = legal_columns();
    if (cols_now.empty()) return 0; // draw
    if (depth == 0) return evaluate_board(playerSym, oppSym);

    if (maximizingPlayer) {
        int maxEval = numeric_limits<int>::min();
        auto cols = cols_now;
        // order center-first
        sort(cols.begin(), cols.end(), [](int a, int b) { return abs(3 - a) < abs(3 - b); });
        for (int col : cols) {
            int r = lowest_row_in_column(col);
            // simulate
            board[r][col] = playerSym;
            ++n_moves;
            int eval = minimax(depth - 1, false, playerSym, oppSym, alpha, beta);
            // undo
            board[r][col] = ' ';
            --n_moves;
            if (eval > maxEval) maxEval = eval;
            if (eval > alpha) alpha = eval;
            if (beta <= alpha) break; // beta cutoff
        }
        return maxEval;
    }
    else {
        int minEval = numeric_limits<int>::max();
        auto cols = cols_now;
        sort(cols.begin(), cols.end(), [](int a, int b) { return abs(3 - a) < abs(3 - b); });
        for (int col : cols) {
            int r = lowest_row_in_column(col);
            // simulate opponent move
            board[r][col] = oppSym;
            ++n_moves;
            int eval = minimax(depth - 1, true, playerSym, oppSym, alpha, beta);
            // undo
            board[r][col] = ' ';
            --n_moves;
            if (eval < minEval) minEval = eval;
            if (eval < beta) beta = eval;
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

// Smart move: create a copy, test each legal column and choose the best by minimax
pair<int, int> FourInARow_Board::smart_move(char symbol) const {
    FourInARow_Board copy = *this; // local copy for safe simulations
    char playerSym = symbol;
    char oppSym = (symbol == 'X') ? 'O' : 'X';

    auto cols = copy.legal_columns();
    if (cols.empty()) return { -1,-1 };

    sort(cols.begin(), cols.end(), [](int a, int b) { return abs(3 - a) < abs(3 - b); });

    int bestScore = numeric_limits<int>::min();
    int bestCol = cols[0];
    int DEPTH = 6; 

    for (int col : cols) {
        int r = copy.lowest_row_in_column(col);
        copy.board[r][col] = playerSym;
        ++copy.n_moves;
        int score = copy.minimax(DEPTH - 1, false, playerSym, oppSym, numeric_limits<int>::min(), numeric_limits<int>::max());
        copy.board[r][col] = ' ';
        --copy.n_moves;
        if (score > bestScore) {
            bestScore = score;
            bestCol = col;
        }
    }
    int finalRow = lowest_row_in_column(bestCol);
    return { finalRow, bestCol };
}

/* ---------------- FourInARow_UI ---------------- */

FourInARow_UI::FourInARow_UI() : UI<char>("Four-in-a-Row (6x7)", 3) {}

Player<char>* FourInARow_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        int ai;
        cout << "Choose AI for " << name << " (1 = Random, 2 = Smart/backtracking): ";
        cin >> ai;
        if (ai == 2) name += " [AI:Smart]";
        else name += " [AI:Random]";
    }
    return new Player<char>(name, symbol, type);
}

Move<char>* FourInARow_UI::get_move(Player<char>* player) {
    Board<char>* bptr = player->get_board_ptr();
    FourInARow_Board* fb = dynamic_cast<FourInARow_Board*>(bptr);
    if (!fb) {
        int r, c; cout << player->get_name() << " enter row col: "; cin >> r >> c;
        return new Move<char>(r, c, player->get_symbol());
    }

    // Computer
    if (player->get_type() == PlayerType::COMPUTER) {
        string nm = player->get_name();
        pair<int, int> mv;
        if (nm.find("[AI:Smart]") != string::npos) mv = fb->smart_move(player->get_symbol());
        else mv = fb->random_move(player->get_symbol());

        if (mv.first < 0) {
            //  pick first legal
            auto cols = fb->legal_columns();
            if (cols.empty()) return new Move<char>(0, 0, player->get_symbol());
            int col = cols[0];
            int row = fb->lowest_row_in_column(col);
            return new Move<char>(row, col, player->get_symbol());
        }
        cout << player->get_name() << " (computer) drops at column " << mv.second << "\n";
        return new Move<char>(mv.first, mv.second, player->get_symbol());
    }

    // Human: ask for a column number (0..6), compute row
    while (true) {
        int col;
        cout << player->get_name() << " enter column (0-6): ";
        cin >> col;
        int row = fb->lowest_row_in_column(col);
        if (row == -1) {
            cout << "Column full or invalid. Choose another.\n";
            continue;
        }
        return new Move<char>(row, col, player->get_symbol());
    }
}
