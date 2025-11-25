/*#ifndef OBSTACLES_CLASSES_H
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

#endif*/ 





#ifndef OBSTACLES_CLASSES_H
#define OBSTACLES_CLASSES_H

#include "BoardGame_Classes.h"
#include <utility>

class Obstacles_Board : public Board<char> {
public:
    Obstacles_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // New helper AI methods (added - do not change game logic)
    std::pair<int, int> random_move() const;
    std::pair<int, int> smart_move(char symbol) const;

private:
    void add_random_obstacles(int count);
    bool has_four_in_row(char sym) const;
};

class Obstacles_UI : public UI<char> {
public:
    Obstacles_UI();
    Move<char>* get_move(Player<char>* player) override;

    // allow AI selection when creating players
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif

