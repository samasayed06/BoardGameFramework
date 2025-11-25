#ifndef INFINITYTTT_CLASSES_H
#define INFINITYTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>

class InfinityTTT_Board : public Board<char> {
public:
    InfinityTTT_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

private:
    std::deque<std::pair<int,int>> move_order; // FIFO of moves (x,y)
    int total_moves;

    bool check_three_in_row(char sym) const;
    bool in_bounds(int x,int y) const;
};

class InfinityTTT_UI : public UI<char> {
public:
    InfinityTTT_UI() {}
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif // INFINITYTTT_CLASSES_H
