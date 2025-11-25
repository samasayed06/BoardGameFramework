#include "SUS_Classes.h"
#include <cstdlib>
#include <cctype>
using namespace std;

SUS_Board::SUS_Board() : Board<char>(3, 3), score_S(0), score_U(0) {
    for (auto& r : board)
        for (auto& c : r)
            c = '-';
}

bool SUS_Board::in_bounds(int x, int y) const {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

// Count S-U-S patterns created by placing at (x, y)
int SUS_Board::count_SUS_at(int x, int y) const {
    char sym = board[x][y];
    int cnt = 0;

    int dirs[8][2] = {
        {1,0},{-1,0},{0,1},{0,-1},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    if (sym == 'U') {
        for (auto& d : dirs) {
            int x1 = x - d[0], y1 = y - d[1];
            int x2 = x + d[0], y2 = y + d[1];
            if (in_bounds(x1, y1) && in_bounds(x2, y2)) {
                if (board[x1][y1] == 'S' && board[x2][y2] == 'S')
                    cnt++;
            }
        }
    }

    if (sym == 'S') {
        for (auto& d : dirs) {
            int xm = x + d[0], ym = y + d[1];
            int xe = x + 2 * d[0], ye = y + 2 * d[1];
            if (in_bounds(xm, ym) && in_bounds(xe, ye)) {
                if (board[xm][ym] == 'U' && board[xe][ye] == 'S')
                    cnt++;
            }
        }
    }

    return cnt;
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char s = move->get_symbol();

    if (!in_bounds(x, y) || board[x][y] != '-')
        return false;

    board[x][y] = s;

    int gained = count_SUS_at(x, y);
    if (s == 'S') score_S += gained;
    else score_U += gained;

    return true;
}

/*
   IMPORTANT:
   Winner must be announced IMMEDIATELY after the first S-U-S.
*/

bool SUS_Board::is_win(Player<char>* player) {
    if (!player) return false;
    char sym = player->get_symbol();
    return get_score_for_symbol(sym) > 0;   // أول SUS = فوز
}

bool SUS_Board::is_lose(Player<char>* player) {
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    // no win & board full
    for (auto& r : board)
        for (auto& c : r)
            if (c == '-') return false;
    return true;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    if (is_win(player)) return true;
    return is_draw(player);
}

int SUS_Board::get_score_for_symbol(char s) const {
    return (s == 'S' ? score_S : score_U);
}

pair<int, int> SUS_Board::random_move() const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '-')
                empty.push_back({ i,j });

    if (empty.empty()) return { -1,-1 };
    return empty[rand() % empty.size()];
}

pair<int, int> SUS_Board::smart_move(char symbol) const {
    vector<pair<int, int>> empty;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '-')
                empty.push_back({ i,j });

    int bestGain = -1;
    pair<int, int> best = empty.empty() ? make_pair(-1, -1) : empty[0];

    for (auto& p : empty) {
        int x = p.first, y = p.second;
        int gain = 0;

        int dirs[8][2] = {
            {1,0},{-1,0},{0,1},{0,-1},
            {1,1},{1,-1},{-1,1},{-1,-1}
        };

        if (symbol == 'U') {
            for (auto& d : dirs) {
                int x1 = x - d[0], y1 = y - d[1];
                int x2 = x + d[0], y2 = y + d[1];
                if (in_bounds(x1, y1) && in_bounds(x2, y2))
                    if (board[x1][y1] == 'S' && board[x2][y2] == 'S')
                        gain++;
            }
        }
        else {
            for (auto& d : dirs) {
                int xm = x + d[0], ym = y + d[1];
                int xe = x + 2 * d[0], ye = y + 2 * d[1];
                if (in_bounds(xm, ym) && in_bounds(xe, ye))
                    if (board[xm][ym] == 'U' && board[xe][ye] == 'S')
                        gain++;
            }
        }

        if (gain > bestGain) {
            bestGain = gain;
            best = p;
        }
    }

    return best;
}

/* ====================== UI ======================== */

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    if (firstPlayer) {
        symbol = 'S';
        firstPlayer = false;
    }
    else {
        symbol = 'U';
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

Move<char>* SUS_UI::get_move(Player<char>* player) {
    SUS_Board* sb = dynamic_cast<SUS_Board*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::COMPUTER) {
        string nm = player->get_name();
        pair<int, int> mv;

        if (nm.find("Smart") != string::npos)
            mv = sb->smart_move(player->get_symbol());
        else
            mv = sb->random_move();

        return new Move<char>(mv.first, mv.second, player->get_symbol());
    }

    int x, y;
    cout << player->get_name()
        << " (" << player->get_symbol() << ") enter row col: ";
    cin >> x >> y;

    return new Move<char>(x, y, player->get_symbol());
}
