
#ifndef INFINITYTTT_CLASSES_H
#define INFINITYTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>
#include <utility>

class InfinityTTT_Board : public Board<char> {
public:
    InfinityTTT_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // AI helpers
    pair<int, int> random_move() const;
    pair<int, int> smart_move(char symbol) const;

private:
    std::deque<std::pair<int, int>> move_order; // FIFO moves
    int total_moves;

    bool check_three_in_row(char sym) const;
    bool in_bounds(int x, int y) const;

    // helper for minimax simulation
    int minimax_sim(char sim_board[3][3], std::deque<std::pair<int, int>> sim_order, int sim_total_moves, char maximizer, char current_player) const;
};

class InfinityTTT_UI : public UI<char> {
public:
    InfinityTTT_UI() {}
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif

