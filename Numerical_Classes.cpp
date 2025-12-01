#include "Numerical_Classes.h"
#include <iostream>
using namespace std;

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Numerical_Board::Numerical_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& c : row)
            c = blank_symbol;
}

// --------------------------------------------------
// update_board
// --------------------------------------------------
bool Numerical_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int val = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    if (board[x][y] != blank_symbol)
        return false;

    if (usedNumbers.count(val))
        return false;

    usedNumbers.insert(val);
    board[x][y] = val;
    n_moves++;

    return true;
}

// --------------------------------------------------
// check winning line = 15
// --------------------------------------------------
bool Numerical_Board::check_player_line(int pid) {
    auto owns = [&](int x) {
        if (pid == 1) return (x % 2 == 1);
        else return (x % 2 == 0 && x != 0);
        };

    // Rows
    for (int r = 0; r < 3; r++) {
        int a = board[r][0], b = board[r][1], c = board[r][2];
        if (a && b && c && a + b + c == 15 &&
            owns(a) && owns(b) && owns(c))
            return true;
    }

    // Columns
    for (int c = 0; c < 3; c++) {
        int a = board[0][c], b = board[1][c], d = board[2][c];
        if (a && b && d && a + b + d == 15 &&
            owns(a) && owns(b) && owns(d))
            return true;
    }

    // Main diagonal
    {
        int a = board[0][0], b = board[1][1], c = board[2][2];
        if (a && b && c && a + b + c == 15 &&
            owns(a) && owns(b) && owns(c))
            return true;
    }

    // Second diagonal
    {
        int a = board[0][2], b = board[1][1], c = board[2][0];
        if (a && b && c && a + b + c == 15 &&
            owns(a) && owns(b) && owns(c))
            return true;
    }

    return false;
}

// --------------------------------------------------
// win/lose/draw
// --------------------------------------------------
bool Numerical_Board::is_win(Player<int>* player) {
    int pid = (player->get_symbol() == 1 ? 1 : 2);
    return check_player_line(pid);
}

bool Numerical_Board::is_lose(Player<int>* player) {
    int pid = (player->get_symbol() == 1 ? 1 : 2);
    return check_player_line(3 - pid);
}

bool Numerical_Board::is_draw(Player<int>* player) {
    return (n_moves == 9 && !is_win(player));
}

// --------------------------------------------------
// UI Constructor
// --------------------------------------------------
Numerical_UI::Numerical_UI()
    : UI<int>("Numerical Tic Tac Toe Game", 4) {}

// --------------------------------------------------
// create player
// --------------------------------------------------
Player<int>* Numerical_UI::create_player(string& name, int symbol, PlayerType type) {
    return new Player<int>(name, symbol, type);
}

// --------------------------------------------------
// setup players
// --------------------------------------------------
Player<int>** Numerical_UI::setup_players() {
    Player<int>** p = new Player<int>*[2];

    cout << "\nPlayer 1 uses ODD numbers.\nEnter name: ";
    string n1; cin >> n1;
    cout << "1. Human\n2. Computer\nChoose: ";
    int c1; cin >> c1;
    PlayerType t1 = (c1 == 2 ? PlayerType::COMPUTER : PlayerType::HUMAN);
    p[0] = create_player(n1, 1, t1);

    cout << "\nPlayer 2 uses EVEN numbers.\nEnter name: ";
    string n2; cin >> n2;
    cout << "1. Human\n2. Computer\nChoose: ";
    int c2; cin >> c2;
    PlayerType t2 = (c2 == 2 ? PlayerType::COMPUTER : PlayerType::HUMAN);
    p[1] = create_player(n2, 2, t2);

    return p;
}

// --------------------------------------------------
// Random valid number generator
// --------------------------------------------------
int Numerical_UI::get_random_valid_number(int playerID, const set<int>& used) {
    vector<int> nums;

    if (playerID == 1) nums = { 1,3,5,7,9 };
    else nums = { 2,4,6,8 };

    vector<int> ok;
    for (int x : nums)
        if (!used.count(x)) ok.push_back(x);

    if (ok.empty()) return -1;

    return ok[rand() % ok.size()];
}

// --------------------------------------------------
// get_move
// --------------------------------------------------
Move<int>* Numerical_UI::get_move(Player<int>* player) {
    Board<int>* b = player->get_board_ptr();
    int pid = (player->get_symbol() == 1 ? 1 : 2);

    // COMPUTER PLAYER
    if (player->get_type() == PlayerType::COMPUTER) {
        cout << "\nComputer " << player->get_name() << " is thinking...\n";

        auto mat = b->get_board_matrix();
        vector<pair<int, int>> empty;

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (mat[r][c] == 0)
                    empty.push_back({ r, c });

        auto chosen = empty[rand() % empty.size()];

        const set<int>& used = ((Numerical_Board*)b)->getUsedNumbers();
        int num = get_random_valid_number(pid, used);

        return new Move<int>(chosen.first, chosen.second, num);
    }

    // HUMAN
    int r, c, num;
    cout << "\n" << player->get_name() << ", enter row col (0-2): ";
    cin >> r >> c;
    cout << "Enter number: ";
    cin >> num;

    return new Move<int>(r, c, num);
}
