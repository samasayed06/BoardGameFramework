#include "MemoryTTT_Classes.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

// ================== MemoryTTT_Board ======================

MemoryTTT_Board::MemoryTTT_Board() : Board<char>(3, 3) {
    // تهيئة اللوحة الظاهرة (كلها فارغة)
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = '-';
    
    // تهيئة اللوحة المخفية (تحتوي على العلامات الحقيقية)
    hidden_board.resize(rows, vector<char>(columns, '-'));
    n_moves = 0;
}

bool MemoryTTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        cout << "Invalid position.\n";
        return false;
    }
    
    if (hidden_board[x][y] != '-') {
        cout << "Cell already occupied!\n";
        return false;
    }

    // وضع العلامة في اللوحة المخفية
    hidden_board[x][y] = sym;
    n_moves++;

    // اللوحة الظاهرة تبقى فارغة (المفهوم الأساسي للعبة)
    // board[x][y] = '-'; // already '-'

    cout << "\n*** Mark placed at (" << x << "," << y << ") and immediately hidden! ***\n";
    
    return true;
}

bool MemoryTTT_Board::check_three_in_row(char sym) const {
    // فحص الصفوف
    for (int i = 0; i < rows; ++i) {
        if (hidden_board[i][0] == sym && 
            hidden_board[i][1] == sym && 
            hidden_board[i][2] == sym)
            return true;
    }

    // فحص الأعمدة
    for (int j = 0; j < columns; ++j) {
        if (hidden_board[0][j] == sym && 
            hidden_board[1][j] == sym && 
            hidden_board[2][j] == sym)
            return true;
    }

    // فحص القطر الرئيسي
    if (hidden_board[0][0] == sym && 
        hidden_board[1][1] == sym && 
        hidden_board[2][2] == sym)
        return true;

    // فحص القطر الثانوي
    if (hidden_board[0][2] == sym && 
        hidden_board[1][1] == sym && 
        hidden_board[2][0] == sym)
        return true;

    return false;
}

bool MemoryTTT_Board::is_win(Player<char>* player) {
    return check_three_in_row(player->get_symbol());
}

bool MemoryTTT_Board::is_lose(Player<char>* player) {
    return false;
}

bool MemoryTTT_Board::is_draw(Player<char>* player) {
    // تحقق من امتلاء اللوحة المخفية
    if (n_moves < 9) return false;
    
    // اللوحة ممتلئة، تحقق من عدم وجود فائز
    return !(check_three_in_row('X') || check_three_in_row('O'));
}

bool MemoryTTT_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// -------------------- AI helpers --------------------

pair<int, int> MemoryTTT_Board::random_move() const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (hidden_board[i][j] == '-')
                empty.push_back({i, j});
    
    if (empty.empty()) return {-1, -1};
    return empty[rand() % empty.size()];
}

pair<int, int> MemoryTTT_Board::smart_move(char symbol) {
    char opp = (symbol == 'X') ? 'O' : 'X';

    // 1) محاولة الفوز مباشرة
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (hidden_board[i][j] != '-') continue;
            
            hidden_board[i][j] = symbol;
            bool win = check_three_in_row(symbol);
            hidden_board[i][j] = '-';
            
            if (win) return {i, j};
        }
    }

    // 2) حجب فوز الخصم
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (hidden_board[i][j] != '-') continue;
            
            hidden_board[i][j] = opp;
            bool oppWin = check_three_in_row(opp);
            hidden_board[i][j] = '-';
            
            if (oppWin) return {i, j};
        }
    }

    // 3) أولوية للمركز
    if (hidden_board[1][1] == '-') return {1, 1};

    // 4) الزوايا
    vector<pair<int, int>> corners = {{0,0}, {0,2}, {2,0}, {2,2}};
    for (auto& c : corners) {
        if (hidden_board[c.first][c.second] == '-')
            return c;
    }

    // 5) أي خلية فارغة
    return random_move();
}

// ================== MemoryTTT_UI ======================

MemoryTTT_UI::MemoryTTT_UI() : UI<char>(3) {
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║   Memory Tic-Tac-Toe (Hidden Marks)   ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    cout << "Rules:\n";
    cout << "  • 3x3 grid\n";
    cout << "  • Marks are HIDDEN after placement\n";
    cout << "  • Remember where you and opponent played!\n";
    cout << "  • First to align 3 marks wins\n\n";
}

void MemoryTTT_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    // عرض اللوحة الفارغة فقط (العلامات مخفية)
    cout << "\n    0   1   2\n";
    cout << "  ┌───┬───┬───┐\n";
    
    for (int i = 0; i < 3; ++i) {
        cout << i << " │";
        for (int j = 0; j < 3; ++j) {
            cout << "   │";
        }
        cout << "\n";
        if (i < 2)
            cout << "  ├───┼───┼───┤\n";
    }
    cout << "  └───┴───┴───┘\n";
}

Player<char>* MemoryTTT_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        int ai_level;
        cout << "Choose AI level for " << name << " (1=Random, 2=Smart): ";
        cin >> ai_level;
        if (ai_level == 2)
            name += " [AI:Smart]";
        else
            name += " [AI:Random]";
    }
    
    return new Player<char>(name, symbol, type);
}

Move<char>* MemoryTTT_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* bptr = player->get_board_ptr();
        MemoryTTT_Board* mb = dynamic_cast<MemoryTTT_Board*>(bptr);
        
        pair<int, int> mv = {-1, -1};
        
        if (mb) {
            string nm = player->get_name();
            if (nm.find("[AI:Smart]") != string::npos) {
                mv = mb->smart_move(player->get_symbol());
            } else {
                mv = mb->random_move();
            }
        }
        
        if (mv.first == -1) {
            mv = mb ? mb->random_move() : make_pair(0, 0);
        }

        cout << "\n" << player->get_name() 
             << " (Computer) played at: (" << mv.first << "," << mv.second << ")\n";

        return new Move<char>(mv.first, mv.second, player->get_symbol());
    }

    // لاعب بشري
    cout << player->get_name() << " (" << player->get_symbol() 
         << "), enter move (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}