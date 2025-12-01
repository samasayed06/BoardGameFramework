#include "UltimateTTT_Classes.h"
#include <iostream>
#include <cstdlib>
using namespace std;

UltimateTTT_Board::UltimateTTT_Board() : Board<char>(9, 9) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            board[i][j] = ' ';

    
        subWinner.assign(3, vector<char>(3, '-'));
    nextR = nextC = -1;
    

}

bool UltimateTTT_Board::in_bounds(int r, int c) const {
    return r >= 0 && r < 9 && c >= 0 && c < 9;
}

char UltimateTTT_Board::compute_sub_winner(int sr, int sc) const {
    int br = sr * 3, bc = sc * 3;
    for (char s : {'X', 'O'}) {
        for (int r = 0; r < 3; r++)
            if (board[br + r][bc] == s && board[br + r][bc + 1] == s && board[br + r][bc + 2] == s)
                return s;
        for (int c = 0; c < 3; c++)
            if (board[br][bc + c] == s && board[br + 1][bc + c] == s && board[br + 2][bc + c] == s)
                return s;
        if (board[br][bc] == s && board[br + 1][bc + 1] == s && board[br + 2][bc + 2] == s)
            return s;
        if (board[br][bc + 2] == s && board[br + 1][bc + 1] == s && board[br + 2][bc] == s)
            return s;
    }
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[br + r][bc + c] == ' ')
                return '-';
    return 'D';
}

bool UltimateTTT_Board::update_board(Move<char>* m) {
    int r = m->get_x(), c = m->get_y();
    char s = m->get_symbol();

    
        if (!in_bounds(r, c)) return false;
    if (board[r][c] != ' ') return false;

    int sr = r / 3, sc = c / 3;
    if (nextR != -1 && nextC != -1)
        if (!(sr == nextR && sc == nextC)) return false;

    board[r][c] = s;

    subWinner[sr][sc] = compute_sub_winner(sr, sc);

    int nr = r % 3, nc = c % 3;
    if (subWinner[nr][nc] == '-') nextR = nr, nextC = nc;
    else nextR = nextC = -1;

    n_moves++;
    return true;
    

}

bool UltimateTTT_Board::main_winner(char s) const {
    for (int r = 0; r < 3; r++)
        if (subWinner[r][0] == s && subWinner[r][1] == s && subWinner[r][2] == s)
            return true;
    for (int c = 0; c < 3; c++)
        if (subWinner[0][c] == s && subWinner[1][c] == s && subWinner[2][c] == s)
            return true;
    if (subWinner[0][0] == s && subWinner[1][1] == s && subWinner[2][2] == s)
        return true;
    if (subWinner[0][2] == s && subWinner[1][1] == s && subWinner[2][0] == s)
        return true;
    return false;
}

bool UltimateTTT_Board::is_win(Player<char>* p) {
    return main_winner(p->get_symbol());
}

bool UltimateTTT_Board::is_lose(Player<char>* p) {
    char o = (p->get_symbol() == 'X') ? 'O' : 'X';
    return main_winner(o);
}

bool UltimateTTT_Board::is_draw(Player<char>* /*p*/) {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (subWinner[r][c] == '-')
                return false;

    
        return !(main_winner('X') || main_winner('O'));
 

}

bool UltimateTTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

vector<pair<int, int>> UltimateTTT_Board::get_legal_moves() const {
    vector<pair<int, int>> v;
    if (nextR != -1 && nextC != -1) {
        int br = nextR * 3, bc = nextC * 3;
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (board[br + r][bc + c] == ' ')
                    v.push_back({ br + r,bc + c });
    }
    else {
        for (int r = 0; r < 9; r++)
            for (int c = 0; c < 9; c++)
                if (board[r][c] == ' ')
                    v.push_back({ r,c });
    }
    return v;
}

pair<int, int> UltimateTTT_Board::random_move(char /*s*/) const {
    auto v = get_legal_moves();
    if (v.empty()) return { -1,-1 };
    return v[rand() % v.size()];
}

pair<int, int> UltimateTTT_Board::smart_move(char s) const {
    auto v = get_legal_moves();
    if (v.empty()) return { -1,-1 };

    
        char o = (s == 'X') ? 'O' : 'X';

    for (auto c : v) {
        int r = c.first, cc = c.second;
        int sr = r / 3, sc = cc / 3;
        int br = sr * 3, bc = sc * 3;
        char sub[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                sub[i][j] = board[br + i][bc + j];
        sub[r - br][cc - bc] = s;

        bool win = false;
        for (int i = 0; i < 3; i++) if (sub[i][0] == s && sub[i][1] == s && sub[i][2] == s) win = true;
        for (int j = 0; j < 3; j++) if (sub[0][j] == s && sub[1][j] == s && sub[2][j] == s) win = true;
        if (sub[0][0] == s && sub[1][1] == s && sub[2][2] == s) win = true;
        if (sub[0][2] == s && sub[1][1] == s && sub[2][0] == s) win = true;

        if (win) return c;
    }

    for (auto c : v) {
        int r = c.first, cc = c.second;
        int sr = r / 3, sc = cc / 3;
        int br = sr * 3, bc = sc * 3;
        char sub[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                sub[i][j] = board[br + i][bc + j];
        sub[r - br][cc - bc] = o;

        bool win = false;
        for (int i = 0; i < 3; i++) if (sub[i][0] == o && sub[i][1] == o && sub[i][2] == o) win = true;
        for (int j = 0; j < 3; j++) if (sub[0][j] == o && sub[1][j] == o && sub[2][j] == o) win = true;
        if (sub[0][0] == o && sub[1][1] == o && sub[2][2] == o) win = true;
        if (sub[0][2] == o && sub[1][1] == o && sub[2][0] == o) win = true;

        if (win) return c;
    }

    for (auto c : v)
        if (c.first % 3 == 1 && c.second % 3 == 1)
            return c;

    for (auto c : v) {
        int rr = c.first % 3, cc = c.second % 3;
        if ((rr == 0 && cc == 0) || (rr == 0 && cc == 2) || (rr == 2 && cc == 0) || (rr == 2 && cc == 2))
            return c;
    }

    return v[rand() % v.size()];
    

}

UltimateTTT_UI::UltimateTTT_UI() : UI<char>("Ultimate Tic Tac Toe", 1) {}

Player<char>* UltimateTTT_UI::create_player(string& n, char sym, PlayerType t) {
    if (t == PlayerType::COMPUTER) {
        int ai;
        cout << "AI for " << n << "? (1=Random / 2=Smart): ";
        cin >> ai;
        if (ai == 2) n += " [AI:Smart]";
        else n += " [AI:Random]";
    }
    return new Player<char>(n, sym, t);
}

Move<char>* UltimateTTT_UI::get_move(Player<char>* p) {
    UltimateTTT_Board* b = dynamic_cast<UltimateTTT_Board*>(p->get_board_ptr());
    if (!b) {
        int r, c;
        cin >> r >> c;
        return new Move<char>(r, c, p->get_symbol());
    }

   
        if (p->get_type() == PlayerType::COMPUTER) {
            pair<int, int> mv;
            if (p->get_name().find("Smart") != string::npos)
                mv = b->smart_move(p->get_symbol());
            else
                mv = b->random_move(p->get_symbol());
            cout << p->get_name() << " played (" << mv.first << "," << mv.second << ")\n";
            return new Move<char>(mv.first, mv.second, p->get_symbol());
        }

    int r, c;
    cout << p->get_name() << " enter row col: ";
    cin >> r >> c;
    return new Move<char>(r, c, p->get_symbol());
    

}
