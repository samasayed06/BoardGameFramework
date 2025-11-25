#include "InfinityTTT_Classes.h"
#include <iostream>
#include <cstdlib>

using namespace std;

InfinityTTT_Board::InfinityTTT_Board() : Board<char>(3,3), total_moves(0) {
    for (auto &r : board)
        for (auto &c : r)
            c = '-';
}

bool InfinityTTT_Board::in_bounds(int x,int y) const {
    return x >= 0 && x < (int)board.size() && y >= 0 && y < (int)board[0].size();
}

bool InfinityTTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (!in_bounds(x,y)) return false;
    if (board[x][y] != '-') return false; // already occupied

    // place the move
    board[x][y] = sym;
    move_order.push_back({x,y});
    total_moves++;

    // If this placement creates a three-in-row for 'sym', we must NOT remove the oldest yet
    // (winner should be detected before any removal). So check immediate win; if win, keep state.
    if (check_three_in_row(sym)) {
        // Winner will be reported by GameManager calling is_win().
        return true;
    }

    // Otherwise, if we've reached a multiple of 3 moves, remove the oldest mark
    if (total_moves % 3 == 0 && !move_order.empty()) {
        auto p = move_order.front();
        move_order.pop_front();
        board[p.first][p.second] = '-';
    }

    return true;
}

bool InfinityTTT_Board::check_three_in_row(char sym) const {
    // rows and cols
    for (int i=0;i<3;++i) {
        if (board[i][0]==sym && board[i][1]==sym && board[i][2]==sym) return true;
        if (board[0][i]==sym && board[1][i]==sym && board[2][i]==sym) return true;
    }
    // diags
    if (board[0][0]==sym && board[1][1]==sym && board[2][2]==sym) return true;
    if (board[0][2]==sym && board[1][1]==sym && board[2][0]==sym) return true;
    return false;
}

bool InfinityTTT_Board::is_win(Player<char>* player) {
    return check_three_in_row(player->get_symbol());
}

bool InfinityTTT_Board::is_draw(Player<char>* player) {
    // It's rare to have a standard draw: here consider draw if no cell is filled (shouldn't happen)
    // or if game cannot continue (we'll treat full board with no winner as draw).
    for (auto &r : board)
        for (auto &c : r)
            if (c == '-') return false;
    return true;
}

bool InfinityTTT_Board::game_is_over(Player<char>* player) {
    // Let GameManager check per-player is_win/is_draw; but return true if any terminal state reached
    // Check for any symbol X or O having three in a row
    if (check_three_in_row('X') || check_three_in_row('O')) return true;
    // or board full
    bool anyEmpty = false;
    for (auto &r : board)
        for (auto &c : r)
            if (c == '-') anyEmpty = true;
    if (!anyEmpty) return true;
    return false;
}


/* ------------------ UI Implementation ------------------ */

Player<char>* InfinityTTT_UI::create_player(string& name, char symbol, PlayerType type) {
    // If caller passed a placeholder symbol, ask (but by default framework often passes symbol)
    char sym = symbol;
    if (sym == '?' || sym == '\0') {
        cout << "Choose symbol for " << name << " (single char, e.g. X or O): ";
        cin >> sym;
        if (sym == '\0') sym = 'X';
    }
    return new Player<char>(name, sym, type);
}

Move<char>* InfinityTTT_UI::get_move(Player<char>* player) {
    int x,y;
    if (player->get_type() == PlayerType::COMPUTER) {
        // simple random move (respecting board emptiness)
        do {
            x = rand() % player->get_board_ptr()->get_rows();
            y = rand() % player->get_board_ptr()->get_columns();
        } while (player->get_board_ptr()->get_cell(x,y) != '-');
        cout << "\n" << player->get_name() << " (Computer) played: (" << x << "," << y << ")\n";
        return new Move<char>(x,y, player->get_symbol());
    }

    cout << player->get_name() << " (" << player->get_symbol() << ") enter (row col): ";
    cin >> x >> y;
    return new Move<char>(x,y, player->get_symbol());
}
