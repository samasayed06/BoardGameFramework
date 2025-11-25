/*#include "Numerical_Classes.h"
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

    if (playerID == 1) nums = {1,3,5,7,9};
    else nums = {2,4,6,8};

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
        vector<pair<int,int>> empty;

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (mat[r][c] == 0)
                    empty.push_back({r, c});

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
}*/ 










#include "Numerical_Classes.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

/* ---------------- Numerical Board ---------------- */

Numerical_Board::Numerical_Board() : Board<int>(3, 3) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) grid[i][j] = -1;
    for (int k = 0; k < 10; ++k) used[k] = false;
}

// Helper: bounds
bool Numerical_Board::in_bounds(int x, int y) const {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

// Update: Move<int> expected to carry x,y,value (value placed)
bool Numerical_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int val = move->get_symbol(); // for numerical game the "symbol" carried is the integer value

    if (!in_bounds(x, y)) return false;
    if (grid[x][y] != -1) return false;
    if (val < 0 || val > 9) return false;
    if (used[val]) return false;

    grid[x][y] = val;
    used[val] = true;
    return true;
}

// Detect if player has a winning line (sum 15) - player's "symbol" parameter is ignored here,
// we just check grid lines for sum 15 and attribute to the player who placed the numbers
bool Numerical_Board::is_win(Player<int>* player) {
    // A win occurs if any row/col/diag has sum 15
    for (int i = 0; i < 3; ++i) {
        if (grid[i][0] != -1 && grid[i][1] != -1 && grid[i][2] != -1)
            if (grid[i][0] + grid[i][1] + grid[i][2] == 15) return true;
    }
    for (int j = 0; j < 3; ++j) {
        if (grid[0][j] != -1 && grid[1][j] != -1 && grid[2][j] != -1)
            if (grid[0][j] + grid[1][j] + grid[2][j] == 15) return true;
    }
    // diags
    if (grid[0][0] != -1 && grid[1][1] != -1 && grid[2][2] != -1)
        if (grid[0][0] + grid[1][1] + grid[2][2] == 15) return true;
    if (grid[0][2] != -1 && grid[1][1] != -1 && grid[2][0] != -1)
        if (grid[0][2] + grid[1][1] + grid[2][0] == 15) return true;
    return false;
}

bool Numerical_Board::is_lose(Player<int>* player) { return false; }

bool Numerical_Board::is_draw(Player<int>* /*player*/) {
    // draw when board full and no 15
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (grid[i][j] == -1) return false;
    // full but no sum15 -> draw
    return !is_win(nullptr);
}

bool Numerical_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

// Return list of numbers that remain valid for the player (odd/even rule)
// Here we assume Player symbol passed is 1 for "odd player" and 2 for "even player" or similar.
// To be robust, we'll accept both conventions: if player symbol is odd -> return odd numbers still unused; else even numbers.
vector<int> num_vector_helper() { return vector<int>(); } // placeholder (not used)

vector<int> available_numbers_for_player_helper(const bool used_[10], bool want_odd) {
    vector<int> out;
    // usual Numerical TicTacToe uses numbers 1..9 unique. We'll provide 1..9.
    for (int v = 1; v <= 9; ++v) {
        if (!used_[v]) {
            if (want_odd && (v % 2 == 1)) out.push_back(v);
            if (!want_odd && (v % 2 == 0)) out.push_back(v);
        }
    }
    return out;
}

vector<int> Numerical_Board::available_numbers_for_player(char player_symbol) const {
    bool want_odd = true;
    // If symbol carries a parity (common frameworks use '1' or '2' as symbol) we accept '1' odd else even.
    if (player_symbol % 2 == 0) want_odd = false;
    // convert used[] to vector for helper
    bool local_used[10];
    for (int i = 0; i < 10; ++i) local_used[i] = used[i];
    vector<int> out;
    for (int v = 1; v <= 9; ++v) {
        if (!local_used[v]) {
            if (want_odd && (v % 2 == 1)) out.push_back(v);
            if (!want_odd && (v % 2 == 0)) out.push_back(v);
        }
    }
    return out;
}

