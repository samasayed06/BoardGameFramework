#include "misere_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
using namespace std;

/* ---------------- Misere_Board ---------------- */
Misere_Board::Misere_Board() : Board<char>(3, 3) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
    n_moves = 0;
}

Misere_Board::~Misere_Board() {}

bool Misere_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    if (r < 0 || r >= 3 || c < 0 || c >= 3 || board[r][c] != ' ')
        return false;
    board[r][c] = move->get_symbol();
    n_moves++;
    return true;
}

bool Misere_Board::has_three_in_row(char mark) {
    for (int i = 0; i < 3; i++)
        if (board[i][0] == mark && board[i][1] == mark && board[i][2] == mark)
            return true;

    for (int j = 0; j < 3; j++)
        if (board[0][j] == mark && board[1][j] == mark && board[2][j] == mark)
            return true;

    if (board[0][0] == mark && board[1][1] == mark && board[2][2] == mark)
        return true;

    if (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark)
        return true;

    return false;
}

bool Misere_Board::is_win(Player<char>* p) {
    char mark = p->get_symbol();
    char opponent = (mark == 'X') ? 'O' : 'X';
    return has_three_in_row(opponent);
}

bool Misere_Board::is_lose(Player<char>* p) {
    char mark = p->get_symbol();
    return has_three_in_row(mark);
}

bool Misere_Board::is_draw(Player<char>* p) {
    return n_moves >= 9 && !has_three_in_row('X') && !has_three_in_row('O');
}

bool Misere_Board::game_is_over(Player<char>* p) {
    return is_lose(p) || is_win(p) || is_draw(p);
}

/* ---------------- Misere_Human ---------------- */
Misere_Human::Misere_Human(string name, char symbol) : Player<char>(name, symbol, PlayerType::HUMAN) {}
Move<char>* Misere_Human::get_move(Player<char>*) {
    int r, c;
    cout << "Enter row and col for " << symbol << ": ";
    cin >> r >> c;
    return new Move<char>(r, c, symbol);
}

/* ---------------- Misere_Computer ---------------- */
Misere_Computer::Misere_Computer(string name, char symbol) : Player<char>(name, symbol, PlayerType::COMPUTER) {
    // seed once if needed
    srand((unsigned)time(nullptr));
}

static bool board_has_three_in_row_on_matrix(const vector<vector<char>>& mat, char mark) {
    for (int i = 0; i < 3; i++)
        if (mat[i][0] == mark && mat[i][1] == mark && mat[i][2] == mark) return true;
    for (int j = 0; j < 3; j++)
        if (mat[0][j] == mark && mat[1][j] == mark && mat[2][j] == mark) return true;
    if (mat[0][0] == mark && mat[1][1] == mark && mat[2][2] == mark) return true;
    if (mat[0][2] == mark && mat[1][1] == mark && mat[2][0] == mark) return true;
    return false;
}

Move<char>* Misere_Computer::get_move(Player<char>*) {
    // If name contains "[AI:Smart]" use heuristic strategy
    string nm = get_name();
    bool useSmart = (nm.find("[AI:Smart]") != string::npos);

    // Access board matrix via boardPtr if available
    vector<vector<char>> mat;
    if (boardPtr) {
        try {
            mat = boardPtr->get_board_matrix();
        }
        catch (...) {
            // fallback: create empty 3x3
            mat = vector<vector<char>>(3, vector<char>(3, ' '));
        }
    }
    else {
        // no board pointer -> randomized fallback
        int r, c;
        do {
            r = rand() % 3; c = rand() % 3;
        } while (false);
        return new Move<char>(r, c, symbol);
    }

    // collect empty cells
    vector<pair<int, int>> empties;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (mat[i][j] == ' ')
                empties.push_back({ i,j });

    if (empties.empty()) return new Move<char>(0, 0, symbol); // shouldn't happen

    if (!useSmart) {
        // original random behavior (choose until empty)
        int idx = rand() % empties.size();
        auto p = empties[idx];
        return new Move<char>(p.first, p.second, symbol);
    }

    // SMART HEURISTIC for Misère:
    // Prefer any move that does NOT create three-in-row for self (i.e., doesn't make you lose immediately).
    // If multiple safe moves, pick one at random.
    // If no safe moves, fallback to random move (must lose next anyway).

    vector<pair<int, int>> safe;
    for (auto& p : empties) {
        int r = p.first, c = p.second;
        // simulate placing our symbol
        mat[r][c] = symbol;
        bool wouldLose = board_has_three_in_row_on_matrix(mat, symbol);
        // restore
        mat[r][c] = ' ';
        if (!wouldLose) safe.push_back(p);
    }

    if (!safe.empty()) {
        int idx = rand() % safe.size();
        auto p = safe[idx];
        return new Move<char>(p.first, p.second, symbol);
    }

    // no safe moves -> pick random (original fallback)
    int idx = rand() % empties.size();
    auto p = empties[idx];
    return new Move<char>(p.first, p.second, symbol);
}

/* ---------------- Misere_UI ---------------- */
Misere_UI::Misere_UI() : UI("Misère Tic Tac Toe (Inverse XO)", 1) {}

Move<char>* Misere_UI::get_move(Player<char>* p) {
    if (auto human = dynamic_cast<Misere_Human*>(p))
        return human->get_move(nullptr);
    else if (auto comp = dynamic_cast<Misere_Computer*>(p))
        return comp->get_move(nullptr);
    else {
        cout << "Unknown player type!" << endl;
        return nullptr;
    }
}

Player<char>* Misere_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::HUMAN) return new Misere_Human(name, symbol);
    else {
        // For COMPUTER, ask which AI and append name
        int ai_choice = 1;
        cout << "Choose AI for " << name << " (1 = Random, 2 = Smart): ";
        cin >> ai_choice;
        if (ai_choice == 2) name += " [AI:Smart]";
        else name += " [AI:Random]";
        return new Misere_Computer(name, symbol);
    }
}
