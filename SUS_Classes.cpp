#include "SUS_Classes.h"
#include <cctype>

SUS_Board::SUS_Board() : Board<char>(3, 3), score_S(0), score_U(0) {
    // initialize board cells to blank symbol used in framework (commonly '-')
    for (auto &r : board)
        for (auto &c : r)
            c = '-';
}

bool SUS_Board::in_bounds(int x, int y) const {
    return x >= 0 && x < (int)board.size() && y >= 0 && y < (int)board[0].size();
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (!in_bounds(x, y)) return false;
    if (board[x][y] != '-') return false; // occupied

    board[x][y] = sym;

    // count new SUS sequences created that include this move
    int added = count_SUS_at(x, y);
    if (sym == 'S') score_S += added;
    else if (sym == 'U') score_U += added;

    return true;
}

int SUS_Board::count_SUS_at(int x, int y) const {
    // Count all S-U-S sequences that include cell (x,y).
    // A sequence is three aligned cells (row/col/diag) forming 'S' 'U' 'S'.
    int dirs[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    int cnt = 0;

    // If placed 'U' at middle, check both ends are 'S'
    if (board[x][y] == 'U') {
        for (int d=0; d<8; ++d) {
            int dx = dirs[d][0], dy = dirs[d][1];
            int x1 = x - dx, y1 = y - dy;
            int x2 = x + dx, y2 = y + dy;
            if (in_bounds(x1,y1) && in_bounds(x2,y2)) {
                if (board[x1][y1]=='S' && board[x2][y2]=='S') cnt++;
            }
        }
        return cnt;
    }

    // If placed 'S' at an end, check middle is 'U' and other end 'S'
    if (board[x][y] == 'S') {
        for (int d=0; d<8; ++d) {
            int dx = dirs[d][0], dy = dirs[d][1];
            int xm = x + dx, ym = y + dy;
            int xe = x + 2*dx, ye = y + 2*dy;
            if (in_bounds(xm,ym) && in_bounds(xe,ye)) {
                if (board[xm][ym]=='U' && board[xe][ye]=='S') cnt++;
            }
        }
    }
    return cnt;
}

bool SUS_Board::is_win(Player<char>* player) {
    // No immediate "three-in-a-row" win condition in SUS.
    // Winner is determined after the board is full by comparing counts.
    return false;
}

bool SUS_Board::is_lose(Player<char>* player) {
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    // draw detection is done after full board; framework may call this.
    for (auto &r : board)
        for (auto &c : r)
            if (c == '-') return false;
    return true;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    // The game ends when board is full (like framework's other games)
    return is_draw(player);
}

int SUS_Board::get_score(Player<char>* player) const {
    char s = player->get_symbol();
    return (s == 'S') ? score_S : score_U;
}

/* ------------------ SUS_UI ------------------ */

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    // If caller provided a placeholder symbol (e.g. '?'), prompt for S or U
    char sym = symbol;
    if (sym == '?' || sym == '\0') {
        cout << "Choose letter for " << name << " (S or U): ";
        cin >> sym;
        sym = toupper(sym);
        if (sym != 'S' && sym != 'U') sym = 'S';
    }
    return new Player<char>(name, sym, type);
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;
    cout << player->get_name() << " (" << player->get_symbol() << ") enter (row col): ";
    cin >> x >> y;
    // Create and return a Move; GameManager will call board->update_board(move)
    return new Move<char>(x, y, player->get_symbol());
}
