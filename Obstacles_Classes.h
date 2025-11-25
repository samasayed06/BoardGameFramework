#ifndef OBSTACLES_CLASSES_H
#define OBSTACLES_CLASSES_H

#include "BoardGame_Classes.h"

class Obstacles_Board : public Board<char> {
public:
    Obstacles_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

private:
    void add_random_obstacles(int count);
    bool has_four_in_row(char sym) const;
};

class Obstacles_UI : public UI<char> {
public:
    Obstacles_UI();
    Move<char>* get_move(Player<char>* player) override;
};

#endif