// Random move for player: choose random empty cell and random available number for player's parity
pair<int, int> Numerical_Board::random_move_for_player(char player_symbol) const {
    vector<pair<int, int>> empties;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) if (grid[i][j] == -1) empties.push_back({ i,j });
    if (empties.empty()) return { -1,-1 };

    bool want_odd = (player_symbol % 2 != 0);
    vector<int> numbers;
    for (int v = 1; v <= 9; ++v) if (!used[v]) {
        if (want_odd && (v % 2 == 1)) numbers.push_back(v);
        if (!want_odd && (v % 2 == 0)) numbers.push_back(v);
    }

    if (numbers.empty()) {
        // fallback: any unused number
        for (int v = 1; v <= 9; ++v) if (!used[v]) numbers.push_back(v);
        if (numbers.empty()) return { -1,-1 };
    }

    auto cell = empties[rand() % empties.size()];
    int val = numbers[rand() % numbers.size()];
    // return encoded as (x*10 + y) in first field and value in second? NO: framework Move<int> expects x,y,value.
    // We'll return pair where .first is x*10 + y and .second = value.
    return { cell.first * 10 + cell.second, val };
}

// Smart move: try immediate win, then block opponent immediate win, else choose center/corner
pair<int, int> Numerical_Board::smart_move_for_player(char player_symbol) const {
    bool want_odd = (player_symbol % 2 != 0);
    // get available cells and numbers
    vector<pair<int, int>> empties;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) if (grid[i][j] == -1) empties.push_back({ i,j });
    if (empties.empty()) return { -1,-1 };

    vector<int> numbers;
    for (int v = 1; v <= 9; ++v) if (!used[v]) {
        if (want_odd && (v % 2 == 1)) numbers.push_back(v);
        if (!want_odd && (v % 2 == 0)) numbers.push_back(v);
    }
    if (numbers.empty()) {
        for (int v = 1; v <= 9; ++v) if (!used[v]) numbers.push_back(v);
        if (numbers.empty()) return { -1,-1 };
    }

    // 1) Try immediate win
    for (auto cell : empties) {
        int x = cell.first, y = cell.second;
        for (int val : numbers) {
            // simulate
            int tmp = grid[x][y];
            // check lines containing (x,y)
            // place val and test if any line becomes sum 15
            bool win = false;
            // row
            int a = (grid[x][0] == -1 ? ((0 == y) ? val : -1000) : grid[x][0]);
            int b = (grid[x][1] == -1 ? ((1 == y) ? val : -1000) : grid[x][1]);
            int c = (grid[x][2] == -1 ? ((2 == y) ? val : -1000) : grid[x][2]);
            if (a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) win = true;

            // col
            a = (grid[0][y] == -1 ? ((0 == x) ? val : -1000) : grid[0][y]);
            b = (grid[1][y] == -1 ? ((1 == x) ? val : -1000) : grid[1][y]);
            c = (grid[2][y] == -1 ? ((2 == x) ? val : -1000) : grid[2][y]);
            if (!win && a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) win = true;

            // diag1
            if (!win && x == y) {
                a = (grid[0][0] == -1 ? ((0 == x && 0 == y) ? val : -1000) : grid[0][0]);
                b = (grid[1][1] == -1 ? ((1 == x && 1 == y) ? val : -1000) : grid[1][1]);
                c = (grid[2][2] == -1 ? ((2 == x && 2 == y) ? val : -1000) : grid[2][2]);
                if (a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) win = true;
            }

            // diag2
            if (!win && x + y == 2) {
                a = (grid[0][2] == -1 ? ((0 == x && 2 == y) ? val : -1000) : grid[0][2]);
                b = (grid[1][1] == -1 ? ((1 == x && 1 == y) ? val : -1000) : grid[1][1]);
                c = (grid[2][0] == -1 ? ((2 == x && 0 == y) ? val : -1000) : grid[2][0]);
                if (a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) win = true;
            }

            if (win) {
                return { x * 10 + y, val };
            }
        }
    }

    // 2) Block opponent immediate win: opponent parity = !want_odd
    bool opp_want_odd = !want_odd;
    vector<int> opp_nums;
    for (int v = 1; v <= 9; ++v) if (!used[v]) {
        if (opp_want_odd && (v % 2 == 1)) opp_nums.push_back(v);
        if (!opp_want_odd && (v % 2 == 0)) opp_nums.push_back(v);
    }
    for (auto cell : empties) {
        int x = cell.first, y = cell.second;
        for (int val : opp_nums) {
            bool opp_can_win = false;
            // simulate opponent placing val at cell: same checks as above
            int a, b, c;
            a = (grid[x][0] == -1 ? ((0 == y) ? val : -1000) : grid[x][0]);
            b = (grid[x][1] == -1 ? ((1 == y) ? val : -1000) : grid[x][1]);
            c = (grid[x][2] == -1 ? ((2 == y) ? val : -1000) : grid[x][2]);
            if (a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) opp_can_win = true;
            a = (grid[0][y] == -1 ? ((0 == x) ? val : -1000) : grid[0][y]);
            b = (grid[1][y] == -1 ? ((1 == x) ? val : -1000) : grid[1][y]);
            c = (grid[2][y] == -1 ? ((2 == x) ? val : -1000) : grid[2][y]);
            if (!opp_can_win && a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) opp_can_win = true;
            if (!opp_can_win && x == y) {
                a = (grid[0][0] == -1 ? ((0 == x && 0 == y) ? val : -1000) : grid[0][0]);
                b = (grid[1][1] == -1 ? ((1 == x && 1 == y) ? val : -1000) : grid[1][1]);
                c = (grid[2][2] == -1 ? ((2 == x && 2 == y) ? val : -1000) : grid[2][2]);
                if (a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) opp_can_win = true;
            }
            if (!opp_can_win && x + y == 2) {
                a = (grid[0][2] == -1 ? ((0 == x && 2 == y) ? val : -1000) : grid[0][2]);
                b = (grid[1][1] == -1 ? ((1 == x && 1 == y) ? val : -1000) : grid[1][1]);
                c = (grid[2][0] == -1 ? ((2 == x && 0 == y) ? val : -1000) : grid[2][0]);
                if (a != -1000 && b != -1000 && c != -1000) if (a + b + c == 15) opp_can_win = true;
            }
            if (opp_can_win) {
                // choose a value ourselves to occupy the cell (use our numbers if possible)
                int useVal = (numbers.size() > 0 ? numbers[0] : ((want_odd) ? 1 : 2));
                return { x * 10 + y, useVal };
            }
        }
    }

    // 3) Otherwise, choose center if available
    for (auto cell : empties) if (cell.first == 1 && cell.second == 1) {
        int chosen = (numbers.size() > 0 ? numbers[0] : 5);
        return { 1 * 10 + 1, chosen };
    }

    // 4) Otherwise corner preference
    vector<pair<int, int>> corners;
    for (auto cell : empties) if ((cell.first == 0 && cell.second == 0) || (cell.first == 0 && cell.second == 2) || (cell.first == 2 && cell.second == 0) || (cell.first == 2 && cell.second == 2)) corners.push_back(cell);
    if (!corners.empty()) {
        int chosen = (numbers.size() > 0 ? numbers[0] : 1);
        return { corners[0].first * 10 + corners[0].second, chosen };
    }

    // 5) fallback first empty with first available number
    if (!empties.empty()) {
        int chosen = (numbers.size() > 0 ? numbers[0] : 1);
        return { empties[0].first * 10 + empties[0].second, chosen };
    }

    return { -1,-1 };
}

