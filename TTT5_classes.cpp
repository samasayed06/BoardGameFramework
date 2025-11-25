#include "TTT5_Classes.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// ================== TTT5 Board ======================

TTT5_Board::TTT5_Board() : Board<char>(5, 5) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            board[i][j] = '-';   // empty cell
    n_moves = 0;
    srand((unsigned)time(nullptr));
}

bool TTT5_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= 5 || y < 0 || y >= 5) {
        cout << "Invalid position.\n";
        return false;
    }

    if (board[x][y] != '-') {
        cout << "Cell is not empty!\n";
        return false;
    }

    board[x][y] = sym;
    n_moves++;
    return true;
}

// Count how many 3-in-a-row sequences for symbol
int TTT5_Board::count_sequences(char sym) const {
    int count = 0;

    // rows
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == sym && board[i][j + 1] == sym && board[i][j + 2] == sym)
                count++;

    // columns
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == sym && board[i + 1][j] == sym && board[i + 2][j] == sym)
                count++;

    // diagonal "\"
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == sym && board[i + 1][j + 1] == sym && board[i + 2][j + 2] == sym)
                count++;

    // diagonal "/"
    for (int i = 0; i < 3; i++)
        for (int j = 2; j < 5; j++)
            if (board[i][j] == sym && board[i + 1][j - 1] == sym && board[i + 2][j - 2] == sym)
                count++;

    return count;
}

bool TTT5_Board::is_win(Player<char>* player) {
    if (n_moves < 24)
        return false; // Game not finished yet

    char sym = player->get_symbol();
    char other = (sym == 'X') ? 'O' : 'X';

    int playerSeq = count_sequences(sym);
    int otherSeq = count_sequences(other);

    return playerSeq > otherSeq;
}

bool TTT5_Board::is_lose(Player<char>* player) {
    return false; // no losing condition
}

bool TTT5_Board::is_draw(Player<char>* player) {
    if (n_moves < 24)
        return false;

    char sym = player->get_symbol();
    char other = (sym == 'X') ? 'O' : 'X';

    return count_sequences(sym) == count_sequences(other);
}

bool TTT5_Board::game_is_over(Player<char>* player) {
    return n_moves >= 24;
}

// ---------------- AI helpers ----------------

// random empty cell
pair<int, int> TTT5_Board::random_move() const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-') empty.push_back({ i,j });
    if (empty.empty()) return { -1,-1 };
    return empty[rand() % empty.size()];
}

// heuristic smart: 1) choose move that creates the most new 3-in-row sequences
// 2) else choose random
pair<int, int> TTT5_Board::smart_move(char symbol) const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == '-') empty.push_back({ i,j });
    if (empty.empty()) return { -1,-1 };

    int bestGain = -1;
    pair<int, int> best = empty[0];

    for (auto& p : empty) {
        int x = p.first, y = p.second;
        // simulate placing symbol temporarily
        const_cast<char&>(board[x][y]) = symbol;
        int newSeq = count_sequences(symbol);
        const_cast<char&>(board[x][y]) = '-';

        // compute how many sequences would be gained compared to current
        int gain = newSeq - count_sequences(symbol);
        if (gain > bestGain) {
            bestGain = gain;
            best = p;
        }
    }

    if (bestGain > 0) return best;
    // else random fallback
    return random_move();
}

// ================== TTT5 UI ======================

TTT5_UI::TTT5_UI() : UI<char>(3) {
    cout << "\n--- Welcome to 5x5 Tic Tac Toe ---\n";
    cout << "Goal: At the end of the game (24 moves), whoever has more 3-in-a-row wins!\n";
}

Player<char>* TTT5_UI::create_player(string& name, char symbol, PlayerType type) {
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

Move<char>* TTT5_UI::get_move(Player<char>* player) {
    int x, y;

    // computer
    if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* bptr = player->get_board_ptr();
        TTT5_Board* tb = dynamic_cast<TTT5_Board*>(bptr);
        pair<int, int> mv = { -1,-1 };

        if (tb) {
            string nm = player->get_name();
            if (nm.find("[AI:Smart]") != string::npos) {
                mv = tb->smart_move(player->get_symbol());
            }
            else {
                mv = tb->random_move();
            }
        }
        else {
            // fallback random
            do {
                x = rand() % player->get_board_ptr()->get_rows();
                y = rand() % player->get_board_ptr()->get_columns();
            } while (player->get_board_ptr()->get_cell(x, y) != '-');
            return new Move<char>(x, y, player->get_symbol());
        }

        if (mv.first == -1) mv = tb ? tb->random_move() : make_pair(0, 0);
        cout << "\n" << player->get_name() << " (Computer) played: (" << mv.first << "," << mv.second << ")\n";
        return new Move<char>(mv.first, mv.second, player->get_symbol());
    }

    // human
    cout << player->get_name() << " (" << player->get_symbol() << ") enter (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}
