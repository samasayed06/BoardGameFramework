#ifndef _TICTACTOE4X4_H
#define _TICTACTOE4X4_H

#include "BoardGame_Classes.h"
#include <utility>

template <typename T>
class TicTacToe4x4_Board : public Board<T> {
public:
    TicTacToe4x4_Board();

    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_lose(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;

    // NEW — Smart AI helper
    std::pair<int, int> random_move() const;
    std::pair<int, int> smart_move(T sym) const;
};

template <typename T>
class TicTacToe4x4_UI : public UI<T> {
public:
    TicTacToe4x4_UI();

    Move<T>* get_move(Player<T>* player) override;

    // NEW — Add Smart/Random AI choice
    Player<T>* create_player(std::string& name, T symbol, PlayerType type) override;
};

#endif