/* ---------------- Numerical UI ---------------- */

Player<int>* Numerical_UI::create_player(string& name, int symbol, PlayerType type) {
    // Add AI choice for computer players
    if (type == PlayerType::COMPUTER) {
        int ai;
        cout << "Choose AI for " << name << " (1 = Random, 2 = Smart): ";
        cin >> ai;
        if (ai == 2) name += " [AI:Smart]";
        else name += " [AI:Random]";
    }

    return new Player<int>(name, symbol, type);
}

// get_move: for human prompt (x y value), for computer call board->random_move_for_player or smart_move_for_player
Move<int>* Numerical_UI::get_move(Player<int>* player) {
    Board<int>* bptr = player->get_board_ptr();
    Numerical_Board* nb = dynamic_cast<Numerical_Board*>(bptr);
    if (!nb) {
        // fallback: input
        int x, y, v;
        cout << player->get_name() << " enter row col value: ";
        cin >> x >> y >> v;
        return new Move<int>(x, y, v);
    }

    if (player->get_type() == PlayerType::COMPUTER) {
        string nm = player->get_name();
        char sym = (char)player->get_symbol();
        if (nm.find("[AI:Smart]") != string::npos) {
            auto p = nb->smart_move_for_player(sym);
            int x = p.first / 10, y = p.first % 10, val = p.second;
            return new Move<int>(x, y, val);
        }
        else {
            auto p = nb->random_move_for_player(sym);
            int x = p.first / 10, y = p.first % 10, val = p.second;
            return new Move<int>(x, y, val);
        }
    }

    // Human move: ask for x y value
    int x, y, v;
    cout << player->get_name() << " enter row col value: ";
    cin >> x >> y >> v;
    return new Move<int>(x, y, v);
}

