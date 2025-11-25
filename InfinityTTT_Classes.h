#ifndef INFINITYTTT_CLASSES_H
#define INFINITYTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>
#include <utility>
#include <string>
#include <limits>

class InfinityTTT_Board : public Board<char> {
public:
    InfinityTTT_Board();

    // framework overrides
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // AI helpers
    std::pair<int, int> random_move() const;
    std::pair<int, int> smart_move(char symbol) const;

private:
    std::deque<std::pair<int, int>> move_order; // FIFO of moves (x,y)
    int total_moves;

    bool in_bounds(int x, int y) const;
    bool check_three_in_row(char sym) const;

    // minimax simulator helper
    int minimax_sim(char sim_board[3][3],
        std::deque<std::pair<int, int>> sim_order,
        int sim_total_moves,
        char maximizer,
        char current_player,
        int depth) const;
};

class InfinityTTT_UI : public UI<char> {
public:
    InfinityTTT_UI() {}
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif // INFINITYTTT_CLASSES_H
